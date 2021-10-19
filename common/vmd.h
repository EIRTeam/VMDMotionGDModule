/*************************************************************************/
/*  vmd.h                                                                */
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

#ifndef VMD_H
#define VMD_H

#include "core/color.h"
#include "core/math/quat.h"
#include "core/math/vector3.h"
#include "core/os/file_access.h"
#include "core/reference.h"
#include "core/ustring.h"
#include "../third_party/shift_jis.h"
#include "standard_skeleton.h"
#include "vmd_utils.h"
#include <tuple>

class VMD : public Reference {
	GDCLASS(VMD, Reference);

protected:
	static void _bind_methods();

public:
	class BoneKeyframe {
	public:
		String name;
		int32_t frame_number;
		Vector3 position;
		Quat rotation;
		struct {
			VMDBezierInterpolator X, Y, Z, rotation;
		} interp;

		int read(FileAccess *file) {
			name = VMDUtils::read_vmd_string(file, 15);
			frame_number = file->get_32();
			position = VMDUtils::read_vector3(file);
			rotation = VMDUtils::read_quat(file);
			interp.X.read_XYXY(file, 4);
			interp.Y.read_XYXY(file, 4);
			interp.Z.read_XYXY(file, 4);
			interp.rotation.read_XYXY(file, 4);
			return OK;
		}
	};

	class FaceKeyframe {
	public:
		String name;
		int32_t frame_number;
		float weight;
		int read(FileAccess *file) {
			name = VMDUtils::read_vmd_string(file, 15);
			frame_number = file->get_32();
			weight = file->get_float();
			return OK;
		}
	};

	class CameraKeyframe {
	public:
		int32_t frame_number;
		float distance;
		Vector3 position;
		Vector3 rotation;
		struct {
			VMDBezierInterpolator X, Y, Z, R, dist, angle;
		} interp;
		float angle;
		bool orthographic;

		int read(FileAccess *file) {
			frame_number = file->get_32();
			distance = file->get_float();
			position = VMDUtils::read_vector3(file);
			rotation = VMDUtils::read_vector3(file);
			interp.X.read_XXYY(file, 1);
			interp.Y.read_XXYY(file, 1);
			interp.Z.read_XXYY(file, 1);
			interp.R.read_XXYY(file, 1);
			interp.dist.read_XXYY(file, 1);
			interp.angle.read_XXYY(file, 1);
			angle = file->get_32();
			orthographic = (int8_t)file->get_8() != 0;
			return OK;
		}
	};

	class LightKeyFrame {
	public:
		int32_t frame_number;
		Color light_color;
		Vector3 position;

		int read(FileAccess *file) {
			frame_number = file->get_32();
			light_color = Color(file->get_float(), file->get_float(), file->get_float(), 1.0);
			position = VMDUtils::read_vector3(file);
			return OK;
		}
	};

	class SelfShadowKeyframe {
	public:
		int32_t frame_number;
		int8_t type;
		float distance;

		int read(FileAccess *file) {
			frame_number = file->get_32();
			type = file->get_8();
			distance = file->get_float();
			return OK;
		}
	};

	class IKKeyframe {
	public:
		int32_t frame_number;
		bool display;
		std::vector<std::tuple<String, bool>> ik_enable;
		int read(FileAccess *file) {
			frame_number = file->get_32();
			display = (int8_t)file->get_8() != 0;
			int32_t ik_enable_count = file->get_32();
			ik_enable.resize(ik_enable_count);
			for (int i = 0; i < ik_enable_count; i++) {
				ik_enable[i] = std::make_tuple(VMDUtils::read_vmd_string(file, 20), file->get_8());
			}
			return OK;
		}
	};

	std::vector<BoneKeyframe> bone_keyframes = {};
	std::vector<FaceKeyframe> face_keyframes = {};
	std::vector<CameraKeyframe> camera_keyframes = {};
	std::vector<LightKeyFrame> light_keyframes = {};
	std::vector<SelfShadowKeyframe> self_shadow_keyframes = {};
	std::vector<IKKeyframe> ik_keyframes = {};

	Error load(String path);
};
#endif