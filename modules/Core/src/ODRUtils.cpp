#include "SCaBOliC/Core/ODRUtils.h"

using namespace SCaBOliC::Core;

ODRUtils::DigitalSet ODRUtils::computeForeground(const Domain& domain,
                                                 const DigitalSet& original,
                                                 const DigitalSet& optRegion,
                                                 OptimizationMode om)
{
    DigitalSet extendedOriginal(original.domain());
    extendedOriginal.insert(original.begin(),original.end());
    extendedOriginal.insert(optRegion.begin(),optRegion.end());

    DigitalSet trustFRG(optRegion.domain());
    DIPaCUS::SetOperations::setDifference(trustFRG, extendedOriginal, optRegion);

    if(om==OptimizationMode::OM_CorrectConcavities)
    {
        DigitalSet isolatedDS = isolatedPoints(domain,original, optRegion);
        trustFRG += isolatedDS;
    }
    return trustFRG;
}

ODRUtils::DigitalSet ODRUtils::computeBackground(const Domain& domain,
                                                 const DigitalSet& trustFRG,
                                                 const DigitalSet& optRegion)
{
    DigitalSet trustBKG(domain);
    DigitalSet tempp(domain);

    tempp += trustFRG;
    tempp += optRegion;

    trustBKG.assignFromComplement(tempp);
    return trustBKG;
}

ODRUtils::DigitalSet ODRUtils::amAroundBoundary(const Domain& domain,
                                                const DigitalSet& original,
                                                const DigitalSet& optRegion,
                                                const unsigned int radius,
                                                const LevelDefinition ld,
                                                const StructuringElement::Type st,
                                                int length)
{
    DigitalSet internRegion = amInternRange(domain,original,optRegion,radius,ld,st,length);
    DigitalSet externRegion = amExternRange(domain,original,optRegion,radius,ld,st,length);


    DigitalSet aroundBoundary(domain);

    aroundBoundary.insert(internRegion.begin(),internRegion.end());
    aroundBoundary.insert(externRegion.begin(),externRegion.end());

    return aroundBoundary;
}

ODRUtils::DigitalSet ODRUtils::amInternRange(const Domain& domain,
                                             const DigitalSet& original,
                                             const DigitalSet& optRegion,
                                             const unsigned int radius,
                                             const LevelDefinition ld,
                                             const StructuringElement::Type st,
                                             int length)
{
    DigitalSet originalPlusOptRegion(domain);
    originalPlusOptRegion.insert(original.begin(),original.end());
    originalPlusOptRegion.insert(optRegion.begin(),optRegion.end());


    DigitalSet lengthEroded (domain);



    DigitalSet internRegion(domain);
    if(ld==LevelDefinition::LD_CloserFromCenter)
    {
        DIPaCUS::Morphology::erode(lengthEroded,originalPlusOptRegion,StructuringElement(st,length+1));
        DIPaCUS::SetOperations::setDifference(internRegion,originalPlusOptRegion,lengthEroded);
    }else if(ld==LevelDefinition::LD_FartherFromCenter)
    {
        DIPaCUS::Morphology::erode(lengthEroded,originalPlusOptRegion,StructuringElement(st,(radius-length)+1));

        DigitalSet radiusEroded (domain);
        DIPaCUS::Morphology::erode(radiusEroded,originalPlusOptRegion,StructuringElement(st,radius+1));
        DIPaCUS::SetOperations::setDifference(internRegion,lengthEroded,radiusEroded);
    }

    for(auto it=optRegion.begin();it!=optRegion.end();++it)
    {
        internRegion.erase(*it);
    }


    return internRegion;
}

ODRUtils::DigitalSet ODRUtils::amExternRange(const Domain& domain,
                                             const DigitalSet& original,
                                             const DigitalSet& optRegion,
                                             const unsigned int radius,
                                             const LevelDefinition ld,
                                             const StructuringElement::Type st,
                                             int length)
{
    DigitalSet originalPlusOptRegion(domain);
    originalPlusOptRegion.insert(original.begin(),original.end());
    originalPlusOptRegion.insert(optRegion.begin(),optRegion.end());

    DigitalSet lengthDilated (domain);


    DigitalSet externRegion(domain);
    if(ld==LevelDefinition::LD_CloserFromCenter)
    {
        DIPaCUS::Morphology::dilate(lengthDilated,originalPlusOptRegion,StructuringElement(st,length));
        DIPaCUS::SetOperations::setDifference(externRegion,lengthDilated,originalPlusOptRegion);
    }else if(ld==LevelDefinition::LD_FartherFromCenter)
    {
        DIPaCUS::Morphology::dilate(lengthDilated,originalPlusOptRegion,StructuringElement(st,(radius-length) ));
        DigitalSet radiusDilated (domain);
        DIPaCUS::Morphology::dilate(radiusDilated,originalPlusOptRegion,StructuringElement(st,radius));

        DIPaCUS::SetOperations::setDifference(externRegion,radiusDilated,lengthDilated);
    }

    for(auto it=optRegion.begin();it!=optRegion.end();++it)
    {
        externRegion.erase(*it);
    }


    return externRegion;
}

ODRUtils::DigitalSet ODRUtils::isolatedPoints(const Domain& domain,
                                              const DigitalSet& original,
                                              const DigitalSet& optRegion)
{
    DigitalSet dilated(domain);

    DIPaCUS::Morphology::dilate(dilated,
                                original,
                                StructuringElement(StructuringElement::RECT,1));

    DigitalSet tempDS(domain);
    DigitalSet isolatedDS(domain);
    DIPaCUS::SetOperations::setDifference(tempDS,dilated,original);
    DIPaCUS::SetOperations::setDifference(isolatedDS,tempDS,optRegion);

    return isolatedDS;

}