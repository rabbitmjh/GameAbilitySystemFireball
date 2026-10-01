// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/MyHUD.h"
#include "Widget/HUDWidget.h"
#include "Blueprint/UserWidget.h"

void AMyHUD::InitHUD(APawn* InPawn)
{
	if (!InPawn) return;
	if (!HUDWidgetClass) return;
	APlayerController* PC = GetOwningPlayerController();
	if (!PC || !PC->IsLocalController()) return;

	if (!HUDWidget)
	{
		HUDWidget = CreateWidget<UHUDWidget>(PC, HUDWidgetClass);
		if (HUDWidget)
		{
			HUDWidget->AddToViewport();
		}
	}
	if (HUDWidget)
	{
		if (InitializedPawn.Get() != InPawn)
		{
			HUDWidget->InitializeWithAbilitySystem(InPawn);
			InitializedPawn = InPawn;
		}
	}
}

void AMyHUD::BeginPlay()
{
	Super::BeginPlay();

	if (APawn* OwningPawn = GetOwningPawn())
	{
		InitHUD(OwningPawn);
	}
}
