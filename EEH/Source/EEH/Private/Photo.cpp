// Fill out your copyright notice in the Description page of Project Settings.


#include "Photo.h"

#include "Engine/TextureRenderTarget2D.h"

APhoto::APhoto()
{
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
}

void APhoto::BeginPlay()
{
	Super::BeginPlay();
	
}

void APhoto::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APhoto::ChangeTexture(UTextureRenderTarget2D* Texture)
{
	auto* Material = Mesh->GetMaterial(0);

	if (auto* DynMaterial = UMaterialInstanceDynamic::Create(Material, Mesh))
	{
		DynMaterial->SetTextureParameterValue(TEXT("Texture"), Texture);
		Mesh->SetMaterial(0, DynMaterial);
	}
}
