// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NativeGameplayTags.h"

/**
 * 
 */
namespace WuKongGameplayTags
{
 /** INPUT TAGS**/
 MYTPPPROJECT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Move)
 MYTPPPROJECT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Look)
 MYTPPPROJECT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_NormalAttack)
 MYTPPPROJECT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_ChargedAttack)
 MYTPPPROJECT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Teleport)
 MYTPPPROJECT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Interact)
 MYTPPPROJECT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_EnhancedAttack)
 MYTPPPROJECT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_DisEnhancedAttack)

 /** PLAY TAGS**/
 MYTPPPROJECT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(PlayTag_Weapon_EnhancedEffector)
}
