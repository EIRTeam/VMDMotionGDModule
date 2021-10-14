/*************************************************************************/
/*  standard_skeleton.h                                                  */
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

#ifndef VMD_STANDARD_SKELETON_H
#define VMD_STANDARD_SKELETON_H
#include "better_enums/better_enums.h"
#include "human_body_bones.h"
#include "vmd_utils.h"
#include <map>
#include <memory>
#include <string>
#include <vector>

BETTER_ENUM(VMDBoneName, int,
		全ての親, センター, グルーブ,
		左足IK親, 左足ＩＫ, 左つま先ＩＫ,
		右足IK親, 右足ＩＫ, 右つま先ＩＫ,
		腰, 下半身, 上半身, 上半身2, 首, 頭,
		左目, 右目, 両目,
		左肩P, 左肩, 左肩C, 左腕, 左腕捩, 左ひじ, 左手捩, 左手首,
		右肩P, 右肩, 右肩C, 右腕, 右腕捩, 右ひじ, 右手捩, 右手首,
		左足, 左ひざ, 左足首, 左つま先,
		右足, 右ひざ, 右足首, 右つま先,
		左足D, 左ひざD, 左足首D, 左足先EX,
		右足D, 右ひざD, 右足首D, 右足先EX,
		左親指０, 左親指１, 左親指２, 左人指１, 左人指２, 左人指３,
		左中指１, 左中指２, 左中指３, 左薬指１, 左薬指２, 左薬指３, 左小指１, 左小指２, 左小指３,
		右親指０, 右親指１, 右親指２, 右人指１, 右人指２, 右人指３,
		右中指１, 右中指２, 右中指３, 右薬指１, 右薬指２, 右薬指３, 右小指１, 右小指２, 右小指３)
class VMDSkeleton;
#include "vmd_skeleton.h"

using opt_h = better_enums::optional<HumanBodyBones>;
class VMDBone {
public:
	VMDBoneName name;
	better_enums::optional<VMDBoneName> parent;
	better_enums::optional<HumanBodyBones> source;
	better_enums::optional<HumanBodyBones> target;
	VMDBone(VMDBoneName _name) :
			name(_name) {
		parent = better_enums::optional<VMDBoneName>();
		source = better_enums::optional<HumanBodyBones>();
		target = better_enums::optional<HumanBodyBones>();
	}
	VMDBone(VMDBoneName _name, VMDBoneName _parent) :
			name(_name),
			parent(better_enums::optional<VMDBoneName>(_parent)) {
		source = better_enums::optional<HumanBodyBones>();
		target = better_enums::optional<HumanBodyBones>();
	}
	VMDBone(VMDBoneName _name, VMDBoneName _parent, HumanBodyBones _source) :
			name(_name),
			parent(better_enums::optional<VMDBoneName>(_parent)),
			source(better_enums::optional<HumanBodyBones>(_source)) {
		target = better_enums::optional<HumanBodyBones>();
	}
	VMDBone(VMDBoneName _name, VMDBoneName _parent, HumanBodyBones _source, HumanBodyBones _target) :
			name(_name),
			parent(better_enums::optional<VMDBoneName>(_parent)),
			source(better_enums::optional<HumanBodyBones>(_source)),
			target(better_enums::optional<HumanBodyBones>(_target)) {}
	VMDBone(VMDBoneName _name, VMDBoneName _parent, opt_h(_source), opt_h(_target)) :
			name(_name),
			parent(better_enums::optional<VMDBoneName>(_parent)),
			source(_source),
			target(_target) {}
};

class VMDConstraint {
public:
	virtual ~VMDConstraint() {}
	virtual void apply_constraint(VMDSkeleton *vmd_skeleton, bool apply_ikq) = 0;
};

class RotAdd : public VMDConstraint {
public:
	VMDBoneName target;
	VMDBoneName source;
	bool minus;
	void apply_constraint(VMDSkeleton *vmd_skeleton, bool apply_ikq) override;
	RotAdd(VMDBoneName _target, VMDBoneName _source, bool _minus = false);
};

class LimbIK : public VMDConstraint {
private:
public:
	VMDBoneName source;
	VMDBoneName target_0;
	VMDBoneName target_1;
	VMDBoneName target_2;
	void apply_constraint(VMDSkeleton *vmd_skeleton, bool apply_ikq) override;
	LimbIK(VMDBoneName _source, VMDBoneName _target_0, VMDBoneName _target_1, VMDBoneName _target_2);
};

class LookAt : public VMDConstraint {
public:
	VMDBoneName source_0;
	VMDBoneName source_1;
	VMDBoneName target_0;
	VMDBoneName target_1;
	void apply_constraint(VMDSkeleton *vmd_skeleton, bool apply_ikq) override;
	LookAt(VMDBoneName _source_0, VMDBoneName _source_1, VMDBoneName _target_0, VMDBoneName _target_1);
};

class StandardSkeleton {
public:
	static std::vector<VMDBone> bones;
	static std::vector<std::unique_ptr<VMDConstraint>> constraints;
	static String fix_bone_name(String bone_name) {
		return bone_name.replace("捩れ", "捩").replace("捻", "捩");
	}
};

// We have to do this because implied conversion from const char*
// to String ignores utf8...
String VMDBone2String(VMDBoneName name);
#endif