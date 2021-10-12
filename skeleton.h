/*************************************************************************/
/*  skeleton.h                                                           */
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

#ifndef VMD_SKELETON_H
#define VMD_SKELETON_H

#include "core/ustring.h"
#include "scene/3d/skeleton.h"
#include "scene/3d/spatial.h"
#include "standard_skeleton.h"
class VMDSkeleton {
public:
	class Bone {
		VMDBone name;
		Spatial *node;
		Vector3 local_position_0;

		Transform target;
		Vector3 target_position;
		Quat target_rotation;

		Skeleton *skeleton;

		bool ik_enabled = false;
		int target_bone_skel_i;

		Bone(VMDBone _name, Spatial *_parent_node, Transform _source, Transform _target, Skeleton *skel, int _target_bone_skel_i) {
			name = _name;
			skeleton = skel;
			target_bone_skel_i = _target_bone_skel_i;

			node = memnew(Spatial);
			node->set_name(String(_name._to_string()));
			_parent_node->add_child(node);

			Transform nd_trf = Transform();
			nd_trf.origin = _source.origin;

			node->set_global_transform(nd_trf);

			local_position_0 = node->get_transform().origin;

			target = _target;
			target_position = nd_trf.xform_inv(target.origin);
			target_rotation = Basis(nd_trf.basis.get_rotation_quat() * target_rotation);
			update_pose();
		}

		void apply_target() {
			target.origin = node->get_global_transform().xform_inv(target.origin);
			target_rotation = Basis(node->get_global_transform().basis.get_rotation_quat() * target_rotation);
			update_pose();
		}

		void update_pose() {
			if (skeleton != nullptr && target_bone_skel_i != -1) {
				skeleton->set_bone_global_pose_override(target_bone_skel_i, target, 1.0, true);
			}
		}

		~Bone() {
			memdelete(node);
		}
	};

	Spatial *root;

	VMDSkeleton()
};
#endif
