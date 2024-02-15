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

	//FPlayerAcceptanceDelegate OnPlayerReady;

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
	void PlayerHasReadyUp();

	UFUNCTION(Server, Reliable)
	void GetReadyAmount();

	UPROPERTY(ReplicatedUsing = OnRep_UpdatePlayersReady)
	int32 NumOfPlayersReady;

	UFUNCTION(BlueprintCallable)
	void StartSprint();

	UFUNCTION(BlueprintCallable)
	void StopSprint();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	float MaxHealth = 100.f;

	UPROPERTY(ReplicatedUsing = OnRep_Health, VisibleAnywhere, BlueprintReadOnly, Category = "Player Stats")
	float Health = 100.f;

	UFUNCTION()
	void OnRep_Health(float LastHealth);

	UFUNCTION()
	void OnRep_UpdatePlayersReady();

	void SetOverlappingObject(AUseableItems* OverlappedObject);

	void OnMatchStateSet(FName State);

	void HandleCooldown();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Character")
	bool bIsSprinting = false;

	UFUNCTION(BlueprintImplementableEvent, Category = "My Character")
	void OnSprintStateChangeBPEvent(bool bNewValue);

	UFUNCTION(Server, Reliable)
	void Server_SetPlayerIsReady();

	UFUNCTION(Server, Reliable)
	void Server_UpdatePlayersReady();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_UpdatePlayersReady();

	void UpdatePlayerReady();
	// Pointer to online session interface
	IOnlineSessionPtr OnlineSessionInterface;

	FTimerHandle StaminaDrainTimer;
	FTimerHandle StaminaRegenTimer;

// Protected controls for creating steam session
protected:

	void UseKeyPressed();
	

private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* OverHeadWidget;

	UPROPERTY(ReplicatedUsing = OnRep_UseableItems)
	class AUseableItems* UseableItems;

	UFUNCTION()
	void OnRep_UseableItems(AUseableItems* LastObject);

	UPROPERTY(VisibleAnywhere)
	class UMissionComponents* Missions;

	UPROPERTY(EditAnywhere, Category = "HUD")
	TSubclassOf<class UUserWidget> MissionBoardWidget;

	UPROPERTY()
	class UMissionBoard* MissionBoard;

	UPROPERTY(ReplicatedUsing = OnRep_MatchState)
	FName MatchState;

	

	UFUNCTION()
	void OnRep_MatchState();

	UPROPERTY()
	class ALobbyGameMode* LobbyGameMode;

	UPROPERTY()
	class AMammothPlayerState* MammothPlayerState;

	/*
	* Player Health and Stamina
	*/

	void UpdateHUDStamina();
	void UpdateHUDHealth();
	

	UPROPERTY(EditAnywhere, Category = "Player Stats")
	float MaxStamina = 100.f;

	UPROPERTY(ReplicatedUsing = OnRep_Stamina, VisibleAnywhere, Category = "Player Stats")
	float Stamina = 100.f;

	// More stuff for Stamina
	UPROPERTY(EditAnywhere, Category = "Stamina")
	float StaminaRegenRate = 10.f;

	UPROPERTY(EditAnywhere, Category = "Stamina")
	float StaminaDrainRate = 15.f;

	void DrainStamina();
	void RegenStamina();

	void SetSprinting(bool bNewSprintState);

	UFUNCTION()
	void OnRep_Stamina();

	UPROPERTY()
	class AMammothPlayerController* MammothPlayerController;
};
