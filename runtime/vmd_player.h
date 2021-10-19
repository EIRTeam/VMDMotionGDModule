/*************************************************************************/
/*  vmd_player.h                                                         */
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

#ifndef VMD_PLAYER_H
#define VMD_PLAYER_H

#include "core/array.h"
#include "../common/morph.h"
#include "../common/motion.h"
#include "scene/3d/spatial.h"
#include <memory>
#include "scene/3d/camera.h"

const float VMD_FPS = 30.0f;

class VMDPlayer : public Spatial {
	GDCLASS(VMDPlayer, Spatial);

private:
	int max_frame = 0;
	int first_frame_number = 0;
	bool apply_ikq = false;
	float anim_scale = 0.08;
	int64_t start_time;
	float time = 0.0f;
	VMDAnimator *animator;
	Vector3 locomotion_scale = Vector3(1, 1, 1);
	VMDMotion motion;
	std::vector<float> scale_overrides;
	std::vector<std::tuple<String, bool>> last_ik_enable;
	int last_ik_enable_frame = 0;
	Transform center_bone;
	Transform waist_bone;
	float last_frame = -1;
	bool control_time_manually = false;
	Camera* camera;
	float camera_scale = 0.08f;

protected:
	void _notification(int p_what);
	static void _bind_methods();

public:
	std::vector<VMDMotion::BoneCurve> bone_curves;
	std::unique_ptr<VMDSkeleton> skeleton;
	VMDMorph morph;
	Error load_motions(PoolStringArray motions);
	void update_frame(float frame);
	float get_suggested_animation_scale() const;
	void apply_bone_frame(float p_frame);
	void apply_face_frame(float p_frame);
	void apply_camera_frame(float p_frame);
	void apply_ik_frame(float frame);
	void set_animation_scale(float p_scale);
	float get_animation_scale() const;
	void set_locomotion_scale(Vector3 p_locomotion_scale);
	Vector3 get_locomotion_scale() const;
	void set_enable_ik_rotation(bool p_enable_rotation);
	bool get_enable_ik_rotation() const;
	float get_playback_position() const;
	void seek(float p_time);
	bool get_control_time_manually() const;
	void set_control_time_manually(bool p_control_time_manually);
	void force_update_frame();
	void set_camera(Node *p_camera);

	void set_camera_scale(float p_camera_scale);
	float get_camera_scale() const;
};

#endif