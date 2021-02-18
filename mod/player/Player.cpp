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
#include "entity/Item.h"
#include "Dirtylogger.h"
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
using namespace SymHook;
THook(
    int64_t,
    MSSYM_B1QE14onPlayerJoinedB1AE16ServerScoreboardB2AAE15UEAAXAEBVPlayerB3AAAA1Z,
    int64_t a1,
    trapdoor::Actor* player) {
    auto modInstance = trapdoor::bdsMod->asInstance<WorldEditMod>();
    auto* region = modInstance->playerRegionCache[player->getNameTag()];
    if (!region) {
        modInstance->playerRegionCache[player->getNameTag()] =
            Region::createRegion(CUBOID, trapdoor::BoundingBox(),
                                 player->getDimensionID());
    }
    return original(a1, player);
}
// player destroy block
THook(
    void,
    MSSYM_B2QUE20destroyBlockInternalB1AA8GameModeB2AAA4AEAAB1UE13NAEBVBlockPosB2AAA1EB1AA1Z,
    uint64_t* self,
    const trapdoor::BlockPos* pos,
    int64_t a3,
    int a4) {
    uint64_t* ptr = self + 1;
    auto player = reinterpret_cast<trapdoor::Actor*>(*ptr);
    // int slot = player->getPlayerInventory()->containsItem("Wooden Pickaxe");
    //  if (slot > -1) {
    auto item = player->getSelectedItem();
    if (item->getId() == 271) {
        auto modInstance = trapdoor::bdsMod->asInstance<WorldEditMod>();
        auto block = player->getBlockSource()->getBlock(pos->x, pos->y, pos->z);
        original(self, pos, a3, a4);
        player->getBlockSource()->setBlock(pos, block);
        auto* region = modInstance->playerRegionCache[player->getNameTag()];
        if (region && region->setMainPos(*pos, player->getDimensionID())) {
            trapdoor::info(player, "于 (%d,%d,%d) 设置主点", pos->x, pos->y,
                           pos->z);
            modInstance->boxDisplayTick = 0;
            modInstance->playerMainPosCache[player->getNameTag()] = *pos;
            modInstance->playerLastPosCache.erase(player->getNameTag());
        } else {
            trapdoor::error(player, "主点设置失败");
        }
    } else {
        original(self, pos, a3, a4);
    }
}
THook(
    void,
    MSSYM_B1QA5useOnB1AA4ItemB2AAA4QEBAB1UE14NAEAVItemStackB2AAA9AEAVActorB2AAA7HHHEMMMB1AA1Z,
    void* item,
    trapdoor::ItemStackBase* itemStack,
    trapdoor::Actor* player,
    int x,
    int y,
    int z,
    unsigned int facing,
    float dx,
    float dy,
    float dz) {
    uint64_t gameTick = player->getLevel()->getGameTick();
    trapdoor::RightClickCache targetCache{gameTick, x, y, z};

    auto& playerCache =
        trapdoor::bdsMod->getPlayerBuffer()[player->getNameTag()]
            .rightClickCache;
    //下面用一个简单的缓存 + 判定消除重复点击
    if (playerCache != targetCache) {
        //响应右键事件
        trapdoor::BlockPos pos(x, y, z);
        const trapdoor::Vec3 vec3(dx, dy, dz);
        trapdoor::bdsMod->useOnHook(player, itemStack->getItemName(), pos,
                                    facing, vec3);
        playerCache = targetCache;
    }
    original(item, itemStack, player, x, y, z, facing, dx, dy, dz);
}