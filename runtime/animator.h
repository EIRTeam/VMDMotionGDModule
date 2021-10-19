/*************************************************************************/
/*  animator.h                                                           */
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

#ifndef VMD_ANIMATOR_H
#define VMD_ANIMATOR_H
#include "core/dictionary.h"
#include "scene/3d/skeleton.h"
#include "scene/3d/spatial.h"
#include "scene/3d/mesh_instance.h"
#include <map>
#include <vector>

class VMDBlendShapeBind : public Reference {
	GDCLASS(VMDBlendShapeBind, Reference);
protected:
	static void _bind_methods();
public:
	String shape_vmd_name;
	MeshInstance* mesh;
	int bind_index;
	float bind_weight;
	void create(String p_shape_vmd_name, Node* p_mesh, int p_bind_index, float p_bind_weight);
};

class VMDAnimator : public Spatial {
	GDCLASS(VMDAnimator, Spatial);

private:
	Dictionary humanoid_bone_map;
	Skeleton *skeleton;
	float humanoid_scale = 0.8f;
	std::map<String, std::vector<Ref<VMDBlendShapeBind>>> blend_shape_binds;

protected:
	static void _bind_methods();
	void _notification(int p_what);

public:
	void set_humanoid_bone_map(Dictionary p_humanoid_bone_map);
	Dictionary get_humanoid_bone_map() const;
	int find_humanoid_bone(String p_bone_name) const;

	void set_humanoid_scale(float scale);
	float get_humanoid_scale() const;

	void push_blend_shape(Ref<VMDBlendShapeBind> p_blend_shape);
	void set_blend_shape_value(String p_vmd_shape_name, float value);

	Skeleton *get_skeleton() const;
	VMDAnimator();
};
#endif