//
// Created by OEOTYAN on 2021/2/8.
//

#include "ExpandRegion.h"

void ExpandRegion::updateBoundingBox() {
    boundingBox.minPos.x = std::min(mainPos.x, vicePos.x);
    boundingBox.minPos.y = std::min(mainPos.y, vicePos.y);
    boundingBox.minPos.z = std::min(mainPos.z, vicePos.z);
    boundingBox.maxPos.x = std::max(mainPos.x, vicePos.x);
    boundingBox.maxPos.y = std::max(mainPos.y, vicePos.y);
    boundingBox.maxPos.z = std::max(mainPos.z, vicePos.z);
}

bool ExpandRegion::setMainPos(const BlockPos& pos, const int& dim) {
    selecting = true;
    dimensionID = dim;
    mainPos = pos;
    vicePos = pos;
    updateBoundingBox();
    return true;
}

bool ExpandRegion::setVicePos(const BlockPos& pos, const int& dim) {
    if (!selecting || dim != dimensionID) {
        return false;
    }
    if (!pos.containedWithin(boundingBox.minPos, boundingBox.maxPos)) {
        mainPos.x = std::min(mainPos.x, pos.x);
        mainPos.y = std::min(mainPos.y, pos.y);
        mainPos.z = std::min(mainPos.z, pos.z);
        vicePos.x = std::max(pos.x, vicePos.x);
        vicePos.y = std::max(pos.y, vicePos.y);
        vicePos.z = std::max(pos.z, vicePos.z);
        updateBoundingBox();
        return true;
    }
    return false;
}
void ExpandRegion::expand(const std::vector<BlockPos>& changes, Actor* player) {
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

void ExpandRegion::contract(const std::vector<BlockPos>& changes,
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

void ExpandRegion::shift(const BlockPos& change) {
    mainPos = mainPos + change;
    vicePos = vicePos + change;
    updateBoundingBox();
}

ExpandRegion::ExpandRegion(const BoundingBox& region, const int& dim)
    : Region(region, dim), mainPos(region.minPos), vicePos(region.maxPos) {
    this->regionType = EXPAND;
}
