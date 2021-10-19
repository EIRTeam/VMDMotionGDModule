#include "scene/3d/spatial.h"
#include "scene/3d/skeleton.h"
#include "core/math/vector3.h"
#include "core/math/transform.h"

class VRMSpringBone : public Resource {
    Skeleton *skeleton;
    Vector<VRMSpringBoneLogic> verlets;
    Transform center;
protected:
    void _notification( int p_what);
    void process(float p_delta);
public:
    float stiffness_force = 1.0;
    float gravity = 0.0;
    Vector3 gravity_dir = Vector3(0.0, -1.0, 0.0);
    float gravity_power = 0.0;
    float hit_radius = 0.05;
};

class VRMSpringBoneLogic : public Reference {
GDCLASS(VRMSpringBoneLogic, Reference);
protected:
    void _notification(int p_what);
public:
    bool force_update = true;
    int bone_idx;
    float radius;
    float length;

    Vector3 bone_axis;
    Vector3 current_tail;
    Vector3 prev_tail;

    Transform initial_transform;

    Transform get_transform(Skeleton *p_skeleton) const;
    Quat get_rotation(Skeleton *p_skeleton) const;
    Transform get_local_transform(Skeleton *p_skeleton) const;
    Quat get_local_rotation(Skeleton *p_skeleton) const;

    void reset(Skeleton *p_skeleton);
    void update(Skeleton *p_skeleton, Transform *p_center, float p_stiffness_force, float p_drag_force, Vector3 p_external, Array p_colliders);

    void collision(Skeleton *p_skeleton, Array p_colliders, Vector3 _next_tail);

    VRMSpringBoneLogic(Skeleton *p_skeleton, int p_bone_idx, Transform *p_center, Vector3 p_local_child_position, Transform p_default_pose);
};