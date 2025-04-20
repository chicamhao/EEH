#include "Control/Capturer.h"

#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"

ACapturer::ACapturer()
{
	PrimaryActorTick.bCanEverTick = true;
	SceneCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("Capturer"));
}

void ACapturer::BeginPlay()
{
	Super::BeginPlay();
}

void ACapturer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACapturer::Capture()
{
	if (RenderTarget)
	{
		RenderTarget->MarkAsGarbage();
		RenderTarget = nullptr;
	}
	RenderTarget = NewObject<UTextureRenderTarget2D>();
	RenderTarget->InitCustomFormat(1024, 1024, PF_B8G8R8A8, true);
	RenderTarget->UpdateResourceImmediate(true);
	SceneCapture->TextureTarget = RenderTarget;

	if (const auto* Child = FindComponentByClass<UChildActorComponent>())
	{
		TArray<UStaticMeshComponent*> Meshes;
		Child->GetChildActor()->GetComponents<UStaticMeshComponent>(Meshes);

		if (Meshes.Num() > 0)
		{
			auto* Mesh = Meshes[0];
			auto* Material = Mesh->GetMaterial(0);

			if (auto* DynMaterial = UMaterialInstanceDynamic::Create(Material, Mesh))
			{
				DynMaterial->SetTextureParameterValue(TEXT("Texture"), RenderTarget);
				Meshes[0]->SetMaterial(0, DynMaterial);
			}
		}
	}
}

