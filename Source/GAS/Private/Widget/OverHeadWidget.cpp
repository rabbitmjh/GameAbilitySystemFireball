// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/OverHeadWidget.h"
#include "Widget/HealthBarWidget.h"
#include "GAS/EnemyAttributeSet.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UOverHeadWidget::InitializeWithAbilitySystem(AActor* InActor)
{
	if (!InActor) return;
	IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(InActor);
	if (!ASI) return;
	UAbilitySystemComponent* AbilitySystemComp = ASI->GetAbilitySystemComponent();
	if (!AbilitySystemComp) return;
	ASC = AbilitySystemComp;

	FOnGameplayAttributeValueChange& HealthChange = ASC->GetGameplayAttributeValueChangeDelegate(UEnemyAttributeSet::GetHealthAttribute());
	HealthChange.AddUObject(this, &UOverHeadWidget::OnHealthChanged);

	FOnGameplayAttributeValueChange& MaxHealthChange = ASC->GetGameplayAttributeValueChangeDelegate(UEnemyAttributeSet::GetMaxHealthAttribute());
	MaxHealthChange.AddUObject(this, &UOverHeadWidget::OnMaxHealthChanged);

	bool bFound = false;
	const float TempCurrent = ASC->GetGameplayAttributeValue(UEnemyAttributeSet::GetHealthAttribute(), bFound);
	CurrentHealth = bFound ? TempCurrent : 0.0f;	// 못찾았으면 0

	bFound = false;
	const float TempMax = ASC->GetGameplayAttributeValue(UEnemyAttributeSet::GetMaxHealthAttribute(), bFound);
	MaxHealth = bFound ? TempMax : 100.0f;	// 못찾았으면 100

	UpdateHealthUI(CurrentHealth, MaxHealth);
}

void UOverHeadWidget::OnHealthChanged(const FOnAttributeChangeData & InData)
{
	CurrentHealth = InData.NewValue;
	UpdateHealthUI(CurrentHealth, MaxHealth);
}

void UOverHeadWidget::OnMaxHealthChanged(const FOnAttributeChangeData & InData)
{
	MaxHealth = InData.NewValue;
	UpdateHealthUI(CurrentHealth, MaxHealth);
}

void UOverHeadWidget::UpdateHealthUI(float InCurrent, float InMax)
{
	const float Percent = FMath::IsNearlyZero(InMax) ? 0.0f : FMath::Clamp(InCurrent / InMax, 0.0f, 1.0f);
	if (HealthBar->ProgressBar)
	{
		HealthBar->ProgressBar->SetPercent(Percent);
	}
	if (HealthBar->BarText)
	{
		HealthBar->BarText->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), InCurrent, InMax)));
	}
}
