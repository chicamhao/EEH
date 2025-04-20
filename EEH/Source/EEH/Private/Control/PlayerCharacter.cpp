#include "Control/PlayerCharacter.h"
#include "Control/Capturer.h"

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

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(FName("Forward"), this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis(FName("Right"), this, &APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis(FName("Turn"), this, &APlayerCharacter::Turn);
	PlayerInputComponent->BindAxis(FName("LookUp"), this, &APlayerCharacter::LookUp);
	PlayerInputComponent->BindAction(FName("Capture"), IE_Pressed, this, &APlayerCharacter::Capture);
}

void APlayerCharacter::MoveForward(const float Value)
{
	AddMovementInput(GetActorForwardVector(), Value);
}

void APlayerCharacter::MoveRight(const float Value)
{
	AddMovementInput(GetActorRightVector(), Value);
}

void APlayerCharacter::LookUp(const float Value)
{
	AddControllerPitchInput(Value);
}

void APlayerCharacter::Turn(const float Value)
{
	AddControllerYawInput(Value);
}

void APlayerCharacter::Capture()
{
	if (const auto* Child = FindComponentByClass<UChildActorComponent>())
	{
		Capturer = Cast<ACapturer>(Child->GetChildActor());
		if (Capturer != nullptr)
		{
			Capturer->Capture();
		}
	}
}
