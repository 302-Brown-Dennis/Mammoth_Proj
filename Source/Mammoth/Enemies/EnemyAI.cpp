// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAI.h"
#include "Components/SphereComponent.h"
#include "AIController.h"
#include "Mammoth/PlayerCharacter_cpp.h"
// Sets default values
AEnemyAI::AEnemyAI()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AgroSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AgroSphere"));
	AgroSphere->SetupAttachment(GetRootComponent());
	AgroSphere->InitSphereRadius(450.f);

	CombatSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CombatSphere"));
	CombatSphere->SetupAttachment(GetRootComponent());
	CombatSphere->InitSphereRadius(100.f);
	MoveAcceptanceDistance = 40.f;
	//AIController = Cast<AAIController>(GetController());
}

// Called when the game starts or when spawned
void AEnemyAI::BeginPlay()
{
	Super::BeginPlay();
	AIController = Cast<AAIController>(GetController());
	

	AgroSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemyAI::AgroSphereOnOverlapBegin);
	AgroSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemyAI::AgroSphereOnOverlapEnd);

	CombatSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemyAI::CombatSphereOnOverlapBegin);
	CombatSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemyAI::CombatSphereOnOverlapEnd);
}

// Called every frame
void AEnemyAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyAI::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	

}

void AEnemyAI::AgroSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (OtherActor)
	{
		APlayerCharacter_cpp* PlayerCharacter_cpp = Cast<APlayerCharacter_cpp>(OtherActor);
		if (PlayerCharacter_cpp)
		{
			MoveToTarget(PlayerCharacter_cpp);
		}
	}
}

void AEnemyAI::AgroSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		APlayerCharacter_cpp* PlayerCharacter_cpp = Cast<APlayerCharacter_cpp>(OtherActor);
		if (PlayerCharacter_cpp)
		{
			SetEnemyMovementStatus(EEnemyMovementState::EMS_Idle);
			if (AIController)
			{
				AIController->StopMovement();
			}
		}
	}
}

void AEnemyAI::CombatSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (OtherActor)
	{
		APlayerCharacter_cpp* PlayerCharacter_cpp = Cast<APlayerCharacter_cpp>(OtherActor);
		if (PlayerCharacter_cpp)
		{
			SetEnemyMovementStatus(EEnemyMovementState::EMS_Attacking);
			
		}
	}
}

void AEnemyAI::CombatSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		APlayerCharacter_cpp* PlayerCharacter_cpp = Cast<APlayerCharacter_cpp>(OtherActor);
		if (PlayerCharacter_cpp)
		{
			SetEnemyMovementStatus(EEnemyMovementState::EMS_MoveToTarget);
			MoveToTarget(PlayerCharacter_cpp);
		}
	}
}

void AEnemyAI::MoveToTarget(APlayerCharacter_cpp* Target)
{
	SetEnemyMovementStatus(EEnemyMovementState::EMS_MoveToTarget);
	if (AIController)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Moving to Target!"));
		FAIMoveRequest MoveRequest;
		MoveRequest.SetGoalActor(Target);
		MoveRequest.SetAcceptanceRadius(MoveAcceptanceDistance);

		FNavPathSharedPtr NavPath;

		AIController->MoveTo(MoveRequest, &NavPath);
	}
	
}

