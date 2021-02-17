cmake_minimum_required(VERSION 3.17)
project(mod)
set(CMAKE_CXX_STANDARD 17)

#因为ABI原因这个项目只能msvc编译
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    add_compile_options(/wd4828 /wd4819 /EHsc /utf-8)
else ()
    message(this project don't support GNU gcc or clang,please use MSVC instead)
endif ()

set(MOD_API_NAME libtrapdoor)
aux_source_directory(${PROJECT_SOURCE_DIR}/api MOD_API_SRC)
add_library(${MOD_API_NAME} STATIC
        api/BDSMod.cpp
        api/block/Block.cpp
        api/block/BlockLegacy.cpp
        api/block/BlockSource.cpp
        api/block/VanillaBlockType.cpp
        api/block/CircuitComponent.cpp
        api/commands/CommandNode.cpp
        api/commands/Command.cpp
        api/commands/CommandManager.cpp
        api/commands/ArgHolder.cpp
        api/entity/Actor.cpp
        api/entity/Item.cpp
        api/entity/PlayerInventory.cpp
        api/entity/PlayerBuffer.cpp
        api/graphics/Particle.cpp
        api/graphics/AABB.h
        api/graphics/BlockPos.cpp
        api/graphics/Vec3.cpp
        api/graphics/Graphics.cpp
        api/graphics/BlockPos.cpp
        api/world/Biome.cpp
        api/world/Dimension.cpp
        api/world/Level.cpp
        api/tools/DirtyLogger.cpp
        api/tools/Message.cpp
        api/tools/MsgBuilder.cpp
        api/tools/ThreadPool.h
        )
target_include_directories(${MOD_API_NAME}
        PUBLIC
        api
        api/block
        api/commands
        api/entity
        api/graphics
        api/world
        api/lib
        api/tools
        )

target_link_libraries(${MOD_API_NAME} ${PROJECT_SOURCE_DIR}/api/lib/detours.lib)

#change your mod name here
set(MOD_NAME WorldEdit-BE)
#add your source file here
add_library(${MOD_NAME}
        SHARED
        mod/dllmain.cpp
        mod/WorldEditMod.cpp
        mod/WorldEditMod.h
        mod/region/ConvexRegion.cpp
        mod/region/CuboidRegion.cpp
        mod/region/ExpandRegion.cpp
        mod/region/PolyRegion.cpp
        mod/region/Region.cpp
        mod/region/SphereRegion.cpp
        mod/player/Player.cpp
        mod/tick/GameTick.cpp
        )

target_include_directories(
        ${MOD_NAME}
        PUBLIC
        mod
        mod/region
        mod/player
        mod/eval
        api
        api/block
        api/commands
        api/entity
        api/graphics
        api/world
        api/lib
        api/tools
)

target_link_libraries(${MOD_NAME} ${MOD_API_NAME})

