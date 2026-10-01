// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/StatWidget.h"
#include "GAS/PlayerAttributeSet.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "Widget/HealthBarWidget.h"
#include "Widget/ManaBarWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UStatWidget::InitializeWithAbilitySystem(AActor* InActor)
{
	if (!InActor) return;
	IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(InActor);
	if (!ASI) return;
	UAbilitySystemComponent* AbilitySystemComp = ASI->GetAbilitySystemComponent();
	if (!AbilitySystemComp) return;
	ASC = AbilitySystemComp;

	FOnGameplayAttributeValueChange& HealthChange = ASC->GetGameplayAttributeValueChangeDelegate(UPlayerAttributeSet::GetHealthAttribute());
	HealthChange.AddUObject(this, &UStatWidget::OnHealthChanged);

	FOnGameplayAttributeValueChange& MaxHealthChange = ASC->GetGameplayAttributeValueChangeDelegate(UPlayerAttributeSet::GetMaxHealthAttribute());
	MaxHealthChange.AddUObject(this, &UStatWidget::OnMaxHealthChanged);

	bool bFound = false;
	const float TempCurrent = ASC->GetGameplayAttributeValue(UPlayerAttributeSet::GetHealthAttribute(), bFound);
	CurrentHealth = bFound ? TempCurrent : 0.0f;	// 못찾았으면 0

	bFound = false;
	const float TempMax = ASC->GetGameplayAttributeValue(UPlayerAttributeSet::GetMaxHealthAttribute(), bFound);
	MaxHealth = bFound ? TempMax : 100.0f;	// 못찾았으면 100

	UpdateHealthUI(CurrentHealth, MaxHealth);

	FOnGameplayAttributeValueChange& ManaChange = ASC->GetGameplayAttributeValueChangeDelegate(UPlayerAttributeSet::GetManaAttribute());
	ManaChange.AddUObject(this, &UStatWidget::OnManaChanged);

	FOnGameplayAttributeValueChange& MaxManaChange = ASC->GetGameplayAttributeValueChangeDelegate(UPlayerAttributeSet::GetMaxManaAttribute());
	MaxManaChange.AddUObject(this, &UStatWidget::OnMaxManaChanged);

	bool bManaFound = false;
	const float TempManaCurrent = ASC->GetGameplayAttributeValue(UPlayerAttributeSet::GetManaAttribute(), bManaFound);
	CurrentMana = bManaFound ? TempManaCurrent : 0.0f;	// 못찾았으면 0

	bManaFound = false;
	const float TempManaMax = ASC->GetGameplayAttributeValue(UPlayerAttributeSet::GetMaxManaAttribute(), bManaFound);
	MaxMana = bManaFound ? TempManaMax : 100.0f;	// 못찾았으면 100

	UpdateManaUI(CurrentMana, MaxMana);
}

void UStatWidget::OnHealthChanged(const FOnAttributeChangeData& InData)
{
	CurrentHealth = InData.NewValue;
	UpdateHealthUI(CurrentHealth, MaxHealth);
}

void UStatWidget::OnMaxHealthChanged(const FOnAttributeChangeData & InData)
{
	MaxHealth = InData.NewValue;
	UpdateHealthUI(CurrentHealth, MaxHealth);
}

void UStatWidget::UpdateHealthUI(float InCurrent, float InMax)
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

void UStatWidget::OnManaChanged(const FOnAttributeChangeData & InData)
{
	CurrentMana = InData.NewValue;
	UpdateManaUI(CurrentMana, MaxMana);
}

void UStatWidget::OnMaxManaChanged(const FOnAttributeChangeData & InData)
{
	MaxMana = InData.NewValue;
	UpdateManaUI(CurrentMana, MaxMana);
}

void UStatWidget::UpdateManaUI(float InCurrent, float InMax)
{
	const float Percent = FMath::IsNearlyZero(InMax) ? 0.0f : FMath::Clamp(InCurrent / InMax, 0.0f, 1.0f);
	if (ManaBar->ProgressBar)
	{
		ManaBar->ProgressBar->SetPercent(Percent);
	}
	if (ManaBar->BarText)
	{
		ManaBar->BarText->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), InCurrent, InMax)));
	}
}
