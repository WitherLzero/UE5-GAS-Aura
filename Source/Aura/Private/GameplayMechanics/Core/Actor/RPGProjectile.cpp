// Copyright rynnli


#include "GameplayMechanics/Core/Actor/RPGProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Components/SphereComponent.h"
#include "Components/AudioComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Aura/Aura.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameplayMechanics/Core/RPGAbilitySystemLibrary.h"
#include "Kismet/GameplayStatics.h"


ARPGProjectile::ARPGProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	
	Sphere= CreateDefaultSubobject<USphereComponent>("Sphere");
	SetRootComponent(Sphere);
	Sphere->SetCollisionObjectType(ECC_Projectile);
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_WorldDynamic,ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_WorldStatic,ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->InitialSpeed = 550.f;
	ProjectileMovement->MaxSpeed = 550.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;
	
	
}


void ARPGProjectile::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(LifeTime);
	
	Sphere->OnComponentBeginOverlap.AddDynamic(this,&ThisClass::OnSphereOverlap);
	LoopingAudioComponent = UGameplayStatics::SpawnSoundAttached(LoopingSound,GetRootComponent());
}

void ARPGProjectile::Destroyed()
{
	if (!bHit && !HasAuthority()) HandleOnHit();

	Super::Destroyed();
}


void ARPGProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
	if (SourceAvatarActor == OtherActor) return;
	if (!bHit) HandleOnHit();

	if (HasAuthority())
	{
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			DamageEffectParams.TargetAbilitySystemComponent = TargetASC;
			FGameplayEffectSpecHandle DamageEffectSpecHandle = URPGAbilitySystemLibrary::MakeDamageEffectSpec(DamageEffectParams);
			TargetASC->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data.Get());
		}
		
		Destroy();
	}
	else  bHit=true; // Determine whether the clients spawned the FXs before server destroyed
	
	
}

void ARPGProjectile::HandleOnHit()
{
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this,ImpactEffect,GetActorLocation());
	UGameplayStatics::PlaySoundAtLocation(this,ImpactSound,GetActorLocation(),FRotator::ZeroRotator);
	if (LoopingAudioComponent) LoopingAudioComponent->Stop();
	bHit = true;
}

