#include "Core/ChangeableObject.h"

#include "Components/DecalComponent.h"

AChangeableObject::AChangeableObject()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	Decal = CreateDefaultSubobject<UDecalComponent>(TEXT("Decal"));
}

void AChangeableObject::BeginPlay()
{
	Super::BeginPlay();
	OriginMesh = Mesh->GetStaticMesh();
	OriginMaterial = Mesh->GetMaterial(0);
}

void AChangeableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AChangeableObject::Validate(const UStaticMesh* NewMesh, const UMaterialInterface* NewMaterial) const
{
	// same visual 
	if (NewMaterial == OriginMaterial && NewMesh == OriginMesh) return false;
	return true;
}

void AChangeableObject::ChangeMaterial(UMaterialInterface* NewMaterial) const
{
	Mesh->SetMaterial(0, NewMaterial);
}

void AChangeableObject::ChangeMesh(UStaticMesh* NewMesh) const
{
	Mesh->SetStaticMesh(NewMesh);
}

void AChangeableObject::SelectObject() const
{
	Decal->SetVisibility(true);
}

void AChangeableObject::DeselectObject() const
{
	Decal->SetVisibility(false);
}

bool AChangeableObject::IsSelected() const
{
	return Decal->IsVisible();
}
