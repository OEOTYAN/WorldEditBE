//
// Created by OEOTYAN on 2021/2/8.
//

#include "CuboidRegion.h"

void CuboidRegion::updateBoundingBox() {
    boundingBox.minPos.x = std::min({mainPos.x, vicePos.x});
    boundingBox.minPos.y = std::min({mainPos.y, vicePos.y});
    boundingBox.minPos.z = std::min({mainPos.z, vicePos.z});
    boundingBox.maxPos.x = std::max({mainPos.x, vicePos.x});
    boundingBox.maxPos.y = std::max({mainPos.y, vicePos.y});
    boundingBox.maxPos.z = std::max({mainPos.z, vicePos.z});
}

bool CuboidRegion::setMainPos(const BlockPos& pos) {
    if (mainPos != pos) {
        mainPos = pos;
        if (!selecting) {
            selecting = true;
            vicePos = pos;
        }
        updateBoundingBox();
        return true;
    }
    return false;
}

bool CuboidRegion::setVicePos(const BlockPos& pos) {
    if (vicePos != pos) {
        vicePos = pos;
        if (!selecting) {
            selecting = true;
            mainPos = pos;
        }
        updateBoundingBox();
        return true;
    }
    return false;
}

// void CuboidRegion::forEachBlockInRegion(
//        const std::function<void(const BlockPos &)> &todo) {
//    for (int x = mainPos.x; x <= vicePos.x; x++)
//        for (int y = mainPos.y; y <= vicePos.y; y++)
//            for (int z = mainPos.z; z <= vicePos.z; z++) {
//                if (isInRegion({x, y, z})) {
//                    todo({x, y, z});
//                }
//            }
//}

CuboidRegion::CuboidRegion(const BoundingBox& region) : Region(region) {
    this->regionType = CUBOID;
    this->mainPos = region.minPos;
    this->vicePos = region.maxPos;
}
