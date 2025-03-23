// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/ChamuCharacter.h"

// Sets default values
AChamuCharacter::AChamuCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AChamuCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AChamuCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AChamuCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAxis(FName("Forward"), this, &AChamuCharacter::MoveForward);
	PlayerInputComponent->BindAxis(FName("Right"), this, &AChamuCharacter::MoveRight);
	PlayerInputComponent->BindAxis(FName("Turn"), this, &AChamuCharacter::Turn);
	PlayerInputComponent->BindAxis(FName("LookUp"), this, &AChamuCharacter::LookUp);
}

void AChamuCharacter::MoveForward(const float Value)
{
	if (Controller != nullptr && Value != 0.0f)
	{
		auto forward = GetActorForwardVector();
		AddMovementInput(forward, Value);
	}	
}

void AChamuCharacter::MoveRight(const float Value)
{
	if (Controller != nullptr && Value != 0.0f)
	{
		auto right = GetActorRightVector();
		AddMovementInput(right, Value);
	}	
}

void AChamuCharacter::LookUp(const float Value)
{
	AddControllerPitchInput(Value);
}

void AChamuCharacter::Turn(const float Value)
{
	AddControllerYawInput(Value);
}