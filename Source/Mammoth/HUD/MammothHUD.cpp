// Player HUD
// Author: Daniel Matthies

#include "MammothHUD.h"
#include "GameFramework/PlayerController.h"
#include "CharacterOverlay.h"

void AMammothHUD::BeginPlay() {
	Super::BeginPlay();

	//AddCharacterOverlay();
}

void AMammothHUD::AddCharacterOverlay() {
	APlayerController* PlayerController = GetOwningPlayerController();

	if (PlayerController && CharacterOverlayClass) {
		CharacterOverlay = CreateWidget<UCharacterOverlay>(PlayerController, CharacterOverlayClass);
		CharacterOverlay->AddToViewport();
	}
}

void AMammothHUD::DrawHUD() {
	Super::DrawHUD();

}