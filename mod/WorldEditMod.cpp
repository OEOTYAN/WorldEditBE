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
							 const Vec3&) {}

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
}
