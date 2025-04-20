// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Photo.generated.h"

UCLASS()
class EEH_API APhoto : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APhoto();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	void ChangeTexture(UTextureRenderTarget2D* Texture);

private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;
};