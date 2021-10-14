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
	ClassDB::bind_method(D_METHOD("get_skeleton"), &VMDAnimator::get_skeleton);
}

void VMDAnimator::set_humanoid_bone_map(Dictionary new_map) {
	humanoid_bone_map = new_map;
}

int VMDAnimator::find_humanoid_bone(String bone_name) {
	if (humanoid_bone_map.has(bone_name)) {
		return skeleton->find_bone(humanoid_bone_map[bone_name]);
	} else {
		return skeleton->find_bone(bone_name);
	}
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

Skeleton *VMDAnimator::get_skeleton() {
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