//
// Created by xhy on 2021/2/3.
//

#include "WorldEditMod.h"
#include "tick/GameTick.h"
#include "tools/StringTool.h"
#include "Message.h"
#include "trapdoor.h"
#include "BlockSource.h"
#include "CommandNode.h"
#include "CommandManager.h"
#include <map>

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

    this->commandManager.registerCmd("/sel", "设置选区", Any, ArgType::STR)
        ->EXE({
            auto subCommand = holder->getString();
            trapdoor::stringReplace(subCommand, " ", "");
            auto region = this->playerRegionCache[player->getNameTag()];
            auto box = trapdoor::BoundingBox();
            auto dim = player->getDimensionID();

            if (subCommand == "cuboid") {
                this->playerRegionCache[player->getNameTag()] =
                    Region::createRegion(CUBOID, box, dim);
                trapdoor::info(player, "§6选区切换为长方体");
            } else if (subCommand == "expand") {
                this->playerRegionCache[player->getNameTag()] =
                    Region::createRegion(EXPAND, box, dim);
                trapdoor::info(player, "§6选区切换为扩展");
            } else if (subCommand == "sphere") {
                this->playerRegionCache[player->getNameTag()] =
                    Region::createRegion(SPHERE, box, dim);
                trapdoor::info(player, "§6选区切换为球体");
            } else if (subCommand == "poly") {
                this->playerRegionCache[player->getNameTag()] =
                    Region::createRegion(POLY, box, dim);
                trapdoor::info(player, "§6选区切换为多边形");
            } else if (subCommand == "convex") {
                this->playerRegionCache[player->getNameTag()] =
                    Region::createRegion(CONVEX, box, dim);
                trapdoor::info(player, "§6选区切换为多面体");
            } else if (subCommand == "clear") {
                this->playerRegionCache[player->getNameTag()]->selecting =
                    false;
                trapdoor::info(player, "§6选区已清空");
            } else {
                trapdoor::error(player, "不存在子命令 %s", subCommand.c_str());
            }
            // delete region;
        });

    this->commandManager
        .registerCmd("/set", "放置 <方块/图案>", Any, ArgType::STR)
        ->EXE({
            auto subCommand = holder->getString();
            trapdoor::Block* block;
            int blockID;
            std::string tmpSubCommand(subCommand);
            trapdoor::stringReplace(tmpSubCommand, " ", "");
            if (trapdoor::isValidIntString(tmpSubCommand)) {
                blockID = strtol(tmpSubCommand.c_str(), nullptr, 10);
                block = getBlockByID((BlockType)blockID);
            } else {
                block = getBlockByName(tmpSubCommand);
            }
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
                trapdoor::error(player, "选区为空");
            }
        });

    this->commandManager
        .registerCmd("/size", "查询选区大小", Any, ArgType::STR)
        ->EXE({
            std::string subCommand = holder->getString();
            DEFFLAG(c, clipboardInfo)
            DETECT_FLAG_ERROR
            auto* region = this->playerRegionCache[player->getNameTag()];
            if (region && !clipboardInfo) {
                if (region->getDimensionID() != player->getDimensionID()) {
                    trapdoor::error(player, "请到选区对应的维度");
                } else {
                    BlockPos volume = region->getBoundBox().maxPos -
                                      region->getBoundBox().minPos +
                                      BlockPos(1);
                    trapdoor::info(player, "选区包围盒大小为 ( %d, %d, %d )",
                                   volume.x, volume.y, volume.z);
                    trapdoor::info(
                        player, "选区包围盒对角线长度为 %0.2f",
                        region->getBoundBox().maxPos.distanceTo(
                            region->getBoundBox().minPos + BlockPos(1)));
                    trapdoor::info(player, "选区体积为 %d 方块",
                                   region->size());
                }
            } else {
                if (clipboardInfo) {
                    if (false /*有无剪贴板*/) {
                        //以后写剪贴板
                    } else {
                        trapdoor::error(player, "剪贴板为空");
                    }
                } else {
                    trapdoor::error(player, "选区为空");
                }
            }
            trapdoor::stringReplace(subCommand, " ", "");
            if (subCommand != "") {
                trapdoor::error(player, "不存在子命令 %s", subCommand.c_str());
            }
        });

    this->commandManager
        .registerCmd("/count", "统计选区内指定方块数量", Any, ArgType::STR)
        ->EXE({
            auto subCommand = holder->getString();
            DETECT_FLAG_ERROR
            std::string tmpSubCommand(subCommand);
            trapdoor::stringReplace(tmpSubCommand, " ", "");
            if (tmpSubCommand == "") {
                trapdoor::error(player, "缺少子命令");
                return;
            }
            int blockID = -1;
            int state = -1;
            std::string blockName = "";
            std::vector<std::string> tokens = stringToTokens(subCommand);
            if (tokens.size() > 2) {
                trapdoor::error(player, "不存在子命令 %s", tokens[2].c_str());
                return;
            }
            if (trapdoor::isValidIntString(tokens[0])) {
                blockID = strtol(tokens[0].c_str(), nullptr, 10);
            } else {
                blockName = tokens[0];
            }
            if (tokens.size() > 1 && trapdoor ::isValidIntString(tokens[1])) {
                state = strtol(tokens[0].c_str(), nullptr, 10);
            } else {
                if (tokens.size() > 1) {
                    trapdoor::error(player, "不存在子命令 %s",
                                    tokens[1].c_str());
                    return;
                }
            }
            auto* region = this->playerRegionCache[player->getNameTag()];
            int64_t num = 0;
            if (region && region->hasSelected()) {
                if (region->getDimensionID() != player->getDimensionID()) {
                    trapdoor::error(player, "请到选区对应的维度");
                } else {  // bug?
                    region->forEachBlockInRegion([&](const BlockPos& pos) {
                        auto block = player->getBlockSource()->getBlock(pos);
                        if (((blockName != "" &&
                              blockName == block->getName()) ||
                             (blockID != -1 &&
                              (BlockType)blockID ==
                                  block->getLegacy()->getBlockID())) &&
                            (state == -1 || state == block->getVariant()))
                            num += 1;
                    });
                    trapdoor::evalMsg("数量: %d", num);
                }
            } else {
                trapdoor::error(player, "选区为空");
            }
        });

    this->commandManager
        .registerCmd("/distr", "统计选区内方块分布率", Any, ArgType::STR)
        ->EXE({
            this->distr.clear();
            auto subCommand = holder->getString();
            DEFFLAG(c, clipboardInfo)
            DEFFLAG(d, separateStates)
            DETECT_FLAG_ERROR
            trapdoor::stringReplace(subCommand, " ", "");
            if (subCommand != "") {
                trapdoor::error(player, "不存在子命令 %s", subCommand.c_str());
                return;
            }
            auto* region = this->playerRegionCache[player->getNameTag()];
            long long total = 0;
            if (region && region->hasSelected() && !clipboardInfo) {
                if (region->getDimensionID() != player->getDimensionID()) {
                    trapdoor::error(player, "请到选区对应的维度");
                } else {  // bug?
                    region->forEachBlockInRegion([&](const BlockPos& pos) {
                        auto block = player->getBlockSource()->getBlock(pos);
                        auto blockName = block->getName();
                        if (separateStates) {
                            blockName = blockName + ":" +
                                        std::to_string(block->getVariant());
                        }
                        if (this->distr.find(blockName) != this->distr.end()) {
                            this->distr[blockName] += 1;
                        } else {
                            this->distr[blockName] = 1;
                        }
                        total += 1;
                    });

                    trapdoor::info(player, "方块总数: %lld", total);
                    this->tmpDistr.clear();
                    for (auto iter = this->distr.begin();
                         iter != this->distr.end(); iter++) {
                        this->tmpDistr.push_back(*iter);
                    }
                    sort(this->tmpDistr.begin(), this->tmpDistr.end(),
                         tmpDistrCmp);
                    for (auto iter = this->tmpDistr.begin();
                         iter != this->tmpDistr.end(); iter++) {
                        trapdoor::info(
                            player, "%lld    (%0.3f%%%%) %s", iter->second,
                            iter->second * 100.0 / total, iter->first.c_str());
                    }
                }
            } else {
                if (clipboardInfo) {
                    if (false /*有无剪贴板*/) {
                        //以后写剪贴板
                    } else {
                        trapdoor::error(player, "剪贴板为空");
                    }
                } else {
                    trapdoor::error(player, "选区为空");
                }
            }
            this->distr.clear();
            this->tmpDistr.clear();
        });

    this->commandManager.registerCmd("/shift", "移动选区", Any, ArgType::STR)
        ->EXE({
            auto subCommand = holder->getString();
            DETECT_FLAG_ERROR
            std::string tmpSubCommand(subCommand);
            trapdoor::stringReplace(tmpSubCommand, " ", "");
            if (tmpSubCommand == "") {
                trapdoor::error(player, "缺少子命令");
                return;
            }
            int amount = 0;
            std::vector<std::string> tokens = stringToTokens(subCommand);
            if (tokens.size() > 2) {
                trapdoor::error(player, "不存在子命令 %s", tokens[2].c_str());
                return;
            }
            if (trapdoor::isValidIntString(tokens[0])) {
                amount = strtol(tokens[0].c_str(), nullptr, 10);
            } else {
                trapdoor::error(player, "不存在子命令 %s", tokens[0].c_str());
            }
            if (tokens.size() > 1) {
            }
            auto* region = this->playerRegionCache[player->getNameTag()];
            if (region && region->hasSelected()) {
                if (region->getDimensionID() != player->getDimensionID()) {
                    trapdoor::error(player, "请到选区对应的维度");
                } else {
                    BlockPos change =
                        player->getViewActor().toDirection() * amount;
                    region->shift(change);
                    trapdoor::evalMsg("选区已移动");
                }
            } else {
                trapdoor::error(player, "选区为空");
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

    this->commandManager.registerCmd("/pos2", "设置副点")->EXE({
        auto pos = player->getStandPosition();
        auto* region = this->playerRegionCache[player->getNameTag()];
        if (region && region->setVicePos(pos, player->getDimensionID())) {
            trapdoor::info(player, "§6于 ( %d, %d, %d ) 设置副点", pos.x, pos.y,
                           pos.z);
            this->boxDisplayTick = 0;
            this->playerLastPosCache[player->getNameTag()] = pos;
        } else {
            trapdoor::error(player, "副点设置失败");
        }
    });
}

void WorldEditMod::initialize() {
    BDSMod::initialize();
    initBlockMap();
}
