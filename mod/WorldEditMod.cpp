//
// Created by xhy on 2021/2/3.
//

#include "WorldEditMod.h"
#include "tick/GameTick.h"
#include "Message.h"
#include "trapdoor.h"
#include "BlockSource.h"

using namespace trapdoor;

void WorldEditMod::useOnHook(Actor* player,
                             const std::string& itemName,
                             BlockPos& pos,
                             unsigned int facing,
                             const Vec3&) {
    if (player->getSelectedItem()->getId() == 271) {
        auto* region = this->playerRegionCache[player->getNameTag()];
        if (region && region->setVicePos(pos, player->getDimensionID())) {
            trapdoor::info(player, "§6于 (%d,%d,%d) 设置副点", pos.x, pos.y,
                           pos.z);
            this->boxDisplayTick = 0;
            this->playerLastPosCache[player->getNameTag()] = pos;
        } else {
            trapdoor::error(player, "副点设置失败");
        }
    }
}

bool WorldEditMod::attackEntityHook(Actor* player, Actor* entity) {
    return true;
}

void WorldEditMod::registerCommands() {
    this->commandManager.setEnablePermissionCheck(false);
    BDSMod::registerCommands();
    this->commandManager
        .registerCmd("/sel", "设置选区", Any, ArgType::STR)
        ->EXE({
            auto regionID = holder->getString();
            auto region = this->playerRegionCache[player->getNameTag()];
            auto box = trapdoor::BoundingBox();
            auto dim = player->getDimensionID();
            if (regionID == "cuboid") {
                this->playerRegionCache[player->getNameTag()] =
                    Region::createRegion(CUBOID, box, dim);
                trapdoor::info(player, "§6选区切换为长方体");
            } else if (regionID == "expand") {
                this->playerRegionCache[player->getNameTag()] =
                    Region::createRegion(EXPAND, box, dim);
                trapdoor::info(player, "§6选区切换为扩展");
            } else if (regionID == "sphere") {
                this->playerRegionCache[player->getNameTag()] =
                    Region::createRegion(SPHERE, box, dim);
                trapdoor::info(player, "§6选区切换为物体");
            } else if (regionID == "poly") {
                this->playerRegionCache[player->getNameTag()] =
                    Region::createRegion(POLY, box, dim);
                trapdoor::info(player, "§6选区切换为多边形");
            } else if (regionID == "convex") {
                this->playerRegionCache[player->getNameTag()] =
                    Region::createRegion(CONVEX, box, dim);
                trapdoor::info(player, "§6选区切换为多面体");
            } else if (regionID == "clear") {
                this->playerRegionCache[player->getNameTag()]->selecting =
                    false;
                trapdoor::info(player, "§6选区已清空");
            } else {
                trapdoor::error(player, "不存在子命令 %s", regionID.c_str());
            }
            delete region;
        });
    this->commandManager
        .registerCmd("/set", "放置 <方块/图案>", Any, ArgType::INT)
        ->EXE({
            auto blockID = holder->getInt();
            auto block = getBlockByID((BlockType)blockID);
            auto* region = this->playerRegionCache[player->getNameTag()];
            int64_t num = 0;
            if (region && region->hasSelected()) {
                if (region->getDimensionID() != player->getDimensionID()) {
                    trapdoor::error(player, "请到选区对应的维度");
                } else {
                    region->forEachBlockInRegion([&](const BlockPos& pos) {
                        player->getBlockSource()->setBlock(&pos, block);
                        num += 1;
                    }); /*
                     if (num <= 1)
                         trapdoor::evalMsg("§6%lld block has changed", num);
                     else*/
                    trapdoor::evalMsg("§6%lld个方块已更改", num);
                }
            } else {
                trapdoor::error(player, "请先划定选区");
            }
        }

        );
    this->commandManager.registerCmd("/pos2", "设置副点")->EXE({
        auto pos = player->getStandPosition();
        auto* region = this->playerRegionCache[player->getNameTag()];
        if (region && region->setVicePos(pos, player->getDimensionID())) {
            trapdoor::info(player, "§6于 (%d,%d,%d) 设置副点", pos.x, pos.y,
                           pos.z);
            this->boxDisplayTick = 0;
            this->playerLastPosCache[player->getNameTag()] = pos;
        } else {
            trapdoor::error(player, "副点设置失败");
        }
    });
    this->commandManager.registerCmd("/pos1", "设置主点")->EXE({
        auto pos = player->getStandPosition();
        auto* region = this->playerRegionCache[player->getNameTag()];
        if (region && region->setMainPos(pos, player->getDimensionID())) {
            trapdoor::info(player, "§6于 (%d,%d,%d) 设置主点", pos.x, pos.y,
                           pos.z);
            this->boxDisplayTick = 0;
            this->playerMainPosCache[player->getNameTag()] = pos;
            this->playerLastPosCache.erase(player->getNameTag());
        } else {
            trapdoor::error(player, "主点设置失败");
        }
    });
}

void WorldEditMod::initialize() {
    BDSMod::initialize();
    initBlockMap();
}
