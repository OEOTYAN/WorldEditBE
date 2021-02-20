//
// Created by OEOTYAN on 2021/2/8.
//

#ifndef WORLDEDIT_EXPANDREGION_H
#define WORLDEDIT_EXPANDREGION_H

#include "Region.h"

class ExpandRegion : public Region {
   public:
    BlockPos mainPos;
    BlockPos vicePos;
    void updateBoundingBox() override;

    explicit ExpandRegion(const BoundingBox& region, const int& dim);

    void expand(const std::vector<BlockPos>& changes, Actor* player) override;

    void contract(const std::vector<BlockPos>& changes, Actor* player) override;

    void shift(const BlockPos& change) override;

    bool setMainPos(const BlockPos& pos, const int& dim) override;

    bool setVicePos(const BlockPos& pos, const int& dim) override;
};

#endif  // WORLDEDIT_EXPANDREGION_H