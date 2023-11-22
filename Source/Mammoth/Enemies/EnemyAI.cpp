// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAI.h"
#include "Components/SphereComponent.h"
#include "AIController.h"
#include "Mammoth/PlayerCharacter_cpp.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Sound/SoundCue.h"
#include "Animation/AnimInstance.h"
#include "TimerManager.h"
#include "Net/UnrealNetwork.h"
#include "Components/CapsuleComponent.h"
// Sets default values
AEnemyAI::AEnemyAI()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set Enemy agro sphere
	AgroSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AgroSphere"));
	AgroSphere->SetupAttachment(GetRootComponent());
	AgroSphere->InitSphereRadius(450.f);

	// Set Enemy combat sphere
	CombatSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CombatSphere"));
	CombatSphere->SetupAttachment(GetRootComponent());
	CombatSphere->InitSphereRadius(100.f);

	// Distance from targer to consider the move complete
	MoveAcceptanceDistance = 40.f;

	// Set Enemy Combat collison sphere
	AttackHitBoxCollison = CreateDefaultSubobject<UBoxComponent>(TEXT("CombatCollisioin"));
	AttackHitBoxCollison->SetupAttachment(GetMesh(), FName("EnemySocket"));
	//AttackHitBoxCollison->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("EnemySocket"));

	// Replicate actor to all remote machines
	bReplicates = true;
	bOverLappingCombatSphere = false;

	// Enemy stats
	EnemyDamage = 10.f;

	// Attack delay settings, can be set in editor
	MinAttackTime = 10.f;
	MaxAttackTime = 20.f;

	DeathDelay = 20.f;

	// Actor network update time in seconds for replications
	NetUpdateFrequency = 66.f;
	MinNetUpdateFrequency = 33.f;


	SetEnemyMovementStatus(EEnemyMovementState::EMS_Idle);
}

// Called when the game starts or when spawned
void AEnemyAI::BeginPlay()
{
	Super::BeginPlay();
	// Get Enemy Ai controller
	AIController = Cast<AAIController>(GetController());
	
	// Bind spheres
	AgroSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemyAI::AgroSphereOnOverlapBegin);
	AgroSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemyAI::AgroSphereOnOverlapEnd);

	CombatSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemyAI::CombatSphereOnOverlapBegin);
	CombatSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemyAI::CombatSphereOnOverlapEnd);

	AttackHitBoxCollison->OnComponentBeginOverlap.AddDynamic(this, &AEnemyAI::AttackHitBoxOnOverlapBegin);
	AttackHitBoxCollison->OnComponentEndOverlap.AddDynamic(this, &AEnemyAI::AttackHitBoxOnOverlapEnd);

	// Set Combat box collision settings, i.e. combat box collision is attached to part that is expected to hit players
	AttackHitBoxCollison->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AttackHitBoxCollison->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	AttackHitBoxCollison->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	AttackHitBoxCollison->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

// Called every frame
void AEnemyAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Get replicated varibles
void AEnemyAI::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AEnemyAI, MovementState);
	//DOREPLIFETIME(AEnemyAI, MetalonMontage);
	DOREPLIFETIME(AEnemyAI, EnemyHealth);
	
}



//
// NOTE: The functions below may need a refactor for multiplayer, having issues when multiple players enter the spheres 
//
// Called to bind functionality to input
void AEnemyAI::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	

}

// When player enters agro sphere AI will move to player
void AEnemyAI::AgroSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (OtherActor && IsAlive())
	{
		APlayerCharacter_cpp* PlayerCharacter_cpp = Cast<APlayerCharacter_cpp>(OtherActor);
		if (PlayerCharacter_cpp)
		{
			MoveToTarget(PlayerCharacter_cpp);
		}
	}
}

// If player leaves sphere stop movement and reset back to idle
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

// When player enters combat sphere start attacking
void AEnemyAI::CombatSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (OtherActor && IsAlive())
	{
		APlayerCharacter_cpp* PlayerCharacter_cpp = Cast<APlayerCharacter_cpp>(OtherActor);
		if (PlayerCharacter_cpp)
		{
			PlayerTarget = PlayerCharacter_cpp;
			bOverLappingCombatSphere = true;
			if (HasAuthority())
			{
				//UE_LOG(LogTemp, Warning, TEXT("IN CombatSphereOnOverlapBegin calling Server attack!!!"));
				Server_Attack();
			}
		}
	}
}

// When player leaves combat sphere starting moveing to player
void AEnemyAI::CombatSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		APlayerCharacter_cpp* PlayerCharacter_cpp = Cast<APlayerCharacter_cpp>(OtherActor);
		if (PlayerCharacter_cpp)
		{
			// if (PlayerCharacter_cpp->CombatTarget == null) then PlayerCharacter_cpp->SetCombatTarget(nullptr)
			bOverLappingCombatSphere = false;
			MoveToTarget(PlayerCharacter_cpp);
			PlayerTarget = nullptr;
			if (HasAuthority())
			{
				GetWorldTimerManager().ClearTimer(AttackTimer);
			}
			
		}
	}
}

// Move to player using nav mesh
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
// END functions that might need changing




