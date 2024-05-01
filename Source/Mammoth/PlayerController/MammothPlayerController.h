// Player Controller
// Author: Daniel Matthies

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MammothPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MAMMOTH_API AMammothPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaTime) override;
	void SetHUDHealth(float Health, float MaxHealth);
	void SetHUDStamina(float Stamina, float MaxStamina);
	void SetHUDAmmo(int32 Ammo);
	void OnMatchStateSet(FName State);
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void OnPossess(APawn* InPawn) override;
	UFUNCTION(BlueprintCallable)
	void InitHUD();
	UFUNCTION(BlueprintCallable)
	void ClearOverlay();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCharacterOverlay* CharacterOverlayRef;
protected:
	virtual void BeginPlay() override;
	void PollInit();
	

private:
	UPROPERTY()
	class AMammothHUD* MammothHUD;
	UPROPERTY()
	class APlayerCharacter_cpp* PlayerCharacter;
	UPROPERTY(ReplicatedUsing = OnRep_MatchState)
	FName MatchState;

	UFUNCTION()
	void OnRep_MatchState();

	
	bool bInitHealth = false;
	bool bInitStamina = false;
	bool bInitAmmo = false;

	float HUDHealth;
	float HUDMaxHealth;
	float HUDStamina;
	float HUDMaxStamina;
	int32 HUDAmmo;

};
