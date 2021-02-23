//
// Created by xhy on 2021/2/3.
//

#ifndef MOD_WORLDEDIT_H
#define MOD_WORLDEDIT_H

#include "BDSMod.h"
#include "region/Regions.h"
#include "player/Player.h"

using namespace trapdoor;

class WorldEditMod : public trapdoor::BDSMod {
   public:
    std::map<std::string, Region*> playerRegionCache;
    std::map<std::string, BlockPos> playerLastPosCache;
    std::map<std::string, BlockPos> playerMainPosCache;
    std::map<std::string, long long> distr;
    std::vector<std::pair<std::string, long long>> tmpDistr;
    bool static tmpDistrCmp(const std::pair<std::string, long long>& a,
                            const std::pair<std::string, long long>& b) {
        return a.second < b.second;
    }
    void useOnHook(Actor* player,
                   const std::string& itemName,
                   BlockPos& pos,
                   unsigned int facing,
                   const Vec3&) override;

    bool attackEntityHook(Actor* player, Actor* entity) override;

    void registerCommands() override;

    void initialize() override;

    size_t boxDisplayTick = 0;
};

#endif  // MOD_WORLDEDIT_H
