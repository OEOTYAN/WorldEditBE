//
// Created by OEOTYAN on 2021/2/8.
//

#ifndef WORLDEDIT_SPHEREREGION_H
#define WORLDEDIT_SPHEREREGION_H

#include "Region.h"

class SphereRegion : public Region {
public:
    BlockPos center;
    float radius{};

    void updateBoundingBox() override;

    explicit SphereRegion(const BoundingBox &region);

    bool setMainPos(const BlockPos &pos) override;

    bool setVicePos(const BlockPos &pos) override;

    bool isInRegion(const BlockPos &pos);
};

#endif    // WORLDEDIT_SPHEREREGION_H