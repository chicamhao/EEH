#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Loader.generated.h"

class URoomDataAsset;
class AChangeableObject;
class UObjectDataAsset;
class UOrganizeDataAsset;

UCLASS()
class EEH_API ALoader : public AActor
{
	GENERATED_BODY()
	
public:	
	ALoader();
	virtual void Tick(float DeltaTime) override;
	
	void Load();
	void OnCaptured(AChangeableObject* Object);

	UFUNCTION(CallInEditor, Category = "Custom")
	void LoadCustom();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom")
	int32 CustomRoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	UOrganizeDataAsset* OrganizeData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	AActor* DoorToNextRoom;
	
protected:
	virtual void BeginPlay() override;
	
private:
	int32 RandMesh(const AChangeableObject* Object, const UObjectDataAsset* Data);
    int32 RandMaterial(const AChangeableObject* Object, const UObjectDataAsset* Data);

	void LoadObjects(int32 RoomIndex);
	void LoadContainers(int32 RoomIndex);

	static void SetActive(AActor* Actor, bool bActive);
	static void Change(UObjectDataAsset* Data, AChangeableObject* Object, int32& MeshIndex, int32& MaterialIndex);

	int32 CurrentRoomIndex;
	URoomDataAsset* CurrentRoom;
	TArray<AActor*> Objects;
	TArray<AActor*> Containers;
	TArray<AChangeableObject*> CapturedRightObjects;
};