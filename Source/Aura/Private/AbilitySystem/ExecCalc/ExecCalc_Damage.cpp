// Copyright rynnli


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "RPGGameplayTags.h"
#include "AbilitySystem/AttributeSets/CombatAttributeSet.h"
#include "AbilitySystem/AttributeSets/VitalAttributeSet.h"

struct RPGDamageStatic
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
	
	RPGDamageStatic()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCombatAttributeSet,Armor,Target,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCombatAttributeSet, ArmorPenetration, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCombatAttributeSet,BlockChance,Target,false);
	}
};

static const RPGDamageStatic DamageStatics()
{
	static RPGDamageStatic DStatics;
	return DStatics;
}


UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                              FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	
	const AActor* SourceAvatar = SourceASC? SourceASC->GetAvatarActor(): nullptr;
	const AActor* TargetAvatar = TargetASC? TargetASC->GetAvatarActor(): nullptr;
	
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;
	
	// Get damage set by caller Magnitude
	float Damage = Spec.GetSetByCallerMagnitude(FRPGGameplayTags::Get().Damage);
	
	// Capture BlockChance on target, and determine if there was a successful Block
	// If Block. halve the damage
	float TargetBlockChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef, EvaluationParameters, TargetBlockChance);
	TargetBlockChance = FMath::Max<float>(TargetBlockChance, 0.f);
	
	const bool bBlocked = FMath::RandRange(1, 100) < TargetBlockChance;
	Damage = bBlocked ? Damage / 2.f : Damage;
	
	float TargetArmor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvaluationParameters, TargetArmor);
	TargetArmor = FMath::Max<float>(TargetArmor, 0.f);
	
	float SourceArmorPenetration = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationDef, EvaluationParameters, SourceArmorPenetration);
	SourceArmorPenetration = FMath::Max<float>(SourceArmorPenetration, 0.f);
	
	// ArmorPenetration ignores a percentage of the Target's Armor.	
	const float EffectiveArmor = TargetArmor *= ( 100 - SourceArmorPenetration * 0.25f ) / 100.f;
	// Armor ignores a percentage of incoming Damage.
	Damage *= ( 100 - EffectiveArmor * 0.333f ) / 100.f;
	
	const FGameplayModifierEvaluatedData EvaluatedData(UVitalAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
