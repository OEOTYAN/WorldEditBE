//
// Created by OEOTYAN on 2021/2/8.
//

#ifndef WORLDEDIT_SPHEREREGION_H
#define WORLDEDIT_SPHEREREGION_H

#include "Region.h"

class SphereRegion : public Region {
   private:
    BlockPos center = {0, -65, 0};
    float radius = 0.5;
    int checkChanges(const std::vector<BlockPos>& changes);

   public:
    void updateBoundingBox() override;

    explicit SphereRegion(const BoundingBox& region, const int& dim);

    void expand(const std::vector<BlockPos>& changes, Actor* player) override;

    void contract(const std::vector<BlockPos>& changes, Actor* player) override;

    void shift(const BlockPos& change) override;

    bool setMainPos(const BlockPos& pos, const int& dim) override;

    Vec3 getCenter() const override { return center.toVec3() + Vec3(0.5f); };

    void drawRegion() override;

    float getRadius() const { return radius; };

    bool setVicePos(const BlockPos& pos, const int& dim) override;

    bool contains(const BlockPos& pos) override;
};

#endif  // WORLDEDIT_SPHEREREGION_H