#include "SCaBOliC/Core/ODRFactory.h"

namespace SCaBOliC {
    namespace Core
    {
        namespace ODRFactory {
            OptimizationDigitalRegions createODR(OptimizationMode optMode,
                                                 ApplicationMode appMode,
                                                 unsigned int radius,
                                                 DigitalSet original)
            {
                typedef DIPaCUS::Misc::DigitalBoundary<DIPaCUS::Neighborhood::EightNeighborhoodPredicate<DigitalSet>> EightNeighborhood;

                Domain domain(original.domain());
                Domain applicationDomain(domain.lowerBound() + Point(radius,radius),
                                         domain.upperBound() - Point(radius,radius));

                DigitalSet emptySet(domain);
                DigitalSet originalBoundary(domain);

                DigitalSet dilated(domain);
                DigitalSet dilatedBoundary(domain);

                DigitalSet eroded(domain);
                DigitalSet erodedBoundary(domain);

                DigitalSet optRegion(domain);
                DigitalSet applicationRegion(applicationDomain);


                DIPaCUS::Morphology::Dilate(dilated,
                                            original,
                                            DIPaCUS::Morphology::RECT,
                                            1);


                DIPaCUS::Morphology::Erode(eroded,
                                           original,
                                           DIPaCUS::Morphology::RECT,
                                           1);
                
                EightNeighborhood en(originalBoundary,original);

                DIPaCUS::SetOperations::SetDifference(dilatedBoundary,dilated,original);
                DIPaCUS::SetOperations::SetDifference(erodedBoundary,original,eroded);


                switch (optMode) {
                    case OptimizationMode::OM_OriginalBoundary: {
                        optRegion = originalBoundary;
                        break;
                    }
                    case OptimizationMode::OM_DilationBoundary: {
                        optRegion = dilatedBoundary;
                        break;
                    }
                    case OptimizationMode::OM_FullForeground: {
                        optRegion = dilated;
                        break;
                    }
                    case OptimizationMode::OM_FullImage: {
                        optRegion.assignFromComplement(emptySet);
                        break;
                    }
                }


                switch (appMode) {
                    case ApplicationMode::AM_OriginalBoundary: {
                        applicationRegion.insert(originalBoundary.begin(),originalBoundary.end());
                        break;
                    }
                    case ApplicationMode::AM_DilatedBoundary: {
                        applicationRegion.insert(dilatedBoundary.begin(),dilatedBoundary.end());
                        break;
                    }
                    case ApplicationMode::AM_FullImage: {
                        applicationRegion.assignFromComplement(emptySet);
                        break;
                    }
                    case ApplicationMode::AM_AroundBoundary: {
                        DigitalSet originalInLargerDomain (applicationDomain);
                        originalInLargerDomain.insert(original.begin(),original.end());

                        DigitalSet ballReachDilation(applicationDomain);
                        DIPaCUS::Morphology::Dilate(ballReachDilation,originalInLargerDomain,DIPaCUS::Morphology::RECT,radius);

                        DigitalSet dilationApplication(applicationDomain);
                        DIPaCUS::SetOperations::SetDifference(dilationApplication,ballReachDilation,originalInLargerDomain);

                        applicationRegion.insert(dilationApplication.begin(),dilationApplication.end());


                        DigitalSet erodedInLargerDomain (applicationDomain);
                        erodedInLargerDomain.insert(eroded.begin(),eroded.end());
                        
                        DigitalSet ballReachErosion(applicationDomain);
                        DIPaCUS::Morphology::Erode(ballReachErosion,erodedInLargerDomain,DIPaCUS::Morphology::RECT,radius);

                        DigitalSet erosionApplication(applicationDomain);
                        DIPaCUS::SetOperations::SetDifference(erosionApplication,erodedInLargerDomain,ballReachErosion);

                        applicationRegion.insert(erosionApplication.begin(),erosionApplication.end());


                        break;
                    }
                }

                DigitalSet trustFRG(domain);
                DIPaCUS::SetOperations::SetDifference(trustFRG, original, optRegion);

                DigitalSet trustBKG(domain);
                DigitalSet tempp(domain);

                tempp += trustFRG;
                tempp += optRegion;

                trustBKG.assignFromComplement(tempp);

                return OptimizationDigitalRegions(domain,
                                                  original,
                                                  optRegion,
                                                  trustFRG,
                                                  trustBKG,
                                                  applicationRegion);
            }

        }

    }
}