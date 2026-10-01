// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/MyEnemyCharacter.h"
#include "GAS/EnemyAttributeSet.h"
#include "Components/WidgetComponent.h"
#include "Widget/OverHeadWidget.h"
#include "Kismet/GameplayStatics.h"

AMyEnemyCharacter::AMyEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	OverHeadWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverHeadWidget"));
	OverHeadWidgetComponent->SetupAttachment(RootComponent);

	OverHeadWidgetComponent->SetWidgetSpace(EWidgetSpace::World);
	OverHeadWidgetComponent->SetDrawSize(FVector2D(150.0f, 20.0f));
	OverHeadWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));
	OverHeadWidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    EnemyAttributeSet = CreateDefaultSubobject<UEnemyAttributeSet>(TEXT("Stat"));
}

UEnemyAttributeSet* AMyEnemyCharacter::GetEnemyAttribute() const
{
    return EnemyAttributeSet;
}

void AMyEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(AbilitySystemComponent))
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);	// 타이밍 문제로 추가 처리
		InitializeOverHeadWidget();
	}
}

void AMyEnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	
}

void AMyEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!AbilitySystemComponent)
	{
		return;
	}

	//static const FGameplayTag MovingTag = FGameplayTag::RequestGameplayTag(FName("GAS.State.Moving"), false);
	//const bool bIsMoving = GetVelocity().SizeSquared2D() >= FMath::Square(MoveThreshold);
	//const bool bHasMovingTag = AbilitySystemComponent->HasMatchingGameplayTag(MovingTag);
	//if (bIsMoving && !bHasMovingTag)
	//{
	//	AbilitySystemComponent->AddLooseGameplayTag(MovingTag);
	//}
	//else if (!bIsMoving && bHasMovingTag)
	//{
	//	AbilitySystemComponent->RemoveLooseGameplayTag(MovingTag);
	//}
}

void AMyEnemyCharacter::InitializeOverHeadWidget()
{
	if (!OverHeadWidgetComponent) return;

	//UE_LOG(LogTemp, Log, TEXT("OverHeadWidgetComponent 있음"));

	if (UUserWidget* UserWidget = OverHeadWidgetComponent->GetUserWidgetObject())
	{
		//UE_LOG(LogTemp, Log, TEXT("UserWidget 있음"));
		if (UOverHeadWidget* OverHeadWidget = Cast<UOverHeadWidget>(UserWidget))
		{
			//UE_LOG(LogTemp, Log, TEXT("UOverHeadWidget 캐스트 성공"));
			OverHeadWidget->InitializeWithAbilitySystem(this);
		}
	}
}

void AMyEnemyCharacter::UpdateOverheadWidgetRotation()
{
	if (!OverHeadWidgetComponent) return;

	if (APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(this, 0))
	{
		// 카메라의 전방 벡터와 정확히 마주보는 방향(-CameraForward, 사이각 180도)으로 회전
		const FVector CameraForward = CameraManager->GetCameraRotation().Vector();
		FRotator WidgetRotation = (-CameraForward).Rotation();

		if (bLockWidgetPitch)
		{
			WidgetRotation.Pitch = 0.0f;
		}
		if (bLockWidgetRoll)
		{
			WidgetRotation.Roll = 0.0f;
		}

		OverHeadWidgetComponent->SetWorldRotation(WidgetRotation);
	}
}
