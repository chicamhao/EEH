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
				if (auto Index = FMath::RandRange(0, Data->RandomMeshes.Num()) < Data->RandomMeshes.Num())
				{
					Object->ChangeMesh(
						Data->RandomMeshes[Index]);

					Index = FMath::RandRange(0, Data->RandomMaterials.Num() - 1);
					Object->ChangeMaterial(Data->RandomMaterials[Index]);
				}
				else
				{
					Object->SetActorHiddenInGame(true);
					Object->SetActorEnableCollision(false);
				}
				break;
			}
			UE_LOG(LogTemp, Warning, TEXT("Can't find Data's %s"), *Object->GetActorLabel());
		}
	}
}

