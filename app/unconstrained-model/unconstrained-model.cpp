#include <iostream>
#include <eigen3/Eigen/Sparse>
#include <DGtal/helpers/StdDefs.h>

#include <DIPaCUS/base/Shapes.h>
#include <DIPaCUS/components/Transform.h>
#include <DIPaCUS/derivates/Misc.h>

#include <LBFGS.h>

using namespace DGtal::Z2i;
using namespace LBFGSpp;

typedef Eigen::SparseMatrix<double> SpMat; // declares a column-major sparse matrix type of double
typedef Eigen::Triplet<double> T;

typedef std::map<unsigned long int, Point> LinelIndexCoordsMap;










#include <adept.h>

using adept::adouble;

adouble algorithm(const adouble x[2])
{
    adouble y = 4.0;
    adouble s = 2.0*x[0] + 3.0*x[1]*x[1];
    y*= sin(s);
    return y;
}


double algorithm_and_gradient(const double xval[2],
                              double dy_dx[2])
{
    adept::Stack stack;
    using adept::adouble;

    adouble x[2] = {xval[0],xval[1]};
    stack.new_recording();

    adouble y = algorithm(x);
    y.set_gradient(1.0);
    stack.compute_adjoint();
    dy_dx[0] = x[0].get_gradient();
    dy_dx[1] = x[1].get_gradient();

    return 1;
}

int main2()
{
    double xval[2] = {1,3};
    double dy_dx[2];

    std::cout << algorithm_and_gradient(xval,dy_dx) << std::endl;
    return 0;
}




class Rosenbrock
{
private:
    int n;
public:
    Rosenbrock(int n_) : n(n_) {}
    double operator()(const Eigen::VectorXd& x, Eigen::VectorXd& grad)
    {
        double fx = 0.0;
        for(int i = 0; i < n; i += 2)
        {
            double t1 = 1.0 - x[i];
            double t2 = 10 * (x[i + 1] - x[i] * x[i]);
            grad[i + 1] = 20 * t2;
            grad[i]     = -2.0 * (x[i] * grad[i + 1] + t1);
            fx += t1 * t1 + t2 * t2;
        }
        return fx;
    }
};


void bfgs()
{
    const int n = 10;
    // Set up parameters
    LBFGSParam<double> param;
    param.epsilon = 1e-6;
    param.max_iterations = 100;

    // Create solver and function object
    LBFGSSolver<double> solver(param);
    Rosenbrock fun(n);

    // Initial guess
    Eigen::VectorXd x = Eigen::VectorXd::Zero(n);
    // x will be overwritten to be the best point found
    double fx;
    int niter = solver.minimize(fun, x, fx);

    std::cout << niter << " iterations" << std::endl;
    std::cout << "x = \n" << x.transpose() << std::endl;
    std::cout << "f(x) = " << fx << std::endl;
}

LinelIndexCoordsMap linelIndexToCoords(int pixelRows, int pixelCols)
{
    int r = pixelRows;
    int c = pixelCols;

    int m = r*c;
    int n = r*(c+1);

    LinelIndexCoordsMap licm;
    for(int j=0;j<m;++j)
    {
        Point pBase = Point( j/c,j%c) * 2;

        unsigned long int i1 = 2*(j/c)*(c+1)+j%c+1;
        Point p1 = pBase - Point(0,1);
        licm.insert( std::make_pair(i1,p1) );

        unsigned long int i2 = (2*(j/c) + 1)*(c+1)+j%c;
        Point p2 = pBase - Point(1,0);
        licm.insert( std::make_pair(i2,p2) );

        unsigned long int i3 = (2*(j/c) + 1)*(c+1)+j%c + 1;
        Point p3 = pBase + Point(1,0);
        licm.insert( std::make_pair(i3,p3) );

        unsigned long int i4 = (2*(j/c+1))*(c+1)+j%c+1;
        Point p4 = pBase + Point(0,1);
        licm.insert( std::make_pair(i4,p4) );
    }

    return licm;
}

