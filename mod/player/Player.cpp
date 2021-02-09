//
// Created by xhy on 2020/8/29.
//
#include <cstdint>
#include "entity/Actor.h"
#include "graphics/BlockPos.h"
#include "lib/mod.h"
#include "lib/SymHook.h"
#include "block/Block.h"
#include "BDSMod.h"
#include "WorldEditMod.h"
#include "Player.h"
#include "BlockSource.h"
#include "Message.h"
/*
// player place block
using namespace SymHook;

THook(
	int64_t,
	MSSYM_B1QE21onBlockPlacedByPlayerB1AE34VanillaServerGameplayEventListenerB2AAA4UEAAB1QE14AW4EventResultB2AAE10AEAVPlayerB2AAA9AEBVBlockB2AAE12AEBVBlockPosB3AAUA1NB1AA1Z,
	void* self,
	trapdoor::Actor* player,
	trapdoor::Block& block,
	const trapdoor::BlockPos& pos,
	bool flag) {
	return original(self, player, block, pos, flag);
}
*/
// player destroy block
using namespace SymHook;

THook(
        void,
        MSSYM_B2QUE20destroyBlockInternalB1AA8GameModeB2AAA4AEAAB1UE13NAEBVBlockPosB2AAA1EB1AA1Z,
        uint64_t *self,
        const trapdoor::BlockPos *pos,
        int64_t a3,
        int a4) {
    uint64_t *ptr = self + 1;
    auto player = reinterpret_cast<trapdoor::Actor *>(*ptr);
    // int slot = player->getPlayerInventory()->containsItem("Wooden Pickaxe");
    //  if (slot > -1) {
    auto modInstance = trapdoor::bdsMod->asInstance<WorldEditMod>();
    auto block = player->getBlockSource()->getBlock(pos->x, pos->y, pos->z);
    auto *region = modInstance->playerRegionCache[player->getNameTag()];
    if (!region)region = Region::createRegion(CUBOID, trapdoor::BoundingBox());
    region->setMainPos(*pos);
    player->getBlockSource()->setBlock(pos, block);
    trapdoor::info(player, "set point %d", pos->x, pos->y, pos->z);
    original(self, pos, a3, a4);
}
