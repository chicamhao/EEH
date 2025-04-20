// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Capturer.generated.h"

class APhoto;

UCLASS()
class EEH_API ACapturer : public AActor
{
	GENERATED_BODY()

public:
	ACapturer();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	void Capture();
	
	UPROPERTY(VisibleAnywhere)
	USceneCaptureComponent2D* SceneCapture;

	UPROPERTY(VisibleAnywhere)
	UTextureRenderTarget2D* RenderTarget;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Capture")
	APhoto* Photo;
};
