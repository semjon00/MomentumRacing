// Fill out your copyright notice in the Description page of Project Settings.

#include "Ship.h"
#include "MomentumRacing.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/CollisionProfile.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "TimerManager.h"
#include <algorithm>

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White,text)

// Sets default values
AShip::AShip()
{	
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	Mesh->SetupAttachment(RootComponent);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	Mesh->BodyInstance.SetCollisionProfileName(UCollisionProfile::PhysicsActor_ProfileName);
	Mesh->SetNotifyRigidBodyCollision(true);
	Mesh->SetSimulatePhysics(true);

	TurnTorque = 40000000.0f;
	AccelerationForce = 800000.0f;
	BoostForce = 1.4 * AccelerationForce;
	BrakeForce = AccelerationForce;
	RegularTopSpeed = 7000.0f;
	BoostTopSpeed = 11000.0f;
	MaxAngularVelocity = 170.0f;
	Boost = 100.0f;
	Downforce = 0.4f * AccelerationForce;
}

// Called when the game starts or when spawned
void AShip::BeginPlay()
{
	Super::BeginPlay();	

	GetWorld()->GetTimerManager().SetTimer(BoostHandle, this, &AShip::AlterBoost, 0.20f, true);
}

// Called every frame
void AShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (IsBoost && Boost >= 10.0f)
	{
		if (Mesh->GetPhysicsLinearVelocity().Size() < BoostTopSpeed) {
			const FVector Rotation = Mesh->GetComponentRotation().Vector();
			FVector Force = BoostForce * Rotation;
			const FVector ModifiedBoostForce = FVector(Force.X, Force.Y, FMath::Clamp(Force.Z, -Downforce, Downforce));
			Mesh->AddForce(ModifiedBoostForce);
		}
	}

	if (IsBrake)
	{
		const FVector Speed = Mesh->GetPhysicsLinearVelocity();
		const FVector Force = -Speed * BrakeForce / Speed.Size();
		const FVector ModifiedBrakeForce = FVector(Force.X, Force.Y, 0.0f);
		Mesh->AddForce(ModifiedBrakeForce);
	}

	Mesh->AddForce(FVector(0.0f, 0.0f, -Downforce));
}

// Called to bind functionality to input
void AShip::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAxis("MoveForward", this, &AShip::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AShip::MoveRight);
	PlayerInputComponent->BindAction("Boost", IE_Pressed, this, &AShip::StartBoosting);
	PlayerInputComponent->BindAction("Boost", IE_Released, this, &AShip::StopBoosting);
	PlayerInputComponent->BindAction("Brake", IE_Pressed, this, &AShip::StartBraking);
	PlayerInputComponent->BindAction("Brake", IE_Released, this, &AShip::StopBraking);
}

void AShip::MoveForward(float Value)
{
	if (Mesh->GetPhysicsLinearVelocity().Size() < RegularTopSpeed) {
		const FVector Rotation = Mesh->GetComponentRotation().Vector();
		const FVector Force = Rotation * Value * AccelerationForce;
		const FVector ModifiedForce = FVector(Force.X, Force.Y, FMath::Clamp(Force.Z, -Downforce * 0.8f, Downforce * 0.8f));
		Mesh->AddForce(ModifiedForce);
	}
}

void AShip::MoveRight(float Value)
{
	if (Mesh->GetPhysicsAngularVelocityInDegrees().Size() < 180.0f) {
		Value = FMath::Clamp(Value, -1.0f, 1.0f);
		const FVector Torque = FVector(0.0f, 0.0f, Value * TurnTorque);
		Mesh->AddTorqueInRadians(Torque);
	}
}

void AShip::StartBoosting()
{
	IsBoost = true;
}

void AShip::StopBoosting()
{
	IsBoost = false;
}

void AShip::StartBraking()
{
	IsBrake = true;
}

void AShip::StopBraking()
{
	IsBrake = false;
}

void AShip::AlterBoost()
{
	Boost += 1.0f;
	if (IsBoost)
	{
		Boost -= 8.0f;
	}
	if (Boost < 0.0f)
		Boost = 0.0f;
	if (Boost > 100.0f)
		Boost = 100.0f;
}

float AShip::GetBoost()
{
	return Boost;
}

