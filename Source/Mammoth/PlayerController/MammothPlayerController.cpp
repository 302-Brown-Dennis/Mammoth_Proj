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
	APlayerCharacter_cpp* PlayerCharacter = Cast<APlayerCharacter_cpp>(InPawn);
	if (PlayerCharacter)
	{
		SetHUDHealth(PlayerCharacter->GetHealth(), PlayerCharacter->GetMaxHealth());
		SetHUDStamina(PlayerCharacter->GetStamina(), PlayerCharacter->GetMaxStamina());
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
void AMammothPlayerController::SetHUDHealth(float Health, float MaxHealth) {
	MammothHUD = MammothHUD == nullptr ? Cast<AMammothHUD>(GetHUD()) : MammothHUD;

	bool mHUDValid = MammothHUD && 
		MammothHUD->CharacterOverlay && 
		MammothHUD->CharacterOverlay->HealthBar && 
		MammothHUD->CharacterOverlay->HealthText;
	

	if (mHUDValid) {
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString(TEXT("HUD valid! setting health")));
		}
		const float HealthPercent = Health / MaxHealth;
		MammothHUD->CharacterOverlay->HealthBar->SetPercent(HealthPercent);
		FString HealthText = FString::Printf(TEXT("%d/%d"), FMath::CeilToInt(Health), FMath::CeilToInt(MaxHealth));
		MammothHUD->CharacterOverlay->HealthText->SetText(FText::FromString(HealthText));
	}
	else
	{
		bInitHealth = true;
		HUDHealth = Health;
		HUDMaxHealth = MaxHealth;
		if (MammothHUD == nullptr)
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString(TEXT("HUD NOT VALID!")));
			}
			return;
		}
		if (MammothHUD->CharacterOverlay == nullptr)
		{
			/*if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString(TEXT("HUD -> CHARACTER OVERLAY NOT VALID!")));
			}*/
			return;
		}
		if (MammothHUD->CharacterOverlay->HealthBar == nullptr)
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString(TEXT("HUD -> CHARACTER OVERLAY -> HEALTHBAR NOT VALID!")));
			}
			return;
		}
		if (MammothHUD->CharacterOverlay->HealthText == nullptr)
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString(TEXT("HUD -> CHARACTER OVERLAY -> HEALT TEXT NOT VALID!")));
			}
			return;
		}
	}
}

void AMammothPlayerController::SetHUDStamina(float Stamina, float MaxStamina) {
	MammothHUD = MammothHUD == nullptr ? Cast<AMammothHUD>(GetHUD()) : MammothHUD;

	bool mHUDValid = MammothHUD &&
		MammothHUD->CharacterOverlay &&
		MammothHUD->CharacterOverlay->StaminaBar;

	if (mHUDValid) {
		const float StaminaPercent = Stamina / MaxStamina;
		MammothHUD->CharacterOverlay->StaminaBar->SetPercent(StaminaPercent);
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
		MammothHUD->CharacterOverlay &&
		MammothHUD->CharacterOverlay->AmmoText;

	if (mHUDValid) {
		FString AmmoText = FString::Printf(TEXT("%d"), FMath::CeilToInt(Ammo));
		MammothHUD->CharacterOverlay->AmmoText->SetText(FText::FromString(AmmoText));
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString(TEXT("HUD not valid saving ammo!")));
		}
		bInitAmmo = true;
		HUDAmmo = Ammo;
	}

}
void AMammothPlayerController::PollInit()
{
	if (CharacterOverlay == nullptr)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString(TEXT("Character overlay is null!")));
		}
		if (MammothHUD && MammothHUD->CharacterOverlay) 
		{
			CharacterOverlay = MammothHUD->CharacterOverlay;
			if (CharacterOverlay)
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
		MammothHUD = MammothHUD == nullptr ? Cast<AMammothHUD>(GetHUD()) : MammothHUD;
		if (MammothHUD)
		{
			MammothHUD->AddCharacterOverlay();
		}
	}
}
void AMammothPlayerController::OnRep_MatchState()
{
	if (MatchState == MatchState::InProgress)
	{
		MammothHUD = MammothHUD == nullptr ? Cast<AMammothHUD>(GetHUD()) : MammothHUD;
		if (MammothHUD)
		{
			MammothHUD->AddCharacterOverlay();
		}
	}
}
