#include "SCaBOliC/Energy/ISQ/Terms/Data/DataTerm.h"

using namespace SCaBOliC::Energy::ISQ;

template<typename TODRFactory>
DataTerm<TODRFactory>::DataTerm(const InputData &id):vm(id.optimizationRegions),
                                                     image(id.image),
                                                     translation(id.translation)
{
    initializeOptimizationData(id,this->vm,this->od);
    configureOptimizationData(id,this->vm,this->od);
}

template<typename TODRFactory>
void DataTerm<TODRFactory>::initializeOptimizationData(const InputData& id,
                                                       const VariableMap& vm,
                                                       OptimizationData& od)
{
    od.numVars = vm.numVars;

    od.localUTM = OptimizationData::UnaryTermsMatrix(2,
                                                     od.numVars);
    od.localUTM.setZero();

    od.localPTM = OptimizationData::PairwiseTermsMatrix(od.numVars,
                                                        od.numVars);
    od.localPTM.setZero();
}

template<typename TODRFactory>
void DataTerm<TODRFactory>::configureOptimizationData(const InputData& id,
                                                      const VariableMap& vm,
                                                      OptimizationData& od)
{
    this->constantFactor = 1;
    this->constantTerm = 0;

    double maxCtrb;
    setCoeffs(od,
              maxCtrb,
              id,
              vm);

    this->normalizationFactor = 1.0/maxCtrb;
    this->weight = id.dataTermWeight;

    od.localUTM*=this->weight*this->normalizationFactor;
    od.localPTM*=this->weight*this->normalizationFactor;

}

template<typename TODRFactory>
void DataTerm<TODRFactory>::setCoeffs(OptimizationData& od,
                                      double& maxCtrb,
                                      const InputData& id,
                                      const VariableMap& vm)
{
    const InputData::OptimizationDigitalRegions ODR = id.optimizationRegions;
    int col,row;
    Index xi;

    typedef DGtal::Z2i::Point Point;

//    cv::Mat imgMap = cv::Mat::zeros( image.size(),image.type() );
//    for(auto it = ODR.optRegion.begin();it!=ODR.optRegion.end();++it)
//    {
//        col = ODR.toImageCoordinates( (*it) )[0] + translation(0);
//        row = (image.rows-1) - (ODR.toImageCoordinates( (*it) )[1] + translation(1));
//
//        imgMap.at<cvColorType>(row,col) = image.at<cvColorType>(row,col);
//    }
//
//    cv::imwrite("imgMap.jpg",imgMap);


    maxCtrb=0;
    for(auto it = ODR.optRegion.begin();it!=ODR.optRegion.end();++it)
    {
        col = ODR.toImageCoordinates( (*it) )[0] + translation(0);
        row = (image.rows-1) - (ODR.toImageCoordinates( (*it) )[1] + translation(1));

        xi = vm.pim.at(*it);

        //Recall solution is inverted at the end.
        od.localUTM(0,xi) = -log( 1 - id.bgDistr(row,col) );
        od.localUTM(1,xi) = -log( 1 - id.fgDistr(row,col) );

        cvColorType v = image.at<cvColorType>(row,col);
        cvColorType vn;
        double homogeneity=0;

        for(auto itp = this->odrFactory.neighBegin(); itp!=this->odrFactory.neighEnd();++itp)
        {
            Point neigh = ODR.toImageCoordinates(*it) + ODR.toImageCoordinates(*itp);
            if(neigh(0)<0 || neigh(1)<0) continue;
            if(neigh(0)>image.cols || neigh(1)>image.rows) continue;
            if(!ODR.trustFRG(neigh)) continue;

            vn = image.at<cvColorType>( (image.rows-1) - ( neigh(1)+translation(1) ),neigh(0)+translation(0));

            homogeneity += exp( - (0.1 +(v-vn).dot(v-vn)/195075.0) );
        }

        //Again, think opposite way
        //od.localUTM(1,xi)+= homogeneity;

        maxCtrb = fabs(od.localUTM(1,xi))>maxCtrb?fabs(od.localUTM(1,xi)):maxCtrb;
        maxCtrb = fabs(od.localUTM(0,xi))>maxCtrb?fabs(od.localUTM(0,xi)):maxCtrb;
    }

}

template<typename TODRFactory>
void DataTerm<TODRFactory>::addCoeff(OptimizationData::PairwiseTermsMatrix& PTM,
                                     double& maxPTM,
                                     Index i1,
                                     Index i2,
                                     Scalar v)
{
    this->crescentOrder(i1,i2);
    PTM.coeffRef(i1,i2) += v;
    maxPTM = fabs(PTM.coeffRef(i1,i2))>maxPTM?fabs(PTM.coeffRef(i1,i2)):maxPTM;
}