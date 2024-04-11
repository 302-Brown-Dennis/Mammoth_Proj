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
	//PlayerCharacter = Cast<APlayerCharacter_cpp>(GetPawn());
	//if (PlayerCharacter)
	//{
	//	if (GEngine)
	//	{
	//		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Cyan, FString(TEXT("Begin play player is GOOD!")));
	//	}
	//}
	//if (MammothHUD == nullptr)
	//{
	//	if (GEngine)
	//	{
	//		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString(TEXT("Begin play HUD is invalid!")));
	//	}
	//}
	
}
void AMammothPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	//PlayerCharacter = Cast<APlayerCharacter_cpp>(InPawn);
	//if (PlayerCharacter)
	//{
	//	//MammothHUD = MammothHUD == nullptr ? Cast<AMammothHUD>(GetHUD()) : MammothHUD;
	//	MammothHUD = Cast<AMammothHUD>(GetHUD());
	//	if (MammothHUD)
	//	{
	//		if (GEngine)
	//		{
	//			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString(TEXT("On Posses HUD good!")));
	//		}
	//		MammothHUD->AddCharacterOverlay();
	//	}
	//	if (MammothHUD == nullptr)
	//	{
	//		if (GEngine)
	//		{
	//			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString(TEXT("On Posses HUD NULL!")));
	//		}
	//	}
	//	
	//	SetHUDHealth(PlayerCharacter->GetHealth(), PlayerCharacter->GetMaxHealth());
	//	SetHUDStamina(PlayerCharacter->GetStamina(), PlayerCharacter->GetMaxStamina());
	//}
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
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString(TEXT("BP Init HUD good!")));
		}
		if (PlayerCharacter)
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString(TEXT("BP Init HUD good, player good! setting HUD")));
			}
			//MammothHUD->AddCharacterOverlay();
			SetHUDHealth(PlayerCharacter->GetHealth(), PlayerCharacter->GetMaxHealth());
			SetHUDStamina(PlayerCharacter->GetStamina(), PlayerCharacter->GetMaxStamina());
			SetHUDAmmo(30);
		}
		if (PlayerCharacter == nullptr)
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString(TEXT("BP Init HUD good, player BAD!")));
			}
		}
		
	
	}
	if (MammothHUD == nullptr)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString(TEXT("BP Init HUD NULL!")));
		}
		MammothHUD = Cast<AMammothHUD>(GetHUD());
		if (MammothHUD)
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString(TEXT("BP Init HUD was null, HUD is now GOOD!")));
			}
			if (PlayerCharacter)
			{
				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString(TEXT("BP Init HUD was null, HUD is now good, player is GOOD!")));
				}
				//SetHUDHealth(PlayerCharacter->GetHealth(), PlayerCharacter->GetMaxHealth());
				//SetHUDStamina(PlayerCharacter->GetStamina(), PlayerCharacter->GetMaxStamina());
				//SetHUDAmmo(30);
			}
			if (PlayerCharacter == nullptr)
			{
				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString(TEXT("BP Init HUD was null, HUD is now good, player is NULL!")));
				}
			}
		}
	}
}
void AMammothPlayerController::ClearOverlay()
{
	MammothHUD->CharacterOverlay = nullptr;
	CharacterOverlayRef = nullptr;
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
	//if (MammothHUD->CharacterOverlay == nullptr) MammothHUD->AddCharacterOverlay();
	
	/*bool mHUDValid = MammothHUD && 
		MammothHUD->CharacterOverlay && 
		MammothHUD->CharacterOverlay->HealthBar && 
		MammothHUD->CharacterOverlay->HealthText;*/

	bool mHUDValid = MammothHUD &&
		CharacterOverlayRef->HealthBar &&
		CharacterOverlayRef->HealthText;
	

	if (mHUDValid) {
		/*if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString(TEXT("HUD valid! setting health")));
		}*/
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
		/*if (MammothHUD == nullptr)
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString(TEXT("SET HEALTH HUD NOT VALID!")));
			}
			return;
		}
		if (MammothHUD->CharacterOverlay == nullptr)
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString(TEXT("SET HEALTH HUD -> CHARACTER OVERLAY NOT VALID!")));
			}
			return;
		}
		if (MammothHUD->CharacterOverlay->HealthBar == nullptr)
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString(TEXT("SET HEALTH HUD -> CHARACTER OVERLAY -> HEALTHBAR NOT VALID!")));
			}
			return;
		}
		if (MammothHUD->CharacterOverlay->HealthText == nullptr)
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString(TEXT("SET HEALTH HUD -> CHARACTER OVERLAY -> HEALT TEXT NOT VALID!")));
			}
			return;
		}*/
	}
}

