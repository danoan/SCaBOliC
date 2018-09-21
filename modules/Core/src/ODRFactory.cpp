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

                Domain applicationDomain(original.domain() );
                Domain domain(original.domain().lowerBound() - Point(radius,radius),
                              original.domain().upperBound() + Point(radius,radius));

                DigitalSet originalInLargerDomain (domain);
                originalInLargerDomain.insert(original.begin(),original.end());

                DigitalSet emptySet(applicationDomain);
                DigitalSet originalBoundary(domain);

                DigitalSet dilated(domain);
                DigitalSet dilatedBoundary(domain);

                DigitalSet eroded(domain);
                DigitalSet erodedBoundary(domain);

                DigitalSet optRegion(domain);
                DigitalSet applicationRegion(applicationDomain);


                DIPaCUS::Morphology::Dilate(dilated,
                                            originalInLargerDomain,
                                            DIPaCUS::Morphology::RECT,
                                            1);


                DIPaCUS::Morphology::Erode(eroded,
                                           originalInLargerDomain,
                                           DIPaCUS::Morphology::RECT,
                                           1);
                
                EightNeighborhood en(originalBoundary,originalInLargerDomain);

                DIPaCUS::SetOperations::SetDifference(dilatedBoundary,dilated,originalInLargerDomain);
                DIPaCUS::SetOperations::SetDifference(erodedBoundary,originalInLargerDomain,eroded);

                DigitalSet& originalAlias = originalInLargerDomain;

                switch (optMode) {
                    case OptimizationMode::OM_OriginalBoundary: {
                        optRegion = originalBoundary;
                        break;
                    }
                    case OptimizationMode::OM_DilationBoundary: {
                        optRegion = dilatedBoundary;
                        originalAlias = dilated;
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
                        DigitalSet originalPlusOptRegion (domain);
                        originalPlusOptRegion.insert(originalAlias.begin(),originalAlias.end());

                        DigitalSet ballReachDilation(domain);
                        DIPaCUS::Morphology::Dilate(ballReachDilation,originalPlusOptRegion,DIPaCUS::Morphology::RECT,radius);

                        DigitalSet dilationApplication(domain);
                        DIPaCUS::SetOperations::SetDifference(dilationApplication,ballReachDilation,originalPlusOptRegion);

                        applicationRegion.insert(dilationApplication.begin(),dilationApplication.end());


                        DigitalSet tempDS (domain);
                        DigitalSet erodedInLargerDomain (domain);
                        tempDS.insert(originalAlias.begin(),originalAlias.end());
                        DIPaCUS::SetOperations::SetDifference(erodedInLargerDomain,tempDS,optRegion);
                        
                        DigitalSet ballReachErosion(domain);
                        DIPaCUS::Morphology::Erode(ballReachErosion,erodedInLargerDomain,DIPaCUS::Morphology::RECT,radius);

                        DigitalSet erosionApplication(domain);
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