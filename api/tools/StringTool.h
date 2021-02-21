//
// Created by OEOTYAN on 2021/02/19.
//

#ifndef WORLDEDIT_STRINGTOOL_H
#define WORLDEDIT_STRINGTOOL_H

#include <string>
#include <algorithm>

#define DEFFLAG(flag, var)                                     \
    bool var = false;                                          \
    std::string _tmpString##var = " -";                        \
    _tmpString##var = _tmpString##var + #flag + " ";           \
    if (subCommand.find(_tmpString##var) != subCommand.npos) { \
        var = true;                                            \
        stringReplace(subCommand, _tmpString##var, " ");       \
    }

#define DETECT_FLAG_ERROR                                          \
    for (char i = 'a'; i <= 'z'; i++) {                            \
        std::string _tmpErrorString = " -";                        \
        _tmpErrorString = _tmpErrorString + i + " ";               \
        if (subCommand.find(_tmpErrorString) != subCommand.npos) { \
            trapdoor::error(player, "此命令不存在参数 -%c", i);    \
            return;                                                \
        }                                                          \
    }

namespace trapdoor {

    namespace {
        template <typename T>
        std::string fto_string(const T a_value) {
            std::ostringstream out;
            out << a_value;
            return out.str();
        }

        std::string toLowerString(std::string str) {
            transform(str.begin(), str.end(), str.begin(),
                      (int (*)(int))tolower);
            return str;
        }

        void stringReplace(std::string& str,
                           const std::string& fstr,
                           const std::string& rep) {
            std::string::size_type pos = 0;
            std::string::size_type a = fstr.length();
            std::string::size_type b = rep.length();
            if (b == 0)
                while ((pos = str.find(fstr, pos)) != std::string::npos) {
                    str.erase(pos, a);
                }
            else
                while ((pos = str.find(fstr, pos)) != std::string::npos) {
                    str.replace(pos, a, rep);
                    pos += b;
                }
        }
    }  // namespace
}  // namespace trapdoor
#endif  // WORLDEDIT_STRINGTOOL_H