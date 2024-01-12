// Player Controller
// Author: Daniel Matthies


#include "MammothPlayerController.h"
#include "Mammoth/HUD/MammothHUD.h"
#include "Mammoth/HUD/CharacterOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void AMammothPlayerController::BeginPlay() {
	Super::BeginPlay();

	MammothHUD = Cast<AMammothHUD>(GetHUD());
}

void AMammothPlayerController::SetHUDHealth(float Health, float MaxHealth) {
	MammothHUD = MammothHUD == nullptr ? Cast<AMammothHUD>(GetHUD()) : MammothHUD;

	bool mHUDValid = MammothHUD && 
		MammothHUD->CharacterOverlay && 
		MammothHUD->CharacterOverlay->HealthBar && 
		MammothHUD->CharacterOverlay->HealthText;

	if (mHUDValid) {
		const float HealthPercent = Health / MaxHealth;
		MammothHUD->CharacterOverlay->HealthBar->SetPercent(HealthPercent);
		FString HealthText = FString::Printf(TEXT("%d/%d"), FMath::CeilToInt(Health), FMath::CeilToInt(MaxHealth));
		MammothHUD->CharacterOverlay->HealthText->SetText(FText::FromString(HealthText));
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
}