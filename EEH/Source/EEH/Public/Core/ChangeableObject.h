#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChangeableObject.generated.h"

UCLASS(BlueprintType)
class EEH_API AChangeableObject : public AActor
{
	GENERATED_BODY()
	
public:	
	AChangeableObject();
	virtual void Tick(float DeltaTime) override;
	
	bool Validate(const UStaticMesh* NewMesh, const UMaterialInterface* NewMaterial) const;
	void ChangeMesh(UStaticMesh* NewMesh) const;
	void ChangeMaterial(UMaterialInterface* NewMaterial) const;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;

	UStaticMesh* OriginMesh;
    UMaterialInterface* OriginMaterial;
};