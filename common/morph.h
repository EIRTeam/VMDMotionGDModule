#ifndef VMD_MORPH_H
#define VMD_MORPH_H

#include "../runtime/animator.h"
#include <vector>
#include <string>
#include <map>
#include "scene/3d/mesh_instance.h"

class VMDMorphBind {
public:
    String vmd_shape_name;
    float weight = 0.0f;
    VMDMorphBind(String p_vmd_shape_name) : vmd_shape_name(p_vmd_shape_name) {};
};

class VMDMorph {
public:
    std::map<String, VMDMorphBind> shapes;
    void set_blend_shape_value(String p_vmd_sample_name, float p_value);
    void apply_targets(VMDAnimator *animator);
    void create_binds(std::vector<String> p_shape_names);
};

#endif