/*************************************************************************/
/*  motion.cpp                                                           */
/*************************************************************************/
/*                       This file is part of:                           */
/*                          SHINOBU ENGINE                               */
/*                      https://ph.eirteam.moe                           */
/*************************************************************************/
/* Copyright (c) 2007-2021 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2021 Godot Engine contributors (cf. AUTHORS.md).   */
/* Copyright (c) 2021-2021 Álex Román Núñez (EIRTeam)                    */
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

#include "motion.h"
#include "vmd.h"
#include <algorithm>

void VMDMotion::add_clip(VMD *vmd) {
	for (VMD::BoneKeyframe keyframe : vmd->bone_keyframes) {
		auto it = bones.find(keyframe.name);
		BoneCurve *curve;
		if (it != bones.end()) {
			curve = &it->second;
		} else {
			bones[keyframe.name] = BoneCurve();
			curve = &bones[keyframe.name];
		}
		curve->keyframes.push_back(keyframe);
	}
	for (VMD::FaceKeyframe keyframe : vmd->face_keyframes) {
		auto it = faces.find(keyframe.name);
		FaceCurve *curve;
		if (it != faces.end()) {
			curve = &it->second;
		} else {
			faces[keyframe.name] = FaceCurve();
			curve = &faces[keyframe.name];
		}
		curve->keyframes.push_back(keyframe);
	}
	for (VMD::IKKeyframe keyframe : vmd->ik_keyframes) {
		ik.keyframes.push_back(keyframe);
	}
}

template <typename T>
bool keyframe_sort(T a, T b) {
	return a.frame_number < b.frame_number;
}

void VMDMotion::process_vmds() {
	// Sort bone keyframes
	for (auto i : bones) {
		std::sort(i.second.keyframes.begin(), i.second.keyframes.end(), keyframe_sort<VMD::BoneKeyframe>);
	}
	std::sort(ik.keyframes.begin(), ik.keyframes.end(), keyframe_sort<VMD::IKKeyframe>);

	// Sort
}

int VMDMotion::get_max_frame() {
	int max_frame = 0;
	for (auto bone : bones) {
		BoneCurve curve = bone.second;
		int max_subframe = 0;
		for (auto kf : curve.keyframes) {
			max_subframe = MAX(kf.frame_number, max_subframe);
		}
		max_frame = MAX(max_subframe, max_frame);
	}
	return max_frame;
}

VMDMotion::VMDMotion() {
}