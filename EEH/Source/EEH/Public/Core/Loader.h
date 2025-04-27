#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Loader.generated.h"

class UObjectDataAsset;
class UOrganizeDataAsset;
class AChangeableObject;

UCLASS()
class EEH_API ALoader : public AActor
{
	GENERATED_BODY()
	
public:	
	ALoader();
	virtual void Tick(float DeltaTime) override;
	
	void Load();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup")
	UOrganizeDataAsset* OrganizeData;

	UFUNCTION(CallInEditor, Category = "Custom")
	void OnLoadButtonClicked();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom")
	int32 CustomRoom;
	
protected:
	virtual void BeginPlay() override;

private:
	int32 RandMesh(const AChangeableObject* Object, const UObjectDataAsset* Data);
	int32 RandMaterial(const AChangeableObject* Object, const UObjectDataAsset* Data);

	void Load(int32 Room);
	static bool SetActive(AActor* Object, bool bActive);
	
	TArray<AActor*> Objects;
	int32 CurrentRoom;
};