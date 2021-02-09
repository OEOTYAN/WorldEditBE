//
// Created by OEOTYAN on 2021/2/8.
//

#ifndef WORLDEDIT_EXPANDREGION_H
#define WORLDEDIT_EXPANDREGION_H

#include "Region.h"

class ExpandRegion : public Region {
    BlockPos pos1;
    BlockPos pos2;
public:
    void updateBoundingBox() override;

    explicit ExpandRegion(const BoundingBox &region);

    bool setMainPos(const BlockPos &pos) override;

    bool setVicePos(const BlockPos &pos) override;
};

#endif    // WORLDEDIT_EXPANDREGION_H