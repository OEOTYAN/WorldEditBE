//
// Created by xhy on 2020/12/12.
//

#include <string>
#include "Graphics.h"
#include "Particle.h"
#include "Vec3.h"
#include "BDSMod.h"

#define MAX_LENGTH (16.0f)
namespace trapdoor {

    namespace {
        //把整数进行二进制分割，获取粒子的生成坐标
        std::map<float, int> binSplit(float start, float end) {
            std::map<float, int> lengthMap;
            float size = end - start;
            if (std::abs(size - round(size)) > 10e-5f)  //非整数长度
                lengthMap.insert({end - 0.5f, 1});
            int length = static_cast<int>(size);
            while (length >= 512) {
                length -= 256;
                auto point = static_cast<float>(128.0 + start);
                start += 256.0;
                lengthMap.insert({point, 256});
            }

            for (auto defaultLength = 256; defaultLength >= 1;
                 defaultLength /= 2) {
                if (length >= defaultLength) {
                    length -= defaultLength;
                    auto point =
                        static_cast<float>(0.5 * defaultLength + start);
                    start += defaultLength;
                    lengthMap.insert({point, defaultLength});
                }
            }
            return lengthMap;
        }

        std::string getLineBackParticleType(int length,
                                            FACING direction,
                                            GRAPHIC_COLOR color) {
            std::string str = "worldedit:line_back";
            str += std::to_string(length);
            switch (direction) {
                case FACING::NEG_Y:
                    str += "Yp";
                    break;
                case FACING::POS_Y:
                    str += "Ym";
                    break;
                case FACING::NEG_Z:
                    str += "Zp";
                    break;
                case FACING::POS_Z:
                    str += "Zm";
                    break;
                case FACING::NEG_X:
                    str += "Xp";
                    break;
                case FACING::POS_X:
                    str += "Xm";
                    break;
            }

            switch (color) {
                case GRAPHIC_COLOR::WHITE:
                    str += "W";
                    break;
                case GRAPHIC_COLOR::RED:
                    str += "R";
                    break;
                case GRAPHIC_COLOR::YELLOW:
                    str += "Y";
                    break;
                case GRAPHIC_COLOR::BLUE:
                    str += "B";
                    break;
                case GRAPHIC_COLOR::GREEN:
                    str += "G";
                    break;
            }
            return str;
        }
        std::string getLineParticleType(int length,
                                        FACING direction,
                                        GRAPHIC_COLOR color) {
            std::string str = "worldedit:line";
            str += std::to_string(length);
            switch (direction) {
                case FACING::NEG_Y:
                    str += "Yp";
                    break;
                case FACING::POS_Y:
                    str += "Ym";
                    break;
                case FACING::NEG_Z:
                    str += "Zp";
                    break;
                case FACING::POS_Z:
                    str += "Zm";
                    break;
                case FACING::NEG_X:
                    str += "Xp";
                    break;
                case FACING::POS_X:
                    str += "Xm";
                    break;
            }

            switch (color) {
                case GRAPHIC_COLOR::WHITE:
                    str += "W";
                    break;
                case GRAPHIC_COLOR::RED:
                    str += "R";
                    break;
                case GRAPHIC_COLOR::YELLOW:
                    str += "Y";
                    break;
                case GRAPHIC_COLOR::BLUE:
                    str += "B";
                    break;
                case GRAPHIC_COLOR::GREEN:
                    str += "G";
                    break;
            }
            return str;
        }
    }  // namespace

