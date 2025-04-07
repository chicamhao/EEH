#pragma once

#include "CoreMinimal.h"
#include "Data/ObjectDataAsset.h"
#include "Engine/DataAsset.h"
#include "RoomDataAsset.generated.h"

UCLASS(BlueprintType)
class EEH_API URoomDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	FString Name;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<UObjectDataAsset*> Objects;
};