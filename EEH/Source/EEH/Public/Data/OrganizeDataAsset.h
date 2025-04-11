#pragma once

#include "CoreMinimal.h"
#include "Data/RoomDataAsset.h"
#include "Engine/DataAsset.h"
#include "OrganizeDataAsset.generated.h"


UCLASS(BlueprintType)
class EEH_API UOrganizeDataAsset : public UDataAsset
{
	GENERATED_BODY()
	public:
    	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
    	FString Name;
    	
    	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
    	TArray<URoomDataAsset*> Rooms;
};
