// Main player class
// Author: All

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "PlayerCharacter_cpp.generated.h"

UCLASS(Blueprintable, config=Game)
class MAMMOTH_API APlayerCharacter_cpp : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter_cpp();

	//Daniel M Added 
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* InstigatorController, AActor* DamageCauser);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PostInitializeComponents() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void StartSprint();

	UFUNCTION(BlueprintCallable)
	void StopSprint();
	
	//UPROPERTY(EditAnywhere, Category = "Player Stats")
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth = 100.f;

	//UPROPERTY(ReplicatedUsing = OnRep_Health, VisibleAnywhere, Category = "Player Stats")

	/** This is the replicated variable! */
	UPROPERTY(ReplicatedUsing = OnRep_Health, EditAnywhere, BlueprintReadWrite, Category = "Health")
	float Health = 100.f;

	UFUNCTION()
	void OnRep_Health(float LastHealth);

	FORCEINLINE float GetHealth() const { return Health; }
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }
	FORCEINLINE float GetStamina() const { return Stamina; }
	FORCEINLINE float GetMaxStamina() const { return MaxStamina; }

	void SetOverlappingObject(AUseableItems* OverlappedObject);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprint")
	bool bIsSprinting = false;

	UFUNCTION(BlueprintImplementableEvent, Category = "Sprint")
	void OnSprintStateChangeBPEvent(bool bNewValue);

	UFUNCTION(BlueprintImplementableEvent, Category = "Sprint")
	void OnDownBPEvent();

	UFUNCTION(BlueprintCallable)
	void UpdateHUDAmmo(int32 Ammo);

	UFUNCTION(BlueprintCallable)
	void UpdateHUDHealthBPCall(float NewHealth);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetReferences")
	TSubclassOf<class UUserWidget> MissionBoardWidget;

	// Pointer to online session interface
	IOnlineSessionPtr OnlineSessionInterface;

	FTimerHandle StaminaDrainTimer;
	FTimerHandle StaminaRegenTimer;

// Protected controls for creating steam session
protected:

	void UseKeyPressed();
	void PollInit();
	

private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* OverHeadWidget;

	UPROPERTY(ReplicatedUsing = OnRep_UseableItems)
	class AUseableItems* UseableItems;

	UFUNCTION()
	void OnRep_UseableItems(AUseableItems* LastObject);

	UPROPERTY(VisibleAnywhere)
	class UMissionComponents* Missions;

	UPROPERTY()
	class UMissionBoard* MissionBoard;

	UPROPERTY()
	class ALobbyGameMode* LobbyGameMode;

	UPROPERTY()
	class AMammothPlayerState* MammothPlayerState;

	/*
	* Player Health and Stamina
	*/

	void UpdateHUDStamina();
	void UpdateHUDHealth();
	

	UPROPERTY(EditAnywhere, Category = "Sprint")
	float MaxStamina = 100.f;

	/** This is the replicated variable! */
	UPROPERTY(ReplicatedUsing = OnRep_Stamina, VisibleAnywhere, Category = "Sprint")
	float Stamina = 100.f;

	// More stuff for Stamina
	UPROPERTY(EditAnywhere, Category = "Sprint")
	float StaminaRegenRate = 10.f;

	UPROPERTY(EditAnywhere, Category = "Sprint")
	float StaminaDrainRate = 15.f;

	void DrainStamina();
	void RegenStamina();

	void SetSprinting(bool bNewSprintState);

	UFUNCTION()
	void OnRep_Stamina();

	UPROPERTY(VisibleAnywhere)
	class AMammothPlayerController* MammothPlayerController;
};
