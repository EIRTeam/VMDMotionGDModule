#include "vrm_springbone.h"
#include "../common/vmd_utils.h"
#include "vrm_collidergroup.h"

Transform VRMSpringBoneLogic::get_transform(Skeleton *p_skeleton) const {
    return p_skeleton->get_global_transform() * p_skeleton->get_bone_global_pose_no_override(bone_idx);
}

Quat VRMSpringBoneLogic::get_rotation(Skeleton *p_skeleton) const {
    return get_transform(p_skeleton).basis.get_rotation_quat();
}

Transform VRMSpringBoneLogic::get_local_transform(Skeleton *p_skeleton) const {
    return p_skeleton->get_bone_global_pose_no_override(bone_idx);
}

Quat VRMSpringBoneLogic::get_local_rotation(Skeleton *p_skeleton) const {
    return get_local_transform(p_skeleton).basis.get_rotation_quat();
}

void VRMSpringBoneLogic::reset(Skeleton *p_skeleton) {
    p_skeleton->set_bone_global_pose_override(bone_idx, initial_transform, 1.0);
}

VRMSpringBoneLogic::VRMSpringBoneLogic(Skeleton *p_skeleton, int p_bone_idx, Transform *p_center, Vector3 p_local_child_position, Transform p_default_pose)
: bone_idx(p_bone_idx), initial_transform(p_default_pose) {
    Vector3 world_child_position = VMDUtils::transform_point(get_transform(p_skeleton), p_local_child_position);
    if (p_center != nullptr) {
        current_tail = VMDUtils::inv_transform_point(*p_center, world_child_position);
    }  else {
        current_tail = world_child_position;
    }
    prev_tail = current_tail;
    bone_axis = p_local_child_position.normalized();
    length = p_local_child_position.length();
}

void VRMSpringBoneLogic::update(Skeleton *p_skeleton, Transform *p_center, float p_stiffness_force, float p_drag_force, Vector3 p_external, Array p_colliders) {
    Vector3 tmp_current_tail;
    Vector3 tmp_previous_tail;

    if (p_center != nullptr) {
        tmp_current_tail = VMDUtils::transform_point(*p_center, current_tail);
        tmp_previous_tail = VMDUtils::transform_point(*p_center, prev_tail);
    } else {
        tmp_current_tail = current_tail;
        tmp_previous_tail = prev_tail;
    }

    Vector3 next_tail = tmp_current_tail + (tmp_current_tail - tmp_previous_tail) * (1.0 - p_drag_force) + ((get_rotation(p_skeleton) * (bone_axis)) * p_stiffness_force).xform(p_external);

    Vector3 origin = get_transform(p_skeleton).origin;
    next_tail = origin + (next_tail - origin).normalized() * length;

    //TODO: next_tail = collision();

    if (p_center != nullptr) {
        prev_tail = VMDUtils::inv_transform_point(*p_center, current_tail);
        current_tail = VMDUtils::inv_transform_point(*p_center, next_tail);
    } else {
        prev_tail = current_tail;
        current_tail = next_tail;
    }

    Quat ft = VMDUtils::quat_from_to_rotation((get_rotation(p_skeleton).xform(bone_axis)), next_tail - get_transform(p_skeleton).origin);
    ft = p_skeleton->get_global_transform().basis.get_rotation_quat().inverse() * ft;
    Quat qt = ft * get_rotation(p_skeleton);
    Transform tr = get_local_transform(p_skeleton);
    qt.normalize();
    tr.basis = Basis(qt);
    p_skeleton->set_bone_global_pose_override(bone_idx, tr, 1.0);
}

void VRMSpringBoneLogic::collision(Skeleton *p_skeleton, Array p_colliders, Vector3 p_next_tail) {
    Vector3 out = p_next_tail;
    for (int i = 0; i < p_colliders.size(); i++) {
            VRMSphereCollider* c = Object::cast_to<VRMSphereCollider>(p_colliders[i]);
            float r = radius + c->get_radius();
            Vector3 diff = out - c->get_position();
            if ((diff.x * diff.x + diff.y * diff.y + diff.z * diff.z) <= r*r) {
                Vector3 normal = out - c->get_position().normalized();
                Vector3 pos_from_collider = c->get_position() + normal * (radius * c->get_radius());
                Vector3 origin = get_transform(p_skeleton).origin;
                out = origin + (pos_from_collider - origin).normalized() * length;
            }
    }
}

void VRMSpringBone::process(float p_delta) {
    if (verlets.empty()) {
        // setup
    }

    float stiffness = stiffness_force * p_delta;
    Vector3 external = gravity_dir * (gravity_power * p_delta);

    for (int i=0; i < verlets.size(); i++) {
    }
}