#include "SCaBOliC/Core/ODRUtils.h"

using namespace SCaBOliC::Core;

ODRUtils::DigitalSet ODRUtils::omFullDomain(const Domain& originalDomain)
{
    DigitalSet fullDomain(originalDomain);
    fullDomain.insert(originalDomain.begin(),originalDomain.end());

    return fullDomain;
}


ODRUtils::DigitalSet ODRUtils::amFullDomain(const Domain& applicationDomain)
{
    return omFullDomain(applicationDomain);
}

ODRUtils::DigitalSet ODRUtils::computeForeground(const DigitalSet& original,
                                                 const DigitalSet& optRegion,
                                                 OptimizationMode om)
{
    DigitalSet extendedOriginal(original.domain());
    extendedOriginal.insert(original.begin(),original.end());
    extendedOriginal.insert(optRegion.begin(),optRegion.end());

    DigitalSet trustFRG(optRegion.domain());
    DIPaCUS::SetOperations::setDifference(trustFRG, extendedOriginal, optRegion);

    if(om==OptimizationMode::OM_DilationBoundary)
    {
        DigitalSet isolatedDS = isolatedPoints(original, optRegion);
        trustFRG += isolatedDS;
    }
    return trustFRG;
}

ODRUtils::DigitalSet ODRUtils::computeBackground(const DigitalSet& trustFRG,
                                                 const DigitalSet& optRegion)
{
    DigitalSet trustBKG(trustFRG.domain());
    DigitalSet tempp(trustFRG.domain());

    tempp += trustFRG;
    tempp += optRegion;

    trustBKG.assignFromComplement(tempp);
    return trustBKG;
}

ODRUtils::DigitalSet ODRUtils::amAroundBoundary(const DigitalSet& original,
                                                const DigitalSet& optRegion,
                                                const unsigned int radius,
                                                const LevelDefinition ld,
                                                const StructuringElement::Type st,
                                                int length)
{
    DigitalSet internRegion = amInternRange(original,optRegion,radius,ld,st,length);
    DigitalSet externRegion = amExternRange(original,optRegion,radius,ld,st,length);


    DigitalSet aroundBoundary(original.domain());

    aroundBoundary.insert(internRegion.begin(),internRegion.end());
    aroundBoundary.insert(externRegion.begin(),externRegion.end());

    return aroundBoundary;
}

ODRUtils::DigitalSet ODRUtils::amInternRange(const DigitalSet& original,
                                             const DigitalSet& optRegion,
                                             const unsigned int radius,
                                             const LevelDefinition ld,
                                             const StructuringElement::Type st,
                                             int length)
{
    DigitalSet originalPlusOptRegion(original.domain());
    originalPlusOptRegion.insert(original.begin(),original.end());
    originalPlusOptRegion.insert(optRegion.begin(),optRegion.end());


    DigitalSet lengthEroded (originalPlusOptRegion.domain());



    DigitalSet internRegion(originalPlusOptRegion.domain());
    if(ld==LevelDefinition::LD_CloserFromCenter)
    {
        DIPaCUS::Morphology::erode(lengthEroded,originalPlusOptRegion,StructuringElement(st,length+1));
        DIPaCUS::SetOperations::setDifference(internRegion,originalPlusOptRegion,lengthEroded);
    }else if(ld==LevelDefinition::LD_FartherFromCenter)
    {
        DIPaCUS::Morphology::erode(lengthEroded,originalPlusOptRegion,StructuringElement(st,(radius-length)+1));

        DigitalSet radiusEroded (originalPlusOptRegion.domain());
        DIPaCUS::Morphology::erode(radiusEroded,originalPlusOptRegion,StructuringElement(st,radius+1));
        DIPaCUS::SetOperations::setDifference(internRegion,lengthEroded,radiusEroded);
    }

    for(auto it=optRegion.begin();it!=optRegion.end();++it)
    {
        internRegion.erase(*it);
    }


    return internRegion;
}

ODRUtils::DigitalSet ODRUtils::amExternRange(const DigitalSet& original,
                                             const DigitalSet& optRegion,
                                             const unsigned int radius,
                                             const LevelDefinition ld,
                                             const StructuringElement::Type st,
                                             int length)
{
    DigitalSet originalPlusOptRegion(original.domain());
    originalPlusOptRegion.insert(original.begin(),original.end());
    originalPlusOptRegion.insert(optRegion.begin(),optRegion.end());

    DigitalSet lengthDilated (originalPlusOptRegion.domain());


    DigitalSet externRegion(originalPlusOptRegion.domain());
    if(ld==LevelDefinition::LD_CloserFromCenter)
    {
        DIPaCUS::Morphology::dilate(lengthDilated,originalPlusOptRegion,StructuringElement(st,length));
        DIPaCUS::SetOperations::setDifference(externRegion,lengthDilated,originalPlusOptRegion);
    }else if(ld==LevelDefinition::LD_FartherFromCenter)
    {
        DIPaCUS::Morphology::dilate(lengthDilated,originalPlusOptRegion,StructuringElement(st,(radius-length) ));
        DigitalSet radiusDilated (originalPlusOptRegion.domain());
        DIPaCUS::Morphology::dilate(radiusDilated,originalPlusOptRegion,StructuringElement(st,radius));

        DIPaCUS::SetOperations::setDifference(externRegion,radiusDilated,lengthDilated);
    }

    for(auto it=optRegion.begin();it!=optRegion.end();++it)
    {
        externRegion.erase(*it);
    }


    return externRegion;
}

ODRUtils::DigitalSet ODRUtils::isolatedPoints(const DigitalSet& original,
                                              const DigitalSet& optRegion)
{
    DigitalSet dilated(original.domain());

    DIPaCUS::Morphology::dilate(dilated,
                                original,
                                StructuringElement(StructuringElement::RECT,1));

    DigitalSet tempDS(original.domain());
    DigitalSet isolatedDS(original.domain());
    DIPaCUS::SetOperations::setDifference(tempDS,dilated,original);
    DIPaCUS::SetOperations::setDifference(isolatedDS,tempDS,optRegion);

    return isolatedDS;

}