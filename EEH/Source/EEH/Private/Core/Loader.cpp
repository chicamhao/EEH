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

void ALoader::OnCaptured(AChangeableObject* Object)
{
	CapturedRightObjects.Add(Object);
	if (CapturedRightObjects.Num() >= CurrentRoom->Quantity)
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White,
			*FString::Printf(TEXT("All changed objects captured, door opened!")));

        SetActive(DoorToNextRoom,false);		
	}
}

void ALoader::LoadObjects(const int32 RoomIndex)
{
	SetActive(DoorToNextRoom, true);
	for (auto* Object : Objects)
	{
		if (auto* Changeable = Cast<AChangeableObject>(Object))
		{
			Changeable->Initialize();
		}
	}
	
	const int32 SafeIndex = (RoomIndex + OrganizeData->Rooms.Num()) % OrganizeData->Rooms.Num();
	CurrentRoom = OrganizeData->Rooms[SafeIndex];
	
	if (CurrentRoom->Quantity == 0) return;
	
	int32 ObjectCount = 0;
	TArray<int32> ObjectIndexes;
	while (ObjectCount < CurrentRoom->Quantity)
	{
		int32 ObjectIndex = FMath::RandRange(0, CurrentRoom->Objects.Num() - 1);
		if (ObjectIndexes.Contains(ObjectIndex))
		{
			continue;
		}
		ObjectIndexes.Add(ObjectIndex);

		const auto& Data = CurrentRoom->Objects[ObjectIndex];

		if (Data == nullptr or Data->Name.IsEmpty()) continue;
		ObjectCount++;
		
		for (const auto& Object : Objects)
		{
			if (Object == nullptr or Object->GetActorLabel().IsEmpty()) continue;
			if (Data->Name[0] != Object->GetActorLabel()[0]) continue;

			const bool bActive = Data->RandMesh(0) >= 0;
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
			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White,
						*FString::Printf(TEXT("Changed %s to %s"), *Object->GetActorLabel(), *Label));
			
			Object->SetActorLabel(Label);
		}
	}
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
		*FString::Printf(TEXT("Room %s has %d/%d objects changed:"), *CurrentRoom->Name, ObjectCount, CurrentRoom->Objects.Num()));
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
	if (Actor == nullptr) return;
	Actor->SetActorHiddenInGame(!bActive);
	Actor->SetActorEnableCollision(bActive);

	if (UMeshComponent* Mesh = Cast<UMeshComponent>(Actor->GetRootComponent()))
	{
		Mesh->SetVisibility(bActive, true);
		Mesh->SetHiddenInGame(!bActive, true);

		Mesh->SetCollisionEnabled(bActive 
			? ECollisionEnabled::QueryAndPhysics 
			: ECollisionEnabled::NoCollision);
	}
	
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
