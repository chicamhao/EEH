#include "Core/Loader.h"

#include "Container.h"
#include "Components/SlateWrapperTypes.h"
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
	LoadObjects(CurrentRoomIndex);

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AContainer::StaticClass(), Containers);
	LoadContainers(CurrentRoomIndex);
}

void ALoader::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALoader::LoadCustom()
{
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AChangeableObject::StaticClass(), Objects);
	LoadObjects(CustomRoom);

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AContainer::StaticClass(), Containers);
	LoadContainers(CustomRoom);
}

void ALoader::Load()
{
	LoadObjects(++CurrentRoomIndex);
	LoadContainers(CurrentRoomIndex);
}

void ALoader::LoadObjects(const int32 RoomIndex)
{
	const int32 SafeIndex = (RoomIndex + OrganizeData->Rooms.Num()) % OrganizeData->Rooms.Num();
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

void ALoader::LoadContainers(int32 RoomIndex)
{
	const int32 SafeIndex = (RoomIndex + OrganizeData->Rooms.Num()) % OrganizeData->Rooms.Num();
	const auto& Room = OrganizeData->Rooms[SafeIndex];
	
	// load containers
	for (const auto& Container : Containers)
	{
		bool bActivate = false;
		for (const auto ContainerName : Room->Containers)
		{
			if (ContainerName.IsEmpty()) continue;
			if (ContainerName[0] == Container->GetActorLabel()[0])
			{
				SetActive(Container, true);
				bActivate = true;
				break;
			}
		}
		if (!bActivate)
		{
			SetActive(Container, false);
		}
	}
}

void ALoader::SetActive(AActor* Actor, bool bActive)
{
	Actor->SetActorHiddenInGame(!bActive);
	Actor->SetActorEnableCollision(bActive);

	TArray<AActor*> ActorChilds;
	Actor->GetAttachedActors(ActorChilds);
	for (const auto* Child : ActorChilds)
	{
		if (UMeshComponent* Mesh = Cast<UMeshComponent>(Child->GetRootComponent()))
		{
			Mesh->SetCollisionEnabled(bActive 
                ? ECollisionEnabled::QueryAndPhysics 
                : ECollisionEnabled::NoCollision);
		}
	}
}

void ALoader::Change(UObjectDataAsset* Data, AChangeableObject* Object, int32& MeshIndex, int32& MaterialIndex)
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
