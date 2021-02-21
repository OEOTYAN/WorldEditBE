//
// Created by xhy on 2021/2/3.
//

#include "VanillaBlockType.h"
#include "BDSMod.h"
#include "tools/DirtyLogger.h"

namespace trapdoor {
    namespace {
        std::unordered_map<trapdoor::BlockType, BlockLegacy*>& getBlockMap() {
            static std::unordered_map<trapdoor::BlockType, BlockLegacy*> map;
            return map;
        }
        std::unordered_map<std::string, BlockLegacy*>& getBlockNameMap() {
            static std::unordered_map<std::string, BlockLegacy*> map;
            return map;
        }
    }  // namespace

    trapdoor::BlockLegacy* getBlockLegacyByID(BlockType type) {
        auto iter = getBlockMap().find(type);
        if (iter != getBlockMap().end()) {
            return iter->second;
        }
        return getBlockMap()[AIR];
    }

    trapdoor::BlockLegacy* getBlockLegacyByName(std::string name) {
        auto iter = getBlockNameMap().find(name);
        if (iter != getBlockNameMap().end()) {
            return iter->second;
        }
        return getBlockMap()[AIR];
    }

    void initBlockMap() {
        using namespace SymHook;
        std::function<bool(trapdoor::BlockLegacy & b)> function(
            [&](trapdoor::BlockLegacy& l) {
                getBlockMap()[l.getBlockID()] = &l;
                getBlockNameMap()[l.tryGetStateBlock(0)->getName()] = &l;
                return true;
            });
        SYM_CALL(
            void (*)(const std::function<bool(BlockLegacy&)>*),
            MSSYM_B1QE12forEachBlockB1AE17BlockTypeRegistryB2AAA4SAXVB2QDA8functionB3ADDA3A6AB1UE16NAEBVBlockLegacyB3AAAA1ZB1AA3stdB3AAAA1Z,
            &function);
        L_INFO("init block map");
    }

    trapdoor::Block* getBlockByID(BlockType type, unsigned short variant) {
        auto* blockLegacy = getBlockLegacyByID(type);
        return blockLegacy->tryGetStateBlock(variant);
    }

    trapdoor::Block* getBlockByName(std::string name, unsigned short variant) {
        auto* blockLegacy = getBlockLegacyByName(name);
        return blockLegacy->tryGetStateBlock(variant);
    }

}  // namespace trapdoor