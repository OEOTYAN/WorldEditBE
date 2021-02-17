//
// Created by OEOTYAN on 2021/2/8.
//

#include "ExpandRegion.h"

void ExpandRegion::updateBoundingBox() {
    boundingBox.minPos.x = std::min({pos1.x, pos2.x});
    boundingBox.minPos.y = std::min({pos1.y, pos2.y});
    boundingBox.minPos.z = std::min({pos1.z, pos2.z});
    boundingBox.maxPos.x = std::max({pos1.x, pos2.x});
    boundingBox.maxPos.y = std::max({pos1.y, pos2.y});
    boundingBox.maxPos.z = std::max({pos1.z, pos2.z});
}

bool ExpandRegion::setMainPos(const BlockPos& pos) {
    selecting = true;
    pos1 = pos;
    pos2 = pos;
    mainPos = pos;
    updateBoundingBox();
    return true;
}

bool ExpandRegion::setVicePos(const BlockPos& pos) {
    if (!selecting) {
        return false;
    }
    if (!pos.containedWithin(boundingBox.minPos, boundingBox.maxPos)) {
        pos1.x = std::min(pos1.x, pos.x);
        pos1.y = std::min(pos1.y, pos.y);
        pos1.z = std::min(pos1.z, pos.z);
        pos2.x = std::max(pos.x, pos2.x);
        pos2.y = std::max(pos.y, pos2.y);
        pos2.z = std::max(pos.z, pos2.z);
        updateBoundingBox();
        return true;
    }
    return false;
}

ExpandRegion::ExpandRegion(const BoundingBox& region)
    : Region(region), pos1(region.minPos), pos2(region.maxPos) {
    this->regionType = EXPAND;
}
