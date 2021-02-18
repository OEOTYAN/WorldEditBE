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
                if (contains({x, y, z})) {
                    todo({x, y, z});
                }
            }
}

void Region::drawRegion() {
    spawnRectangleParticle(this->getBoundBox().toAABB(), GRAPHIC_COLOR::YELLOW,
                           dimensionID);
};

Region::Region(const trapdoor::BoundingBox& b, int dim)
    : boundingBox(b), dimensionID(dim) {}

Region* Region::createRegion(RegionType type, const BoundingBox& box, int dim) {
    switch (type) {
        case CUBOID:
            return new CuboidRegion(box, dim);
        case EXPAND:
            return new ExpandRegion(box, dim);
        case SPHERE:
            return new SphereRegion(box, dim);
        case POLY:  // NOLINT
            return new PolyRegion(box, dim);
        case CONVEX:
            return new ConvexRegion(box, dim);
        default:
            return new CuboidRegion(box, dim);
    }
}
