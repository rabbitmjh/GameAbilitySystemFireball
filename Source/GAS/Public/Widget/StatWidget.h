// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayEffectTypes.h"
#include "StatWidget.generated.h"

class UHealthBarWidget;
class UManaBarWidget;
/**
 * 
 */
UCLASS()
class GAS_API UStatWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void InitializeWithAbilitySystem(AActor* InActor);

	void OnHealthChanged(const FOnAttributeChangeData& InData);
	void OnMaxHealthChanged(const FOnAttributeChangeData& InData);
	void UpdateHealthUI(float InCurrent, float InMax);

	void OnManaChanged(const FOnAttributeChangeData& InData);
	void OnMaxManaChanged(const FOnAttributeChangeData& InData);
	void UpdateManaUI(float InCurrent, float InMax);

protected:
	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<UAbilitySystemComponent> ASC;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentHealth = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentMana = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxMana = 100.0f;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<UHealthBarWidget> HealthBar;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<UManaBarWidget> ManaBar;
};
