//
// Created by OEOTYAN on 2021/2/8.
//

#ifndef WORLDEDIT_CUBOIDREGION_H
#define WORLDEDIT_CUBOIDREGION_H

#include "Region.h"

class CuboidRegion : Region {
   public:
	BlockPos pos1;
	BlockPos pos2;
	void updateBoundingBox();
	CuboidRegion(const BoundingBox& region);
	bool setMainPos(const BlockPos& pos);
	bool setVicePos(const BlockPos& pos);
	void forEachBlockInRegion(const std::function<void(BlockPos*)>& todo);
};

#endif	// WORLDEDIT_CUBOIDREGION_H