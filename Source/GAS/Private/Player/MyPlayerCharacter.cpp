// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/MyPlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Framework/MyHUD.h"
#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "GAS/PlayerAttributeSet.h"
#include "Actor/Fireball.h"

AMyPlayerCharacter::AMyPlayerCharacter()
{
	PlayerAttributeSet = CreateDefaultSubobject<UPlayerAttributeSet>(TEXT("Stat"));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
}

void AMyPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (APlayerController* PC = Cast<APlayerController>(NewController))
	{
		// 플레이어 일때만 처리
		if (AMyHUD* MyHUD = Cast<AMyHUD>(PC->GetHUD()))
		{
			MyHUD->InitHUD(this);	// 레이스 컨디션 대비
		}
	}

	GiveDefaultAbilities();
}

void AMyPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (FireAction)
		{
			EnhancedInput->BindAction(FireAction, ETriggerEvent::Started, this, &AMyPlayerCharacter::OnFire);
		}
	}
}

void AMyPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!AbilitySystemComponent)
	{
		return;
	}
}

void AMyPlayerCharacter::GiveDefaultAbilities()
{
	if (!AbilitySystemComponent)
	{
		return;
	}

	if (!AbilitySystemComponent->AbilityActorInfo.IsValid())
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}

	if (FireballAbilityClass)
	{
		if (FGameplayAbilitySpec* ExistingSpec = AbilitySystemComponent->FindAbilitySpecFromClass(FireballAbilityClass))
		{
			ExistingSpec->Level = DefaultAlilityLevel;
			AbilitySystemComponent->MarkAbilitySpecDirty(*ExistingSpec);
		}
		else
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(FireballAbilityClass, DefaultAlilityLevel));
		}
	}
}

void AMyPlayerCharacter::OnFire()
{
	if (AbilitySystemComponent && FireballAbilityClass)
	{
		AbilitySystemComponent->TryActivateAbilityByClass(FireballAbilityClass);
	}

}

UPlayerAttributeSet* AMyPlayerCharacter::GetPlayerAttribute() const
{
	return PlayerAttributeSet;
}
