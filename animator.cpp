#include "animator.h"

#include "scene/main/node.h"

void VMDAnimator::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_humanoid_bone_map", "bone_map"), &VMDAnimator::set_humanoid_bone_map);
    ClassDB::bind_method(D_METHOD("get_skeleton"), &VMDAnimator::get_skeleton);
}

void VMDAnimator::set_humanoid_bone_map(Dictionary new_map) {
    humanoid_bone_map = new_map;
}

int VMDAnimator::find_humanoid_bone(String bone_name) {
    return humanoid_bone_map.get(bone_name, 0);
}

void VMDAnimator::_notification(int p_what) {
    switch(p_what) {
        case NOTIFICATION_PARENTED: {
            skeleton = Object::cast_to<Skeleton>(get_parent());
        } break;
        case NOTIFICATION_UNPARENTED: {
            skeleton = nullptr;
        } break;
    }
}

Skeleton* VMDAnimator::get_skeleton() {
    return skeleton;
}

VMDAnimator::VMDAnimator() {
    skeleton = nullptr;
}

void VMDAnimator::set_humanoid_scale(float scale) {
    humanoid_scale = scale;
}

float VMDAnimator::get_humanoid_scale() {
    return humanoid_scale;
}