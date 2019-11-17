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

	//{
	//	UE_LOG(LogTemp, Display, TEXT("CurrentVelocity is %s"), *CurrentVelocity.ToString());
	//	if (!CurrentVelocity.IsZero())
	//	{
	//		FVector NewLocation = GetActorLocation() + (CurrentVelocity * DeltaTime);
	//		SetActorLocation(NewLocation);
	//	}
	//}

}

// Called to bind functionality to input
void AShip::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAxis("MoveForward", this, &AShip::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AShip::MoveRight);
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
	const FVector Torque = FVector(0.0f, 0.0f, Value * TurnTorque);
	Mesh->AddTorqueInRadians(Torque);
	//UE_LOG(LogTemp, Display, TEXT("Added %s torque"), *Torque.ToString());
}
