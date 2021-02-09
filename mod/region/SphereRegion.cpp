//
// Created by OEOTYAN on 2021/2/8.
//

#include "SphereRegion.h"

void SphereRegion::updateBoundingBox() {
	boundingBox.minPos.x = center.x - (int)(radius + 0.5);
	boundingBox.minPos.y = max(center.y - (int)(radius + 0.5), 0);
	boundingBox.minPos.z = center.z - (int)(radius + 0.5);
	boundingBox.maxPos.x = center.x + (int)(radius + 0.5);
	boundingBox.maxPos.y = min(center.y + (int)(radius + 0.5), 255);
	boundingBox.maxPos.z = center.z + (int)(radius + 0.5);
}
SphereRegion::SphereRegion(const BoundingBox& region)
	: pos1(region.minPos),
	  pos2(region.maxPos),
	  boundingBox.maxPos(region.maxPos),
	  boundingBox.minPos(region.minPos){};
bool SphereRegion::setMainPos(const BlockPos& pos) {
	selecting = 1;
	center = pos;
	updateBoundingBox();
	return true;
}
bool SphereRegion::setVicePos(const BlockPos& pos) {
	if (!selecting) {
		return false;
	}
	float dis = pos2.distanceTo(center);
	if (dis > radius) {
		radius = dis + 0.5;
		updateBoundingBox();
		return true;
	}
	return false;
}
bool SphereRegion::isInRegion(const BlockPos& pos) {
	return pos.distanceTo(center) <= radius;
}