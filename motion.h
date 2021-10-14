/*************************************************************************/
/*  motion.h                                                             */
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

#ifndef VMD_MOTION_H
#define VMD_MOTION_H
#include "core/array.h"
#include "core/math/quat.h"
#include "core/math/vector3.h"
#include "core/ustring.h"
#include "vmd.h"
#include <map>
#include <optional>

class VMDMotion {
private:
public:
	class BoneCurve {
	public:
		std::vector<VMD::BoneKeyframe> keyframes;
		void sample(float frame_number, Vector3 *position, Quat *rotation) {
			VMDUtils::BSplitResult<VMD::BoneKeyframe> out = VMDUtils::binary_split(
					keyframes,
					[frame_number](VMD::BoneKeyframe f) -> bool {
						return f.frame_number >= frame_number;
					});
			Vector3 pos = out.has_last_false ? out.last_false.position : Vector3();
			Quat rot = out.has_last_false ? out.last_false.rotation : Quat();
			int prev_number = out.has_last_false ? out.last_false.frame_number : 0;

			if (out.has_first_true) {
				VMD::BoneKeyframe next_frame = out.first_true;
				float x = next_frame.interp.X.inv_lerp(prev_number, next_frame.frame_number, frame_number);
				float y = next_frame.interp.Y.inv_lerp(prev_number, next_frame.frame_number, frame_number);
				float z = next_frame.interp.Z.inv_lerp(prev_number, next_frame.frame_number, frame_number);
				float r = next_frame.interp.rotation.inv_lerp(prev_number, next_frame.frame_number, frame_number);
				pos.x = Math::lerp(pos.x, next_frame.position.x, x);
				pos.y = Math::lerp(pos.y, next_frame.position.y, y);
				pos.z = Math::lerp(pos.z, next_frame.position.z, z);
				rot = rotation->slerp(next_frame.rotation, r);
			}

			*position = pos;
			*rotation = rot;
		}

		Vector3 estimate_rotation_center_from_pos() {
			Vector3 A0, A1, A2, B;
			for (uint64_t i = 1; i < keyframes.size(); i++) {
				Vector3 p = keyframes[i].position;
				float b = p.length_squared() / 2.0f;
				float w = p.distance_to(keyframes[i - 1].position) + p.distance_to(keyframes[i + 1].position);
				A0 += w * p.x * p;
				A1 += w * p.y * p;
				A2 += w * p.z * p;
				B += w * b * p;
			}
			Transform trf = Transform(Basis(A0, A1, A2), Vector3());
			return trf.inverse().xform(B);
		}
	};

	class FaceCurve {
	public:
		std::vector<VMD::FaceKeyframe> keyframes;
		float sample(float frame_number) {
			/*
			VMD::FaceKeyframe *prev_frame = nullptr, *next_frame = nullptr;
			VMDUtils::binary_split(
					keyframes,
					[frame_number](VMD::FaceKeyframe f) -> bool {
						return f.frame_number >= frame_number;
					},
					prev_frame,
					next_frame);
			int prev_number = prev_frame != nullptr ? prev_frame->frame_number : 0;
			float weight = prev_frame != nullptr ? prev_frame->weight : 0.0f;

			if (next_frame != nullptr) {
				float w = Math::inverse_lerp(prev_number, next_frame->frame_number, frame_number);
				weight = Math::lerp(weight, next_frame->weight, w);
			}
			*/
			return 0.0;
		}
	};

	class IKCurve {
	public:
		std::vector<VMD::IKKeyframe> keyframes;
		std::vector<std::tuple<String, bool>> sample(float frame_number) {
			std::vector<std::tuple<String, bool>> ik_enable;
			VMDUtils::BSplitResult<VMD::IKKeyframe> out = VMDUtils::binary_split(
					keyframes,
					[frame_number](VMD::IKKeyframe f) -> bool {
						return f.frame_number - 1 >= frame_number;
					});

			if (out.has_last_false) {
				return out.last_false.ik_enable;
			}

			return ik_enable;
		}
	};

	void add_clip(VMD *vmd);
	std::map<String, BoneCurve> bones;
	std::map<String, FaceCurve> faces;
	IKCurve ik;
	int get_max_frame();
	void process_vmds();

	VMDMotion();
};
#endif