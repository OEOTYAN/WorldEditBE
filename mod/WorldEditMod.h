//
// Created by xhy on 2021/2/3.
//

#ifndef MOD_WORLDEDIT_H
#define MOD_WORLDEDIT_H

#include "BDSMod.h"

using namespace trapdoor;

class WorldEditMod : public trapdoor::BDSMod {
	void useOnHook(Actor* player,
				   const std::string& itemName,
				   BlockPos& pos,
				   unsigned int facing,
				   const Vec3&) override;

	bool attackEntityHook(Actor* player, Actor* entity) override;

	void registerCommands() override;
};

#endif	// MOD_WORLDEDIT_H
