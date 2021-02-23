//
// Created by xhy on 2020/10/22.
//

#include "Vec3.h"
#include "BlockPos.h"

namespace trapdoor {
    Vec3::Vec3(int _x) : Vec3(_x, _x, _x) {}

    Vec3::Vec3(float _x) : Vec3(_x, _x, _x) {}

    float Vec3::distanceTo(const Vec3& vec3) const {
        return sqrt((vec3.x - x) * (vec3.x - x) + (vec3.y - y) * (vec3.y - y) +
                    (vec3.z - z) * (vec3.z - z));
    }

    float Vec3::chebyshevDistanceTo(const Vec3& vec3) const {
        return std::max(std::max(std::abs(vec3.x - x), std::abs(vec3.y - y)),
                        std::abs(vec3.z - z));
    }

    float Vec3::chebyshevLength() const {
        return std::max(std::max(std::abs(x), std::abs(y)), std::abs(z));
    }

    float Vec3::length() const { return sqrt(x * x + y * y + z * z); }

    std::string Vec3::toString() const {
        char buffer[64];
        sprintf(buffer, "[%.2f %.2f %.2f]", x, y, z);
        return std::string(buffer);
    }

    bool Vec3::operator!=(const Vec3& v) const {
        return x != v.x || y != v.y || z != v.z;
    }

    bool Vec3::operator==(const Vec3& v) const {
        return x == v.x && y == v.y && z == v.z;
    }

    BlockPos Vec3::toBlockPos() const {
        auto cx = x < 0 ? x - 1 : x;
        auto cy = y < 0 ? y - 1 : y;
        auto cz = z < 0 ? z - 1 : z;
        return {cx, cy, cz};
    }

    std::string Vec3::toDirString() const {
        FACING fx = this->x > 0 ? FACING::POS_X : FACING::NEG_X;
        FACING fz = this->x > 0 ? FACING::POS_Z : FACING::NEG_Z;
        std::string s;
        if (std::abs(this->x) >= std::abs(this->z)) {
            s = facingToDirString(fx);
            s += " (";
            s += facingToString(fx);
            s += ")";
        } else if (1.732 * std::abs(this->z) > std::abs(this->x)) {
            s = facingToDirString(fz);
            s += " (";
            s += facingToString(fz);
            s += ")";
        }
        return s;
    }

    BlockPos Vec3::toDirection() const {
        FACING fx = this->x > 0 ? FACING::POS_X : FACING::NEG_X;
        FACING fy = this->y > 0 ? FACING::POS_Y : FACING::NEG_Y;
        FACING fz = this->z > 0 ? FACING::POS_Z : FACING::NEG_Z;
        std::string s;
        if (std::abs(this->dot(Vec3(0, 1, 0))) > 0.8f){
            return facingToBlockPos(fy);
        }
            if (std::abs(this->x) >= std::abs(this->z)) {
                return facingToBlockPos(fx);
            }
        return facingToBlockPos(fz);
    }

    bool Vec3::operator<(const Vec3& rhs) const {
        if (x < rhs.x)
            return true;
        if (rhs.x < x)
            return false;
        if (y < rhs.y)
            return true;
        if (rhs.y < y)
            return false;
        return z < rhs.z;
    }

    Vec3 Vec3::operator+(const Vec3& v) const {
        return {this->x + v.x, this->y + v.y, this->z + v.z};
    }

    Vec3 Vec3::operator-(const Vec3& v) const {
        return {this->x - v.x, this->y - v.y, this->z - v.z};
    }

    Vec3 Vec3::operator-() const { return {-x, -y, -z}; }

    Vec3 Vec3::operator*(float times) const {
        return {x * times, y * times, z * times};
    }

    Vec3 Vec3::operator*(const Vec3& v) const {
        return {x * v.x, y * v.y, z * v.z};
    }

    Vec3 Vec3::cross(const Vec3& v) const {
        return {y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x};
    }

    float Vec3::dot(const Vec3& v) const {
        return {x * v.x + y * v.y + z * v.z};
    }
    Vec3 Vec3::normalize() const {
        float length = this->length();
        return {x / length, y / length, z / length};
    }
}  // namespace trapdoor