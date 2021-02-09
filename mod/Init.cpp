//
// Created by xhy on 2020/10/29.
//
#include "commands/Command.h"
#include "tools/Message.h"
#include "tools/DirtyLogger.h"
#include "BDSMod.h"

using namespace SymHook;

/*
 * this is the level tick function
 * in the new version this hook will be remove
 */
THook(void,
      MSSYM_B1QA4tickB1AE11ServerLevelB2AAA7UEAAXXZ,
      trapdoor::Level *serverLevel) {
    if (!trapdoor::bdsMod) {
        L_ERROR("mod is nullptr");
    }
    if (!trapdoor::bdsMod->getLevel()) {
        trapdoor::bdsMod->setLevel(serverLevel);
        trapdoor::bdsMod->initialize();
        /*
         * you can init some of you config here
         */
    }
    original(serverLevel);
}
