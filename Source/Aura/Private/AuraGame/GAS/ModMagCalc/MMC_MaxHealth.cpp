// Copyright rynnli


#include "AuraGame/GAS/ModMagCalc/MMC_MaxHealth.h"

#include "AuraGame/GAS/AttributeSets/PrimaryAttributeSet.h"
#include "RPGFramework/Interaction/CharacterDataInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	VigorDef.AttributeToCapture = UPrimaryAttributeSet::GetVigorAttribute();
	VigorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	VigorDef.bSnapshot = false;
	
	RelevantAttributesToCapture.Add(VigorDef);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	
	FAggregatorEvaluateParameters Params;
	Params.SourceTags = SourceTags;
	Params.TargetTags = TargetTags;
	
	float Vigor = 0.0f;
	GetCapturedAttributeMagnitude(VigorDef,Spec,Params,Vigor);
	Vigor = FMath::Max(Vigor, 0.0f);
	
	ICharacterDataInterface* Interface = Cast<ICharacterDataInterface>(Spec.GetContext().GetSourceObject());
	const int32 PlayerLevel = Interface->GetCharacterLevel();
	
	return 80.f + 2.5f * Vigor + 10.f * PlayerLevel;
}
