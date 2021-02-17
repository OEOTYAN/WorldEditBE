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
    printf("use on %s\n", itemName.c_str());
    if (itemName == "Cactus") {
        auto* region = this->playerRegionCache[player->getNameTag()];
        this->playerLastPosCache[player->getNameTag()] = pos;
        if (!region)
            region = Region::createRegion(CUBOID, trapdoor::BoundingBox());
        if (region->setVicePos(pos)) {
            trapdoor::info(player, "set point 2 %d %d %d", pos.x, pos.y, pos.z);
            this->boxDisplayTick = 0;
        } else {
            trapdoor::error(player, "fail to set point2");
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
        .registerCmd("/sel", "select region", Any, ArgType::STR)
        ->EXE({
            auto regionID = holder->getString();
            auto region = this->playerRegionCache[player->getNameTag()];
            auto box = region ? region->getBoundBox() : trapdoor::BoundingBox();
            if (regionID == "cuboid") {
                this->playerRegionCache[player->getNameTag()] =
                    Region::createRegion(CUBOID, box);
                trapdoor::info(player, "region switch to cuboid");
            } else if (regionID == "expand") {
                this->playerRegionCache[player->getNameTag()] =
                    Region::createRegion(EXPAND, box);
                trapdoor::info(player, "region switch to expand");
            } else if (regionID == "sphere") {
                this->playerRegionCache[player->getNameTag()] =
                    Region::createRegion(SPHERE, box);
                trapdoor::info(player, "region switch to sphere");
            } else if (regionID == "poly") {
                this->playerRegionCache[player->getNameTag()] =
                    Region::createRegion(POLY, box);
                trapdoor::info(player, "region switch to poly");
            } else if (regionID == "convex") {
                this->playerRegionCache[player->getNameTag()] =
                    Region::createRegion(CONVEX, box);
                trapdoor::info(player, "region switch to convex");
            } else if (regionID == "clear") {
                this->playerRegionCache[player->getNameTag()]->selecting =
                    false;
                trapdoor::info(player, "region switch to convex");
            } else {
                trapdoor::error(player, "error");
            }
            delete region;
        });
    this->commandManager.registerCmd("/set", "set block", Any, ArgType::INT)
        ->EXE({
            auto blockID = holder->getInt();
            auto block = getBlockByID((BlockType)blockID);
            auto* region = this->playerRegionCache[player->getNameTag()];
            if (region && region->hasSelected()) {
                region->forEachBlockInRegion([&](const BlockPos& pos) {
                    player->getBlockSource()->setBlock(&pos, block);
                });
            } else {
                trapdoor::error(player, "请先划定选区");
            }
        }

        );
    this->commandManager.registerCmd("/pos2", "设置点2")->EXE({
        auto pos = player->getStandPosition();
        this->playerLastPosCache[player->getNameTag()] = pos;
        auto* region = this->playerRegionCache[player->getNameTag()];
        if (!region)
            region = Region::createRegion(CUBOID, trapdoor::BoundingBox());
        if (region->setVicePos(pos)) {
            trapdoor::info(player, "set point 2 %d %d %d", pos.x, pos.y, pos.z);
            this->boxDisplayTick = 0;
        } else {
            trapdoor::error(player, "fail to set point2");
        }
    });
    this->commandManager.registerCmd("/pos1", "设置点1")->EXE({
        auto pos = player->getStandPosition();
        this->playerMainPosCache[player->getNameTag()] = pos;
        auto* region = this->playerRegionCache[player->getNameTag()];
        if (!region)
            region = Region::createRegion(CUBOID, trapdoor::BoundingBox());
        if (region->setMainPos(pos)) {
            trapdoor::info(player, "set point1 %d %d %d", pos.x, pos.y, pos.z);
            this->boxDisplayTick = 0;
        } else {
            trapdoor::error(player, "fail to set point1");
        }
    });
}

void WorldEditMod::initialize() {
    BDSMod::initialize();
    initBlockMap();
}
