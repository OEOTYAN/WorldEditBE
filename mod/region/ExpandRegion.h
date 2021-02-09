//
// Created by OEOTYAN on 2021/2/8.
//

#ifndef WORLDEDIT_EXPANDREGION_H
#define WORLDEDIT_EXPANDREGION_H

#include "Region.h"

class ExpandRegion : Region {
   public:
	BlockPos pos1;
	BlockPos pos2;
	void updateBoundingBox();
	ExpandRegion(const BoundingBox& region);
	bool setMainPos(const BlockPos& pos);
	bool setVicePos(const BlockPos& pos);
};

#endif	// WORLDEDIT_EXPANDREGION_H