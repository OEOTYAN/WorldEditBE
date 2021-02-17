//
// Created by OEOTYAN on 2021/2/8.
//

#ifndef WORLDEDIT_POLYREGION_H
#define WORLDEDIT_POLYREGION_H

#include "Region.h"
#include <vector>
#include <algorithm>

using namespace trapdoor;

class PolyRegion : public Region {
   public:
    std::vector<BlockPos2> points;
    int minY;
    int maxY;
    BlockPos mainPos;

    void updateBoundingBox() override;

    explicit PolyRegion(const BoundingBox& region);

    bool setMainPos(const BlockPos& pos) override;

    bool setVicePos(const BlockPos& pos) override;

    bool isInRegion(const BlockPos& pos) override;
};

#endif  // WORLDEDIT_POLYREGION_H