    void drawLine(const Vec3& originPoint,
                  FACING direction,
                  float length,
                  GRAPHIC_COLOR color,
                  int dimType) {
        if (length <= 0)
            return;
        float start = 0, end = 0;
        switch (direction) {
            case FACING::NEG_Y:
                start = originPoint.y - length;
                end = originPoint.y;
                break;
            case FACING::POS_Y:
                start = originPoint.y;
                end = originPoint.y + length;
                break;
            case FACING::NEG_Z:
                start = originPoint.z - length;
                end = originPoint.z;
                break;
            case FACING::POS_Z:
                start = originPoint.z;
                end = originPoint.z + length;
                break;
            case FACING::NEG_X:
                start = originPoint.x - length;
                end = originPoint.x;
                break;
            case FACING::POS_X:
                start = originPoint.x;
                end = originPoint.x + length;
                break;
        }
        // split point list;

        auto list = binSplit(start, end);
        std::map<Vec3, int> positionList;
        if (facingIsX(direction)) {
            for (auto i : list)
                positionList.insert(
                    {{i.first, originPoint.y, originPoint.z}, i.second});
        } else if (facingIsY(direction)) {
            for (auto i : list)
                positionList.insert(
                    {{originPoint.x, i.first, originPoint.z}, i.second});
        } else if (facingIsZ(direction)) {
            for (auto i : list)
                positionList.insert(
                    {{originPoint.x, originPoint.y, i.first}, i.second});
        }

        for (auto points : positionList) {
            auto particleType =
                getLineParticleType(points.second, direction, color);
            auto particleTypeInv =
                getLineParticleType(points.second, invFacing(direction), color);
            auto particleBackType =
                getLineBackParticleType(points.second, direction, color);
            auto particleBackTypeInv = getLineBackParticleType(
                points.second, invFacing(direction), color);
            spawnParticle(points.first, particleType, dimType);
            spawnParticle(points.first, particleTypeInv, dimType);
            spawnParticle(points.first, particleBackType, dimType);
            spawnParticle(points.first, particleBackTypeInv, dimType);
        }
    }
    void drawOrientedLine(Vec3 start, Vec3 end, int dimType) {
        Vec3 tmpStart, tmpEnd, vector;
        if (start.y > end.y) {
            tmpStart = end;
            tmpEnd = start;
        } else {
            tmpStart = start;
            tmpEnd = end;
        }
        float tmpChebyshevLength;
        do {
            vector = tmpEnd - tmpStart;
            float chebyshevLength = vector.chebyshevLength();
            tmpChebyshevLength = chebyshevLength;
            bool isGreater = false;
            if (chebyshevLength > MAX_LENGTH) {
                isGreater = true;
            }
            int length;
            if (!isGreater) {
                length = round(vector.length());
            }
            vector = vector * (MAX_LENGTH / chebyshevLength);
            vector.x = round(vector.x);
            vector.y = round(vector.y);
            vector.z = round(vector.z);
            Vec3 tmpVector = vector;
            vector = vector.y >= 0 ? vector : -vector;
            std::string num = std::to_string((int)vector.x) + "_" +
                              std::to_string((int)vector.y) + "_" +
                              std::to_string((int)vector.z) + "_";
            stringReplace(num, "-", "_");
            if (isGreater)
                length = round(vector.length());
            float tmpLength = length;
            vector = vector.normalize();
            auto mstart = tmpStart;
            for (auto defaultLength = (int)(MAX_LENGTH); defaultLength >= 1;
                 defaultLength /= 2) {
                if (length >= defaultLength) {
                    length -= defaultLength;
                    auto point =
                        (vector * (float)(0.5f * defaultLength) + mstart) +
                        Vec3(0.5f);
                    mstart = mstart + vector * (float)defaultLength;
                    std::string particleType = "worldedit:orientedline" + num;
                    std::string particleTypeInv =
                        "worldedit:orientedline" + num;
                    std::string particleBackType =
                        "worldedit:orientedline_back" + num;
                    std::string particleBackTypeInv =
                        "worldedit:orientedline_back" + num;
                    std::string mlength = std::to_string(defaultLength);
                    // std::string mlength = "1";
                    particleType += mlength + "mY";
                    particleTypeInv += mlength + "pY";
                    particleBackType += mlength + "mY";
                    particleBackTypeInv += mlength + "pY";
                    spawnParticle(point, particleType, dimType);
                    spawnParticle(point, particleTypeInv, dimType);
                    spawnParticle(point, particleBackType, dimType);
                    spawnParticle(point, particleBackTypeInv, dimType);
                }
            }
            tmpStart = tmpStart + tmpVector.normalize() * tmpLength;
        } while (tmpChebyshevLength > MAX_LENGTH);
    }
}  // namespace trapdoor
