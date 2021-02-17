//
// Created by OEOTYAN on 2021/2/8.
//

#ifndef WORLDEDIT_REGION_H
#define WORLDEDIT_REGION_H

#include "graphics/AABB.h"
#include <functional>
#include <algorithm>
using namespace trapdoor;
enum RegionType {
    CUBOID = 0,
    EXPAND = 1,
    SPHERE = 2,
    POLY = 3,
    CONVEX = 4,
};

class Region {
   protected:
    RegionType regionType = CUBOID;
    BoundingBox boundingBox{};

   public:
    bool selecting = false;
    explicit Region(const trapdoor::BoundingBox& b);

    trapdoor::BoundingBox getBoundBox() const { return this->boundingBox; }
    RegionType getRegionType() const { return this->regionType; }

    virtual ~Region() = default;

    virtual void updateBoundingBox() = 0;

    // virtual void shift(const BlockPos& change) = 0;
    virtual bool setMainPos(const BlockPos& pos) = 0;

    virtual bool setVicePos(const BlockPos& pos) = 0;

    virtual bool isInRegion(const BlockPos& pos) { return true; };

    virtual void forEachBlockInRegion(
        const std::function<void(const BlockPos&)>& todo);

    inline bool hasSelected() { return this->selecting; }

    static Region* createRegion(RegionType type,
                                const trapdoor::BoundingBox& box);
};

#endif  // WORLDEDIT_REGION_H