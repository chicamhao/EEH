#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class ACapturer;

UCLASS()
class EEH_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

protected:
	virtual void BeginPlay() override;

	public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
private:
	void MoveForward(const float Value);
	void MoveRight(const float Value);
	void Turn(const float Value);
	void LookUp(const float Value);
	void Capture();
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Capture")
	ACapturer* Capturer;
};
