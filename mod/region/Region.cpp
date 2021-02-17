//
// Created by OEOTYAN on 2021/2/8.
//

#include "Region.h"
#include "Regions.h"

void Region::forEachBlockInRegion(
    const std::function<void(const BlockPos&)>& todo) {
    for (int x = boundingBox.minPos.x; x <= boundingBox.maxPos.x; x++)
        for (int y = boundingBox.minPos.y; y <= boundingBox.maxPos.y; y++)
            for (int z = boundingBox.minPos.z; z <= boundingBox.maxPos.z; z++) {
                if (isInRegion({x, y, z})) {
                    todo({x, y, z});
                }
            }
}

Region::Region(const trapdoor::BoundingBox& b) : boundingBox(b) {}

Region* Region::createRegion(RegionType type, const BoundingBox& box) {
    switch (type) {
        case CUBOID:
            return new CuboidRegion(box);
        case EXPAND:
            return new ExpandRegion(box);
        case SPHERE:
            return new SphereRegion(box);
        case POLY:  // NOLINT
            return new PolyRegion(box);
        case CONVEX:
            return new ConvexRegion(box);
        default:
            return new CuboidRegion(box);
    }
}
