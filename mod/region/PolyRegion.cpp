//
// Created by OEOTYAN on 2021/2/8.
//

#include "PolyRegion.h"
//
//void PolyRegion::updateBoundingBox() {
//	boundingBox.minPos.y = minY;
//	boundingBox.maxPos.y = maxY;
//	for_each(points.begin(), points.end(), [&](BlockPos2 value) {
//		boundingBox.minPos.x = min(boundingBox.minPos.x, value.x);
//		boundingBox.minPos.z = min(boundingBox.minPos.z, value.z);
//		boundingBox.maxPos.x = max(boundingBox.maxPos.x, value.x);
//		boundingBox.maxPos.z = max(boundingBox.maxPos.z, value.z);
//	});
//}
//PolyRegion::PolyRegion(const BoundingBox& region)
//	: pos1(region.minPos),
//	  pos2(region.maxPos),
//	  boundingBox.maxPos(region.maxPos),
//	  boundingBox.minPos(region.minPos){};
//bool PolyRegion::setMainPos(const BlockPos& pos) {
//	selecting = 1;
//	minY = pos.y;
//	maxY = pos.y;
//	while (points.size() != 0) {
//		points.pop_back();
//	}
//	points.push_back({pos.x, pos.z});
//	updateBoundingBox();
//}
//bool PolyRegion::setVicePos(const BlockPos& pos) {
//	if (!selecting) {
//		return false;
//	}
//	BlockPos2 lastPoint = points.back();
//	if (lastPoint != {pos.x, pos.z}) {
//		minY = min(minY, pos.y);
//		maxY = max(maxY, pos.y);
//		points.push_back({pos.x, pos.z});
//		updateBoundingBox();
//		return true;
//	}
//	return false;
//}
//bool PolyRegion::isInRegion(const BlockPos& pos) {
//	if (points.size() < 3 || pos.y < minY || pos.y > maxY) {
//		return false;
//	}
//	bool inside = false;
//	int x1;
//	int z1;
//	int x2;
//	int z2;
//	long long crossproduct;
//	BlockPos2 point = points.back();
//	for_each(points.begin(), points.end(), [&](BlockPos2 value) {
//		if (value == pos)
//			return true;
//		if (value.x > point.x) {
//			x1 = point.x;
//			x2 = value.x;
//			z1 = point.z;
//			z2 = value.z;
//		} else {
//			x1 = value.x;
//			x2 = point.x;
//			z1 = value.z;
//			z2 = point.z;
//		}
//		if (x1 <= pos.x && pos.x <= x2) {
//			crossproduct =
//				((long long)pos.z - (long long)z1) * (long long)(x2 - x1) -
//				((long long)z2 - (long long)z1) * (long long)(pos.x - x1);
//			if (crossproduct == 0) {
//				if ((z1 <= pos.z) == (pos.z <= z2)) {
//					return true;
//				}
//			} else if (crossproduct < 0 && (x1 != pos.x)) {
//				inside = !inside;
//			}
//		}
//		point.x = value.x;
//		point.z = value.z;
//	});
//	return inside;
//}