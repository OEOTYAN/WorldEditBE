//
// Created by OEOTYAN on 2021/2/8.
//

#ifndef WORLDEDIT_SPHEREREGION_H
#define WORLDEDIT_SPHEREREGION_H

#include "Region.h"

class SphereRegion : public Region {
    BlockPos center = {0, -65, 0};
    float radius = 0.5;

   public:
    void updateBoundingBox() override;

    explicit SphereRegion(const BoundingBox& region, const int& dim);

    bool setMainPos(const BlockPos& pos, const int& dim) override;

    BlockPos getCenter() const { return center; };

    void drawRegion() override;

    float getRadius() const { return radius; };

    bool setVicePos(const BlockPos& pos, const int& dim) override;

    bool isInRegion(const BlockPos& pos) override;
};

#endif  // WORLDEDIT_SPHEREREGION_H