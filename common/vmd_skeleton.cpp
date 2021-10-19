/*************************************************************************/
/*  vmd_skeleton.cpp                                                     */
/*************************************************************************/
/*                       This file is part of:                           */
/*                  SHINOBU ENGINE ANIMATION MODULE                      */
/*                      https://ph.eirteam.moe                           */
/*************************************************************************/
/* Copyright (c) 2007-2021 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2021 Godot Engine contributors (cf. AUTHORS.md).   */
/* Copyright (c) 2021 Álex Román Núñez (EIRTeam)                         */
/* Copyright (c) 2021 lox9973                                            */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#include "vmd_skeleton.h"
#include "../runtime/animator.h"
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
	root->set_global_transform(tf);
	bones.resize(VMDBoneName::_size());

	for (auto tmplt : StandardSkeleton::bones) {
		Spatial *parent_node = tmplt.parent ? bones[*tmplt.parent]->node : root;
		int source_bone_skel_i = -1;
		int target_bone_skel_i = -1;
		Transform source_transform;
		bool has_source = false;
		if (tmplt.source) {
			source_bone_skel_i = animator->find_humanoid_bone(tmplt.source->_to_string());
			source_transform = VMDUtils::get_bone_global_rest(skeleton, source_bone_skel_i);
			has_source = true;
		}
		if (tmplt.target) {
			target_bone_skel_i = animator->find_humanoid_bone(tmplt.target->_to_string());
		}
		if (!has_source) {
			has_source = source_overrides.has(VMDBone2String(tmplt.name));
		}
		Transform position_transform = source_overrides.get(VMDBone2String(tmplt.name), source_transform);
		Transform target = !tmplt.target ? Transform() : VMDUtils::get_bone_global_rest(skeleton, target_bone_skel_i);
		bones[tmplt.name] = std::make_unique<VMDSkeleton::Bone>(tmplt.name, parent_node, has_source, position_transform, target, skeleton, target_bone_skel_i);
	}
}

void VMDSkeleton::apply_targets() {
	for (auto &bone : bones) {
		bone->apply_target();
	}
}

void VMDSkeleton::apply_constraints(bool apply_ik, bool apply_ikq) {
	for (auto const &constraint : StandardSkeleton::constraints) {
		constraint->apply_constraint(this, apply_ikq);
	}
}

VMDSkeleton::~VMDSkeleton() {
}

VMDSkeleton::Bone::Bone(VMDBoneName _name, Spatial *_parent_node, bool has_source, Transform source, Transform _target, Skeleton *skel, int _target_bone_skel_i) :
		name(_name) {
	skeleton = skel;
	target_bone_skel_i = _target_bone_skel_i;

	node = memnew(Spatial);
	node->set_name(VMDBone2String(name));
	_parent_node->add_child(node);

	Transform nd_trf = node->get_global_transform();
	if (has_source) {
		nd_trf.origin = source.origin;
	}

	node->set_global_transform(nd_trf);

	local_position_0 = node->get_transform().origin;

	if (target_bone_skel_i != -1) {
		target = _target;
		target_position = nd_trf.xform_inv(target.origin);
		target_rotation = Basis(nd_trf.basis.get_rotation_quat().inverse() * target_rotation);
	}
}

void VMDSkeleton::Bone::apply_target() {
	target.origin = node->get_global_transform().xform(target_position);
	target.basis = Basis(node->get_global_transform().basis.get_rotation_quat() * target_rotation);
	update_pose();
}

void VMDSkeleton::Bone::update_pose() {
	if (skeleton != nullptr && target_bone_skel_i != -1) {
		skeleton->set_bone_global_pose_override(target_bone_skel_i, target, 1.0, true);
	}
}

VMDSkeleton::Bone::~Bone() {
}