#include "Core/ChangeableObject.h"

#include "Components/DecalComponent.h"

AChangeableObject::AChangeableObject()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	Decal = CreateDefaultSubobject<UDecalComponent>(TEXT("Decal"));
	DeselectObject();
}

void AChangeableObject::BeginPlay()
{
	Super::BeginPlay();
	OriginMesh = Mesh->GetStaticMesh();
	OriginMaterial = Mesh->GetMaterial(0);

	Decal->SetWorldLocation(Mesh->GetComponentLocation());
}

void AChangeableObject::Initialize()
{
	DeselectObject();
	bIsCaptured = bIsChanged = false;
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

void AChangeableObject::ChangeMaterial(UMaterialInterface* NewMaterial)
{
	Mesh->SetMaterial(0, NewMaterial);
	OriginMaterial = NewMaterial;
	bIsChanged = true;
}

void AChangeableObject::ChangeMesh(UStaticMesh* NewMesh)
{
	Mesh->SetStaticMesh(NewMesh);
	OriginMesh = NewMesh;
	bIsChanged = true;
}

void AChangeableObject::SelectObject() const
{
	// Create a dynamic material instance from the decal's material
	UMaterialInstanceDynamic* DynMaterial = Decal->CreateDynamicMaterialInstance();
	
	// Set the base color (as a linear color)
	if (DynMaterial)
	{
		DynMaterial->SetVectorParameterValue(FName("BaseColor"), FColor::Blue);
		Decal->SetMaterial(0, DynMaterial);
	}
	
	Decal->SetVisibility(true);
}

void AChangeableObject::DeselectObject() const
{
	Decal->SetVisibility(false);
}

void AChangeableObject::CaptureObject()
{
	// Create a dynamic material instance from the decal's material
	UMaterialInstanceDynamic* DynMaterial = Decal->CreateDynamicMaterialInstance();

	const FLinearColor Color = bIsChanged ? FLinearColor::Green : FLinearColor::Red;

	// Set the base color (as a linear color)
	if (DynMaterial)
	{
		DynMaterial->SetVectorParameterValue(FName("BaseColor"), Color);
		Decal->SetMaterial(0, DynMaterial);
	}
	bIsCaptured = true;
}