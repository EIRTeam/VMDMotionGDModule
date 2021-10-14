/*************************************************************************/
/*  vmd_utils.h                                                          */
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

#ifndef VMD_UTILS_H
#define VMD_UTILS_H

#include "core/math/math_funcs.h"
#include "core/math/quat.h"
#include "core/math/vector3.h"
#include "core/os/file_access.h"
#include "core/typedefs.h"
#include "scene/3d/skeleton.h"

#include "shift_jis.h"

#include <functional>
#include <string>
#include <vector>

class VMDBezierInterpolator {
public:
	float X0, Y0, X1, Y1;
	float inv_lerp(float a, float b, float x) {
		x = Math::inverse_lerp(a, b, x);
		float t = 0.5f;

		float p = 0.25f;

		while (p > 1e-6) {
			t -= p * SGN(t * (3 * (1 - t) * (X0 + t * (X1 - X0)) + t * t) - x);
			p *= 0.5;
		}
		return t * (3 * (1 - t) * (Y0 + t * (Y1 - Y0)) + t * t);
	}

	bool is_linear() {
		return X0 == Y0 && X1 == Y1;
	}

	void read_XYXY(FileAccess *file, uint8_t stride) {
		std::vector<char> vec;
		vec.resize(4);
		file->get_buffer(reinterpret_cast<uint8_t *>(vec.data()), stride);
		X0 = static_cast<float>(vec[0]) / 127.0f;
		file->get_buffer(reinterpret_cast<uint8_t *>(vec.data()), stride);
		Y0 = static_cast<float>(vec[0]) / 127.0f;
		file->get_buffer(reinterpret_cast<uint8_t *>(vec.data()), stride);
		X1 = static_cast<float>(vec[0]) / 127.0f;
		file->get_buffer(reinterpret_cast<uint8_t *>(vec.data()), stride);
		Y1 = static_cast<float>(vec[0]) / 127.0f;
	}
	void read_XXYY(FileAccess *file, uint8_t stride) {
		std::vector<char> vec;
		vec.resize(4);
		file->get_buffer(reinterpret_cast<uint8_t *>(vec.data()), stride);
		X0 = static_cast<float>(vec[0]) / 127.0f;
		file->get_buffer(reinterpret_cast<uint8_t *>(vec.data()), stride);
		X1 = static_cast<float>(vec[0]) / 127.0f;
		file->get_buffer(reinterpret_cast<uint8_t *>(vec.data()), stride);
		Y0 = static_cast<float>(vec[0]) / 127.0f;
		file->get_buffer(reinterpret_cast<uint8_t *>(vec.data()), stride);
		Y1 = static_cast<float>(vec[0]) / 127.0f;
	}
};

class VMDUtils {
public:
	static String read_vmd_string(FileAccess *file, int length) {
		std::vector<char> vec;
		vec.resize(length);

		file->get_buffer((uint8_t *)vec.data(), length);
		std::string str(vec.data());
		std::string utf8_str = sj2utf8(str);
		String gd_str;
		gd_str.parse_utf8(utf8_str.c_str(), utf8_str.size());
		gd_str = gd_str.trim_suffix("\0");
		gd_str = gd_str.trim_suffix("?");
		gd_str = gd_str.trim_suffix("\0");
		return gd_str;
	}
	static Vector3 read_vector3(FileAccess *file) {
		Vector3 v;
		v.x = file->get_float();
		v.y = file->get_float();
		v.z = file->get_float();
		return v;
	}
	static Quat read_quat(FileAccess *file) {
		Quat v;
		v.x = file->get_float();
		v.y = file->get_float();
		v.z = file->get_float();
		v.w = file->get_float();
		return v;
	}
	template <typename T>
	struct BSplitResult {
		bool has_first_true = true;
		T first_true;
		bool has_last_false = false;
		T last_false;
	};
	template <typename T, typename Func>
	static BSplitResult<T> binary_split(std::vector<T> list, Func pred) {
		uint64_t i = 0;
		uint64_t j = list.size();
		while (i < j) {
			int k = (i + j) / 2;
			if (pred(list[k])) {
				j = k;
			} else {
				i = k + 1;
			}
		}
		BSplitResult<T> out;
		if (i < list.size()) {
			out.has_first_true = true;
			out.first_true = list[i];
		}
		if (i > 0) {
			out.has_last_false = true;
			out.last_false = list[i - 1];
		}
		return out;
	}

	static Transform get_bone_global_rest(Skeleton *skel, int bone_i) {
		if (bone_i == -1) {
			return Transform();
		}
		Transform final_transform = skel->get_bone_rest(bone_i);
		int bone_parent = skel->get_bone_parent(bone_i);
		while (bone_parent != -1) {
			final_transform = skel->get_bone_rest(bone_parent) * final_transform;
			bone_parent = skel->get_bone_parent(bone_parent);
		}
		return final_transform;
	}
	static Quat quat_from_to_rotation(Vector3 from, Vector3 to) {
		from = from.normalized();
		to = to.normalized();
		Quat q;
		float d = from.dot(to);
		if (d >= 1.0) {
			return Quat();
		} else if (d < (1.0e-6 - 1.0)) {
			Vector3 axis = Vector3(1, 0, 0).cross(from);
			if (axis.length_squared() < (1e-06 * 1e-06)) {
				axis = Vector3(0, 1, 0).cross(from);
			}
			q.set_axis_angle(axis.normalized(), Math_PI);
		} else {
			q = Quat();
			float s = Math::sqrt((1.0 + d) * 2.0);
			float invs = 1.0 / s;
			Vector3 c = from.cross(to);

			q.x = c.x * invs;
			q.y = c.y * invs;
			q.z = c.z * invs;
			q.w = s * 0.5;
		}
		return q.normalized();
	}
};

#endif