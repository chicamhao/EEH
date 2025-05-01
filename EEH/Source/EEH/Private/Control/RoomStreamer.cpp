#include "Control/RoomStreamer.h"
#include "Components/BoxComponent.h"
#include "Core/Loader.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

ARoomStreamer::ARoomStreamer()
{
	PrimaryActorTick.bCanEverTick = true;
	// overlap volume to trigger streaming
	OverlapVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapVolume"));
	RootComponent = OverlapVolume;

	OverlapVolume->OnComponentBeginOverlap.AddUniqueDynamic(this, &ARoomStreamer::OverlapBegins);
}

void ARoomStreamer::BeginPlay()
{
	Super::BeginPlay();
}

void ARoomStreamer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARoomStreamer::OverlapBegins(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// load changeable object
	if (Loader)
	{
		Loader->Load();
	}
	
	// teleport
	if (TeleportTarget)
	{
		ACharacter* Character = UGameplayStatics::GetPlayerCharacter(this, 0);
		Character->TeleportTo(TeleportTarget->GetActorLocation(), Character->GetActorRotation());
		Character->GetController()->SetControlRotation((TeleportTarget->GetActorRotation()));
	}
}


