// Player Controller
// Author: Daniel Matthies

#include "MammothPlayerController.h"
#include "Mammoth/HUD/MammothHUD.h"
#include "Mammoth/HUD/CharacterOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Net/UnrealNetwork.h"
#include "Mammoth/GameModes/LevelsGameMode.h"
#include "Mammoth/PlayerCharacter_cpp.h"


void AMammothPlayerController::BeginPlay() {
	Super::BeginPlay();

	MammothHUD = Cast<AMammothHUD>(GetHUD());
}
void AMammothPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}
void AMammothPlayerController::InitHUD()
{
	
	if (MammothHUD == nullptr)
	{
		MammothHUD = Cast<AMammothHUD>(GetHUD());
	}
	if (PlayerCharacter == nullptr)
	{
		PlayerCharacter = Cast<APlayerCharacter_cpp>(GetPawn());
	}
	if (MammothHUD)
	{
		if (MammothHUD->CharacterOverlay == nullptr)
		{
			MammothHUD->AddCharacterOverlay();
			CharacterOverlayRef = MammothHUD->CharacterOverlay;
		}
		if (PlayerCharacter)
		{
			SetHUDHealth(PlayerCharacter->GetHealth(), PlayerCharacter->GetMaxHealth());
			SetHUDStamina(PlayerCharacter->GetStamina(), PlayerCharacter->GetMaxStamina());
			SetHUDAmmo(30);
		}
		/*if (PlayerCharacter == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("HUD was initialized, failed to get Player Character."));
		}*/
	
	}
	/*else
	{
		UE_LOG(LogTemp, Error, TEXT("Feel through InitHUD."));
		if (MammothHUD) { UE_LOG(LogTemp, Error, TEXT("Feel through InitHUD, HUD was valid.")); }
		else if (MammothHUD == nullptr) { UE_LOG(LogTemp, Error, TEXT("Feel through InitHUD, HUD was invalid.")); }
		if (PlayerCharacter) { UE_LOG(LogTemp, Error, TEXT("Feel through InitHUD, Player Character was valid.")); }
		else if (PlayerCharacter == nullptr) { UE_LOG(LogTemp, Error, TEXT("Feel through InitHUD, Player Character was invalid.")); }
	}*/
}
void AMammothPlayerController::ClearOverlay()
{
	if (CharacterOverlayRef)
	{
		CharacterOverlayRef = nullptr;
	}
	if (MammothHUD)
	{
		if (MammothHUD->CharacterOverlay)
		{
			MammothHUD->CharacterOverlay = nullptr;
		}
	}
}
void AMammothPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMammothPlayerController, MatchState);
	
}
void AMammothPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//PollInit();
}
void AMammothPlayerController::SetHUDHealth(float Health, float MaxHealth) 
{
	MammothHUD = MammothHUD == nullptr ? Cast<AMammothHUD>(GetHUD()) : MammothHUD;

	bool mHUDValid = MammothHUD &&
		CharacterOverlayRef->HealthBar &&
		CharacterOverlayRef->HealthText;
	
	if (mHUDValid) 
	{
		const float HealthPercent = Health / MaxHealth;
		CharacterOverlayRef->HealthBar->SetPercent(HealthPercent);
		FString HealthText = FString::Printf(TEXT("%d/%d"), FMath::CeilToInt(Health), FMath::CeilToInt(MaxHealth));
		CharacterOverlayRef->HealthText->SetText(FText::FromString(HealthText));
	}
	else
	{
		bInitHealth = true;
		HUDHealth = Health;
		HUDMaxHealth = MaxHealth;
	}
}

void AMammothPlayerController::SetHUDStamina(float Stamina, float MaxStamina) 
{
	MammothHUD = MammothHUD == nullptr ? Cast<AMammothHUD>(GetHUD()) : MammothHUD;

	bool mHUDValid = MammothHUD &&
		CharacterOverlayRef &&
		CharacterOverlayRef->StaminaBar;

	if (mHUDValid) 
	{
		const float StaminaPercent = Stamina / MaxStamina;
		CharacterOverlayRef->StaminaBar->SetPercent(StaminaPercent);
	}
	else
	{
		bInitStamina = true;
		HUDStamina = Stamina;
		HUDMaxStamina = MaxStamina;
	}
}

void AMammothPlayerController::SetHUDAmmo(int32 Ammo)
{
	MammothHUD = MammothHUD == nullptr ? Cast<AMammothHUD>(GetHUD()) : MammothHUD;

	bool mHUDValid = MammothHUD &&
		CharacterOverlayRef &&
		CharacterOverlayRef->AmmoText;

	if (mHUDValid) 
	{
		FString AmmoText = FString::Printf(TEXT("%d"), FMath::CeilToInt(Ammo));
		CharacterOverlayRef->AmmoText->SetText(FText::FromString(AmmoText));
	}
	else
	{
		bInitAmmo = true;
		HUDAmmo = Ammo;
	}

}
void AMammothPlayerController::PollInit()
{
	if (CharacterOverlayRef == nullptr)
	{
		if (MammothHUD && MammothHUD->CharacterOverlay) 
		{
			CharacterOverlayRef = MammothHUD->CharacterOverlay;
			if (CharacterOverlayRef)
			{
				if (bInitHealth) SetHUDHealth(HUDHealth, HUDMaxHealth);			
				if (bInitStamina) SetHUDStamina(HUDStamina, HUDMaxStamina);
				if (bInitAmmo) SetHUDAmmo(HUDAmmo);
			}
		}
	}
}
void AMammothPlayerController::OnMatchStateSet(FName State)
{
	MatchState = State;
	if (MatchState == MatchState::InProgress)
	{
		if (MammothHUD)
		{
			if (PlayerCharacter)
			{
			}
		}
	}
}
void AMammothPlayerController::OnRep_MatchState()
{
	if (MatchState == MatchState::InProgress)
	{
		if (MammothHUD)
		{
			if (PlayerCharacter)
			{
			}
		}
	}
}
