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

	int32 RandMesh(uint32 Min) { return FMath::RandRange(Min, RandomMeshes.Num() - 1); }
	int32 RandMaterial(uint32 Min) { return FMath::RandRange(Min, RandomMaterials.Num() - 1); }

	UStaticMesh* GetMesh(uint32 Index) { return RandomMeshes[Index]; }
	UMaterialInterface* GetMaterial(uint32 Index) { return RandomMaterials[Index]; }
};