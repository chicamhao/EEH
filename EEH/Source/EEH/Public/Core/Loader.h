// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChangeableObject.h"
#include "Data/RoomDataAsset.h"
#include "GameFramework/Actor.h"
#include "Loader.generated.h"

UCLASS()
class EEH_API ALoader : public AActor
{
	GENERATED_BODY()
	
public:	
	ALoader();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	void Load();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	URoomDataAsset* RoomDataAsset;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
	TArray<AChangeableObject*> Objects;
};