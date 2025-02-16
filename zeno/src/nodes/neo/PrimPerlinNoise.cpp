#include <zeno/zeno.h>
#include <zeno/types/StringObject.h>
#include <zeno/types/PrimitiveObject.h>
#include <zeno/funcs/PrimitiveUtils.h>
#include <zeno/types/NumericObject.h>
#include <zeno/utils/wangsrng.h>
#include <zeno/utils/perlin.h>
#include <zeno/utils/variantswitch.h>
#include <zeno/utils/arrayindex.h>
#include <zeno/para/parallel_for.h>
#include <zeno/utils/vec.h>
#include <zeno/utils/log.h>
#include <cstring>
#include <cstdlib>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace zeno {

ZENO_API void primPerlinNoise(PrimitiveObject *prim, std::string inAttr, std::string outAttr, std::string outType, float scale, float detail, float roughness, float disortion, vec3f offset) {
    prim->attr_visit(inAttr, [&] (auto const &inArr) {
        std::visit([&] (auto outTypeId) {
            using InT = std::decay_t<decltype(inArr[0])>;
            using OutT = decltype(outTypeId);
            auto &outArr = prim->add_attr<OutT>(outAttr);
            parallel_for((size_t)0, inArr.size(), [&] (size_t i) {
                vec3f p;
                InT inp = inArr[i];
                if constexpr (std::is_same_v<InT, float>) {
                    p = {inp, 0, 0};
                } else if constexpr (std::is_same_v<InT, vec2f>) {
                    p = {inp[0], inp[1], 0};
                } else if constexpr (std::is_same_v<InT, vec3f>) {
                    p = inp;
                } else {
                    throw makeError<TypeError>(typeid(vec3f), typeid(InT), "input type");
                }
                if constexpr (std::is_same_v<OutT, float>) {
                    outArr[i] = PerlinNoise::perlin(p[0], p[1], p[2]);
                } else if constexpr (std::is_same_v<OutT, vec3f>) {
                    outArr[i] = {
                        PerlinNoise::perlin(p[0], p[1], p[2]),
                        PerlinNoise::perlin(p[1], p[2], p[0]),
                        PerlinNoise::perlin(p[2], p[0], p[1]),
                    };
                } else {
                    throw makeError<TypeError>(typeid(vec3f), typeid(OutT), "outType");
                }
            });
        }, enum_variant<std::variant<float, vec3f>>(array_index_safe({"float", "vec3f"}, outType, "outType")));
    });
}

namespace {

struct PrimPerlinNoise : INode {
    virtual void apply() override {
        auto prim = get_input<PrimitiveObject>("prim");
        auto scale = get_input2<float>("scale");
        auto detail = get_input2<float>("detail");
        auto roughness = get_input2<float>("roughness");
        auto disortion = get_input2<float>("disortion");
        auto offset = get_input2<vec3f>("offset");
        auto outAttr = get_input2<std::string>("outAttr");
        auto inAttr = get_input2<std::string>("inAttr");
        auto outType = get_input2<std::string>("outType");
        primPerlinNoise(prim.get(), inAttr, outAttr, outType, scale, detail, roughness, disortion, offset);
        set_output("prim", get_input("prim"));
    }
};

ZENDEFNODE(PrimPerlinNoise, {
    {
    {"PrimitiveObject", "prim"},
    {"string", "inAttr", "pos"},
    {"string", "outAttr", "clr"},
    {"float", "scale", "5"},
    {"float", "detail", "2"},
    {"float", "roughness", "0.5"},
    {"float", "disortion", "0"},
    {"vec3f", "offset", "0,0,0"},
    {"enum float vec3f", "outType", "float"},
    },
    {
    {"PrimitiveObject", "prim"},
    },
    {
    },
    {"primitive"},
});

}
}
