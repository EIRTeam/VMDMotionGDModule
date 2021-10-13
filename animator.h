#ifndef VMD_ANIMATOR_H
#define VMD_ANIMATOR_H
#include "scene/3d/spatial.h"
#include "core/dictionary.h"
#include "scene/3d/skeleton.h"
class VMDAnimator : public Spatial {
    GDCLASS(VMDAnimator, Spatial);
private:
    Dictionary humanoid_bone_map;
    Skeleton* skeleton;
    float humanoid_scale = 0.8f;
protected:
    static void _bind_methods();
    void _notification(int p_what);
public:
    void set_humanoid_bone_map(Dictionary dict);
    int find_humanoid_bone(String bone_name);

    void set_humanoid_scale(float scale);
    float get_humanoid_scale();

    Skeleton* get_skeleton();
    VMDAnimator();
};
#endif