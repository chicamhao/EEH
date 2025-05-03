// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "Core/ChangeableObject.h"
#include "Core/Loader.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(FName("Forward"), this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis(FName("Right"), this, &APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis(FName("Turn"), this, &APlayerCharacter::Turn);
	PlayerInputComponent->BindAxis(FName("LookUp"), this, &APlayerCharacter::LookUp);

	GetWorld()->GetTimerManager().SetTimer(MyTimerHandle, this, &APlayerCharacter::SelectObject, 0.1f, true);
	PlayerInputComponent->BindAction(FName("Capture"), IE_Pressed, this, &APlayerCharacter::CaptureObject);
}

void APlayerCharacter::MoveForward(const float Value)
{
	if (Controller && Value != 0.0f)
	{
		AddMovementInput(GetActorForwardVector(), Value);
	}	
}

void APlayerCharacter::MoveRight(const float Value)
{
	if (Controller && Value != 0.0f)
	{
		AddMovementInput(GetActorRightVector(), Value);
	}	
}

void APlayerCharacter::LookUp(const float Value)
{
	AddControllerPitchInput(Value);
}

void APlayerCharacter::Turn(const float Value)
{
	AddControllerYawInput(Value);
}

void APlayerCharacter::SelectObject()
{
	TArray<AActor*> Objects;
	const TArray<AActor*> IgnoreObjects;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));
	
	FHitResult HitResult;
	
	const APlayerCameraManager* Camera = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	const bool bHit = UKismetSystemLibrary::LineTraceSingleForObjects(
		this,
		Camera->GetCameraLocation(),
		Camera->GetCameraLocation() + Camera->GetActorForwardVector() * FVector(1000, 1000, 1000),
		ObjectTypes,
		false,
		IgnoreObjects,
		EDrawDebugTrace::None,
		HitResult,
		true
		);
		
	if (bHit)
	{
		if (SelectedObject != nullptr)
		{
			if (!SelectedObject->IsCaptured())
			{
				SelectedObject->DeselectObject();
			}
			SelectedObject = nullptr;
		}
		
		if (AChangeableObject* Object = Cast<AChangeableObject>(HitResult.HitObjectHandle.FetchActor()); Object != nullptr)
		{
			if (!Object->IsCaptured())
			{
				Object->SelectObject();
				SelectedObject = Object;
			}
		}
	}
}

void APlayerCharacter::CaptureObject()
{
	if (SelectedObject != nullptr)
	{
		SelectedObject->CaptureObject();

		if (!SelectedObject->IsChanged())
		{
			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,
				*FString::Printf(TEXT("WRONG-Captured: %s"), *SelectedObject->GetActorLabel()));
		}
		else
		{
			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
				*FString::Printf(TEXT("RIGHT-Captured: %s"), *SelectedObject->GetActorLabel()));

			AActor* Actor = UGameplayStatics::GetActorOfClass(GetWorld(), ALoader::StaticClass());
			if (ALoader* Loader = Cast<ALoader>(Actor))
			{
				Loader->OnCaptured(SelectedObject);
			}
		}
	}
}