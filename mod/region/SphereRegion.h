//
// Created by OEOTYAN on 2021/2/8.
//

#ifndef WORLDEDIT_SPHEREREGION_H
#define WORLDEDIT_SPHEREREGION_H

#include "Region.h"

class SphereRegion : public Region {
    BlockPos center;

   public:
    float radius{};

    void updateBoundingBox() override;

    explicit SphereRegion(const BoundingBox& region);

    bool setMainPos(const BlockPos& pos) override;

    BlockPos getCenter() { return center; };

    bool setVicePos(const BlockPos& pos) override;

    bool isInRegion(const BlockPos& pos) override;
};

#endif  // WORLDEDIT_SPHEREREGION_H