// Fill out your copyright notice in the Description page of Project Settings.

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
	void ChangeMesh(UStaticMesh* NewMesh);
	void ChangeMaterial(UMaterialInterface* NewMaterial);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;
};