//
// Created by OEOTYAN on 2021/2/8.
//

#ifndef WORLDEDIT_CONVEXREGION_H
#define WORLDEDIT_CONVEXREGION_H

#include "Region.h"
#include "Vec3.h"
#include <unordered_set>
#include <vector>
#include <xhash>
#include <algorithm>

using namespace trapdoor;

class Triangle;
class Edge {
   public:
    Vec3 start;
    Vec3 end;

    explicit Edge(Vec3 _start, Vec3 _end) : start(_start), end(_end){};
    bool operator==(const Edge& other) const;
    Triangle createTriangle(Vec3 vertex);
};
class _hash {
   public:
    size_t operator()(const Edge& rc) const {
        return (std::hash<float>()(rc.start.x) ^
                std::hash<float>()(rc.start.y) ^
                std::hash<float>()(rc.start.z)) ^
               (std::hash<float>()(rc.end.x) ^ std::hash<float>()(rc.end.y) ^
                std::hash<float>()(rc.end.z));
    }
    size_t operator()(const BlockPos& rc) const {
        return (std::hash<int>()(rc.x) ^ std::hash<int>()(rc.y) ^
                std::hash<int>()(rc.z));
    }
};
class Triangle {
   public:
    Vec3 vertices[3];
    Vec3 normal;
    float maxDotProduct;

    Triangle() = default;
    Triangle(Vec3 v0, Vec3 v1, Vec3 v2);
    Vec3 getVertex(int index) { return vertices[index]; };
    Edge getEdge(int index);
    bool below(Vec3 pt) { return normal.dot(pt) < maxDotProduct; }
    bool above(Vec3 pt) { return normal.dot(pt) > maxDotProduct; }
    bool operator==(const Triangle& v) const;
};

class ConvexRegion : public Region {
   private:
    bool addVertex(BlockPos vertex);
    bool containsRaw(Vec3 pt);

   public:
    std::unordered_set<BlockPos, _hash> vertices;

    std::vector<Triangle> triangles;

    std::unordered_set<BlockPos, _hash> vertexBacklog;

    BlockPos centerAccum;

    Triangle lastTriangle;
    bool hasLast = false;

    void updateBoundingBox() override;

    explicit ConvexRegion(const BoundingBox& region);

    bool setMainPos(const BlockPos& pos) override;

    bool setVicePos(const BlockPos& pos) override;

    bool isInRegion(const BlockPos& pos) override;
};

#endif  // WORLDEDIT_CONVEXREGION_H