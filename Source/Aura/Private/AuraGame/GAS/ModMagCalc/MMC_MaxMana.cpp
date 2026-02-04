// Copyright rynnli



#include "AuraGame/GAS/ModMagCalc/MMC_MaxMana.h"

#include "AuraGame/GAS/AttributeSets/PrimaryAttributeSet.h"
#include "RPGFramework/Interaction/CharacterDataInterface.h"

UMMC_MaxMana::UMMC_MaxMana()
{
	IntDef.AttributeToCapture = UPrimaryAttributeSet::GetIntelligenceAttribute();
	IntDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	IntDef.bSnapshot = false;
	
}

float UMMC_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	
	FAggregatorEvaluateParameters Params;
	Params.SourceTags = SourceTags;
	Params.TargetTags = TargetTags;
	
	float Int = 0.f;
	GetCapturedAttributeMagnitude(IntDef,Spec,Params,Int);
	Int = FMath::Max(Int,0.f);
	
	ICharacterDataInterface* Interface = Cast<ICharacterDataInterface>(Spec.GetContext().GetSourceObject());
	const int32 PlayerLevel = Interface->GetCharacterLevel();
	
	return 50.f + 2.5f * Int + 15.f * PlayerLevel;
}
