/*************************************************************************/
/*  vmd.cpp                                                              */
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

#include "vmd.h"
#include "core/os/os.h"
#include "motion.h"
#include "vmd_utils.h"

Error VMD::load(String path) {
	Error err;
	FileAccess *file_access = FileAccess::open(path, FileAccess::READ, &err);
	if (file_access) {
		String version = VMDUtils::read_vmd_string(file_access, 30);
		if (!version.begins_with("Vocaloid Motion Data")) {
			print_error("Invalid VMD file!");
			return ERR_FILE_CORRUPT;
		}

		String name = VMDUtils::read_vmd_string(file_access, 20);

		int32_t bone_frame_count = file_access->get_32();
		bone_keyframes.resize(bone_frame_count);
		for (int i = 0; i < bone_frame_count; i++) {
			bone_keyframes[i].read(file_access);
		}

		if (file_access->get_position() == file_access->get_len()) {
			return OK;
		}

		int32_t face_frame_count = file_access->get_32();
		face_keyframes.resize(face_frame_count);
		for (int i = 0; i < face_frame_count; i++) {
			face_keyframes[i].read(file_access);
		}

		if (file_access->get_position() == file_access->get_len()) {
			return OK;
		}

		int32_t camera_frame_count = file_access->get_32();
		camera_keyframes.resize(camera_frame_count);
		for (int i = 0; i < camera_frame_count; i++) {
			camera_keyframes[i].read(file_access);
		}

		if (file_access->get_position() == file_access->get_len()) {
			return OK;
		}

		int32_t light_frame_count = file_access->get_32();
		light_keyframes.resize(light_frame_count);
		for (int i = 0; i < light_frame_count; i++) {
			light_keyframes[i].read(file_access);
		}

		if (file_access->get_position() == file_access->get_len()) {
			return OK;
		}

		int32_t self_shadow_frame_count = file_access->get_32();
		self_shadow_keyframes.resize(self_shadow_frame_count);
		for (int i = 0; i < self_shadow_frame_count; i++) {
			self_shadow_keyframes[i].read(file_access);
		}

		if (file_access->get_position() == file_access->get_len()) {
			return OK;
		}

		int32_t ik_frame_count = file_access->get_32();
		ik_keyframes.resize(ik_frame_count);
		for (int i = 0; i < ik_frame_count; i++) {
			ik_keyframes[i].read(file_access);
		}

		return OK;
	}
	return err;
}

void VMD::_bind_methods() {
	ClassDB::bind_method(D_METHOD("load", "file_path"), &VMD::load);
}