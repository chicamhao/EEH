#include "Core/Loader.h"

#include "Data/OrganizeDataAsset.h"
#include "Core/ChangeableObject.h"
#include "Data/RoomDataAsset.h"
#include "Kismet/GameplayStatics.h"

ALoader::ALoader()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ALoader::OnLoadButtonClicked()
{
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AChangeableObject::StaticClass(), Objects);
	Load(CustomRoom);
}

void ALoader::BeginPlay()
{
	Super::BeginPlay();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AChangeableObject::StaticClass(), Objects);
	Load(0);
}

void ALoader::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALoader::Load()
{
	Load(++CurrentRoom);
}

void ALoader::Load(int32 RoomIndex)
{
	int32 Index = (RoomIndex + OrganizeData->Rooms.Num()) % OrganizeData->Rooms.Num();
	const auto& Room = OrganizeData->Rooms[Index];
	
	for (const auto& Object : Objects)
	{
		if (Object == nullptr or Object->GetActorLabel().IsEmpty()) continue;
		
		for (const auto& Data : Room->Objects)
		{
			if (Data->Name.IsEmpty()) continue;
			if (Data->Name[0] != Object->GetActorLabel()[0]) continue;

			int32 MeshIndex = MeshIndex = Data->RandMesh(-1);
			const int32 MaterialIndex = Data->RandMaterial(0);
			FString Label;
				
			// activate object, change mesh & material
			if (SetActive(Object, MeshIndex >= 0))
			{
				auto* Changeable = Cast<AChangeableObject>(Object);
				Changeable->ChangeMesh(Data->GetMesh(MeshIndex));
				Changeable->ChangeMaterial(Data->GetMaterial(MaterialIndex));
				Label = FString::Printf(TEXT("%s%d%d"), *Data->Name, MeshIndex, MaterialIndex);
			}
			// deactivate object
			else
			{
				Label = FString::Printf(TEXT("%s-"), *Data->Name);
			}
			Object->SetActorLabel(Label);

			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, *Label);
			break;
		}
		UE_LOG(LogTemp, Warning, TEXT("Can't find Data of %s"), *Object->GetActorLabel());
	}
	
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, *Room->Name);
}

bool ALoader::SetActive(AActor* Object, bool bActive)
{
	Object->SetActorHiddenInGame(!bActive);
	Object->SetActorEnableCollision(bActive);
	return bActive;
}