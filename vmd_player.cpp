#include "vmd_player.h"
#include "vmd.h"
#include "motion.h"
#include "core/os/os.h"
#include <memory>
Error VMDPlayer::load_motions(PoolStringArray motions) {
    print_line(vformat("%d", motions.size()));
    ERR_FAIL_COND_V_MSG(animator == nullptr, ERR_INVALID_PARAMETER, "You should really have an animator as the parent of the vmd player");
    std::vector<std::unique_ptr<VMD>> vmds;
    motion = VMDMotion();
    
    for (int i = 0; i < motions.size(); i++) {
        String str = motions.get(i);
        VMD vmd;
        Error err = vmd.load(str);
        motion.add_clip(&vmd);
        ERR_FAIL_COND_V_MSG(err != OK, err, vformat("Error loading vmd file from %s", str));
    }

    motion.process_vmds();

    for (auto it = motion.bones.begin(); it != motion.bones.cend();) {
        String bone_name = StandardSkeleton::fix_bone_name(it->first);
        if (bone_name != it->first) {
            print_line(vformat("Bone rename %s => %s", it->first, bone_name));
            motion.bones.erase(it++);
            motion.bones.insert(std::make_pair(it->first, it->second));
        } else {
            ++it;
        }
    }

    bone_curves.clear();

    bone_curves.reserve(VMDBoneName::_size());

    for (auto bone : VMDBoneName::_values()) {
        auto it = motion.bones.find(String(bone._to_string()));
        if (it != motion.bones.end()) {
            bone_curves.push_back(it->second);
        } else {
            VMDMotion::BoneCurve curve;
            bone_curves.push_back(curve);
        }
    }

    max_frame = motion.get_max_frame();
    for (auto bone : std::vector<VMDBoneName> { VMDBoneName::全ての親, 
            VMDBoneName::センター,
            VMDBoneName::左足ＩＫ,
            VMDBoneName::右足ＩＫ }) {
        std::vector<VMD::BoneKeyframe> keyframes = bone_curves[bone].keyframes;
        if (keyframes.size() > 2 && keyframes[0].frame_number == 0) {
            bool linear_motion_t = keyframes[0].position != keyframes[1].position \
                && keyframes[1].interp.X.is_linear() && keyframes[1].interp.Y.is_linear() \
                && keyframes[1].interp.Z.is_linear();
            bool linear_motion_q = keyframes[0].rotation != keyframes[1].rotation \
                && keyframes[1].interp.rotation.is_linear();
            if (linear_motion_t || linear_motion_q) {
                first_frame_number = MAX(first_frame_number, keyframes[1].frame_number);
                print_line(vformat("Skipping frame (%s, %d)", keyframes[1].name, keyframes[1].frame_number));
            }
        }
    }

    for (auto bone : std::vector<VMDBoneName> {VMDBoneName::左足ＩＫ, VMDBoneName::右足ＩＫ}) {
        VMDMotion::BoneCurve curve = bone_curves[bone];
        for (VMD::BoneKeyframe kf : curve.keyframes) {
            if (kf.rotation != Quat()) {
                apply_ikq = true;
                break;
            }
        } 
        if (apply_ikq) {
            break;
        }
    }

    if (!skeleton) {
        print_line("CREATING SKELETON!");
        anim_scale = 0.07*animator->get_humanoid_scale();
        skeleton = std::make_unique<VMDSkeleton>(animator, this);
        // TODO: Port source overrides for some avatar bones
        // https://gitlab.com/lox9973/VMDMotion/-/blob/master/Script/Runtime/VMDPlayer.cs#L83
    }
    print_line("DAME DA NE!");

    for (auto bone : std::vector<VMDBoneName> { VMDBoneName::左足ＩＫ, VMDBoneName::左つま先ＩＫ,
            VMDBoneName::右足ＩＫ, VMDBoneName::右つま先ＩＫ }) {
        skeleton->bones[bone].ik_enabled = bone_curves[bone].keyframes.size() > 1;
    }

    scale_overrides.resize(skeleton->bones.size());
    print_line(vformat("%d", skeleton->bones.size()));
    std::fill(scale_overrides.begin(), scale_overrides.end(), 0.0);

    for (auto bone : std::vector<VMDBoneName> {VMDBoneName::左つま先ＩＫ, VMDBoneName::右つま先ＩＫ})  {
        Vector3 curve_local_pos_0 = -(bone_curves[bone].estimate_rotation_center_from_pos());
        Vector3 bone_local_pos_0 = skeleton->bones[bone].local_position_0;
        scale_overrides[bone] = bone_local_pos_0.length() / curve_local_pos_0.length();
    }

    set_process(true);
    start_time = OS::get_singleton()->get_ticks_msec();
    
    return OK;
}

void VMDPlayer::_notification(int p_what) {
    switch(p_what) {
        case NOTIFICATION_PARENTED: {
            animator = Object::cast_to<VMDAnimator>(get_parent());
        } break;
        case NOTIFICATION_UNPARENTED: {
            animator = nullptr;
        } break;
        case NOTIFICATION_PROCESS: {
            time = (OS::get_singleton()->get_ticks_msec()-start_time) / 1000.0;
            float frame = time * VMD_FPS;
            update_frame(frame);
        } break;
    }
}

void VMDPlayer::update_frame(float frame) {
    apply_bone_frame(frame);
}

void VMDPlayer::_bind_methods() {
    ClassDB::bind_method(D_METHOD("load_motions", "motions"), &VMDPlayer::load_motions);
}

void VMDPlayer::apply_bone_frame(float frame) {
    frame = MAX(frame, 0.0f);
    for (uint64_t i = 0; i < skeleton->bones.size(); i++) {
        VMDSkeleton::Bone *bone = &skeleton->bones[i];
        VMDMotion::BoneCurve *curve = &bone_curves[i];
        Vector3 position;
        Quat rotation;
        curve->sample(frame, &position, &rotation);
        float scal = scale_overrides[bone->name];
        if (scal == 0) {
            scal = anim_scale;
        }
        position *= scal;

        if (bone->name == +VMDBoneName::全ての親 || bone->name == +VMDBoneName::センター ||
                bone->name == +VMDBoneName::左足ＩＫ || bone->name == +VMDBoneName::右足ＩＫ) {
            position *= locomotion_scale;
        }

        Transform bone_trf = bone->node->get_transform();
        bone_trf.origin = position + bone->local_position_0;
        bone_trf.basis = Basis(rotation);
        bone->node->set_transform(bone_trf);
    }
}

void VMDPlayer::apply_ik_frame(float frame) {
    frame = MAX(frame, 0.0f);
    std::vector<std::tuple<String, bool>> current_ik_enable = motion.ik.sample(frame);
    if (last_ik_enable_frame == Math::floor(frame)) {
        return;
    }
    last_ik_enable = current_ik_enable;
    last_ik_enable_frame = Math::floor(frame);

    for (auto pair : current_ik_enable) {
        String name_str = std::get<0>(pair);
        bool enable = std::get<1>(pair);
        std::wstring ws = name_str.c_str();
        auto name_opt = VMDBoneName::_from_string_nothrow(std::string(ws.begin(), ws.end()).c_str());
        if (name_opt) {
            VMDBoneName name = *name_opt;
            skeleton->bones[name].ik_enabled = enable;
        }
    }

}