/*************************************************************************/
/*  standard_skeleton.cpp                                                */
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

#include "standard_skeleton.h"

#include <tuple>

using opt_b = better_enums::optional<VMDBoneName>;
using opt_h = better_enums::optional<HumanBodyBones>;

using H = HumanBodyBones;
using V = VMDBoneName;

std::vector<VMDBone> StandardSkeleton::bones = []{
        std::vector<VMDBone> v = {
                VMDBone(V::全ての親),
                VMDBone(V::センター, V::全ての親, H::Spine),
                VMDBone(V::グルーブ, V::センター),
                VMDBone(V::腰, V::グルーブ),
                VMDBone(V::下半身,V::腰, H::Spine, H::Hips),
                VMDBone(V::上半身, V::腰, H::Spine, H::Spine),
                VMDBone(V::上半身2, V::上半身, H::Chest, H::Chest),
                VMDBone(V::首, V::上半身2, H::Neck, H::Neck),
                VMDBone(V::頭, V::首, H::Head, H::Head),
                VMDBone(V::両目, V::頭),
                VMDBone(V::左目, V::頭, H::LeftEye, H::LeftEye),
                VMDBone(V::右目, V::頭, H::RightEye, H::RightEye),

                VMDBone(V::左肩P, V::上半身2, H::LeftShoulder),
                VMDBone(V::左肩P, V::左肩P, opt_h(), opt_h(H::LeftShoulder)),
                VMDBone(V::左肩C, V::左肩, H::LeftUpperArm),
                VMDBone(V::左腕, V::左肩C, opt_h(), opt_h(H::LeftUpperArm)),
                VMDBone(V::左腕捩, V::左腕, H::LeftLowerArm),
                VMDBone(V::左ひじ, V::左腕捩, opt_h(), opt_h(H::LeftLowerArm)),
                VMDBone(V::左手捩, V::左ひじ, H::LeftHand),
                VMDBone(V::左手首, V::左手捩, opt_h(), opt_h(H::LeftHand)),

                VMDBone(V::右肩P, V::上半身2, H::RightShoulder),
                VMDBone(V::右肩P, V::右肩P, opt_h(), opt_h(H::RightShoulder)),
                VMDBone(V::右肩C, V::右肩, H::RightUpperArm),
                VMDBone(V::右腕, V::右肩C, opt_h(), opt_h(H::RightUpperArm)),
                VMDBone(V::右腕捩, V::右腕, H::RightLowerArm),
                VMDBone(V::右ひじ, V::右腕捩, opt_h(), opt_h(H::RightLowerArm)),
                VMDBone(V::右手捩, V::右ひじ, H::RightHand),
                VMDBone(V::右手首, V::右手捩, opt_h(), opt_h(H::RightHand)),

                VMDBone(V::左足IK親, V::全ての親, H::LeftFoot),
                VMDBone(V::左足ＩＫ, V::左足IK親),
                VMDBone(V::左つま先ＩＫ, V::左足ＩＫ, H::LeftToes),
                VMDBone(V::左足, V::下半身, H::LeftUpperLeg),
                VMDBone(V::左足D, V::下半身, H::LeftUpperLeg, H::LeftUpperLeg),
                VMDBone(V::左ひざ, V::左足, H::LeftLowerLeg),
                VMDBone(V::左ひざD, V::左足D, H::LeftLowerLeg, H::LeftLowerLeg),
                VMDBone(V::左足首, V::左ひざ, H::LeftFoot),
                VMDBone(V::左足首D, V::左ひざD, H::LeftFoot, H::LeftFoot),
                VMDBone(V::左つま先, V::左足首, H::LeftToes),
                VMDBone(V::左足先EX, V::左足首D, H::LeftToes, H::LeftToes),

                VMDBone(V::右足IK親, V::全ての親, H::RightFoot),
                VMDBone(V::右足ＩＫ, V::右足IK親),
                VMDBone(V::右つま先ＩＫ, V::右足ＩＫ, H::RightToes),
                VMDBone(V::右足, V::下半身, H::RightUpperLeg),
                VMDBone(V::右足D, V::下半身, H::RightUpperLeg, H::RightUpperLeg),
                VMDBone(V::右ひざ, V::右足, H::RightLowerLeg),
                VMDBone(V::右ひざD, V::右足D, H::RightLowerLeg, H::RightLowerLeg),
                VMDBone(V::右足首, V::右ひざ, H::RightFoot),
                VMDBone(V::右足首D, V::右ひざD, H::RightFoot, H::RightFoot),
                VMDBone(V::右つま先, V::右足首, H::RightToes),
                VMDBone(V::右足先EX, V::右足首D, H::RightToes, H::RightToes),
        };

        std::vector<std::tuple<V, V, V, V, H, H, H>> finger_groups = {
                std::make_tuple(V::左手首, V::左親指０, V::左親指１, V::左親指２, H::LeftThumbProximal, H::LeftThumbIntermediate, H::LeftThumbDistal),
                std::make_tuple(V::左手首, V::左人指１, V::左人指２, V::左人指３, H::LeftIndexProximal, H::LeftIndexIntermediate, H::LeftIndexDistal),
                std::make_tuple(V::左手首, V::左中指１, V::左中指２, V::左中指３, H::LeftMiddleProximal, H::LeftMiddleIntermediate, H::LeftMiddleDistal),
                std::make_tuple(V::左手首, V::左薬指１, V::左薬指２, V::左薬指３, H::LeftRingProximal, H::LeftRingIntermediate, H::LeftRingDistal),
                std::make_tuple(V::左手首, V::左小指１, V::左小指２, V::左小指３, H::LeftLittleProximal, H::LeftLittleIntermediate, H::LeftLittleDistal),

                std::make_tuple(V::右手首, V::右親指０, V::右親指１, V::右親指２, H::RightThumbProximal, H::RightThumbIntermediate, H::RightThumbDistal),
                std::make_tuple(V::右手首, V::右人指１, V::右人指２, V::右人指３, H::RightIndexProximal, H::RightIndexIntermediate, H::RightIndexDistal),
                std::make_tuple(V::右手首, V::右中指１, V::右中指２, V::右中指３, H::RightMiddleProximal, H::RightMiddleIntermediate, H::RightMiddleDistal),
                std::make_tuple(V::右手首, V::右薬指１, V::右薬指２, V::右薬指３, H::RightRingProximal, H::RightRingIntermediate, H::RightRingDistal),
                std::make_tuple(V::右手首, V::右小指１, V::右小指２, V::右小指３, H::RightLittleProximal, H::RightLittleIntermediate, H::RightLittleDistal),
        };

        v.reserve(v.size() + finger_groups.size()*3);

        for (auto x : finger_groups) {
                v.push_back(VMDBone(std::get<1>(x), std::get<0>(x), std::get<4>(x), std::get<4>(x)));
                v.push_back(VMDBone(std::get<2>(x), std::get<1>(x), std::get<5>(x), std::get<5>(x)));
                v.push_back(VMDBone(std::get<3>(x), std::get<2>(x), std::get<6>(x), std::get<6>(x)));
        }

        return v;
}();

