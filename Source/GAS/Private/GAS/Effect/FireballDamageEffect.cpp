#include "GAS/Effect/FireballDamageEffect.h"
#include "GAS/EnemyAttributeSet.h"

UFireballDamageEffect::UFireballDamageEffect()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;
	FGameplayModifierInfo& Modifier = Modifiers.AddDefaulted_GetRef();
	Modifier.Attribute = UEnemyAttributeSet::GetDamageAttribute();
	Modifier.ModifierOp = EGameplayModOp::Additive;
	Modifier.ModifierMagnitude = FScalableFloat(10.0f);
}
