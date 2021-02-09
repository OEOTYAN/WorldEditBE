//
// Created by OEOTYAN on 2021/2/8.
//

#include "CuboidRegion.h"

void CuboidRegion::updateBoundingBox() {
	boundingBox.minPos.x = min(pos1.x, pos2.x);
	boundingBox.minPos.y = min(pos1.y, pos2.y);
	boundingBox.minPos.z = min(pos1.z, pos2.z);
	boundingBox.maxPos.x = max(pos1.x, pos2.x);
	boundingBox.maxPos.y = max(pos1.y, pos2.y);
	boundingBox.maxPos.z = max(pos1.z, pos2.z);
}
CuboidRegion::CuboidRegion(const BoundingBox& region)
	: pos1(region.minPos),
	  pos2(region.maxPos),
	  boundingBox.maxPos(region.maxPos),
	  boundingBox.minPos(region.minPos){};
bool CuboidRegion::setMainPos(const BlockPos& pos) {
	if (pos1 != pos) {
		pos1 = pos;
		if (!selecting) {
			selecting = 1;
			pos2 = pos;
		}
		updateBoundingBox();
		return true;
	}
	return false;
}
bool CuboidRegion::setVicePos(const BlockPos& pos) {
	if (pos2 != pos) {
		pos2 = pos;
		if (!selecting) {
			selecting = 1;
			pos1 = pos;
		}
		updateBoundingBox();
		return true;
	}
	return false;
}
void CuboidRegion::forEachBlockInRegion(
	const std::function<void(BlockPos*)>& todo) {
	for (int x = pos1.x; x <= pos2.x; x++)
		for (int y = pos1.y; y <= pos2.y; y++)
			for (int z = pos1.z; z <= pos2.z; z++) {
				if (isInRegion({x, y, z})) {
					todo({x, y, z});
				}
			}
}