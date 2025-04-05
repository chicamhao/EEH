// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelStreamer.h"
#include "Kismet/GameplayStatics.h"

ALevelStreamer::ALevelStreamer()
{
	PrimaryActorTick.bCanEverTick = true;

	// overlap volume to trigger level streaming
	OverlapVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapVolume"));
	RootComponent = OverlapVolume;

	OverlapVolume->OnComponentBeginOverlap.AddUniqueDynamic(this, &ALevelStreamer::OverlapBegins);
}

void ALevelStreamer::BeginPlay()
{
	Super::BeginPlay();
}

void ALevelStreamer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALevelStreamer::OverlapBegins(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacter* Character = UGameplayStatics::GetPlayerCharacter(this, 0);
	if (LevelToLoad != "")
	{
		FLatentActionInfo LatentInfo;
		UGameplayStatics::LoadStreamLevel(this, LevelToLoad, true, true, LatentInfo);		
	}	
}



