// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayAbility_Fireball.generated.h"

/**
 * 
 */
UCLASS()
class GAS_API UGameplayAbility_Fireball : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UGameplayAbility_Fireball();

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	virtual bool CheckCost(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fireball")
	TSubclassOf<AActor> FireballClass;

	FName FireSocketName = TEXT("SK_Fire");

};
