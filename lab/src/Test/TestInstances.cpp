#include <Test/TestInstances.h>

using namespace SCaBOliC::Lab::Test;

TestInstances::QPBOSolverType TestInstances::vectorOfSolver[4] = {QPBOSolverType::ImproveProbe,
                                                                  QPBOSolverType::Simple,
                                                                  QPBOSolverType::Probe,
                                                                  QPBOSolverType::Improve};

TestInstances::OptimizationMode TestInstances::vectorOfOM[2] = {TestInstances::OptimizationMode::OM_OriginalBoundary,
                                                                TestInstances::OptimizationMode::OM_DilationBoundary};

TestInstances::ApplicationMode TestInstances::vectorOfAM[4] = {TestInstances::ApplicationMode::AM_InternRange,
                                                               TestInstances::ApplicationMode::AM_AroundBoundary,
                                                               TestInstances::ApplicationMode::AM_InverseAroundBoundary,
                                                               TestInstances::ApplicationMode::AM_OptimizationBoundary,
                                                              };

TestInstances::MyGenerator::Index TestInstances::indexLims[3] = {3,1,3};
//int TestInstances::indexLims[3] = {1,1,1};

TestInstances::TestInstances(std::string imagePath):imagePath(imagePath),
                                                    gen( TestInstances::indexLims )
{
    memset(currSequence,0,sizeof(MyGenerator::Index)*3);
}

TestInstances::UserInput TestInstances::next(bool& success)
{
    success = gen.next(currSequence);

    if(success)
    {
        return UserInput (imagePath,
                          vectorOfSolver[currSequence[0]],
                          vectorOfOM[currSequence[1]],
                          vectorOfAM[currSequence[2]],
                          TestInstances::ApplicationCenter::AC_PIXEL,
                          TestInstances::CountingMode::CM_PIXEL);
    } else
    {
        return UserInput (imagePath,
                          vectorOfSolver[0],
                          vectorOfOM[0],
                          vectorOfAM[0],
                          TestInstances::ApplicationCenter::AC_PIXEL,
                          TestInstances::CountingMode::CM_PIXEL);
    }

}