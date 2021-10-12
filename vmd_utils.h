/*************************************************************************/
/*  vmd_utils.h                                                          */
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

#ifndef VMD_UTILS_H
#define VMD_UTILS_H

#include "core/math/math_funcs.h"
#include "core/math/quat.h"
#include "core/math/vector3.h"
#include "core/os/file_access.h"
#include "core/typedefs.h"

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
		String gd_str = String(utf8_str.c_str());
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
	template <typename T, typename Func>
	static void binary_split(std::vector<T> list, Func pred, T *last_false, T *first_true) {
		int i = 0;
		int j = list.size();
		while (i < j) {
			int k = (i + j) / 2;
			if (pred(list[k])) {
				j = k;
			} else {
				i = k + 1;
			}
		}
		first_true = i < list.size() ? &list[i] : nullptr;
		last_false = i > 0 ? &list[i - 1] : nullptr;
	}
};
#endif