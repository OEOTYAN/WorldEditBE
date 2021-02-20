//
// Created by OEOTYAN on 2021/2/8.
//

#ifndef WORLDEDIT_CUBOIDREGION_H
#define WORLDEDIT_CUBOIDREGION_H

#include "Region.h"

class CuboidRegion : public Region {
   public:
    BlockPos mainPos;
    BlockPos vicePos;
    void updateBoundingBox() override;

    explicit CuboidRegion(const BoundingBox& region, const int& dim);

    void expand(const std::vector<BlockPos>& changes, Actor* player) override;

    void contract(const std::vector<BlockPos>& changes, Actor* player) override;

    void shift(const BlockPos& change) override;

    bool setMainPos(const BlockPos& pos, const int& dim) override;

    bool setVicePos(const BlockPos& pos, const int& dim) override;

    // void forEachBlockInRegion(const std::function<void(const BlockPos &)>
    // &todo) override;

    Region* createRegion(RegionType type,
                         trapdoor::BoundingBox boundingBox,
                         int dim);
};

#endif  // WORLDEDIT_CUBOIDREGION_H