#ifndef VMD_STANDARD_SKELETON_BONES_H
#define VMD_STANDARD_SKELETON_BONES_H
#include "better_enums/better_enums.h"

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
#endif