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
			if (!Data->Name.Contains(Object->GetActorLabel()))
				continue;

			int32 MeshIndex = MeshIndex = Data->RandMesh(-1);
			int32 MaterialIndex = Data->RandMaterial(0);
			
			// activate object, change mesh & material
			if (MeshIndex >= 0)
			{
				Object->ChangeMesh(Data->GetMesh(MeshIndex));
				Object->ChangeMaterial(Data->GetMaterial(MaterialIndex));
			}
			// deactivate object
			else
			{
				Object->SetActorHiddenInGame(true);
				Object->SetActorEnableCollision(false);
			}
			
			auto Label = FString::Printf(TEXT("%s%d%d"), *Data->Name, MeshIndex + 1, MaterialIndex + 1);
			Object->SetActorLabel(Label);
			break;
		}
		UE_LOG(LogTemp, Warning, TEXT("Can't find Data's %s"), *Object->GetActorLabel());
	}
}