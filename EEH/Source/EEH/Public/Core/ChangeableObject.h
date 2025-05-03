#pragma once

#include "CoreMinimal.h"
#include "Components/DecalComponent.h"
#include "GameFramework/Actor.h"
#include "ChangeableObject.generated.h"

UCLASS(BlueprintType)
class EEH_API AChangeableObject : public AActor
{
	GENERATED_BODY()
	
public:	
	AChangeableObject();
	virtual void Tick(float DeltaTime) override;

	void Initialize();
	bool Validate(const UStaticMesh* NewMesh, const UMaterialInterface* NewMaterial) const;
	void ChangeMesh(UStaticMesh* NewMesh);
	void ChangeMaterial(UMaterialInterface* NewMaterial);

	void SelectObject() const;
	void DeselectObject() const;
	void CaptureObject();

	bool IsSelected() const { return Decal->IsVisible(); };
	bool IsCaptured() const { return bIsCaptured; };
	bool IsChanged() const { return bIsChanged; };

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, Blueprintable)
	UDecalComponent* Decal;

	UPROPERTY(VisibleAnywhere)
	bool bIsCaptured;
	
	UPROPERTY(VisibleAnywhere)
	bool bIsChanged;
	
protected:
	virtual void BeginPlay() override;

private:	
	UStaticMesh* OriginMesh;
	UMaterialInterface* OriginMaterial;
};