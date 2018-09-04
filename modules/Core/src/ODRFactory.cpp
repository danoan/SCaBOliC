#include "SCaBOliC//Core/ODRFactory.h"

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



                EightNeighborhood(originalBoundary, original);

                DIPaCUS::Morphology::Dilate(dilated,
                                            original,
                                            DIPaCUS::Morphology::RECT,
                                            1);

                EightNeighborhood(dilatedBoundary, dilated);

                DIPaCUS::Morphology::Erode(eroded,
                                           original,
                                           DIPaCUS::Morphology::RECT,
                                           1);
                EightNeighborhood(erodedBoundary, eroded);

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
                        applicationRegion = originalBoundary;
                        break;
                    }
                    case ApplicationMode::AM_DilatedBoundary: {
                        applicationRegion = dilatedBoundary;
                        break;
                    }
                    case ApplicationMode::AM_FullImage: {
                        applicationRegion.assignFromComplement(emptySet);
                        break;
                    }
                    case ApplicationMode::AM_AroundBoundary: {
                        applicationRegion = dilatedBoundary;
                        applicationRegion += erodedBoundary;

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