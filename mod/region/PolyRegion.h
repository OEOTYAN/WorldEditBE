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

    explicit PolyRegion(const BoundingBox& region, const int& dim);

    void expand(const std::vector<BlockPos>& changes, Actor* player) override;

    void contract(const std::vector<BlockPos>& changes, Actor* player) override;

    void shift(const BlockPos& change) override;

    void drawRegion() override;

    bool setMainPos(const BlockPos& pos, const int& dim) override;

    bool setVicePos(const BlockPos& pos, const int& dim) override;

    bool contains(const BlockPos& pos) override;
};

#endif  // WORLDEDIT_POLYREGION_H