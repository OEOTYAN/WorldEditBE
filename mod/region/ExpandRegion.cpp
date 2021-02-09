//
// Created by OEOTYAN on 2021/2/8.
//

#include "ExpandRegion.h"

void ExpandRegion::updateBoundingBox() {
    using std::max, std::min;
    boundingBox.minPos.x = min({pos1.x, pos2.x});
    boundingBox.minPos.y = min({pos1.y, pos2.y});
    boundingBox.minPos.z = min({pos1.z, pos2.z});
    boundingBox.maxPos.x = max({pos1.x, pos2.x});
    boundingBox.maxPos.y = max({pos1.y, pos2.y});
    boundingBox.maxPos.z = max({pos1.z, pos2.z});
}

bool ExpandRegion::setMainPos(const BlockPos &pos) {
    selecting = true;
    pos1 = pos;
    pos2 = pos;
    updateBoundingBox();
    return true;
}

bool ExpandRegion::setVicePos(const BlockPos &pos) {
    if (!selecting) {
        return false;
    }
    if (!pos2.containedWithin(boundingBox.minPos, boundingBox.maxPos)) {
        using std::min, std::max;
        pos1.x = min({pos1.x, pos.x});
        pos1.y = min({pos1.y, pos.y});
        pos1.z = min({pos1.z, pos.z});
        pos2.x = max({pos.x, pos2.x});
        pos2.y = max({pos.y, pos2.y});
        pos2.z = max({pos.z, pos2.z});
        updateBoundingBox();
        return true;
    }
    return false;
}

ExpandRegion::ExpandRegion(const BoundingBox &region) : Region(region), pos1(region.minPos), pos2(region.maxPos) {
    this->regionType = EXPAND;

}
