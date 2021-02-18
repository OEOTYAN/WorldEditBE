//
// Created by OEOTYAN on 2021/2/15.
//

#include "graphics/BlockPos.h"
#include "graphics/Particle.h"
#include "graphics/AABB.h"
#include "lib/mod.h"
#include "lib/SymHook.h"
#include "block/Block.h"
#include "BDSMod.h"
#include "WorldEditMod.h"
#include "BlockSource.h"
#include "Message.h"
#include "DirtyLogger.h"
#include "commands/Command.h"
#include "GameTick.h"

using namespace SymHook;
THook(void,
      MSSYM_B1QA4tickB1AE11ServerLevelB2AAA7UEAAXXZ,
      trapdoor::Level* serverLevel) {
    if (!trapdoor::bdsMod) {
        L_ERROR("mod is nullptr");
    }
    if (!trapdoor::bdsMod->getLevel()) {
        trapdoor::bdsMod->setLevel(serverLevel);
        trapdoor::bdsMod->asInstance<WorldEditMod>()->initialize();
    }
    auto modInstance = trapdoor::bdsMod->asInstance<WorldEditMod>();
    if (modInstance->boxDisplayTick % 40 == 0) {
        trapdoor::bdsMod->getLevel()->forEachPlayer(
            [&](trapdoor::Actor* player) {
                auto* region =
                    modInstance->playerRegionCache[player->getNameTag()];
                if (region && region->hasSelected()) {
                    auto dim = region->getDimensionID();
                    region->drawRegion();
                    auto lastPos =
                        modInstance->playerLastPosCache[player->getNameTag()];
                    spawnRectangleParticle(
                        {lastPos.toVec3() - Vec3(0.07f, 0.07f, 0.07f),
                         lastPos.toVec3() + Vec3(1.07f, 1.07f, 1.07f)},
                        GRAPHIC_COLOR::BLUE, dim);
                    auto mainPos =
                        modInstance->playerMainPosCache[player->getNameTag()];
                    spawnRectangleParticle(
                        {mainPos.toVec3() - Vec3(0.07f, 0.07f, 0.07f),
                         mainPos.toVec3() + Vec3(1.07f, 1.07f, 1.07f)},
                        GRAPHIC_COLOR::RED, dim);
                }
            });
    }
    modInstance->boxDisplayTick += 1;
    original(serverLevel);
}