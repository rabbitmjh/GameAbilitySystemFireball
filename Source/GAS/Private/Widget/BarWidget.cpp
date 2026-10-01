// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/BarWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UBarWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (ProgressBar && BarFillImage)
	{

		FProgressBarStyle Style = ProgressBar->GetWidgetStyle();
		FSlateBrush NewBrush;
		NewBrush.SetResourceObject(BarFillImage);

		Style.SetFillImage(NewBrush);
		ProgressBar->SetWidgetStyle(Style);
	}
}

void UBarWidget::InitBar()
{
	UpdateBar(CurrentNum, MaxNum);
}

void UBarWidget::UpdateBar(float InCurrent, float InMax)
{
	const float Percent = FMath::IsNearlyZero(InMax) ? 0.0f : FMath::Clamp(InCurrent / InMax, 0.0f, 1.0f);

	if (ProgressBar)
	{
		ProgressBar->SetPercent(Percent);
	}
	if (BarText)
	{
		BarText->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), InCurrent, InMax)));
	}
}