// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/MyCharacter.h"
#include "GameplayAbilitySpecHandle.h"
#include "MyPlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
class UGameplayAbility;
class UPlayerAttributeSet;
class AFireball;
/**
 * 
 */
UCLASS()
class GAS_API AMyPlayerCharacter : public AMyCharacter
{
	GENERATED_BODY()

public:
	AMyPlayerCharacter();

	void OnFire();

	UPlayerAttributeSet* GetPlayerAttribute() const;

protected:
	virtual void PossessedBy(AController* NewController) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaTime) override;

	virtual void GiveDefaultAbilities();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float BaseWalkSpeed = 600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float MoveThreshold = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
	TSubclassOf<UGameplayAbility> FireballAbilityClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability", meta = (ClampMin = "1"))
	int32 DefaultAlilityLevel = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> FireAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	TObjectPtr<UPlayerAttributeSet> PlayerAttributeSet;

private:
	FDelegateHandle MoveSpeedChangedDelegateHandle;
	static constexpr int32 SprintInputID = 100;

};
