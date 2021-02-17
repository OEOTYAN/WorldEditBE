//
// Created by OEOTYAN on 2021/2/8.
//

#include "ConvexRegion.h"

void ConvexRegion::updateBoundingBox() {
    boundingBox.minPos = *vertices.begin();
    boundingBox.maxPos = *vertices.begin();
    for_each(vertices.begin(), vertices.end(), [&](BlockPos value) {
        boundingBox.minPos.x = std::min(boundingBox.minPos.x, value.x);
        boundingBox.minPos.y = std::min(boundingBox.minPos.y, value.y);
        boundingBox.minPos.z = std::min(boundingBox.minPos.z, value.z);
        boundingBox.maxPos.x = std::max(boundingBox.maxPos.x, value.x);
        boundingBox.maxPos.y = std::max(boundingBox.maxPos.y, value.y);
        boundingBox.maxPos.z = std::max(boundingBox.maxPos.z, value.z);
    });
}

ConvexRegion::ConvexRegion(const BoundingBox& region, const int& dim)
    : Region(region, dim) {
    vertices.clear();
    triangles.clear();
    vertexBacklog.clear();
    hasLast = false;
    this->regionType = CONVEX;
}

bool ConvexRegion::containsRaw(Vec3 pt) {
    if (lastTriangle.above(pt) && hasLast == true) {
        return false;
    }

    for (Triangle triangle : triangles) {
        if (lastTriangle == triangle) {
            continue;
        }

        if (triangle.above(pt)) {
            hasLast = true;
            lastTriangle = triangle;
            return false;
        }
    }

    return true;
}

bool ConvexRegion::addVertex(BlockPos vertex) {
    hasLast = false;
    if (vertices.find(vertex) != vertices.end()) {
        return false;
    }
    Vec3 vertexD = vertex.toVec3();
    if (vertices.size() == 3) {
        if (vertexBacklog.find(vertex) != vertexBacklog.end()) {
            return false;
        }
        if (containsRaw(vertexD)) {
            vertexBacklog.insert(vertex);
            return true;
        }
    }
    vertices.insert(vertex);
    updateBoundingBox();
    centerAccum = centerAccum + vertex;

    switch (vertices.size()) {
        case 0:
        case 1:
        case 2: {
            return true;
        };
        case 3: {
            std::vector<BlockPos> v;
            v.assign(vertices.begin(), vertices.end());
            triangles.push_back(
                Triangle(v[0].toVec3(), v[1].toVec3(), v[2].toVec3()));
            triangles.push_back(
                Triangle(v[0].toVec3(), v[2].toVec3(), v[1].toVec3()));
            return true;
        };
        default:
            break;
    }
    std::unordered_set<Edge, _hash> borderEdges;
    for (auto iter = triangles.begin(); iter != triangles.end();) {
        if ((*iter).above(vertexD)) {
            for (int i = 0; i < 3; ++i) {
                Edge edge = (*iter).getEdge(i);
                if (borderEdges.find(edge) == borderEdges.end()) {
                    borderEdges.insert(edge);
                } else {
                    borderEdges.erase(edge);
                }
            }
            iter = triangles.erase(iter);
        } else
            ++iter;
    }
    for (Edge edge : borderEdges) {
        triangles.push_back(edge.createTriangle(vertexD));
    }

    if (!vertexBacklog.empty()) {
        vertices.erase(vertex);
        std::unordered_set<BlockPos, _hash> vertexBacklog2(vertexBacklog);
        vertexBacklog.clear();
        for (BlockPos vertex2 : vertexBacklog2) {
            addVertex(vertex2);
        }
        vertices.insert(vertex);
    }

    return true;
}

bool ConvexRegion::setMainPos(const BlockPos& pos, const int& dim) {
    dimensionID = dim;
    selecting = 1;
    hasLast = false;
    vertices.clear();
    triangles.clear();
    vertexBacklog.clear();
    return addVertex(pos);
}

bool ConvexRegion::setVicePos(const BlockPos& pos, const int& dim) {
    if (!selecting || dim != dimensionID) {
        return false;
    }
    return addVertex(pos);
}

bool ConvexRegion::isInRegion(const BlockPos& pos) {
    if (triangles.empty()) {
        return false;
    }

    if (!pos.containedWithin(boundingBox.minPos, boundingBox.maxPos)) {
        return false;
    }

    return containsRaw(pos.toVec3());
}

bool Edge::operator==(const Edge& other) const {
    return (start == other.start && end == other.end) ||
           (end == other.start && start == other.end);
}

Triangle Edge::createTriangle(Vec3 vertex) {
    return Triangle(start, end, vertex);
}

Triangle::Triangle(Vec3 v0, Vec3 v1, Vec3 v2) {
    vertices[0] = v0;
    vertices[1] = v1;
    vertices[2] = v2;
    normal = ((v1 - v0).cross(v2 - v0)).normalize();
    maxDotProduct =
        std::max(std::max(normal.dot(v0), normal.dot(v1)), normal.dot(v2));
}
bool Triangle::operator==(const Triangle& v) const {
    return (v.maxDotProduct == maxDotProduct) && (v.normal == normal) &&
           (v.vertices == vertices);
}
Edge Triangle::getEdge(int index) {
    if (index == 2)
        return Edge(vertices[index], vertices[0]);
    return Edge(vertices[index], vertices[index + 1]);
}