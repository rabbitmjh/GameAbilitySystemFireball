// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BarWidget.generated.h"

class UProgressBar;
class UTextBlock;
/**
 * 
 */
UCLASS()
class GAS_API UBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativePreConstruct() override;

	void InitBar();

	void UpdateBar(float InCurrent, float InMax);


	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<UProgressBar> ProgressBar;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> BarText;
protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UTexture2D> BarFillImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentNum = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxNum = 100.0f;
};
