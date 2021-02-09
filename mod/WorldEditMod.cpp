//
// Created by xhy on 2021/2/3.
//

#include "WorldEditMod.h"
#include "Message.h"

using namespace trapdoor;

void WorldEditMod::useOnHook(Actor* player,
							 const std::string& itemName,
							 BlockPos& pos,
							 unsigned int facing,
							 const Vec3&) {
	if (itemName == "Wooden Pickaxe")
		(this->playerRegionCache[player->getNameTag()]).setVicePos(pos);
}

bool WorldEditMod::attackEntityHook(Actor* player, Actor* entity) {
	return true;
}

void WorldEditMod::registerCommands() {
	this->commandManager.setEnablePermissionCheck(false);
	BDSMod::registerCommands();
	this->commandManager.registerCmd("here", "send your info")->EXE({
		auto pos = player->getPos()->toBlockPos();
		trapdoor::broadcastMsg("%s @[%d %d %d] ", player->getNameTag().c_str(),
							   pos.x, pos.y, pos.z);
	});
	this->commandManager
		.registerCmd("/sel", "select region", Any, ArgType::STR)
		->EXE({
			auto regionID = holder->getString();
			if (regionID == "cuboid") {
				this->playerRegionTypeCache[player->getNameTag()] = CUBOID;
				this->playerRegionCache[player->getNameTag()] = CuboidRegion(
					this->playerRegionCache[player->getNameTag()].boundingBox);
			} else if (regionID == "expand") {
				this->playerRegionTypeCache[player->getNameTag()] = EXPAND;
				this->playerRegionCache[player->getNameTag()] = ExpandRegion(
					this->playerRegionCache[player->getNameTag()].boundingBox);
			} else if (regionID == "sphere") {
				this->playerRegionTypeCache[player->getNameTag()] = SPHERE;
				this->playerRegionCache[player->getNameTag()] = SphereRegion();
			} else if (regionID == "poly") {
				this->playerRegionTypeCache[player->getNameTag()] = POLY;
				this->playerRegionCache[player->getNameTag()] = PolyRegion();
			} /*else if (regionID == "convex") {
				this->playerRegionTypeCache[player->getNameTag()] = CONVEX;
				this->playerRegionCache[player->getNameTag()] = ConvexRegion();
			}*/
		});
	this->commandManager.registerCmd("/set", "set block", Any, ArgType::INT)
		->EXE({
			auto blockID = holder->getInt();
			auto block = getBlockByID((BlockType)blockID);
			(this->playerRegionCache[player->getNameTag()])
				.forEachBlockInRegion([&](BlockPos val) {
					player->getBlockSource()->setBlock(val, block);
				});
		});
}
