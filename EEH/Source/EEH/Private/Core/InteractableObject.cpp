// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/InteractableObject.h"
#include "Components/BoxComponent.h"

AInteractableObject::AInteractableObject()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	// overlap volume to trigger interact event.
	OverlapVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapVolume"));
	OverlapVolume->SetupAttachment(RootComponent);
	OverlapVolume->OnComponentBeginOverlap.AddUniqueDynamic(this, &AInteractableObject::OverlapBegins);
}

void AInteractableObject::BeginPlay()
{
	Super::BeginPlay();
}

void AInteractableObject::OverlapBegins(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// nothing to do
}

void AInteractableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}