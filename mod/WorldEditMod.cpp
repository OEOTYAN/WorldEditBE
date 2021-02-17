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
    // L_INFO("use on %s\n", itemName.c_str());
    bool isWand = false;
    auto item = player->getSelectedItem();
    if (item->getId() == 271)
        isWand = true;
    if (isWand) {
        auto* region = this->playerRegionCache[player->getNameTag()];
        if (region && region->setVicePos(pos, player->getDimensionID())) {
            trapdoor::info(player, "set point 2 %d %d %d", pos.x, pos.y, pos.z);
            this->boxDisplayTick = 0;
            this->playerLastPosCache[player->getNameTag()] = pos;
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
            auto box = trapdoor::BoundingBox();
            auto dim = player->getDimensionID();
            if (regionID == "cuboid") {
                this->playerRegionCache[player->getNameTag()] =
                    Region::createRegion(CUBOID, box, dim);
                trapdoor::info(player, "§6region switch to cuboid");
            } else if (regionID == "expand") {
                this->playerRegionCache[player->getNameTag()] =
                    Region::createRegion(EXPAND, box, dim);
                trapdoor::info(player, "§6region switch to expand");
            } else if (regionID == "sphere") {
                this->playerRegionCache[player->getNameTag()] =
                    Region::createRegion(SPHERE, box, dim);
                trapdoor::info(player, "§6region switch to sphere");
            } else if (regionID == "poly") {
                this->playerRegionCache[player->getNameTag()] =
                    Region::createRegion(POLY, box, dim);
                trapdoor::info(player, "§6region switch to poly");
            } else if (regionID == "convex") {
                this->playerRegionCache[player->getNameTag()] =
                    Region::createRegion(CONVEX, box, dim);
                trapdoor::info(player, "§6region switch to convex");
            } else if (regionID == "clear") {
                this->playerRegionCache[player->getNameTag()]->selecting =
                    false;
                trapdoor::info(player, "§6region clear");
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
            int64_t num = 0;
            if (region && region->hasSelected()) {
                if (region->getDimensionID() != player->getDimensionID()) {
                    trapdoor::error(player, "请到选区对应的维度");
                } else {
                    region->forEachBlockInRegion([&](const BlockPos& pos) {
                        player->getBlockSource()->setBlock(&pos, block);
                        num += 1;
                    });
                    if (num <= 1)
                        trapdoor::evalMsg("§6%lld block has changed", num);
                    else
                        trapdoor::evalMsg("§6%lld blocks had changed", num);
                }
            } else {
                trapdoor::error(player, "请先划定选区");
            }
        }

        );
    this->commandManager.registerCmd("/pos2", "设置点2")->EXE({
        auto pos = player->getStandPosition();
        auto* region = this->playerRegionCache[player->getNameTag()];
        if (region && region->setVicePos(pos, player->getDimensionID())) {
            trapdoor::info(player, "§6set point 2 %d %d %d", pos.x, pos.y,
                           pos.z);
            this->boxDisplayTick = 0;
            this->playerLastPosCache[player->getNameTag()] = pos;
        } else {
            trapdoor::error(player, "fail to set point2");
        }
    });
    this->commandManager.registerCmd("/pos1", "设置点1")->EXE({
        auto pos = player->getStandPosition();
        auto* region = this->playerRegionCache[player->getNameTag()];
        if (region && region->setMainPos(pos, player->getDimensionID())) {
            trapdoor::info(player, "§6set point1 %d %d %d", pos.x, pos.y,
                           pos.z);
            this->boxDisplayTick = 0;
            this->playerMainPosCache[player->getNameTag()] = pos;
        } else {
            trapdoor::error(player, "fail to set point1");
        }
    });
}

void WorldEditMod::initialize() {
    BDSMod::initialize();
    initBlockMap();
}