std::vector<std::unique_ptr<VMDConstraint>> StandardSkeleton::constraints = []{
        std::vector<std::unique_ptr<VMDConstraint>> v;
        v.emplace_back(std::make_unique<RotAdd>(V::左目, V::両目));
        v.emplace_back(std::make_unique<RotAdd>(V::右目, V::両目));
        v.emplace_back(std::make_unique<RotAdd>(V::左肩C, V::左肩P, true));
        v.emplace_back(std::make_unique<RotAdd>(V::右肩C, V::右肩P, true));

        v.emplace_back(std::make_unique<LimbIK>(V::左足ＩＫ, V::左足, V::左ひざ, V::左足首));
        v.emplace_back(std::make_unique<LookAt>(V::左足ＩＫ, V::左つま先ＩＫ, V::左足首, V::左つま先));
        v.emplace_back(std::make_unique<RotAdd>(V::左足D, V::左足));
        v.emplace_back(std::make_unique<RotAdd>(V::左ひざD, V::左ひざ));
        v.emplace_back(std::make_unique<RotAdd>(V::左足首D, V::左足首));

        v.emplace_back(std::make_unique<LimbIK>(V::右足ＩＫ, V::右足, V::右ひざ, V::右足首));
        v.emplace_back(std::make_unique<LookAt>(V::右足ＩＫ, V::右つま先ＩＫ, V::右足首, V::右つま先));
        v.emplace_back(std::make_unique<RotAdd>(V::右足D, V::右足));
        v.emplace_back(std::make_unique<RotAdd>(V::右ひざD, V::右ひざ));
        v.emplace_back(std::make_unique<RotAdd>(V::右足首D, V::右足首));
        return v;
}();

RotAdd::RotAdd(VMDBoneName _target, VMDBoneName _source, bool _minus) :
        target(_target), 
        source(_source), 
        minus(_minus) {}
