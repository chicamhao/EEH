#include "Core/Loader.h"

#include "Data/OrganizeDataAsset.h"
#include "Core/ChangeableObject.h"
#include "Data/RoomDataAsset.h"
#include "Kismet/GameplayStatics.h"

ALoader::ALoader(): OrganizeData(nullptr), CustomRoom(0), CurrentRoom(0)
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
	
	int32 ObjectCount = 0;	
	for (const auto& Object : Objects)
	{
		if (ObjectCount > Room->Quantity) break;
		if (Object->GetActorLabel().IsEmpty()) continue;
		
		for (const auto& Data : Room->Objects)
		{
			if (Data->Name.IsEmpty()) continue;
			if (Data->Name[0] != Object->GetActorLabel()[0]) continue;
			
			FString Label;
			const bool bActive = Data->RandMesh(-1) >= 0;

			// show/hide object
			SetActive(Object, bActive);
			
			// change mesh & material
			if (bActive)
			{
				int32 MaterialIndex = 0;
				int32 MeshIndex = 0;
				Change(Data, Cast<AChangeableObject>(Object), MeshIndex, MaterialIndex);

				Label = FString::Printf(TEXT("%s%d%d"), *Data->Name, MeshIndex, MaterialIndex);
			}
			else
			{
				Label = FString::Printf(TEXT("%s-"), *Data->Name);
			}
			Object->SetActorLabel(Label);

			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, *Label);
			ObjectCount++;
		}
	}
	
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, *Room->Name);
}

void ALoader::SetActive(AActor* Object, bool bActive)
{
	Object->SetActorHiddenInGame(!bActive);
	Object->SetActorEnableCollision(bActive);
}

void ALoader::Change(UObjectDataAsset* Data, const AChangeableObject* Object,
	int32& MeshIndex, int32& MaterialIndex)
{
	UStaticMesh* NewMesh;
	UMaterialInterface* NewMaterial;
	do 
	{
		MeshIndex = Data->RandMesh(0);
		NewMesh = Data->GetMesh(MeshIndex);
		
		MaterialIndex = Data->RandMaterial(0);
		NewMaterial = Data->GetMaterial(MaterialIndex);
	}
	while (!Object->Validate(NewMesh, NewMaterial));

	Object->ChangeMesh(NewMesh);
	Object->ChangeMaterial(NewMaterial);
}
