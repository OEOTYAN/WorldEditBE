//
// Created by OEOTYAN on 2021/2/8.
//

#include "SphereRegion.h"

void SphereRegion::updateBoundingBox() {
    auto newRadius = (int)(radius);
    boundingBox.minPos.x = center.x - newRadius;
    boundingBox.minPos.y = std::max({center.y - newRadius, 0});
    boundingBox.minPos.z = center.z - newRadius;
    boundingBox.maxPos.x = center.x + newRadius;
    boundingBox.maxPos.y = std::min({center.y + newRadius, 255});
    boundingBox.maxPos.z = center.z + newRadius;
}

bool SphereRegion::setMainPos(const BlockPos& pos, const int& dim) {
    selecting = true;
    dimensionID = dim;
    center = pos;
    radius = 0.5;
    updateBoundingBox();
    return true;
}

bool SphereRegion::setVicePos(const BlockPos& pos, const int& dim) {
    if (!selecting || dim != dimensionID) {
        return false;
    }
    float dis = pos.distanceTo(center);
    if (dis > radius) {
        radius = dis + 0.5f;
        updateBoundingBox();
        return true;
    }
    return false;
}

bool SphereRegion::isInRegion(const BlockPos& pos) {
    return pos.distanceTo(center) <= radius;
}

SphereRegion::SphereRegion(const BoundingBox& region, const int& dim)
    : Region(region, dim) {
    this->selecting = false;
    this->regionType = SPHERE;
}
