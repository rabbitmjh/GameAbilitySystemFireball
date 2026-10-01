// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/GameplayAbility_Fireball.h"
#include "GAS/PlayerAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "GameplayEffect.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"

UGameplayAbility_Fireball::UGameplayAbility_Fireball()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGameplayAbility_Fireball::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	ACharacter* Player = Cast<ACharacter>(GetAvatarActorFromActorInfo());

	if (!Player || !FireballClass || !Player->GetMesh() || !Player->GetWorld() || !Player->GetMesh()->DoesSocketExist(FireSocketName))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);	// 마지막 true는 정상적인 종료가 아니라는 표시
		return;
	}

	// 코스트와 쿨다운 검사 후, 가능하면 적용
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);	// 마지막 true는 정상적인 종료가 아니라는 표시
		return;
	}

	const FVector SpawnLocation = Player->GetMesh()->GetSocketLocation(FireSocketName);
	const FRotator SpawnRotation = Player->GetActorRotation();

	FActorSpawnParameters Params;
	Params.Owner = Player;
	Params.Instigator = Player;
	Params.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	GetWorld()->SpawnActor<AActor>(
		FireballClass,
		SpawnLocation,
		SpawnRotation,
		Params);

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);	// 성공적으로 끝났다.

}

bool UGameplayAbility_Fireball::CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo * ActorInfo, OUT FGameplayTagContainer * OptionalRelevantTags) const
{
	// 필요한 데이터들 있는지 확인
	UGameplayEffect* CostGE = GetCostGameplayEffect();
	if (!CostGE) return true;

	UAbilitySystemComponent* ASC = ActorInfo ? ActorInfo->AbilitySystemComponent.Get() : nullptr;
	if (!ASC) return false;

	if (!ASC->HasAttributeSetForAttribute(UPlayerAttributeSet::GetManaAttribute())
		|| !ASC->HasAttributeSetForAttribute(UPlayerAttributeSet::GetManaCostAttribute()))
		return false;

	// 시전자의 현재 Mana값 가져오기
	const float CurrentMana = ASC->GetNumericAttribute(UPlayerAttributeSet::GetManaAttribute());

	// 필요 소모량을 알기 위해 Spec 생성
	const FGameplayEffectContextHandle EffectContext = MakeEffectContext(Handle, ActorInfo);
	const float AbilityLevel = GetAbilityLevel(Handle, ActorInfo);
	FGameplayEffectSpec Spec(CostGE, EffectContext, AbilityLevel);
	Spec.CalculateModifierMagnitudes();	// 이 스펙에 해당하는 모디파이어 계산

	// GE에서 ManaCost 추출하기
	float ManaCost = 0.0f;
	bool bFoundStaminaModifier = false;
	for (int32 ModIndex = 0; ModIndex < Spec.Modifiers.Num(); ModIndex++)
	{
		if (Spec.Def && Spec.Def->Modifiers.IsValidIndex(ModIndex))
		{
			const FGameplayModifierInfo& ModDef = Spec.Def->Modifiers[ModIndex];
			if (ModDef.Attribute == UPlayerAttributeSet::GetManaCostAttribute())
			{
				const FModifierSpec& ModSpec = Spec.Modifiers[ModIndex];
				ManaCost += ModSpec.GetEvaluatedMagnitude();
				bFoundStaminaModifier = true;
			}
		}
	}

	// GE에 StaminaCost관련 모디파이어가 없으면 기본 로직 실행
	if (!bFoundStaminaModifier)
	{
		return Super::CheckCost(Handle, ActorInfo, OptionalRelevantTags);
	}

	// 충분한 스테미너가 있으면 성공
	if (ManaCost <= CurrentMana)
	{
		return true;
	}

	// 스테미너가 없어서 실패했다고 OptionalRelevantTags에 ActivateFailCostTag 기록하기
	const FGameplayTag& CostTag = UAbilitySystemGlobals::Get().ActivateFailCostTag;
	if (OptionalRelevantTags && CostTag.IsValid())
	{
		OptionalRelevantTags->AddTag(CostTag);
	}

	return false;
}

