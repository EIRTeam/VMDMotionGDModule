/*************************************************************************/
/*  vmd_skeleton.h                                                       */
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

#ifndef VMD_SKELETON_H
#define VMD_SKELETON_H

#include "animator.h"
#include "core/dictionary.h"
#include "core/ustring.h"
#include "scene/3d/skeleton.h"
#include "scene/3d/spatial.h"
#include "standard_skeleton.h"
#include <vector>
class VMDSkeleton {
public:
	class Bone {
	public:
		VMDBoneName name;
		Spatial *node;
		Vector3 local_position_0;

		Transform target;
		Vector3 target_position;
		Quat target_rotation;

		Skeleton *skeleton;

		bool ik_enabled = false;
		int target_bone_skel_i;
		Bone() :
				name(VMDBoneName::右手首){};
		Bone(VMDBoneName _name, Spatial *_parent_node, bool has_source, Transform source, Transform _target, Skeleton *skel, int _target_bone_skel_i);
		void apply_target();
		void update_pose();

		~Bone();
	};

	Spatial *root;
	Skeleton *skeleton;
	std::vector<std::unique_ptr<VMDSkeleton::Bone>> bones;

	void apply_targets();
	void apply_constraints(bool apply_ik = true, bool apply_ikq = false);
	void apply_rot_add(Spatial *target, Transform source, bool minus);

	VMDSkeleton(VMDAnimator *animator, Spatial *root_override, Dictionary source_overrides = {});
	~VMDSkeleton();
};
#endif
