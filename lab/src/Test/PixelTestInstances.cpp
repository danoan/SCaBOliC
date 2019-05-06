#include <SCaBOliC/lab/Test/PixelTestInstances.h>

using namespace SCaBOliC::Lab::Test;

PixelTestInstances::QPBOSolverType PixelTestInstances::vectorOfSolver[4] = {QPBOSolverType::Improve,
                                                                            QPBOSolverType::ImproveProbe,
                                                                            QPBOSolverType::Simple,
                                                                            QPBOSolverType::Probe};

PixelTestInstances::OptimizationMode PixelTestInstances::vectorOfOM[2] = {PixelTestInstances::OptimizationMode::OM_OriginalBoundary,
                                                                PixelTestInstances::OptimizationMode::OM_DilationBoundary};

PixelTestInstances::ApplicationMode PixelTestInstances::vectorOfAM[2] = {PixelTestInstances::ApplicationMode::AM_OptimizationBoundary,
                                                                         PixelTestInstances::ApplicationMode::AM_AroundBoundary};


bool PixelTestInstances::vectorOfOptInAppl[2] = {false, true};
bool PixelTestInstances::vectorOfInversion[2] = {false, true};

PixelTestInstances::MyGenerator::Index PixelTestInstances::indexLims[5] = {0,0,0,0,0};
//int TestInstances::indexLims[3] = {1,1,1};

PixelTestInstances::PixelTestInstances(std::string imagePath):imagePath(imagePath),
                                                    gen( PixelTestInstances::indexLims )
{
    memset(currSequence,0,sizeof(MyGenerator::Index)*5);
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
                          PixelTestInstances::CountingMode::CM_PIXEL,
                          vectorOfOptInAppl[currSequence[3]],
                          vectorOfInversion[currSequence[4]]);
    } else
    {
        return UserInput (imagePath,
                          vectorOfSolver[0],
                          vectorOfOM[0],
                          vectorOfAM[0],
                          PixelTestInstances::ApplicationCenter::AC_PIXEL,
                          PixelTestInstances::CountingMode::CM_PIXEL,
                          vectorOfOptInAppl[currSequence[0]],
                          vectorOfInversion[currSequence[0]]);
    }

}