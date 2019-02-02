#include <SCaBOliC/lab/Test/PixelTestInstances.h>

using namespace SCaBOliC::Lab::Test;

PixelTestInstances::QPBOSolverType PixelTestInstances::vectorOfSolver[4] = {QPBOSolverType::ImproveProbe,
                                                                  QPBOSolverType::Simple,
                                                                  QPBOSolverType::Probe,
                                                                  QPBOSolverType::Improve};

PixelTestInstances::OptimizationMode PixelTestInstances::vectorOfOM[2] = {PixelTestInstances::OptimizationMode::OM_OriginalBoundary,
                                                                PixelTestInstances::OptimizationMode::OM_DilationBoundary};

PixelTestInstances::ApplicationMode PixelTestInstances::vectorOfAM[3] = {PixelTestInstances::ApplicationMode::AM_AroundBoundary,
                                                               PixelTestInstances::ApplicationMode::AM_InverseAroundBoundary,
                                                               PixelTestInstances::ApplicationMode::AM_OptimizationBoundary,
                                                              };

PixelTestInstances::MyGenerator::Index PixelTestInstances::indexLims[3] = {3,1,2};
//int TestInstances::indexLims[3] = {1,1,1};

PixelTestInstances::PixelTestInstances(std::string imagePath):imagePath(imagePath),
                                                    gen( PixelTestInstances::indexLims )
{
    memset(currSequence,0,sizeof(MyGenerator::Index)*3);
}

PixelTestInstances::UserInput PixelTestInstances::next(bool& success)
{
    success = gen.next(currSequence);

    if(success)
    {
        return UserInput (imagePath,
                          vectorOfSolver[currSequence[0]],
                          vectorOfOM[currSequence[1]],
                          vectorOfAM[currSequence[2]],
                          PixelTestInstances::ApplicationCenter::AC_PIXEL,
                          PixelTestInstances::CountingMode::CM_PIXEL);
    } else
    {
        return UserInput (imagePath,
                          vectorOfSolver[0],
                          vectorOfOM[0],
                          vectorOfAM[0],
                          PixelTestInstances::ApplicationCenter::AC_PIXEL,
                          PixelTestInstances::CountingMode::CM_PIXEL);
    }

}