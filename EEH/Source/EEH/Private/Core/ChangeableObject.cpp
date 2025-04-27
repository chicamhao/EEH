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

 bool AChangeableObject::Validate(const UStaticMesh* NewMesh, const UMaterialInterface* NewMaterial) const
 {
 	if (NewMaterial == OriginMaterial && NewMesh == OriginMesh) return false;
	return true;
 }

void AChangeableObject::ChangeMaterial(UMaterialInterface* NewMaterial) const
{
	if (Mesh)
	{
		Mesh->SetMaterial(0, NewMaterial);
	}
}

void AChangeableObject::ChangeMesh(UStaticMesh* NewMesh) const 
{
	if (Mesh)
	{
		Mesh->SetStaticMesh(NewMesh);
	}
}