//
// Created by OEOTYAN on 2021/2/8.
//

#include "SphereRegion.h"

void SphereRegion::updateBoundingBox() {
    auto newRadius = (int)(radius);
    boundingBox.minPos.x = center.x - newRadius;
    boundingBox.minPos.y = std::max(center.y - newRadius, 0);
    boundingBox.minPos.z = center.z - newRadius;
    boundingBox.maxPos.x = center.x + newRadius;
    boundingBox.maxPos.y = std::min(center.y + newRadius, 255);
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

int SphereRegion::checkChanges(const std::vector<BlockPos>& changes) {
    BlockPos tmp(0);
    for (auto change : changes) {
        tmp = tmp + trapdoor::abs(change);
    }
    if (std::abs(tmp.x) == std::abs(tmp.y) &&
        std::abs(tmp.y) == std::abs(tmp.z)) {
        return std::abs(tmp.x) / 2;
    }
    return -1;
}

void SphereRegion::expand(const std::vector<BlockPos>& changes, Actor* player) {
    int check = checkChanges(changes);
    if (check == -1) {
        trapdoor::error(player, "该选区只可各向同性扩展");
    } else {
        radius += check;
        updateBoundingBox();
    }
}

void SphereRegion::contract(const std::vector<BlockPos>& changes,
                            Actor* player) {
    int check = checkChanges(changes);
    if (check == -1) {
        trapdoor::error(player, "该选区只可各向同性收缩");
    } else {
        radius += check;
        updateBoundingBox();
    }
}

void SphereRegion::shift(const BlockPos& change) {
    center = center + change;
    updateBoundingBox();
}

bool SphereRegion::contains(const BlockPos& pos) {
    return pos.distanceTo(center) <= radius;
}

SphereRegion::SphereRegion(const BoundingBox& region, const int& dim)
    : Region(region, dim) {
    this->selecting = false;
    this->regionType = SPHERE;
}
void SphereRegion::drawRegion() {
    spawnRectangleParticle({center.toVec3(), center.toVec3() + Vec3(1.0f)},
                           GRAPHIC_COLOR::GREEN, dimensionID);
};