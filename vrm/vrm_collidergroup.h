#include "scene/3d/spatial.h"
#include "core/math/vector3.h"
#include "scene/3d/skeleton.h"
#include "core/vector.h"
#include "core/math/plane.h"

class VRMColliderGroup {
    Vector<VRMSphereCollider> colliders;
    Vector <Plane> sphere_colliders;
public:
    void setup();
    VRMColliderGroup(Skeleton *p_skeleton, int p_bone_idx, Vector<Plane> p_colliders);
};

class VRMCollider {
public:
    virtual void update(Skeleton* p_skeleton);
    virtual Vector3 get_position() const;
    virtual float get_radius() const;
};

class VRMSphereCollider: public VRMCollider {
private:
    int bone_idx;
    Vector3 offset;
    float radius;
    Vector3 position;
public:
    void update(Skeleton* p_skeleton) override;
    Vector3 get_position() const override;
    float get_radius() const override;

};