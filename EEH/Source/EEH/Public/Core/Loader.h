#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Loader.generated.h"

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

	UFUNCTION(CallInEditor, Category = "Custom")
	void LoadCustom();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom")
	int32 CustomRoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	UOrganizeDataAsset* OrganizeData;
	
protected:
	virtual void BeginPlay() override;
	
private:
	int32 RandMesh(const AChangeableObject* Object, const UObjectDataAsset* Data);
    int32 RandMaterial(const AChangeableObject* Object, const UObjectDataAsset* Data);
	void Load(int32 RoomIndex);
	void SetActive(AActor* Actor, bool bActive) const;
	static void Change(UObjectDataAsset* Data, AChangeableObject* Object,
	                   int32& MeshIndex, int32& MaterialIndex);
	
	TArray<AActor*> Objects;
	int32 CurrentRoomIndex;
};