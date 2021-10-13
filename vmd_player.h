#ifndef VMD_PLAYER_H
#define VMD_PLAYER_H

#include "scene/3d/spatial.h"
#include "core/array.h"
#include "motion.h"
#include <memory>

const float VMD_FPS = 30.0f;

class VMDPlayer : public Spatial {
    GDCLASS(VMDPlayer, Spatial);
private:
    
    int max_frame = 0;
    int first_frame_number = 0;
    bool apply_ikq = false;
    float anim_scale = 0.08;
    int64_t start_time;
    float time = 0.0f;
    VMDAnimator* animator;
    Vector3 locomotion_scale = Vector3(1, 1, 1);
    VMDMotion motion;
    std::vector<float> scale_overrides;
    std::vector<std::tuple<String, bool>> last_ik_enable;
    int last_ik_enable_frame = 0;
protected:
    void _notification(int p_what);
    static void _bind_methods();
public:
    std::vector<VMDMotion::BoneCurve> bone_curves;
    std::unique_ptr<VMDSkeleton> skeleton;
    Error load_motions(PoolStringArray motions);
    void update_frame(float frame);
    void apply_bone_frame(float frame);
    void apply_ik_frame(float frame);
};

#endif