//
// Created by OEOTYAN on 2021/2/8.
//

#ifndef WORLDEDIT_CUBOIDREGION_H
#define WORLDEDIT_CUBOIDREGION_H

#include "Region.h"

class CuboidRegion : public Region {
private:
    BlockPos mainPos;
    BlockPos vicePos;
public:
    void updateBoundingBox() override;

    explicit CuboidRegion(const BoundingBox &region);

    bool setMainPos(const BlockPos &pos) override;

    bool setVicePos(const BlockPos &pos) override;

    //void forEachBlockInRegion(const std::function<void(const BlockPos &)> &todo) override;

    Region *createRegion(RegionType type, trapdoor::BoundingBox boundingBox);
};

#endif    // WORLDEDIT_CUBOIDREGION_H