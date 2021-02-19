#include "Eval.h"
#include "trapdoor.h"

namespace mod {
    namespace {
        std::map<std::string, double>& getInBuildVariables() {
            static std::map<std::string, double> variables;
            variables["pi"] = 3.14159265358979323846;
            variables["e"] = 2.7182818284590452354;
            return variables;
        }
    }  // namespace

    void eval(trapdoor::Actor* player, const std::string& str) {
        auto pos = player->getPos()->toBlockPos();
        getInBuildVariables()["x"] = pos.x;
        getInBuildVariables()["y"] = pos.y - 1;
        getInBuildVariables()["z"] = pos.z;
        getInBuildVariables()["cx"] = pos.toChunkPos().x;
        getInBuildVariables()["cz"] = pos.toChunkPos().z;
        EvalFunctions f;
        std::string s(str);
        std::string origin = s;
        s = toLowerString(s);
        stringReplace(s, "--", "+");
        stringReplace(s, "and", "&&");
        stringReplace(s, "xor", "^");
        stringReplace(s, "or", "||");
        stringReplace(s, "--", "+");
        stringReplace(s, "mod", "%");
        stringReplace(s, "==", "=");
        stringReplace(s, "π", "pi");
        std::ostringstream oss;
        oss.precision(22);
        auto x1 = cpp_eval::eval<double>(s.c_str(), getInBuildVariables(), f);
        if (abs(x1 - round(x1)) < 10E-8) {
            x1 = round(x1);
        }
        if (x1 == -0)
            x1 = 0;
        oss << x1;
        trapdoor::evalMsg("%s=§l§b%s", origin.c_str(), oss.str().c_str());
    }
}  // namespace mod