/*************************************************************************/
/*  vmd_player.cpp                                                       */
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

#include "vmd_player.h"
#include "core/os/os.h"
#include "../common/motion.h"
#include "../common/vmd.h"
#include <memory>

Error VMDPlayer::load_motions(PoolStringArray motions) {
	print_line(vformat("%d", motions.size()));
	ERR_FAIL_COND_V_MSG(animator == nullptr, ERR_INVALID_PARAMETER, "You should really have an animator as the parent of the vmd player");
	std::vector<std::unique_ptr<VMD>> vmds;
	motion = VMDMotion();

	for (int i = 0; i < motions.size(); i++) {
		String str = motions.get(i);
		VMD vmd;
		Error err = vmd.load(str);
		motion.add_clip(&vmd);
		ERR_FAIL_COND_V_MSG(err != OK, err, vformat("Error loading vmd file from %s", str));
	}

	motion.process_vmds();

	for (auto it = motion.bones.begin(); it != motion.bones.cend();) {
		String bone_name = StandardSkeleton::fix_bone_name(it->first);
		if (bone_name != it->first) {
			print_line(vformat("Bone rename %s => %s", it->first, bone_name));
			motion.bones.erase(it++);
			motion.bones.insert(std::make_pair(it->first, it->second));
		} else {
			++it;
		}
	}

	bone_curves.clear();

	bone_curves.reserve(VMDBoneName::_size());

	for (auto bone : VMDBoneName::_values()) {
		auto it = motion.bones.find(VMDBone2String(bone));
		if (it != motion.bones.end()) {
			bone_curves.push_back(it->second);
		} else {
			VMDMotion::BoneCurve curve;
			bone_curves.push_back(curve);
		}
	}

	max_frame = motion.get_max_frame();
	for (auto bone : std::vector<VMDBoneName>{ VMDBoneName::全ての親,
				 VMDBoneName::センター,
				 VMDBoneName::左足ＩＫ,
				 VMDBoneName::右足ＩＫ }) {
		std::vector<VMD::BoneKeyframe> keyframes = bone_curves[bone].keyframes;
		if (keyframes.size() > 2 && keyframes[0].frame_number == 0) {
			bool linear_motion_t = keyframes[0].position != keyframes[1].position && keyframes[1].interp.X.is_linear() && keyframes[1].interp.Y.is_linear() && keyframes[1].interp.Z.is_linear();
			bool linear_motion_q = keyframes[0].rotation != keyframes[1].rotation && keyframes[1].interp.rotation.is_linear();
			if (linear_motion_t || linear_motion_q) {
				first_frame_number = MAX(first_frame_number, keyframes[1].frame_number);
				print_line(vformat("Skipping frame (%s, %d)", keyframes[1].name, keyframes[1].frame_number));
			}
		}
	}

	for (auto bone : std::vector<VMDBoneName>{ VMDBoneName::左足ＩＫ, VMDBoneName::右足ＩＫ }) {
		VMDMotion::BoneCurve curve = bone_curves[bone];
		for (VMD::BoneKeyframe kf : curve.keyframes) {
			if (kf.rotation != Quat()) {
				apply_ikq = true;
				break;
			}
		}
		if (apply_ikq) {
			break;
		}
	}

	if (!skeleton) {
		anim_scale = 0.07*animator->get_humanoid_scale();
		skeleton = std::make_unique<VMDSkeleton>(animator, this);
		// TODO: Port source overrides for some avatar bones
		// https://gitlab.com/lox9973/VMDMotion/-/blob/master/Script/Runtime/VMDPlayer.cs#L83
	}

	for (auto bone : std::vector<VMDBoneName>{ VMDBoneName::左足ＩＫ, VMDBoneName::左つま先ＩＫ,
				 VMDBoneName::右足ＩＫ, VMDBoneName::右つま先ＩＫ }) {
		skeleton->bones[bone]->ik_enabled = bone_curves[bone].keyframes.size() > 1;
	}

	scale_overrides.resize(skeleton->bones.size());
	std::fill(scale_overrides.begin(), scale_overrides.end(), 0.0);

	for (auto bone : std::vector<VMDBoneName>{ VMDBoneName::左つま先ＩＫ, VMDBoneName::右つま先ＩＫ }) {
		Vector3 curve_local_pos_0 = -(bone_curves[bone].estimate_rotation_center_from_pos());
		Vector3 bone_local_pos_0 = skeleton->bones[bone]->local_position_0;
		scale_overrides[bone] = bone_local_pos_0.length() / curve_local_pos_0.length();
	}

	std::vector<String> morph_names;
	for (auto it : motion.faces) {
		morph_names.push_back(it.first);
	}

	morph.create_binds(morph_names);

	set_process(true);
	start_time = OS::get_singleton()->get_ticks_msec();

	return OK;
}

