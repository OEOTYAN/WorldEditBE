//
// Created by OEOTYAN on 2021/2/8.
//

#ifndef WORLDEDIT_EXPANDREGION_H
#define WORLDEDIT_EXPANDREGION_H

#include "Region.h"

class ExpandRegion : public Region {
   public:
    BlockPos pos1;
    BlockPos pos2 = {0, -65, 0};
    BlockPos mainPos;
    void updateBoundingBox() override;

    explicit ExpandRegion(const BoundingBox& region, const int& dim);

    bool setMainPos(const BlockPos& pos, const int& dim) override;

    bool setVicePos(const BlockPos& pos, const int& dim) override;
};

#endif  // WORLDEDIT_EXPANDREGION_H