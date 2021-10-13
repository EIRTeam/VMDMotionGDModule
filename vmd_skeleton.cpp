#include "vmd_skeleton.h"
#include "animator.h"
#include "vmd_utils.h"

VMDSkeleton::VMDSkeleton(VMDAnimator *animator, Spatial *root_override, Dictionary source_overrides) {
    root = memnew(Spatial);
    root->set_name("MMDArmature");
    skeleton = animator->get_skeleton();
    if (root_override == nullptr) {
        skeleton->add_child(root);
    } else {
        root_override->add_child(root);
    }
    Transform tf = root->get_global_transform();
    tf.basis.set_axis(0, Vector3(-1, 0, 0));
    bones.resize(VMDBoneName::_size());

    for (auto tmplt : StandardSkeleton::bones) {
        Spatial* parent_node = tmplt.parent ? bones[*tmplt.parent].node : root;
        String test = tmplt.parent ? "Found parent" : "not found";
        if (tmplt.parent) {
            print_line(vformat("%d", tmplt.parent->_to_index()));
        }
        print_line(test);
        int source_bone_skel_i = -1;
        int target_bone_skel_i = -1;
        Transform source_transform;

        if (tmplt.source) {
            source_bone_skel_i = animator->find_humanoid_bone(tmplt.source->_to_string());
            source_transform = VMDUtils::get_bone_global_rest(skeleton, source_bone_skel_i);
        }
        if (tmplt.target) {
            target_bone_skel_i = animator->find_humanoid_bone(tmplt.target->_to_string());
        }

        Transform position_transform = source_overrides.get(tmplt.name._to_string(), source_transform);
        Transform target = !tmplt.target ? Transform() : VMDUtils::get_bone_global_rest(skeleton, target_bone_skel_i);
        bones[tmplt.name] = VMDSkeleton::Bone(tmplt.name, parent_node, position_transform, target, skeleton, target_bone_skel_i);
    }
}

void VMDSkeleton::apply_targets() {
    for (VMDSkeleton::Bone bone : bones) {
        bone.apply_target();
    }
}

void VMDSkeleton::apply_constraints(bool apply_ik, bool apply_ikq) {
    for (auto const& constraint : StandardSkeleton::constraints) {
        constraint->apply_constraint(this, apply_ikq);
    }
}

VMDSkeleton::~VMDSkeleton() {
}

VMDSkeleton::Bone::Bone(VMDBoneName _name, Spatial *_parent_node, Transform source, Transform _target, Skeleton *skel, int _target_bone_skel_i)
: name(_name) {
    skeleton = skel;
    target_bone_skel_i = _target_bone_skel_i;

    node = memnew(Spatial);
    node->set_name(vformat("Bone %d", _name._to_index()));
    _parent_node->add_child(node);

    Transform nd_trf = Transform();
    nd_trf.origin = source.origin;

    node->set_global_transform(nd_trf);

    local_position_0 = node->get_transform().origin;

    target = _target;
    target_position = nd_trf.xform_inv(target.origin);
    target_rotation = Basis(nd_trf.basis.get_rotation_quat() * target_rotation);
}

void VMDSkeleton::Bone::apply_target() {
    target.origin = node->get_global_transform().xform_inv(target.origin);
    target_rotation = Basis(node->get_global_transform().basis.get_rotation_quat() * target_rotation);
    update_pose();
}

void VMDSkeleton::Bone::update_pose() {
    if (skeleton != nullptr && target_bone_skel_i != -1) {
        skeleton->set_bone_global_pose_override(target_bone_skel_i, target, 1.0, true);
    }
}

VMDSkeleton::Bone::~Bone() {
    memdelete(node);
}