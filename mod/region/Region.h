//
// Created by OEOTYAN on 2021/2/8.
//

#ifndef WORLDEDIT_REGION_H
#define WORLDEDIT_REGION_H

#include "graphics\AABB.h"
#include <functional>

using namespace trapdoor;
enum RegionType {
	CUBOID = 0,
	EXPAND = 1,
	SPHERE = 2,
	POLY = 3,
	CONVEX = 4,
};
class Region {
   public:
	bool selecting = 0;
	RegionType regionType = CUBOID;
	BoundingBox boundingBox = {{0, 0, 0}, {0, 0, 0}};
	virtual void updateBoundingBox();
	//virtual void shift(const BlockPos& change) = 0;
	virtual bool setMainPos(const BlockPos& pos) = 0;
	virtual bool setVicePos(const BlockPos& pos) = 0;
	virtual bool isInRegion(const BlockPos& pos) { return 1; };
	virtual void forEachBlockInRegion(
		const std::function<void(BlockPos*)>& todo);
};

#endif	// WORLDEDIT_REGION_H