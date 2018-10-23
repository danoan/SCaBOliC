#ifndef SCABOLIC_EXPINPUT_H
#define SCABOLIC_EXPINPUT_H

#include <model/ImageInput.h>
#include <DGtal/helpers/StdDefs.h>
#include <DGtal/shapes/parametric/AccFlower2D.h>
#include <DGtal/shapes/parametric/Ball2D.h>
#include <DGtal/shapes/parametric/NGon2D.h>
#include <DGtal/shapes/parametric/Ellipse2D.h>
#include <Utils/ShapeDigitizer.h>

namespace SCaBOliC
{
    namespace Lab
    {
        namespace Experiment
        {
            namespace ExpInput
            {
                typedef Core::ODRModel::OptimizationMode OptimizationMode;
                typedef Core::ODRModel::ApplicationMode ApplicationMode;

                typedef Optimization::QPBOSolverType QPBOSolverType;

                typedef Lab::Model::ImageInput ImageInput;

                typedef std::list<ImageInput> ExpInputSet;

                typedef std::vector<QPBOSolverType> SolverStrategyVector;
                typedef std::vector<OptimizationMode> OptModeVector;
                typedef std::vector<ApplicationMode> AppModeVector;

                class ParameterVariation
                {
                public:
                    typedef SCaBOliC::Lab::Utils::Generator<3> MyGenerator;
                    struct ParameterVariationInstance
                    {
                        ParameterVariationInstance(){}
                        ParameterVariationInstance(QPBOSolverType solverType,
                                                   OptimizationMode optMode,
                                                   ApplicationMode appMode):solverType(solverType),
                                                                            optimizationMode(optMode),
                                                                            applicationMode(appMode){}

                        QPBOSolverType solverType;
                        OptimizationMode optimizationMode;
                        ApplicationMode applicationMode;
                    };
                private:
                    static MyGenerator initGenerator(const SolverStrategyVector& sss,
                                                     const OptModeVector& oms,
                                                     const AppModeVector& ams)
                    {
                        MyGenerator::Index indexLims[3] = {sss.size()-1,oms.size(),ams.size()-1};
                        return MyGenerator(indexLims);
                    }

                public:
                    ParameterVariation(const SolverStrategyVector& sss,
                                       const OptModeVector& oms,
                                       const AppModeVector& ams):sss(sss),
                                                                 oms(oms),
                                                                 ams(ams),
                                                                 mygen(initGenerator(sss,oms,ams)){}

                    bool next(ParameterVariationInstance& pvi)
                    {
                        bool success = mygen.next(currsequence);

                        if(success)
                        {
                            if(!sss.empty())
                                pvi.solverType = sss[currsequence[0]];
                            if(!oms.empty())
                                pvi.optimizationMode = oms[currsequence[1]];
                            if(!ams.empty())
                                pvi.applicationMode = ams[currsequence[2]];
                        }

                        return success;
                    }

                    void restart()
                    {
                        MyGenerator::Index indexLims[3] = {sss.size()-1,oms.size(),ams.size()-1};
                        mygen.restart(indexLims);
                    }

                private:
                    SCaBOliC::Lab::Utils::Generator<3> mygen;

                    const SolverStrategyVector& sss;
                    const OptModeVector& oms;
                    const AppModeVector& ams;
                    MyGenerator::Index currsequence[3] = {0,0,0};

                };


                class DigitizerInput
                {
                public:
                    typedef DGtal::Z2i::Space Space;

                    typedef DGtal::AccFlower2D<Space> Flower;
                    typedef DGtal::Ball2D<Space> Ball;
                    typedef DGtal::NGon2D<Space> NGon;
                    typedef DGtal::Ellipse2D<Space> Ellipse;

                private:
                    static constexpr double r=40;

                    template<typename TShape>
                    static ImageInput toImage(TShape shape, double h, const std::string& name)
                    {
                        return
                                SCaBOliC::Lab::Utils::ShapeDigitizer<TShape>::digitize(shape,
                                                                               h,
                                                                               imageOutputFolder,
                                                                               name);
                    }

                public:
                    static ImageInput ball(double h=1.0){ Ball s(0,0,r); return toImage(s,h,"ball"); }
                    static ImageInput flower(double h=1.0){ Flower s(0,0,r,20,2,1); return toImage(s,h,"flower"); }
                    static ImageInput triangle(double h=1.0){ NGon s(0,0,r,3,1); return toImage(s,h,"triangle"); }
                    static ImageInput square(double h=1.0){ NGon s(0,0,r,4,1); return toImage(s,h,"square"); }
                    static ImageInput pentagon(double h=1.0){ NGon s(0,0,r,5,1); return toImage(s,h,"pentagon"); }
                    static ImageInput heptagon(double h=1.0){ NGon s(0,0,r,5,1); return toImage(s,h,"heptagon"); }
                    static ImageInput ellipse(double h=1.0){ Ellipse s(0,0,r,r-10,0); return toImage(s,h,"ellipse"); }

                    static const std::string imageOutputFolder;

                };

            }
        }
    }
}


#endif //SCABOLIC_EXPINPUT_H
