// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/Pawn.h"
#include "Ship.generated.h"

class UStaticMeshComponent;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class MOMENTUMRACING_API AShip : public APawn
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera;

public:
	// Sets default values for this pawn's properties
	AShip();

	UPROPERTY(EditAnywhere)
	float TurnTorque;

	UPROPERTY(EditAnywhere)
	float AccelerationForce;

	UPROPERTY(EditAnywhere)
	float BoostForce;

	UPROPERTY(EditAnywhere)
	float BrakeForce;

	UPROPERTY(EditAnywhere)
	float RegularTopSpeed;

	UPROPERTY(EditAnywhere)
	float BoostTopSpeed;

	UPROPERTY(EditAnywhere)
	float MaxAngularVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Boost;

	FTimerHandle BoostHandle;

	UFUNCTION(BlueprintCallable)
	void SetHUDWidget(TSubclassOf<UUserWidget> HUDWidgetClass);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UUserWidget> HUDWidgetClass;

	UPROPERTY()
	UUserWidget* CurrentWidget;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward(float Value);

	void MoveRight(float Value);

	void StartBoosting();

	void StopBoosting();

	void StartBraking();

	void StopBraking();

	void AlterBoost();

	float GetBoost();

	FVector CurrentVelocity;

	bool IsBoost;

	bool IsBrake;

};
