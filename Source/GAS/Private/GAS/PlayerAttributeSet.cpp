// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/PlayerAttributeSet.h"
#include "Net/UnrealNetwork.h"

UPlayerAttributeSet::UPlayerAttributeSet()
{
	InitHealth(100.0f);
	InitMaxHealth(100.0f);

	InitMana(300.0f);
	InitMaxMana(300.0f);

	InitAttackPower(10.0f);
	InitCriticalChance(0.2f);
	InitDefencePower(5.0f);
	InitMoveSpeed(100.0f);

	InitDamage(0.0f);
	InitManaCost(0.0f);
}

void UPlayerAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
	else if (Attribute == GetMaxHealthAttribute())
	{
		NewValue = FMath::Max(0.0f, NewValue);
	}
	else if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxMana());
	}
	else if (Attribute == GetMaxManaAttribute() || Attribute == GetMoveSpeedAttribute())
	{
		NewValue = FMath::Max(0.0f, NewValue);
	}
	else if (Attribute == GetDefencePowerAttribute())
	{
		NewValue = FMath::Max(0.0f, NewValue);
	}
}

void UPlayerAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute == GetMaxHealthAttribute())
	{
		UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent();

		if (!ASC)
		{
			return;
		}

		if (NewValue > OldValue)
		{
			const float IncreaseAmount = NewValue - OldValue;

			ASC->ApplyModToAttributeUnsafe(GetHealthAttribute(), EGameplayModOp::AddBase, IncreaseAmount);
		}
		else if (GetHealth() > NewValue)
		{
			const float DecreaseAmount = GetHealth() - NewValue;

			ASC->ApplyModToAttributeUnsafe(GetHealthAttribute(), EGameplayModOp::AddBase, -DecreaseAmount);
		}
	}

}

void UPlayerAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	// Instant 일때만 실행이 됨
	Super::PostGameplayEffectExecute(Data);

	//if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	//{
	//	// 이팩트로 인해 변경된 어트리뷰트가 Health다
	//}

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		const float LocalDamage = GetDamage();
		SetDamage(0.0f);	// [가장 중요] : 메타 어트리뷰트는 사용했으면 비워야 한다.

		if (LocalDamage > 0)
		{
			float FinalDamage = FMath::Max(0, LocalDamage - GetDefencePower());	// 각 종 계산 추가(방어력, 최소대미지보장, 쉴드, 피해 증가 등등)
			UE_LOG(LogTemp, Log, TEXT("최초대미지: %f 방어력: %f 최종 대미지: %f"), LocalDamage, GetDefencePower(), FinalDamage);

			FinalDamage = FMath::Max(1.0f, FinalDamage);	// 최소 대미지 보장

			const float NewHealth = FMath::Clamp(GetHealth() - FinalDamage, 0.0f, GetMaxHealth());
			SetHealth(NewHealth);
		}
	}
	else if (Data.EvaluatedData.Attribute == GetManaCostAttribute())
	{
		float LocalManaCost = GetManaCost();
		SetManaCost(0.0f);	// // [가장 중요] : 메타 어트리뷰트는 사용했으면 비워야 한다.

		if (LocalManaCost > 0)
		{
			float FinalManaCost = LocalManaCost;
			FinalManaCost = FMath::Max(0.0f, FinalManaCost);

			const float NewMana = FMath::Clamp(GetMana() - LocalManaCost, 0.0f, GetMaxMana());

			SetMana(NewMana);
		}
	}
}

void UPlayerAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
}

void UPlayerAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, Health, OldHealth);
}

void UPlayerAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, MaxHealth, OldMaxHealth);
}
