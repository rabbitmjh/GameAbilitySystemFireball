// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Fireball.h"
#include "Components/PrimitiveComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GAS/EnemyAttributeSet.h"
#include "GAS/Effect/FireballDamageEffect.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
AFireball::AFireball()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FClassFinder<UGameplayEffect> BurnEffect(
		TEXT("/Game/Blueprints/GAS/Effect/GE_Burn"));
	if (BurnEffect.Succeeded())
	{
		BurnEffectClass = BurnEffect.Class;
	}

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SetRootComponent(SphereComponent);

	Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	Movement->InitialSpeed = 1500.0f;
	Movement->MaxSpeed = 1500.0f;
	Movement->bShouldBounce = true;
}

// Called when the game starts or when spawned
void AFireball::BeginPlay()
{
	Super::BeginPlay();
	SphereComponent->IgnoreActorWhenMoving(GetOwner(), true);
	SphereComponent->IgnoreActorWhenMoving(GetInstigator(), true);

	OnActorHit.AddDynamic(this, &AFireball::OnHit);
}

// Called every frame
void AFireball::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFireball::OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!HasAuthority() || bHitted || !OtherActor || OtherActor == this ||
		OtherActor == GetOwner() || OtherActor == GetInstigator())
	{
		return;
	}

	bHitted = true;
	UAbilitySystemComponent* TargetASC =
		UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);
	UAbilitySystemComponent* SourceASC =
		UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetInstigator());
	if (SourceASC && TargetASC &&
		TargetASC->HasAttributeSetForAttribute(UEnemyAttributeSet::GetDamageAttribute()))
	{
		const FGameplayTag BurnTag = FGameplayTag::RequestGameplayTag(FName(TEXT("State.Burn")));
		const float ImpactDamage = TargetASC->HasMatchingGameplayTag(BurnTag) ? 20.0f : 10.0f;
		FGameplayEffectContextHandle Context = SourceASC->MakeEffectContext();
		Context.AddSourceObject(this);
		Context.AddHitResult(Hit);
		FGameplayEffectSpecHandle Spec = SourceASC->MakeOutgoingSpec(
			UFireballDamageEffect::StaticClass(), 1.0f, Context);
		if (Spec.IsValid())
		{
			Spec.Data->SetSetByCallerMagnitude(FName(TEXT("FireballDamage")), ImpactDamage);
			SourceASC->ApplyGameplayEffectSpecToTarget(*Spec.Data.Get(), TargetASC);
		}

		if (BurnEffectClass)
		{
			FGameplayEffectSpecHandle BurnSpec = SourceASC->MakeOutgoingSpec(BurnEffectClass, 1.0f, Context);
			if (BurnSpec.IsValid())
			{
				FGameplayTagContainer BurnTags;
				BurnTags.AddTag(BurnTag);
				TargetASC->RemoveActiveEffectsWithGrantedTags(BurnTags);
				SourceASC->ApplyGameplayEffectSpecToTarget(*BurnSpec.Data.Get(), TargetASC);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Fireball: BurnEffectClass is not assigned"));
		}
	}
	if (HitVFX)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitVFX, Hit.Location, Hit.ImpactPoint.Rotation());
	}
	Destroy();
}