float VMDPlayer::get_suggested_animation_scale() const {
	return 0.07 * animator->get_humanoid_scale();
}

void VMDPlayer::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_PARENTED: {
			animator = Object::cast_to<VMDAnimator>(get_parent());
		} break;
		case NOTIFICATION_UNPARENTED: {
			animator = nullptr;
		} break;
		case NOTIFICATION_PROCESS: {
			if (!control_time_manually) {
				time = (OS::get_singleton()->get_ticks_msec() - start_time) / 1000.0;
			}
			float frame = time * VMD_FPS;
			update_frame(frame);
		} break;
	}
}

void VMDPlayer::update_frame(float frame) {
	frame = CLAMP(frame, 0, max_frame);
	apply_ik_frame(frame);
	apply_bone_frame(frame);
	apply_face_frame(frame);
	skeleton->apply_constraints(true, false);
	skeleton->apply_targets();
	morph.apply_targets(animator);
	if (camera) {
		apply_camera_frame(frame);
	}
	last_frame = frame;
}

void VMDPlayer::_bind_methods() {
	ClassDB::bind_method(D_METHOD("load_motions", "motions"), &VMDPlayer::load_motions);
	ClassDB::bind_method(D_METHOD("get_suggested_animation_scale"), &VMDPlayer::get_suggested_animation_scale);
	
	ClassDB::bind_method(D_METHOD("set_animation_scale", "animation_scale"), &VMDPlayer::set_animation_scale);
	ClassDB::bind_method(D_METHOD("get_animation_scale"), &VMDPlayer::get_animation_scale);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "animation_scale"), "set_animation_scale", "get_animation_scale");

	ClassDB::bind_method(D_METHOD("set_locomotion_scale", "locomotion_scale"), &VMDPlayer::set_locomotion_scale);
	ClassDB::bind_method(D_METHOD("get_locomotion_scale"), &VMDPlayer::get_locomotion_scale);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "locomotion_scale"), "set_locomotion_scale", "get_locomotion_scale");

	ClassDB::bind_method(D_METHOD("set_control_time_manually", "control_time_manually"), &VMDPlayer::set_control_time_manually);
	ClassDB::bind_method(D_METHOD("get_control_time_manually"), &VMDPlayer::get_control_time_manually);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "control_time_manually"), "set_control_time_manually", "get_control_time_manually");
	
	ClassDB::bind_method(D_METHOD("seek", "time"), &VMDPlayer::seek);

	ClassDB::bind_method(D_METHOD("set_enable_ik_rotation", "enable_ik_rotation"), &VMDPlayer::set_enable_ik_rotation);
	ClassDB::bind_method(D_METHOD("get_enable_ik_rotation"), &VMDPlayer::get_enable_ik_rotation);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "enable_ik_rotation"), "set_enable_ik_rotation", "get_enable_ik_rotation");

	ClassDB::bind_method(D_METHOD("force_update_frame"), &VMDPlayer::force_update_frame);
	ClassDB::bind_method(D_METHOD("get_playback_position"), &VMDPlayer::get_playback_position);

	ClassDB::bind_method(D_METHOD("set_camera", "camera"),  &VMDPlayer::set_camera);
	//ClassDB::bind_method(D_METHOD("get_camera"),  &VMDPlayer::get_camera);

	ClassDB::bind_method(D_METHOD("set_camera_scale", "camera_scale"), &VMDPlayer::set_camera_scale);
	ClassDB::bind_method(D_METHOD("get_camera_scale"), &VMDPlayer::get_camera_scale);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "camera_scale"), "set_camera_scale", "get_camera_scale");
}

void VMDPlayer::apply_face_frame(float p_frame) {
	p_frame = MAX(p_frame, 0);

	for (auto &it : motion.faces) {
		float out = it.second.sample(p_frame);
		auto morphit = morph.shapes.find(it.first);
		if (morphit != morph.shapes.end()) {
			morphit->second.weight = out;
		}
	}
}

