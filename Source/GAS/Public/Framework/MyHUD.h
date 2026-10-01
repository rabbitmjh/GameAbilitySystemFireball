// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MyHUD.generated.h"

class UHUDWidget;
/**
 * 
 */
UCLASS()
class GAS_API AMyHUD : public AHUD
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void InitHUD(APawn* InPawn);

	UFUNCTION(BlueprintCallable)
	UHUDWidget* GetHUDWidget() const { return HUDWidget; }

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UHUDWidget> HUDWidgetClass;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UHUDWidget> HUDWidget;

private:
	TWeakObjectPtr<APawn> InitializedPawn;

};
