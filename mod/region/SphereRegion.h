//
// Created by OEOTYAN on 2021/2/8.
//

#ifndef WORLDEDIT_SPHEREREGION_H
#define WORLDEDIT_SPHEREREGION_H

#include "Region.h"

class SphereRegion : Region {
   public:
	BlockPos center;
	float radius;
	void updateBoundingBox();
	SphereRegion(const BoundingBox& region);
	bool setMainPos(const BlockPos& pos);
	bool setVicePos(const BlockPos& pos);
	bool isInRegion(const BlockPos& pos);
};

#endif	// WORLDEDIT_SPHEREREGION_H