void pim(SpMat& mat,int pixelRows, int pixelCols)
{
    int c = pixelCols;

    int m = mat.cols();
    int n = mat.rows();

    std::vector<T> coefficients;

    for(int j=0;j<m;++j)
    {
        coefficients.push_back( T(2*(j/c)*(c+1)+j%c+1,j,1) );
        coefficients.push_back( T( (2*(j/c) + 1)*(c+1)+j%c, j, -1 ) );
        coefficients.push_back( T( (2*(j/c) + 1)*(c+1)+j%c + 1,j,1 ) );
        coefficients.push_back( T( (2*(j/c+1))*(c+1)+j%c+1,j,-1 ) );
    }

    mat.setFromTriplets(coefficients.begin(),coefficients.end());
}

void ballM(std::vector<T>& coefficients,int pixelRows, int pixelCols, double radius,double gridStep,const Domain& domain)
{
    LinelIndexCoordsMap LICM = linelIndexToCoords(pixelRows,pixelCols);
    double iRadius=radius*2;

    Point size = domain.upperBound() - domain.lowerBound() + Point(1,1);
    Domain iDomain( Point(0,0), 2*size);
    DigitalSet iPixels( iDomain );
    for(auto p:iDomain)
    {
        if(p[0]%2==1 && p[1]%2==1) iPixels.insert(p);
    }

    DigitalSet intersection(iDomain);
    DIPaCUS::Misc::DigitalBallIntersection DBI(iRadius,iPixels);
    for(auto pair:LICM)
    {
        unsigned long int lIndex = pair.first;
        Point lCoords = pair.second;

        intersection.clear();
        DBI(intersection,lCoords);

        for(auto p:intersection)
        {
            int r = (p[1]-1)/2;
            int c = (p[0]-1)/2;
            int index = r*pixelCols + c;

            coefficients.push_back( T(index,lIndex,1) );
        }
    }


}

void preprocess(SpMat& mat,int nPixels,int nLinels,int pRows,int pCols,double radius, double h, const Domain& domain,const std::string& filepath,bool load=false)
{
    std::vector<T> coefficients;
    if(!load)
    {
        ballM(coefficients,pRows,pCols,radius,h,domain);

        std::ofstream ofs(filepath,std::ios_base::out);
        for(auto t:coefficients)
        {
            ofs.write( (char*) &t.row(),sizeof(t.row()));
            ofs.write( (char*) &t.col(),sizeof(t.col()));

            ofs.write( (char*) &t.value(),sizeof(t.value()));
        }

        ofs.flush();
        ofs.close();
    }else
    {
        std::ifstream ifs(filepath,std::ios_base::in);
        int row,col;
        double value;
        while(!ifs.eof())
        {
            ifs.read( (char*) &row ,sizeof(int) );
            ifs.read( (char*) &col,sizeof(int) );
            ifs.read( (char*) &value ,sizeof(double) );

            coefficients.push_back( T(row,col,value) );
        }
    }

    mat.setFromTriplets(coefficients.begin(),coefficients.end());

}

int main(int argc,char* argv[])
{
    double h = std::atof(argv[1]);
    double radius = std::atof(argv[2]);
    std::string ballMPath = argv[3];
    bool load = std::strcmp(argv[4],"load")==0?true:false;

    DigitalSet _shape = DIPaCUS::Shapes::square(h);
    DigitalSet shape = DIPaCUS::Transform::bottomLeftBoundingBoxAtOrigin(_shape);

    Point size = shape.domain().upperBound() - shape.domain().lowerBound() + Point(1,1);
    int pRows = size[1];
    int pCols = size[0];

    int m = pRows*pCols;
    int n = (2*pRows+1)*(pCols+1);

    SpMat PIM(n,m);
    pim(PIM,pRows,pCols);

    SpMat BALLM(m,n);
    preprocess(BALLM,m,n,pRows,pCols,radius,h,shape.domain(),ballMPath,load);

    return 0;
}