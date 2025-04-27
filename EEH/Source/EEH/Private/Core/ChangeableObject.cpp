 #include "Core/ChangeableObject.h"

AChangeableObject::AChangeableObject()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
}

void AChangeableObject::BeginPlay()
{
	Super::BeginPlay();
}

void AChangeableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AChangeableObject::ChangeMaterial(UMaterialInterface* NewMaterial)
{
	if (Mesh)
	{
		Mesh->SetMaterial(0, NewMaterial);
	}
}

void AChangeableObject::ChangeMesh(UStaticMesh* NewMesh)
{
	if (Mesh)
	{
		Mesh->SetStaticMesh(NewMesh);
	}
}