void RotAdd::apply_constraint(VMDSkeleton *vmd_skeleton, bool apply_ikq) {
        Spatial *trg = vmd_skeleton->bones[target].node;
        Spatial *src = vmd_skeleton->bones[source].node;

        if (minus) {
                Spatial* parent = Object::cast_to<Spatial>(src->get_parent());
                ERR_FAIL_COND_MSG(!parent, "Parent is not a Spatial?");
                Basis out = parent->get_global_transform().basis * src->get_global_transform().basis.inverse() * trg->get_global_transform().basis;
                
                Transform trg_trf = trg->get_global_transform();
                trg_trf.basis = out;
                trg->set_global_transform(trg_trf);
        } else {
                Transform trg_trf = trg->get_transform();
                trg_trf.basis = src->get_transform().basis * trg->get_transform().basis;
                trg->set_transform(trg_trf);
        }
}

static float calc_bend(Vector3 v0, Vector3 v1, float dist) {
        Vector2 u0 = Vector2(v0.y, v0.z);
        Vector2 u1 = Vector2(v1.y, v1.z);
        float dot = (dist*dist - v0.length_squared() - v1.length_squared())/2 - v0.x*v1.x;
        u1 = Vector2(u0.x*u1.x + u0.y*u1.y, u0.x*u1.y - u1.x*u0.y);;
        return MAX(0.0, acos(CLAMP(dot/u1.length(), -1, 1)) - Math::atan2(u1.y, u1.x));
}

LimbIK::LimbIK(VMDBoneName _source, VMDBoneName _target_0, VMDBoneName _target_1, VMDBoneName _target_2) :
        source(_source), 
        target_0(_target_0), 
        target_1(_target_1), 
        target_2(_target_2) {}

void LimbIK::apply_constraint(VMDSkeleton *vmd_skeleton, bool apply_ikq) {
        Spatial *upper_leg = vmd_skeleton->bones[target_0].node;
        Spatial *lower_leg = vmd_skeleton->bones[target_1].node;
        Spatial *foot = vmd_skeleton->bones[target_2].node;
        VMDSkeleton::Bone foot_ik = vmd_skeleton->bones[source];

        if (!foot_ik.ik_enabled) {
                return;
        }

        Vector3 local_target = upper_leg->get_global_transform().xform_inv(foot_ik.node->get_global_transform().origin);
        float bend = -calc_bend(lower_leg->get_transform().origin, foot->get_transform().origin, local_target.length());
        
        {
                Transform lower_leg_trf = lower_leg->get_transform();
                lower_leg_trf.basis = Basis(Quat(sin(bend/2.0), 0, 0, cos(bend/2.0)));
                lower_leg->set_transform(lower_leg_trf);
        }

        Vector3 from = upper_leg->get_global_transform().xform_inv(foot->get_global_transform().origin);
        Vector3 to = local_target;
        
        {
                Transform upper_leg_trf = upper_leg->get_transform();
                upper_leg_trf.basis = Basis(upper_leg_trf.basis.get_rotation_quat() * VMDUtils::quat_from_to_rotation(from, to));
                upper_leg->set_transform(upper_leg_trf);
        }
}

LookAt::LookAt(VMDBoneName _source_0, VMDBoneName _source_1, VMDBoneName _target_0, VMDBoneName _target_1) :
        source_0(_source_0), 
        source_1(_source_1), 
        target_0(_target_0), 
        target_1(_target_1) {}
void LookAt::apply_constraint(VMDSkeleton *vmd_skeleton, bool apply_ikq) {
        Spatial *foot = vmd_skeleton->bones[target_0].node;
        Spatial *toe = vmd_skeleton->bones[target_1].node;
        VMDSkeleton::Bone foot_ik = vmd_skeleton->bones[source_0];
        VMDSkeleton::Bone toe_ik = vmd_skeleton->bones[source_1];

        if (!foot_ik.ik_enabled) {
                return;
        }

        if (apply_ikq) {
                Transform foot_trf = foot->get_global_transform();
                foot_trf.basis = foot_ik.node->get_global_transform().basis;
                foot->set_global_transform(foot_trf);
        }

        if (toe_ik.ik_enabled) {
                Transform foot_trf = foot->get_global_transform();
                Basis basis = VMDUtils::quat_from_to_rotation(toe->get_transform().origin, foot_trf.xform_inv(toe_ik.node->get_global_transform().origin));
                foot_trf.basis *= basis;
                foot->set_global_transform(basis);
        }
}