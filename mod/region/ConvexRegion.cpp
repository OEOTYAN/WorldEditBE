//
// Created by OEOTYAN on 2021/2/8.
//

#include "ConvexRegion.h"
//
//void ConvexRegion::updateBoundingBox() {
//    for_each(points.begin(), points.end(), [&](BlockPos value) {
//        boundingBox.minPos.x = min(boundingBox.minPos.x, value.x);
//        boundingBox.minPos.y = min(boundingBox.minPos.y, value.y);
//        boundingBox.minPos.z = min(boundingBox.minPos.z, value.z);
//        boundingBox.maxPos.x = max(boundingBox.maxPos.x, value.x);
//        boundingBox.maxPos.y = max(boundingBox.maxPos.y, value.y);
//        boundingBox.maxPos.z = max(boundingBox.maxPos.z, value.z);
//    });
//}
//
//ConvexRegion::ConvexRegion(const BoundingBox &region)
//        : pos1(region.minPos),
//          pos2(region.maxPos),
//          boundingBox
//
//.
//maxPos(region
//.maxPos),
//boundingBox.
//minPos(region
//.minPos){
//};
//
//bool PolyRegion::setMainPos(const BlockPos &pos) {
//    selecting = 1;
//    while (points.size() != 0) {
//        points.pop_back();
//    }
//    points.push_back(pos);
//    updateBoundingBox();
//}
//
//bool PolyRegion::setVicePos(const BlockPos &pos) {
//    return false;
//}
//
//bool PolyRegion::isInRegion(const BlockPos &pos) {
//    return false;
//}