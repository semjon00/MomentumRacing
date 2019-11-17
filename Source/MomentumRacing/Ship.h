// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//UPROPERTY(EditAnywhere)
	//UStaticMeshComponent* OurVisibleComponent;

	//UPROPERTY(EditAnywhere)
	//class UPhysicalMaterial* physMat;

	void MoveForward(float Value);

	void MoveRight(float Value);

	void StartBoosting();

	void StopBoosting();

	void StartBraking();

	void StopBraking();

	FVector CurrentVelocity;

	bool IsBoost;

	bool IsBrake;

};
