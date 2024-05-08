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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float EnemyMaxHealth = 100.0f;
	/** This is the replicated variable! */
	UPROPERTY(ReplicatedUsing = OnRep_EnemyHealth, EditAnywhere, BlueprintReadWrite, Category = "Health")
	float EnemyHealth = 100.0f;

	UFUNCTION()
	void OnRep_EnemyHealth(float LastHealth);

	/*
	* 
	* Combat
	* 
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TSubclassOf<UDamageType> DamageTypeClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	class UBoxComponent* AttackHitBoxCollison;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float EnemyDamage;

	FTimerHandle DeathTimer;

	FTimerHandle DeathTimeAni;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float DeathDelay;

	
	void SetEnemyMovementStatus(EEnemyMovementState State);

	FORCEINLINE EEnemyMovementState GetEnemyMovementStatus() { return MovementState; }
	

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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
	class APlayerCharacter_cpp* PlayerTarget = nullptr;

	// TESTTTT
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	class UEnemyHealthBarOverlay* HealthBarComponent;

	bool bAttacking = false;

	void PlayDeathMontage();

	UPROPERTY(ReplicatedUsing = OnRep_MovementStateChanged, VisibleAnywhere, Category = "AI")
	EEnemyMovementState MovementState;

	UFUNCTION()
	void OnRep_MovementStateChanged();

	// return player char
	UFUNCTION(BlueprintCallable, Category = "AI Targeting")
	APlayerCharacter_cpp* GetPlayerTarget();
	

	/*
	* Health bar functionality
	*/
	UFUNCTION(NetMulticast, Reliable)
	void UpdateHealthBarVisibilty();


	void UpdateEnemyHealthBar();

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

	UFUNCTION(BlueprintCallable)
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
	bool bWasAttacked = false;
	bool bWasSpawned = false;
	

};
