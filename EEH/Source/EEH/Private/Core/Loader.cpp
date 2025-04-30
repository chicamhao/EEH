#include "Core/Loader.h"

#include "Core/ChangeableObject.h"
#include "Data/OrganizeDataAsset.h"
#include "Kismet/GameplayStatics.h"

ALoader::ALoader()
{
	PrimaryActorTick.bCanEverTick = true;
	OrganizeData = CreateDefaultSubobject<UOrganizeDataAsset>("OrganizeData");
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

void ALoader::LoadCustom()
{
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AChangeableObject::StaticClass(), Objects);
	Load(CustomRoom);
}

void ALoader::Load()
{
	Load(++CurrentRoomIndex);
}

void ALoader::Load(int32 RoomIndex)
{
	int32 SafeIndex = (RoomIndex + OrganizeData->Rooms.Num()) % OrganizeData->Rooms.Num();
	const auto& Room = OrganizeData->Rooms[SafeIndex];
	
	if (Room->Quantity == 0) return;
	
	int32 ObjectCount = 0;
	while (ObjectCount < Room->Quantity)
	{
		const auto& Data = Room->Objects[
			FMath::RandRange(0, Room->Objects.Num() - 1)
			];

		if (Data->Name.IsEmpty()) continue;
		ObjectCount++;
		
		for (const auto& Object : Objects)
		{

			if (Object == nullptr or Object->GetActorLabel().IsEmpty()) continue;
			if (Data->Name[0] != Object->GetActorLabel()[0]) continue;

			const bool bActive = Data->RandMesh(-1) >= 0;
			SetActive(Object, bActive);
			
			FString Label;
			if (bActive) // change mesh & material
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
		}
	}
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, *Room->Name);
}

void ALoader::SetActive(AActor* Object, bool bActive) const
{
	Object->SetActorHiddenInGame(!bActive);
    Object->SetActorEnableCollision(bActive);
}

void ALoader::Change(UObjectDataAsset* Data, AChangeableObject* Object,
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
