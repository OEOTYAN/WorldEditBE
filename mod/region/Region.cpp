//
// Created by OEOTYAN on 2021/2/8.
//

#include "Region.h"

void Region::forEachBlockInRegion(const std::function<void(BlockPos*)>& todo) {
	for (int x = boundingBox.minPos.x; x <= boundingBox.maxPos.x; x++)
		for (int y = boundingBox.minPos.y; y <= boundingBox.maxPos.y; y++)
			for (int z = boundingBox.minPos.z; z <= boundingBox.maxPos.z; z++) {
				if (isInRegion({x, y, z})) {
					todo({x, y, z});
				}
			}
}