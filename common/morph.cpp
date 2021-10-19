#include "morph.h"

void VMDMorph::create_binds(std::vector<String> p_shape_names) {
    shapes.clear();
    for (String s : p_shape_names) {
        shapes.insert(std::make_pair(s, VMDMorphBind(s)));
    }
}

void VMDMorph::apply_targets(VMDAnimator *animator) {
    for (auto &it : shapes) {
        animator->set_blend_shape_value(it.first, it.second.weight);        
    }
}