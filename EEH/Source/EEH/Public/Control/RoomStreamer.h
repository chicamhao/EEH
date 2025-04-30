#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoomStreamer.generated.h"

class ALoader;
class UBoxComponent;

UCLASS()
class EEH_API ARoomStreamer : public AActor
{
	GENERATED_BODY()

public:
	ARoomStreamer();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup")
	AActor* TeleportTarget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup")
	ALoader* Loader;
	
	UFUNCTION()
	void OverlapBegins(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	UBoxComponent* OverlapVolume;
};