// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayEffectTypes.h"
#include "OverHeadWidget.generated.h"

class UHealthBarWidget;
/**
 * 
 */
UCLASS()
class GAS_API UOverHeadWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	virtual void InitializeWithAbilitySystem(AActor* InActor);

protected:
	virtual void OnHealthChanged(const FOnAttributeChangeData& InData);
	virtual void OnMaxHealthChanged(const FOnAttributeChangeData& InData);
	virtual void UpdateHealthUI(float InCurrent, float InMax);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentHealth = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth = 100.0f;

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<UHealthBarWidget> HealthBar;

	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<UAbilitySystemComponent> ASC;
};