void VMDPlayer::apply_bone_frame(float frame) {
	frame = MAX(frame, 0.0f);
	for (uint64_t i = 0; i < skeleton->bones.size(); i++) {
		VMDSkeleton::Bone *bone = skeleton->bones[i].get();
		VMDMotion::BoneCurve *curve = &bone_curves[i];
		Vector3 position;
		Quat rotation;
		curve->sample(frame, &position, &rotation);
		float scal = scale_overrides[bone->name];
		if (scal == 0) {
			scal = anim_scale;
		}
		position *= scal;

		if (bone->name == +VMDBoneName::全ての親 || bone->name == +VMDBoneName::センター ||
				bone->name == +VMDBoneName::左足ＩＫ || bone->name == +VMDBoneName::右足ＩＫ) {
			position *= locomotion_scale;
		}

		Transform bone_trf = bone->node->get_transform();
		bone_trf.origin = position + bone->local_position_0;
		bone_trf.basis = Basis(rotation);
		bone->node->set_transform(bone_trf);
	}
}

void VMDPlayer::apply_ik_frame(float frame) {
	frame = MAX(frame, 0.0f);
	std::vector<std::tuple<String, bool>> current_ik_enable = motion.ik.sample(frame);
	if (last_ik_enable_frame == Math::floor(frame)) {
		return;
	}
	last_ik_enable = current_ik_enable;
	last_ik_enable_frame = Math::floor(frame);

	for (auto pair : current_ik_enable) {
		String name_str = std::get<0>(pair);
		bool enable = std::get<1>(pair);
		std::wstring ws = name_str.c_str();
		auto name_opt = VMDBoneName::_from_string_nothrow(std::string(ws.begin(), ws.end()).c_str());
		if (name_opt) {
			VMDBoneName name = *name_opt;
			skeleton->bones[name]->ik_enabled = enable;
		}
	}
}

void VMDPlayer::apply_camera_frame(float p_frame) {
	p_frame = MAX(p_frame, 0);
	VMDMotion::CameraCurve::CameraSampleResult camera_sample = motion.camera.sample(p_frame);
	Vector3 target_pos = camera_sample.position;
	target_pos.z *= -1;
	Quat quat;
	Vector3 rot = camera_sample.rotation;
	quat.set_euler(rot);

	Transform camera_trf;
	camera_trf.basis = Basis(quat);
	camera_trf.origin = (target_pos + (quat.xform(Vector3(0.0, 0.0, -1.0))) * camera_sample.distance) * camera_scale;
	camera_trf.rotate(Vector3(0.0, 1.0, 0.0), Math::deg2rad(180.0f));
	camera->set_global_transform(camera_trf);
	camera->set_fov(camera_sample.fov);
}

void VMDPlayer::set_animation_scale(float p_scale) {
	anim_scale = p_scale;
}

float VMDPlayer::get_animation_scale() const {
	return anim_scale;
}

void VMDPlayer::set_locomotion_scale(Vector3 p_locomotion_scale) {
	locomotion_scale = p_locomotion_scale;
}

Vector3 VMDPlayer::get_locomotion_scale() const {
	return locomotion_scale;
}

void VMDPlayer::set_enable_ik_rotation(bool p_enable_rotation) {
	apply_ikq = p_enable_rotation;
}

bool VMDPlayer::get_enable_ik_rotation() const {
	return apply_ikq;
}

float VMDPlayer::get_playback_position() const {
	return last_frame / VMD_FPS;
}

void VMDPlayer::seek(float p_time) {
	if (control_time_manually) {
		time = p_time;
	} else {
		start_time = OS::get_singleton()->get_ticks_msec() - p_time * 1000;
	}
}

bool VMDPlayer::get_control_time_manually() const {
	return control_time_manually;
}

void VMDPlayer::set_control_time_manually(bool p_control_time_manually) {
	control_time_manually = p_control_time_manually;
}

void VMDPlayer::force_update_frame() {
	update_frame(time);
}

void VMDPlayer::set_camera(Node *p_camera) {
	camera = Object::cast_to<Camera>(p_camera);
	ERR_FAIL_COND(!camera);
}

float VMDPlayer::get_camera_scale() const {
	return camera_scale;
}

void VMDPlayer::set_camera_scale(float p_camera_scale) {
	camera_scale = p_camera_scale;
}