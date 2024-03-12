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
	EMS_Idle			UMETA(DisplayName = "Idle"),
	EMS_MoveToTarget	UMETA(DisplayName = "MoveToTarget"),
	EMS_Attacking		UMETA(DisplayName = "Attacking"),
	EMS_Dead			UMETA(DisplayName = "Dead"),
	EMS_Alive			UMETA(DisplayName = "Alive"),
	EMS_Test			UMETA(DisplayName = "Test"),

	EMS_MAX				UMETA(DisplayName = "DefaultMAX")
};

UCLASS()
class MAMMOTH_API AEnemyAI : public ACharacter
{
	GENERATED_BODY()

public:

	// Sets default values for this character's properties
	AEnemyAI();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

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
	* Move,ent
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

	UPROPERTY(EditAnywhere, Category = "AI")
	class UNiagaraSystem* BloodEffects;

	UPROPERTY()
	class UNiagaraComponent* BloodComponent;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	//UAnimMontage* HitReactMontage;

	
	/*
	* 
	* Misc
	* 
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class AAIController* AIController;

	// CombatCollisionFunctions
#pragma region CombatCollisionFunctions
	UFUNCTION()
	virtual void AgroSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void AgroSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	virtual void CombatSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void CombatSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void AttackHitBoxOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void AttackHitBoxOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	void ActivateAttackHitBoxCollision();

	UFUNCTION(BlueprintCallable)
	void DeactivateAttackHitBoxCollision();
#pragma endregion

	UFUNCTION(BlueprintCallable)
	void MoveToTarget(APlayerCharacter_cpp* Target);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
	bool bOverLappingCombatSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
	class APlayerCharacter_cpp* PlayerTarget = nullptr;


	// TESTTTT
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	class UEnemyHealthBarOverlay* HealthBarComponent;

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

	// return player char
	UFUNCTION(BlueprintCallable, Category = "AI Targeting")
	APlayerCharacter_cpp* GetPlayerTarget();
	
	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayAttackMontage();

	/*
	* Health bar functionality
	*/
	UFUNCTION(NetMulticast, Reliable)
	void UpdateHealthBarVisibilty();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastUpdateEnemyHealthBar();

	void UpdateEnemyHealthBar();
	//


	void PlayHitReactMontage();

	void PlayBlood();

	UFUNCTION(Server, Reliable)
	void ServerOnDeathMontage();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastOnDeathMontage();

	UFUNCTION(BlueprintCallable)
	void OnDeath();

	UFUNCTION(BlueprintCallable)
	bool GetWasAttacked();

	UFUNCTION(BlueprintCallable)
	bool IsAlive();

	void DestroyEnemy();

	UFUNCTION(BlueprintCallable)
	float GetEnemyHealth() const;

	UFUNCTION(BlueprintCallable)
	float GetEnemyMaxHealth() const;



	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayBloodEffects();

	UFUNCTION(BlueprintCallable)
	void SetBulletHitlocation(FVector HitLocation);

	UFUNCTION(BlueprintCallable)
	bool GetWasSpawned();

	void SetWasSpawned();

	UFUNCTION(BlueprintCallable)
	bool bHasPlayerTarget();

	UFUNCTION(BlueprintImplementableEvent, Category = "AISpawn")
	void OnEnemySpawned(bool bEnemyWasSpawned);

	UFUNCTION(BlueprintImplementableEvent, Category = "AI")
	void EventOnEnemyDeath();


protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnMovementStateSet();
	UFUNCTION()
	void ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, 
		class AController* InstigatorController, AActor* DamageCauser);
	
private:

	UPROPERTY()
	class UEnemyHealthBarOverlay* EnemyHealthBarOverlayclass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "UI")
	class UWidgetComponent* EnemyHealthBarOverlay;

	FVector BulletHitLocation;
	bool bHasOverlappedAgroSphere = false;
	bool bWasAttacked = false;
	bool bWasSpawned = false;
	

};