//
// WIP: Still need to add sound effects, particles effects, etc..
// This function is also the one that gives damage to players(Still needs to be implemented)
// 
// When the component on the enemy mesh that collides with the player do some stuff i.e. play a hit sounds, spawn particles, etc..
void AEnemyAI::AttackHitBoxOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		APlayerCharacter_cpp* PlayerCharacter = Cast<APlayerCharacter_cpp>(OtherActor);
		if (PlayerCharacter)
		{
			// if (PlayerCharacter->HitEffects)
			const USkeletalMeshSocket* TipSocket = GetMesh()->GetSocketByName("TipSocket");
			if (TipSocket)
			{
				FVector SocketLocation = TipSocket->GetSocketLocation(GetMesh());
				// UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PlayerCharacter->HitEffects, SocketLocation, FRotator(0.f), false)
			}

			// if (PlayerCharacter->HitSound)
				// UGameplayStatics::PlaySound2D(this, PlayerCharacter->HitSounds)

			//
			// THIS still needs to be implemented on the player character
			// 
			// Apply damage to player
			if (DamageTypeClass)
			{
				//UGameplayStatics::ApplyDamage(PlayerCharacter, EnemyDamage, this, DamageTypeClass);
			}
		}
	}
}

//
// Currently unused
// 
// When the attacking component stops colliding with player
void AEnemyAI::AttackHitBoxOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

// Activate the collision hit box on the attacking component
void AEnemyAI::ActivateCollision()
{
	AttackHitBoxCollison->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

// Deactivate the collision hit box on the attacking component
void AEnemyAI::DeactivateCollision()
{
	AttackHitBoxCollison->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Stop AI movement, Check if already attacking, start playing attack animation across server using OnRep_Attack()
void AEnemyAI::Server_Attack_Implementation()
{
	if (IsAlive())
	{
		if (AIController)
		{
			AIController->StopMovement();
			SetEnemyMovementStatus(EEnemyMovementState::EMS_Attacking);
		}
		if (!IsAttacking())
		{
			bAttacking = true;
			OnRep_Attack();
		}
	}
	
}

// Replicate the attack animation
void AEnemyAI::OnRep_Attack()
{
	if (HasAuthority())
	{
		SetEnemyMovementStatus(EEnemyMovementState::EMS_Attacking);
		PlayAttackMontage();
		//UE_LOG(LogTemp, Warning, TEXT("IN OnRep_Attack and Has authority!!!"));	
	}
}

// Sets a new enemy movement, replicate movement state using OnRep_MovementStateChanged()
void AEnemyAI::SetEnemyMovementStatus(EEnemyMovementState State)
{
	MovementState = State;
	OnMovementStateSet();
}

// Replicate a movement state change
void AEnemyAI::OnRep_MovementStateChanged()
{
	OnMovementStateSet();
}

// Uses a switch to activate a new movement state
void AEnemyAI::OnMovementStateSet()
{
	switch (MovementState)
	{
	case EEnemyMovementState::EMS_Idle:
		MovementState = EEnemyMovementState::EMS_Idle;
		break;

	case EEnemyMovementState::EMS_Attacking:
		//UE_LOG(LogTemp, Warning, TEXT("CASE EMS ATTACKING!!!"));
		MovementState = EEnemyMovementState::EMS_Attacking;
		break;

	case EEnemyMovementState::EMS_MoveToTarget:
		MovementState = EEnemyMovementState::EMS_MoveToTarget;

	default:
		MovementState = EEnemyMovementState::EMS_MAX;
		break;
	}
}

// Calls Server_AttackEnd() for replication
void AEnemyAI::AttackEnd()
{
	
	bAttacking = false;
	if (bOverLappingCombatSphere)
	{
		float AttackTime = FMath::FRandRange(MinAttackTime, MaxAttackTime);
		//UE_LOG(LogTemp, Warning, TEXT("Attack Time: %f"), AttackTime);
		// Set up a timer for the attack delay
		GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemyAI::OnRep_Attack, AttackTime);

	}
	
}

// Replicate the attack animation
void AEnemyAI::PlayAttackMontage_Implementation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && CombatMontage)
	{
		AnimInstance->Montage_Play(CombatMontage);
		AnimInstance->Montage_JumpToSection(FName("Attack"), CombatMontage);
	}
}

void AEnemyAI::PlayHitReactMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && CombatMontage)
	{
		AnimInstance->Montage_Play(CombatMontage);
		AnimInstance->Montage_JumpToSection(FName("DamageReact"), CombatMontage);
	}
}

void AEnemyAI::OnRep_EnemyHealth(float LastHealth)
{
	// Update enemy health bar function()
	if (EnemyHealth < LastHealth)
	{
		PlayHitReactMontage();
	}
	if (EnemyHealth == 0.f)
	{
		Server_Die();
	}
}
void AEnemyAI::ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatorController, AActor* DamageCauser)
{
	float DamageToHealth = Damage;

	EnemyHealth = FMath::Clamp(EnemyHealth - DamageToHealth, 0.f, EnemyMaxHealth);


	// Update enemy health bar
	
}
void AEnemyAI::Server_Die_Implementation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && CombatMontage)
	{
		AnimInstance->Montage_Play(CombatMontage);
		AnimInstance->Montage_JumpToSection(FName("Death"), CombatMontage);
	}
	SetEnemyMovementStatus(EEnemyMovementState::EMS_Dead);

	AttackHitBoxCollison->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CombatSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AgroSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEnemyAI::OnDeath()
{
	GetMesh()->bPauseAnims = true;
	GetMesh()->bNoSkeletonUpdate = true;

	GetWorldTimerManager().SetTimer(DeathTimer, this, &AEnemyAI::DestroyEnemy, DeathDelay);
}
bool AEnemyAI::IsAlive()
{
	return GetEnemyMovementStatus() != EEnemyMovementState::EMS_Dead;
}
void AEnemyAI::DestroyEnemy()
{
	Destroy();
}
