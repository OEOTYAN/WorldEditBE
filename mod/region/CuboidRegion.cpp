//
// Created by OEOTYAN on 2021/2/8.
//

#include "CuboidRegion.h"

void CuboidRegion::updateBoundingBox() {
    boundingBox.minPos.x = std::min(mainPos.x, vicePos.x);
    boundingBox.minPos.y = std::min(mainPos.y, vicePos.y);
    boundingBox.minPos.z = std::min(mainPos.z, vicePos.z);
    boundingBox.maxPos.x = std::max(mainPos.x, vicePos.x);
    boundingBox.maxPos.y = std::max(mainPos.y, vicePos.y);
    boundingBox.maxPos.z = std::max(mainPos.z, vicePos.z);
}

bool CuboidRegion::setMainPos(const BlockPos& pos, const int& dim) {
    if (mainPos != pos) {
        if (!selecting) {
            dimensionID = dim;
            selecting = true;
            vicePos = pos;
        } else {
            if (dim != dimensionID)
                return false;
        }
        mainPos = pos;
        updateBoundingBox();
        return true;
    }
    return false;
}

bool CuboidRegion::setVicePos(const BlockPos& pos, const int& dim) {
    if (vicePos != pos) {
        if (!selecting) {
            dimensionID = dim;
            selecting = true;
            mainPos = pos;
        } else {
            if (dim != dimensionID)
                return false;
        }
        vicePos = pos;
        updateBoundingBox();
        return true;
    }
    return false;
}

void CuboidRegion::expand(const std::vector<BlockPos>& changes, Actor* player) {
    for (auto change : changes) {
        if (change.x > 0) {
            if (std::max(mainPos.x, vicePos.x) == mainPos.x) {
                mainPos = mainPos + BlockPos(change.x, 0, 0);
            } else {
                vicePos = vicePos + BlockPos(change.x, 0, 0);
            }
        } else {
            if (std::min(mainPos.x, vicePos.x) == mainPos.x) {
                mainPos = mainPos + BlockPos(change.x, 0, 0);
            } else {
                vicePos = vicePos + BlockPos(change.x, 0, 0);
            }
        }

        if (change.y > 0) {
            if (std::max(mainPos.y, vicePos.y) == mainPos.y) {
                mainPos = mainPos + BlockPos(0, change.y, 0);
            } else {
                vicePos = vicePos + BlockPos(0, change.y, 0);
            }
        } else {
            if (std::min(mainPos.y, vicePos.y) == mainPos.y) {
                mainPos = mainPos + BlockPos(0, change.y, 0);
            } else {
                vicePos = vicePos + BlockPos(0, change.y, 0);
            }
        }

        if (change.z > 0) {
            if (std::max(mainPos.z, vicePos.z) == mainPos.z) {
                mainPos = mainPos + BlockPos(0, 0, change.z);
            } else {
                vicePos = vicePos + BlockPos(0, 0, change.z);
            }
        } else {
            if (std::min(mainPos.z, vicePos.z) == mainPos.z) {
                mainPos = mainPos + BlockPos(0, 0, change.z);
            } else {
                vicePos = vicePos + BlockPos(0, 0, change.z);
            }
        }
    }

    updateBoundingBox();
}

void CuboidRegion::contract(const std::vector<BlockPos>& changes,
                            Actor* player) {
    for (auto change : changes) {
        if (change.x < 0) {
            if (std::max(mainPos.x, vicePos.x) == mainPos.x) {
                mainPos = mainPos + BlockPos(change.x, 0, 0);
            } else {
                vicePos = vicePos + BlockPos(change.x, 0, 0);
            }
        } else {
            if (std::min(mainPos.x, vicePos.x) == mainPos.x) {
                mainPos = mainPos + BlockPos(change.x, 0, 0);
            } else {
                vicePos = vicePos + BlockPos(change.x, 0, 0);
            }
        }

        if (change.y < 0) {
            if (std::max(mainPos.y, vicePos.y) == mainPos.y) {
                mainPos = mainPos + BlockPos(0, change.y, 0);
            } else {
                vicePos = vicePos + BlockPos(0, change.y, 0);
            }
        } else {
            if (std::min(mainPos.y, vicePos.y) == mainPos.y) {
                mainPos = mainPos + BlockPos(0, change.y, 0);
            } else {
                vicePos = vicePos + BlockPos(0, change.y, 0);
            }
        }

        if (change.z < 0) {
            if (std::max(mainPos.z, vicePos.z) == mainPos.z) {
                mainPos = mainPos + BlockPos(0, 0, change.z);
            } else {
                vicePos = vicePos + BlockPos(0, 0, change.z);
            }
        } else {
            if (std::min(mainPos.z, vicePos.z) == mainPos.z) {
                mainPos = mainPos + BlockPos(0, 0, change.z);
            } else {
                vicePos = vicePos + BlockPos(0, 0, change.z);
            }
        }
    }

    updateBoundingBox();
}

void CuboidRegion::shift(const BlockPos& change) {
    mainPos = mainPos + change;
    vicePos = vicePos + change;
    updateBoundingBox();
}

// void CuboidRegion::forEachBlockInRegion(
//        const std::function<void(const BlockPos &)> &todo) {
//    for (int x = mainPos.x; x <= vicePos.x; x++)
//        for (int y = mainPos.y; y <= vicePos.y; y++)
//            for (int z = mainPos.z; z <= vicePos.z; z++) {
//                if (contains({x, y, z})) {
//                    todo({x, y, z});
//                }
//            }
//}

CuboidRegion::CuboidRegion(const BoundingBox& region, const int& dim)
    : Region(region, dim) {
    this->regionType = CUBOID;
    this->mainPos = region.minPos;
    this->vicePos = region.maxPos;
}
