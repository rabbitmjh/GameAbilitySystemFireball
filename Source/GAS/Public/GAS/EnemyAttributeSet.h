// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectExtension.h"
#include "EnemyAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class GAS_API UEnemyAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UEnemyAttributeSet();
	// CurrentValue 변경 전에 실행되는 함수
	// 값의 Clamping용도로 사용
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	// CurrentValue 변경 후에 실행되는 함수
	// 값의 변화 감지나, UI에 반영하기 위해 사용
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	// 이펙트가 적용 된 후에 실행되는 함수
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(BlueprintReadOnly, Category = "Base Stat", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS_BASIC(UEnemyAttributeSet, Health);

	UPROPERTY(BlueprintReadOnly, Category = "Base Stat", ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS_BASIC(UEnemyAttributeSet, MaxHealth);

	UPROPERTY(BlueprintReadOnly, Category = "Base Stat", Replicated)
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS_BASIC(UEnemyAttributeSet, Mana);

	UPROPERTY(BlueprintReadOnly, Category = "Base Stat", Replicated)
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS_BASIC(UEnemyAttributeSet, MaxMana);

	UPROPERTY(BlueprintReadOnly, Category = "Base Stat")
	FGameplayAttributeData AttackPower;
	ATTRIBUTE_ACCESSORS_BASIC(UEnemyAttributeSet, AttackPower);

	UPROPERTY(BlueprintReadOnly, Category = "Base Stat")
	FGameplayAttributeData CriticalChance;
	ATTRIBUTE_ACCESSORS_BASIC(UEnemyAttributeSet, CriticalChance);

	UPROPERTY(BlueprintReadOnly, Category = "Base Stat")
	FGameplayAttributeData DefencePower;
	ATTRIBUTE_ACCESSORS_BASIC(UEnemyAttributeSet, DefencePower);

	UPROPERTY(BlueprintReadOnly, Category = "Movement Stat")
	FGameplayAttributeData MoveSpeed;
	ATTRIBUTE_ACCESSORS_BASIC(UEnemyAttributeSet, MoveSpeed);

	UPROPERTY(BlueprintReadOnly, Category = "Meta Attribute")
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS_BASIC(UEnemyAttributeSet, Damage);

	UPROPERTY(BlueprintReadOnly, Category = "Meta Attribute")
	FGameplayAttributeData ManaCost;
	ATTRIBUTE_ACCESSORS_BASIC(UEnemyAttributeSet, ManaCost);


	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth);

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);
};
