// Fill out your copyright notice in the Description page of Project Settings.


#include "MomentumRacing.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/CollisionProfile.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Ship.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White,text)

// Sets default values
AShip::AShip()
{
	//static ConstructorHelpers::FObjectFinder<UObject> physMatHelper(TEXT("PhysicalMaterial'/Engine/EngineMaterials/PhysMat_Ice.PhysMat_Ice'"));
	//physMat = (UPhysicalMaterial*) physMatHelper.Object;
	
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

	//RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	//UCameraComponent* OurCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("OurCamera"));
	//OurVisibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OurVisibleComponent"));
	//OurCamera->SetupAttachment(RootComponent);
	//OurCamera->SetRelativeLocation(FVector(-250.0f, 0.0f, 250.0f));
	//OurCamera->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));
	//OurVisibleComponent->SetupAttachment(RootComponent);
	//OurVisibleComponent->BodyInstance.SetCollisionProfileName(UCollisionProfile::PhysicsActor_ProfileName);
	//OurVisibleComponent->SetNotifyRigidBodyCollision(true);
	//OurVisibleComponent->SetSimulatePhysics(true);

	TurnTorque = 7500000.0f;
	AccelerationForce = 100000.0f;
	BoostForce = 4 * AccelerationForce;
	BrakeForce = BoostForce;
}

// Called when the game starts or when spawned
void AShip::BeginPlay()
{
	Super::BeginPlay();	

	//OurVisibleComponent->SetMaterial(0, physMat);
}

// Called every frame
void AShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (IsBoost)
	{
		const FVector Rotation = Mesh->GetComponentRotation().Vector();
		FVector Force = BoostForce * Rotation;
		Mesh->AddForce(Force);
	}

	if (IsBrake)
	{
		const FVector Speed = Mesh->GetPhysicsLinearVelocity();
		const FVector Force = -Speed * BrakeForce / Speed.Size();
		Mesh->AddForce(Force);
	}

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
	const FVector Rotation = Mesh->GetComponentRotation().Vector();
	const FVector Force = Rotation * Value * AccelerationForce;
	Mesh->AddForce(Force);
	//UE_LOG(LogTemp, Display, TEXT("Added %s force."), *Force.ToString());
}

void AShip::MoveRight(float Value)
{
	Value = FMath::Clamp(Value, -1.0f, 1.0f);
	const FVector Torque = FVector(0.0f, 0.0f, Value * TurnTorque);
	Mesh->AddTorqueInRadians(Torque);
	//UE_LOG(LogTemp, Display, TEXT("Added %s torque"), *Torque.ToString());
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

