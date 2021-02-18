//
// Created by OEOTYAN on 2021/2/8.
//

#include "PolyRegion.h"

void PolyRegion::updateBoundingBox() {
    boundingBox.minPos.y = minY;
    boundingBox.maxPos.y = maxY;
    boundingBox.minPos.x = points[0].x;
    boundingBox.minPos.z = points[0].z;
    boundingBox.maxPos.x = points[0].x;
    boundingBox.maxPos.z = points[0].z;
    for_each(points.begin(), points.end(), [&](BlockPos2 value) {
        boundingBox.minPos.x = std::min(boundingBox.minPos.x, value.x);
        boundingBox.minPos.z = std::min(boundingBox.minPos.z, value.z);
        boundingBox.maxPos.x = std::max(boundingBox.maxPos.x, value.x);
        boundingBox.maxPos.z = std::max(boundingBox.maxPos.z, value.z);
    });
}
PolyRegion::PolyRegion(const BoundingBox& region, const int& dim)
    : Region(region, dim) {
    points.clear();
    this->regionType = POLY;
}
bool PolyRegion::setMainPos(const BlockPos& pos, const int& dim) {
    mainPos = pos;
    dimensionID = dim;
    selecting = 1;
    minY = pos.y;
    maxY = pos.y;
    points.clear();
    points.push_back({pos.x, pos.z});
    updateBoundingBox();
    return true;
}
bool PolyRegion::setVicePos(const BlockPos& pos, const int& dim) {
    if (!selecting || dim != dimensionID) {
        return false;
    }
    for (auto iter = points.cbegin(); iter != points.cend(); iter++) {
        if (*iter == BlockPos2(pos.x, pos.z)) {
            minY = std::min(minY, pos.y);
            maxY = std::max(maxY, pos.y);
            updateBoundingBox();
            return true;
        }
    }
    minY = std::min(minY, pos.y);
    maxY = std::max(maxY, pos.y);
    points.push_back({pos.x, pos.z});
    updateBoundingBox();
    return true;
}

void PolyRegion::drawRegion() {
    auto size = points.size();
    for (int i = 0; i < size; i++) {
        spawnRectangleParticle(
            {Vec3(points[i].x, minY, points[i].z),
             Vec3(points[i].x, maxY, points[i].z) + Vec3(1, 1, 1)},
            GRAPHIC_COLOR::GREEN, dimensionID);
    }
    for (int y : {minY, maxY}) {
        for (int i = 0; i < size - 1; i++) {
            drawObliqueLine(Vec3(points[i].x, y, points[i].z),
                            Vec3(points[i + 1].x, y, points[i + 1].z),
                            GRAPHIC_COLOR::YELLOW, dimensionID);
        }
        drawObliqueLine(Vec3(points[0].x, y, points[0].z),
                        Vec3(points[size - 1].x, y, points[size - 1].z),
                        GRAPHIC_COLOR::YELLOW, dimensionID);
    }
};

bool PolyRegion::contains(const BlockPos& pos) {
    if (points.size() < 3 || pos.y < minY || pos.y > maxY) {
        return false;
    }
    bool inside = false;
    int x1;
    int z1;
    int x2;
    int z2;
    long long crossproduct;
    BlockPos2 point = points.back();

    for (auto iter = points.cbegin(); iter != points.cend(); iter++) {
        auto value = *iter;
        if (value == BlockPos2(pos.x, pos.z))
            return true;
        if (value.x > point.x) {
            x1 = point.x;
            x2 = value.x;
            z1 = point.z;
            z2 = value.z;
        } else {
            x1 = value.x;
            x2 = point.x;
            z1 = value.z;
            z2 = point.z;
        }
        if (x1 <= pos.x && pos.x <= x2) {
            crossproduct =
                ((long long)pos.z - (long long)z1) * (long long)(x2 - x1) -
                ((long long)z2 - (long long)z1) * (long long)(pos.x - x1);
            if (crossproduct == 0) {
                if ((z1 <= pos.z) == (pos.z <= z2)) {
                    return true;
                }
            } else if (crossproduct < 0 && (x1 != pos.x)) {
                inside = !inside;
            }
        }
        point.x = value.x;
        point.z = value.z;
    }
    return inside;
}