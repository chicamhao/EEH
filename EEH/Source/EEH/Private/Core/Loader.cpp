#include "Core/Loader.h"

ALoader::ALoader()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ALoader::BeginPlay()
{
	Super::BeginPlay();
	Load();
}

void ALoader::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALoader::Load()
{
	for (const auto& Object : Objects)
	{
		for (const auto& Data : RoomDataAsset->Objects)
		{
			if (Data->Name.Contains(Object->GetActorLabel()))
			{
				Object->ChangeMesh(
					Data->RandomMeshes[
						FMath::RandRange(0, Data->RandomMeshes.Num() - 1)]);
				Object->ChangeMaterial(
					Data->RandomMaterials[
						FMath::RandRange(0, Data->RandomMaterials.Num() - 1)]);
				break;
			}
			UE_LOG(LogTemp, Warning, TEXT("Can't find Data's %s"), *Object->GetActorLabel());
		}
	}
}

