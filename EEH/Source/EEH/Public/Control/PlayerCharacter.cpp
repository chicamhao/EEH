// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "Core/ChangeableObject.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(FName("Forward"), this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis(FName("Right"), this, &APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis(FName("Turn"), this, &APlayerCharacter::Turn);
	PlayerInputComponent->BindAxis(FName("LookUp"), this, &APlayerCharacter::LookUp);
	PlayerInputComponent->BindAction(FName("SelectObject"), IE_Pressed, this, &APlayerCharacter::SelectObject);
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

	FHitResult HitResult;
	
	const APlayerCameraManager* Camera = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	const bool bHit = UKismetSystemLibrary::LineTraceSingleForObjects(
		this,
		Camera->GetCameraLocation(),
		Camera->GetActorForwardVector() * 500.f,
		ObjectTypes,
		false,
		IgnoreObjects,
		EDrawDebugTrace::ForDuration,
		HitResult,
		true
		);
	if (bHit)
	{
		for (const AActor* Actor: Objects)
		{
			if (const AChangeableObject* Object = Cast<AChangeableObject>(Actor); Object != nullptr)
			{
				Object->SelectObject();
			}
		}
	}
}
