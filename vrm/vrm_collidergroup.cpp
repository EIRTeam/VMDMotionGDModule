#include "vrm_collidergroup.h"
#include "../common/vmd_utils.h"

void VRMSphereCollider::update(Skeleton *p_skeleton) {
    if (bone_idx != -1) {
        position = VMDUtils::transform_point(p_skeleton->get_global_transform() * p_skeleton->get_bone_global_pose_no_override(bone_idx), offset);
    }
}

Vector3 VRMSphereCollider::get_position() const {
    return position;
}

float VRMSphereCollider::get_radius() const {
    return radius; 
}

VRMColliderGroup::VRMColliderGroup(Skeleton *p_skeleton, int p_bone_idx, Vector<Plane> p_colliders) {
    
}