void AMammothPlayerController::SetHUDStamina(float Stamina, float MaxStamina) {

	MammothHUD = MammothHUD == nullptr ? Cast<AMammothHUD>(GetHUD()) : MammothHUD;
	//if (MammothHUD->CharacterOverlay == nullptr) MammothHUD->AddCharacterOverlay();

	bool mHUDValid = MammothHUD &&
		CharacterOverlayRef &&
		CharacterOverlayRef->StaminaBar;

	if (mHUDValid) {
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
	//if (MammothHUD->CharacterOverlay == nullptr) MammothHUD->AddCharacterOverlay();

	bool mHUDValid = MammothHUD &&
		CharacterOverlayRef &&
		CharacterOverlayRef->AmmoText;

	if (mHUDValid) {
		FString AmmoText = FString::Printf(TEXT("%d"), FMath::CeilToInt(Ammo));
		CharacterOverlayRef->AmmoText->SetText(FText::FromString(AmmoText));
	}
	else
	{
	
		bInitAmmo = true;
		HUDAmmo = Ammo;
	/*	if (MammothHUD == nullptr)
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString(TEXT("SET AMMO HUD NOT VALID!")));
			}
			return;
		}
		if (MammothHUD->CharacterOverlay == nullptr)
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString(TEXT("SET AMMO HUD -> CHARACTER OVERLAY NOT VALID!")));
			}
			return;
		}
		if (MammothHUD->CharacterOverlay->AmmoText == nullptr)
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString(TEXT("SET AMMO HUD -> CHARACTER OVERLAY -> AmmoText NOT VALID!")));
			}
			return;
		}*/
	}

}
void AMammothPlayerController::PollInit()
{
	if (CharacterOverlayRef == nullptr)
	{
		if (GEngine)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString(TEXT("Character overlay is null!")));
		}
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
		//MammothHUD = MammothHUD == nullptr ? Cast<AMammothHUD>(GetHUD()) : MammothHUD;
		if (MammothHUD)
		{
			//if (MammothHUD->CharacterOverlay == nullptr) MammothHUD->AddCharacterOverlay();
			if (PlayerCharacter)
			{
				//MammothHUD->AddCharacterOverlay();
				//SetHUDHealth(PlayerCharacter->GetHealth(), PlayerCharacter->GetMaxHealth());
				//SetHUDStamina(PlayerCharacter->GetStamina(), PlayerCharacter->GetMaxStamina());
				//SetHUDHealth(PlayerCharacter->GetHealth(), PlayerCharacter->GetMaxHealth());
				//SetHUDStamina(PlayerCharacter->GetStamina(), PlayerCharacter->GetMaxStamina());
			}
			//MammothHUD->AddCharacterOverlay();
		}
	}
}
void AMammothPlayerController::OnRep_MatchState()
{
	if (MatchState == MatchState::InProgress)
	{
		//MammothHUD = MammothHUD == nullptr ? Cast<AMammothHUD>(GetHUD()) : MammothHUD;
		if (MammothHUD)
		{
			//if (MammothHUD->CharacterOverlay == nullptr) MammothHUD->AddCharacterOverlay();
			if (PlayerCharacter)
			{
				//MammothHUD->AddCharacterOverlay();
				//SetHUDHealth(PlayerCharacter->GetHealth(), PlayerCharacter->GetMaxHealth());
				//SetHUDStamina(PlayerCharacter->GetStamina(), PlayerCharacter->GetMaxStamina());
				//SetHUDHealth(PlayerCharacter->GetHealth(), PlayerCharacter->GetMaxHealth());
				//SetHUDStamina(PlayerCharacter->GetStamina(), PlayerCharacter->GetMaxStamina());
			}
			//MammothHUD->AddCharacterOverlay();
		}
	}
}
