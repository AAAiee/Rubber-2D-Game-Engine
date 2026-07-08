#pragma once
#include <RB.h>
#include <array>


static  inline std::array<RB::AssetMetaData, 2> s_Texture2DImage = {
   RB::AssetMetaData{ "BackGround"  ,RB::AssetType::Texture2D,   "Asset/background/background.png"},
   RB::AssetMetaData{ "UI"          ,RB::AssetType::Texture2D,   "Asset/ui/ui_heart.png" }
};

// Atlas 
static  inline std::array<RB::AnimationResourcesMetaData, 17> s_Atlas = {
	RB::AnimationResourcesMetaData{"Enemy_Left_Aim",         RB::AssetType::AnimationAtlas,    R"(Asset\enemy\aim)",                 9 },
	RB::AnimationResourcesMetaData{"Enemy_Left_BarbBreak",   RB::AssetType::AnimationAtlas,    R"(Asset\enemy\barb_break)" ,         3 },
	RB::AnimationResourcesMetaData{"Enemy_Left_BarbLoose",   RB::AssetType::AnimationAtlas,    R"(Asset\enemy\barb_loose)" ,         5 },
	RB::AnimationResourcesMetaData{"Enemy_Left_DashInAir",   RB::AssetType::AnimationAtlas,    R"(Asset\enemy\dash_in_air)" ,        2 },
	RB::AnimationResourcesMetaData{"Enemy_Left_DashOnFloor", RB::AssetType::AnimationAtlas,    R"(Asset\enemy\dash_on_floor)" ,      2 },
	RB::AnimationResourcesMetaData{"Enemy_Left_Fall",        RB::AssetType::AnimationAtlas,    R"(Asset\enemy\fall)" ,               4 },
	RB::AnimationResourcesMetaData{"Enemy_Left_Idle",        RB::AssetType::AnimationAtlas,    R"(Asset\enemy\idle)" ,               6 },
	RB::AnimationResourcesMetaData{"Enemy_Left_Jump",        RB::AssetType::AnimationAtlas,    R"(Asset\enemy\jump)" ,               8 },
	RB::AnimationResourcesMetaData{"Enemy_Left_Run",         RB::AssetType::AnimationAtlas,    R"(Asset\enemy\run)" ,                8 },
	RB::AnimationResourcesMetaData{"Enemy_Left_Silk",        RB::AssetType::AnimationAtlas,    R"(Asset\enemy\silk)" ,               9 },
	RB::AnimationResourcesMetaData{"Enemy_Left_Squat",       RB::AssetType::AnimationAtlas,    R"(Asset\enemy\squat)" ,              10},
	RB::AnimationResourcesMetaData{"Enemy_Left_Sword",       RB::AssetType::AnimationAtlas,    R"(Asset\enemy\sword)" ,              3 },
	RB::AnimationResourcesMetaData{"Enemy_Left_ThrowBarb",   RB::AssetType::AnimationAtlas,    R"(Asset\enemy\throw_barb)" ,         8 },
	RB::AnimationResourcesMetaData{"Enemy_Left_ThrowSilk",   RB::AssetType::AnimationAtlas,    R"(Asset\enemy\throw_silk)" ,         17},
	RB::AnimationResourcesMetaData{"Enemy_Left_ThrowSword",  RB::AssetType::AnimationAtlas,    R"(Asset\enemy\throw_sword)" ,        3 },
	RB::AnimationResourcesMetaData{"Enemy_Left_VDashInAir",  RB::AssetType::AnimationAtlas,    R"(Asset\enemy\vfx_dash_in_air)" ,    5 },
	RB::AnimationResourcesMetaData{"Enemy_Left_VDashOnFloor",RB::AssetType::AnimationAtlas,    R"(Asset\enemy\vfx_dash_on_floor)"   ,6 }
};

//SpriteSheet
static  inline std::array<RB::AnimationResourcesMetaData, 13> s_SpriteSheet = {
	RB::AnimationResourcesMetaData{"Player_Right_Attack",      RB::AssetType::AnimationSpriteSheet,     R"(Asset\player\attack.png)",                 5 },
	RB::AnimationResourcesMetaData{"Player_Right_Dead",        RB::AssetType::AnimationSpriteSheet,     R"(Asset\player\dead.png)" ,                  6 },
	RB::AnimationResourcesMetaData{"Player_Right_Fall",        RB::AssetType::AnimationSpriteSheet,     R"(Asset\player\fall.png)" ,                  5 },
	RB::AnimationResourcesMetaData{"Player_Right_Idle",        RB::AssetType::AnimationSpriteSheet,     R"(Asset\player\idle.png)" ,                  5 },
	RB::AnimationResourcesMetaData{"Player_Right_Jump",        RB::AssetType::AnimationSpriteSheet,     R"(Asset\player\jump.png)" ,                  5 },
	RB::AnimationResourcesMetaData{"Player_Right_Roll",        RB::AssetType::AnimationSpriteSheet,     R"(Asset\player\roll.png)" ,                  7 },
	RB::AnimationResourcesMetaData{"Player_Right_Run",         RB::AssetType::AnimationSpriteSheet,     R"(Asset\player\run.png)" ,                   10},
	RB::AnimationResourcesMetaData{"Player_VAttackDown",       RB::AssetType::AnimationSpriteSheet,     R"(Asset\player\vfx_attack_down.png)" ,       5 },
	RB::AnimationResourcesMetaData{"Player_VAttackLeft",       RB::AssetType::AnimationSpriteSheet,     R"(Asset\player\vfx_attack_left.png)" ,       5 },
	RB::AnimationResourcesMetaData{"Player_VAttackRight",      RB::AssetType::AnimationSpriteSheet,     R"(Asset\player\vfx_attack_right.png)" ,      5 },
	RB::AnimationResourcesMetaData{"Player_VAttackUp",         RB::AssetType::AnimationSpriteSheet,     R"(Asset\player\vfx_attack_up.png)" ,         5 },
	RB::AnimationResourcesMetaData{"Player_VJump",       RB::AssetType::AnimationSpriteSheet,     R"(Asset\player\vfx_jump.png)" ,              5 },
	RB::AnimationResourcesMetaData{"Player_VLand",       RB::AssetType::AnimationSpriteSheet,     R"(Asset\player\vfx_land.png)" ,              2 }
};
