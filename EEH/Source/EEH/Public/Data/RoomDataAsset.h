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
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Quantity;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<UObjectDataAsset*> Objects;
};