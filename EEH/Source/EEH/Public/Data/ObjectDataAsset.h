#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ObjectDataAsset.generated.h"

UCLASS(BlueprintType)
class EEH_API UObjectDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	TArray<UStaticMesh*> RandomMeshes;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	TArray<UMaterialInterface*> RandomMaterials;
};