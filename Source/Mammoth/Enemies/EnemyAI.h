// Loosley based on Stephen Ulibarri Udemy course https://www.udemy.com/course/unreal-engine-the-ultimate-game-developer-course/
// Adapted to work with multiplayer
// Author: Dennis Brown

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyAI.generated.h"

UENUM(BlueprintType)
enum class EEnemyMovementState :uint8
{
	EMS_Idle			UMETA(DeplayName = "Idle"),
	EMS_MoveToTarget	UMETA(DeplayName = "MoveToTarget"),
	EMS_Attacking		UMETA(DeplayName = "Attacking"),
	EMS_Dead			UMETA(DeplayName = "Dead"),
	EMS_Alive			UMETA(DeplayName = "Alive"),

	EMS_MAX				UMETA(DeplayName = "DefaultMAX")
};

UCLASS()
class MAMMOTH_API AEnemyAI : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyAI();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/*
	* 
	* Health
	* 
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Stats")
	float EnemyMaxHealth = 100.f;

	UPROPERTY(ReplicatedUsing = OnRep_EnemyHealth, VisibleAnywhere, Category = "Enemy Stats")
	float EnemyHealth = 100.f;

	UFUNCTION()
	void OnRep_EnemyHealth(float LastHealth);

	/*
	* 
	* Combat
	* 
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class USphereComponent* AgroSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	USphereComponent* CombatSphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TSubclassOf<UDamageType> DamageTypeClass;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	class UBoxComponent* AttackHitBoxCollison;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float MinAttackTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float MaxAttackTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Stats")
	float EnemyDamage;

	FTimerHandle AttackTimer;

	FTimerHandle DeathTimer;

	FTimerHandle DeathTimeAni;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float DeathDelay;

	/*
	* 
	* MoveMent
	* 
	*/
	void SetEnemyMovementStatus(EEnemyMovementState State);

	FORCEINLINE EEnemyMovementState GetEnemyMovementStatus() { return MovementState; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float MoveAcceptanceDistance;

	
	/*
	* 
	* VFX
	* 
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	class UParticleSystem* HitEffects;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "AI")
	class USoundCue* HitSound;

	UPROPERTY(EditAnywhere, Category = "AI")
	float HitSoundVolume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	USoundCue* AttackSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	class UAnimMontage* CombatMontage;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	//UAnimMontage* HitReactMontage;

	
	/*
	* 
	* Misc
	* 
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class AAIController* AIController;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnMovementStateSet();
	UFUNCTION()
	void ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* InstigatorController, AActor* DamageCauser);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	virtual void AgroSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	UFUNCTION()
	virtual void AgroSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	virtual void CombatSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	UFUNCTION()
	virtual void CombatSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	void MoveToTarget(class APlayerCharacter_cpp* Target);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
	bool bOverLappingCombatSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
	APlayerCharacter_cpp* PlayerTarget;

	UFUNCTION()
	void AttackHitBoxOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void AttackHitBoxOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	void ActivateCollision();

	UFUNCTION(BlueprintCallable)
	void DeactivateCollision();

	bool bAttacking = false;
	
	FORCEINLINE bool IsAttacking() const { return bAttacking; }

	UFUNCTION(Server, Reliable)
	void Server_Attack();

	UFUNCTION()
	void StartAttack();

	void PlayDeathMontage();

	UPROPERTY(ReplicatedUsing = OnRep_MovementStateChanged, VisibleAnywhere, Category = "AI")
	EEnemyMovementState MovementState;
	
	UPROPERTY()
	class AEnemyAIController* TAIController;

	UFUNCTION()
	void OnRep_MovementStateChanged();

	UFUNCTION(BlueprintCallable)
	void AttackEnd();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayAttackMontage();

	void PlayHitReactMontage();

	UFUNCTION(Server, Reliable)
	void Server_Die();

	UFUNCTION(BlueprintCallable)
	void OnDeath();

	bool IsAlive();

	void DestroyEnemy();

	UFUNCTION(BlueprintCallable)
	float GetEnemyHealth() const;

	UFUNCTION(BlueprintCallable)
	float GetEnemyMaxHealth() const;
	
	private:
		bool bHasOverlappedAgroSphere = false;
};
