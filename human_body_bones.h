/*************************************************************************/
/*  human_body_bones.h                                                   */
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

#include "better_enums/better_enums.h"

BETTER_ENUM(HumanBodyBones, int,
		// This is the Hips bone
		hips = 0,

		// This is the Left Upper Leg bone
		leftUpperLeg = 1,

		// This is the Right Upper Leg bone
		rightUpperLeg = 2,

		// This is the Left Knee bone
		leftLowerLeg = 3,

		// This is the Right Knee bone
		rightLowerLeg = 4,

		// This is the Left Ankle bone
		leftFoot = 5,

		// This is the Right Ankle bone
		rightFoot = 6,

		// This is the first Spine bone
		spine = 7,

		// This is the Chest bone
		chest = 8,

		// This is the UpperChest bone
		upperChest = 54,

		// This is the Neck bone
		neck = 9,

		// This is the Head bone
		head = 10,

		// This is the Left Shoulder bone
		leftShoulder = 11,

		// This is the Right Shoulder bone
		rightShoulder = 12,

		// This is the Left Upper Arm bone
		leftUpperArm = 13,

		// This is the Right Upper Arm bone
		rightUpperArm = 14,

		// This is the Left Elbow bone
		leftLowerArm = 15,

		// This is the Right Elbow bone
		rightLowerArm = 16,

		// This is the Left Wrist bone
		leftHand = 17,

		// This is the Right Wrist bone
		rightHand = 18,

		// This is the Left Toes bone
		leftToes = 19,

		// This is the Right Toes bone
		rightToes = 20,

		// This is the Left Eye bone
		leftEye = 21,

		// This is the Right Eye bone
		rightEye = 22,

		// This is the Jaw bone
		jaw = 23,

		leftThumbProximal = 24,
		leftThumbIntermediate = 25,
		leftThumbDistal = 26,

		leftIndexProximal = 27,
		leftIndexIntermediate = 28,
		leftIndexDistal = 29,

		leftMiddleProximal = 30,
		leftMiddleIntermediate = 31,
		leftMiddleDistal = 32,

		leftRingProximal = 33,
		leftRingIntermediate = 34,
		leftRingDistal = 35,

		leftLittleProximal = 36,
		leftLittleIntermediate = 37,
		leftLittleDistal = 38,

		rightThumbProximal = 39,
		rightThumbIntermediate = 40,
		rightThumbDistal = 41,

		rightIndexProximal = 42,
		rightIndexIntermediate = 43,
		rightIndexDistal = 44,

		rightMiddleProximal = 45,
		rightMiddleIntermediate = 46,
		rightMiddleDistal = 47,

		rightRingProximal = 48,
		rightRingIntermediate = 49,
		rightRingDistal = 50,

		rightLittleProximal = 51,
		rightLittleIntermediate = 52,
		rightLittleDistal = 53,

		//app0:/game_dataUpperChest = 54,

		// This is the Last bone index delimiter
		LastBone = 55)