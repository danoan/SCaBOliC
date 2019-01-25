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

ODRUtils::DigitalSet ODRUtils::amAroundBoundary(const DigitalSet& original,
                                                const DigitalSet& optRegion,
                                                StructuringElement::Type st,
                                                int length)
{
    DigitalSet internRegion = amInternRange(original,optRegion,st,length);
    DigitalSet externRegion = amExternRange(original,optRegion,st,length);

    for(auto it=optRegion.begin();it!=optRegion.end();++it)
    {
        internRegion.erase(*it);
        externRegion.erase(*it);
    }

    DigitalSet aroundBoundary(original.domain());

    aroundBoundary.insert(externRegion.begin(),externRegion.end());
    aroundBoundary.insert(internRegion.begin(),internRegion.end());

    return aroundBoundary;
}

ODRUtils::DigitalSet ODRUtils::amInternRange(const DigitalSet& original,
                                             const DigitalSet& optRegion,
                                             const StructuringElement::Type st,
                                             int length)
{
    DigitalSet originalPlusOptRegion(original.domain());
    originalPlusOptRegion.insert(original.begin(),original.end());
    originalPlusOptRegion.insert(optRegion.begin(),optRegion.end());

    DigitalSet eroded (originalPlusOptRegion.domain());
    DIPaCUS::Morphology::erode(eroded,originalPlusOptRegion,StructuringElement(st,length+1));

    DigitalSet internRegion(originalPlusOptRegion.domain());
    DIPaCUS::SetOperations::setDifference(internRegion,originalPlusOptRegion,eroded);

    return internRegion;
}

ODRUtils::DigitalSet ODRUtils::amExternRange(const DigitalSet& original,
                                             const DigitalSet& optRegion,
                                             const StructuringElement::Type st,
                                             int length)
{
    DigitalSet originalPlusOptRegion(original.domain());
    originalPlusOptRegion.insert(original.begin(),original.end());
    originalPlusOptRegion.insert(optRegion.begin(),optRegion.end());

    DigitalSet dilated (originalPlusOptRegion.domain());
    DIPaCUS::Morphology::dilate(dilated,originalPlusOptRegion,StructuringElement(st,length));

    DigitalSet externRegion(originalPlusOptRegion.domain());
    DIPaCUS::SetOperations::setDifference(externRegion,dilated,originalPlusOptRegion);

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