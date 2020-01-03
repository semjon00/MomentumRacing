// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UtilLibrary.generated.h"

/**
 * 
 */
UCLASS()
class MOMENTUMRACING_API UUtilLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintPure, Category="Utilities/Array")
	static float SumArray(TArray<float> arr);
};
