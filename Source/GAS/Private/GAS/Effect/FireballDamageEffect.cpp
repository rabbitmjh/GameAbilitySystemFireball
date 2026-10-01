#include "GAS/Effect/FireballDamageEffect.h"
#include "GAS/EnemyAttributeSet.h"

UFireballDamageEffect::UFireballDamageEffect()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;
	FGameplayModifierInfo& Modifier = Modifiers.AddDefaulted_GetRef();
	Modifier.Attribute = UEnemyAttributeSet::GetDamageAttribute();
	Modifier.ModifierOp = EGameplayModOp::Additive;
	FSetByCallerFloat DamageMagnitude;
	DamageMagnitude.DataName = FName(TEXT("FireballDamage"));
	Modifier.ModifierMagnitude = FGameplayEffectModifierMagnitude(DamageMagnitude);
}
