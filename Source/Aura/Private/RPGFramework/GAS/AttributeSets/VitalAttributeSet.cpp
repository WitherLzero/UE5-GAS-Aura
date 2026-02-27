#include "RPGFramework/GAS/AttributeSets/VitalAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "RPGFramework/Types/RPGGameplayTags.h"
#include "RPGFramework/GAS/RPGAbilitySystemLibrary.h"
#include "RPGFramework/Player/RPGPlayerController.h"
#include "GameFramework/Character.h"
#include "RPGFramework/GAS/RPGAbilitySystemComponent.h"
#include "RPGFramework/Interaction/PlayerInterface.h"

UVitalAttributeSet::UVitalAttributeSet()
{
	const FRPGGameplayTags GameplayTags = FRPGGameplayTags::Get();

	TagsToAttributes.Add(GameplayTags.Attributes_Vital_HealthRegeneration,GetHealthRegenerationAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Vital_ManaRegeneration,GetManaRegenerationAttribute);
}

void UVitalAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UVitalAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UVitalAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UVitalAttributeSet, HealthRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UVitalAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UVitalAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UVitalAttributeSet, ManaRegeneration, COND_None, REPNOTIFY_Always);
}

void UVitalAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
	}
}


void UVitalAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}
	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
	}
	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		const float LocalIncomingDamage = GetIncomingDamage();
		SetIncomingDamage(0.f);
		if (LocalIncomingDamage > 0.f)
		{
			const float NewHealth = GetHealth() - LocalIncomingDamage;
			SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));
			
			const bool bBlocked = URPGAbilitySystemLibrary::IsBlockedHit(EffectProps.EffectContextHandle);
			const bool bCriticalHit = URPGAbilitySystemLibrary::IsCriticalHit(EffectProps.EffectContextHandle);
			ShowFloatingText(LocalIncomingDamage,bBlocked,bCriticalHit);
			
			const bool bFatal = NewHealth <= 0.f;
			if (!bFatal)
			{
				FGameplayTagContainer TagContainer;
				TagContainer.AddTag(FRPGGameplayTags::Get().Effects_HitReact);
				EffectProps.TargetASC->TryActivateAbilitiesByTag(TagContainer);
			}
			else
			{
				URPGAbilitySystemComponent* OwnerASC = Cast<URPGAbilitySystemComponent>(GetOwningAbilitySystemComponent());
				OwnerASC->OnOutOfHealth.Broadcast(EffectProps.SourceAvatarActor);
			}
		}
	}
	if (Data.EvaluatedData.Attribute == GetIncomingXPAttribute())
	{
		const float LocalIncomingXP = GetIncomingXP();
		SetIncomingXP(0.f);

		if (EffectProps.SourceCharacter->Implements<UPlayerInterface>())
		{
			IPlayerInterface::Execute_AddToXP(EffectProps.SourceCharacter,LocalIncomingXP);
		}
		
		
	}
}

void UVitalAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
	
	if (Attribute == GetMaxHealthAttribute() && bTopOfHealth)
	{
		SetHealth(GetMaxHealth());
		bTopOfHealth = false;
	}
	if (Attribute == GetMaxManaAttribute() && bTopOfMana)
	{
		SetMana(GetMaxMana());
		bTopOfMana = false;
	}
}

void UVitalAttributeSet::ShowFloatingText(const float Damage, bool bBlockedHit, bool bCriticalHit) const
{
	if (EffectProps.SourceCharacter != EffectProps.TargetCharacter)
	{
		if (ARPGPlayerController* PC = Cast<ARPGPlayerController>(EffectProps.SourceCharacter->Controller))
		{
			PC->ShowDamageNumber(EffectProps.TargetCharacter,Damage,bBlockedHit,bCriticalHit);
			return;
		}
		if (ARPGPlayerController* PC = Cast<ARPGPlayerController>(EffectProps.TargetCharacter->Controller))
		{
			PC->ShowDamageNumber(EffectProps.TargetCharacter,Damage,bBlockedHit,bCriticalHit);
		}
	}
}

void UVitalAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UVitalAttributeSet, Health, OldHealth);
}

void UVitalAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UVitalAttributeSet, MaxHealth, OldMaxHealth);
}

void UVitalAttributeSet::OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UVitalAttributeSet, HealthRegeneration, OldHealthRegeneration);
}

void UVitalAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UVitalAttributeSet, Mana, OldMana);
}

void UVitalAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UVitalAttributeSet, MaxMana, OldMaxMana);
}

void UVitalAttributeSet::OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UVitalAttributeSet, ManaRegeneration, OldManaRegeneration);
}
