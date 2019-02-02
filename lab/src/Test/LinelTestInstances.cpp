#include <SCaBOliC/lab/Test/LinelTestInstances.h>

using namespace SCaBOliC::Lab::Test;

LinelTestInstances::QPBOSolverType LinelTestInstances::vectorOfSolver[4] = {QPBOSolverType::ImproveProbe,
                                                                            QPBOSolverType::Simple,
                                                                            QPBOSolverType::Probe,
                                                                            QPBOSolverType::Improve};

LinelTestInstances::OptimizationMode LinelTestInstances::vectorOfOM[2] = {LinelTestInstances::OptimizationMode::OM_OriginalBoundary,
                                                                          LinelTestInstances::OptimizationMode::OM_DilationBoundary};

LinelTestInstances::ApplicationMode LinelTestInstances::vectorOfAM[5] = {LinelTestInstances::ApplicationMode::AM_InternRange,
                                                                         LinelTestInstances::ApplicationMode::AM_InverseInternRange,
                                                                         LinelTestInstances::ApplicationMode::AM_AroundIntern,
                                                                         LinelTestInstances::ApplicationMode::AM_InverseAroundIntern,
                                                                         LinelTestInstances::ApplicationMode::AM_OptimizationBoundary,
};

LinelTestInstances::MyGenerator::Index LinelTestInstances::indexLims[3] = {3,1,4};
//int TestInstances::indexLims[3] = {1,1,1};

LinelTestInstances::LinelTestInstances(std::string imagePath):imagePath(imagePath),
                                                              gen( LinelTestInstances::indexLims )
{
    memset(currSequence,0,sizeof(MyGenerator::Index)*3);
}

LinelTestInstances::UserInput LinelTestInstances::next(bool& success)
{
    success = gen.next(currSequence);

    if(success)
    {
        return UserInput (imagePath,
                          vectorOfSolver[currSequence[0]],
                          vectorOfOM[currSequence[1]],
                          vectorOfAM[currSequence[2]],
                          LinelTestInstances::ApplicationCenter::AC_LINEL,
                          LinelTestInstances::CountingMode::CM_PIXEL);
    } else
    {
        return UserInput (imagePath,
                          vectorOfSolver[0],
                          vectorOfOM[0],
                          vectorOfAM[0],
                          LinelTestInstances::ApplicationCenter::AC_LINEL,
                          LinelTestInstances::CountingMode::CM_PIXEL);
    }

}