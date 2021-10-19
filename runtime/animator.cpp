/*************************************************************************/
/*  animator.cpp                                                         */
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

#include "animator.h"

#include "scene/main/node.h"

void VMDAnimator::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_humanoid_bone_map", "bone_map"), &VMDAnimator::set_humanoid_bone_map);
	ClassDB::bind_method(D_METHOD("get_humanoid_bone_map"), &VMDAnimator::get_humanoid_bone_map);
	ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "humanoid_bone_map"), "set_humanoid_bone_map", "get_humanoid_bone_map");

	ClassDB::bind_method(D_METHOD("find_humanoid_bone", "bone_name"), &VMDAnimator::find_humanoid_bone);

	ClassDB::bind_method(D_METHOD("set_humanoid_scale", "bone_map"), &VMDAnimator::set_humanoid_scale);
	ClassDB::bind_method(D_METHOD("get_humanoid_scale"), &VMDAnimator::get_humanoid_scale);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "humanoid_scale"), "set_humanoid_scale", "get_humanoid_scale");

	ClassDB::bind_method(D_METHOD("get_skeleton"), &VMDAnimator::get_skeleton);

	ClassDB::bind_method(D_METHOD("push_blend_shape", "blend_shape"), &VMDAnimator::push_blend_shape);
}

int VMDAnimator::find_humanoid_bone(String p_bone_name) const {
	if (skeleton) {
		if (humanoid_bone_map.has(p_bone_name)) {
			return skeleton->find_bone(humanoid_bone_map[p_bone_name]);
		} else {
			return skeleton->find_bone(p_bone_name);
		}
	}
	return -1;
}

void VMDAnimator::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_PARENTED: {
			skeleton = Object::cast_to<Skeleton>(get_parent());
		} break;
		case NOTIFICATION_UNPARENTED: {
			skeleton = nullptr;
		} break;
	}
}

Skeleton *VMDAnimator::get_skeleton() const {
	return skeleton;
}

VMDAnimator::VMDAnimator() {
	skeleton = nullptr;
}

void VMDAnimator::set_humanoid_scale(float scale) {
	humanoid_scale = scale;
}

float VMDAnimator::get_humanoid_scale() const {
	return humanoid_scale;
}

void VMDAnimator::set_humanoid_bone_map(Dictionary p_humanoid_bone_map) {
	humanoid_bone_map = p_humanoid_bone_map;
}

Dictionary VMDAnimator::get_humanoid_bone_map() const {
	return humanoid_bone_map;
}

void VMDAnimator::push_blend_shape(Ref<VMDBlendShapeBind> p_blend_shape) {
	ERR_FAIL_COND(!p_blend_shape.is_valid());
	auto it = blend_shape_binds.find(p_blend_shape->shape_vmd_name);
	if (it != blend_shape_binds.end()) {
		it->second.push_back(p_blend_shape);
	} else {
		std::pair<String, std::vector<Ref<VMDBlendShapeBind>>> pair;
		pair.first = p_blend_shape->shape_vmd_name;
		pair.second.push_back(p_blend_shape);
		blend_shape_binds.insert(pair);
	}
}

void VMDAnimator::set_blend_shape_value(String p_vmd_shape_name, float value) {
	auto it = blend_shape_binds.find(p_vmd_shape_name);
	if (it != blend_shape_binds.end()) {
		for (auto group : it->second) {
			MeshInstance *mesh = group->mesh;
			if (mesh != nullptr) {
				float weight = 0.99999 * group->bind_weight / 100.0f;
				mesh->set(vformat("blend_shapes/morph_%d", group->bind_index), value * weight);
			}
		}
	}
}

void VMDBlendShapeBind::_bind_methods() {
	ClassDB::bind_method(D_METHOD("create", "shape_vmd_name", "mesh", "bind_index", "bind_weight"), &VMDBlendShapeBind::create);
}

void VMDBlendShapeBind::create(String p_shape_vmd_name, Node* p_mesh, int p_bind_index, float p_bind_weight) {
	shape_vmd_name = p_shape_vmd_name;
	mesh = Object::cast_to<MeshInstance>(p_mesh);
	bind_index = p_bind_index;
	bind_weight = p_bind_weight;
}

