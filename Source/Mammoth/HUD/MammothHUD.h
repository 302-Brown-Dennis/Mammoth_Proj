// Player HUD
// Author: Daniel Matthies

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MammothHUD.generated.h"

/**
 * 
 */
UCLASS()
class MAMMOTH_API AMammothHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	virtual void DrawHUD() override;

	UPROPERTY(EditAnywhere, Category = "Player Stats")
	TSubclassOf<class UUserWidget> CharacterOverlayClass;
	void AddCharacterOverlay();
	class UCharacterOverlay* CharacterOverlay;

	
	

protected:
	virtual void BeginPlay() override;
	
};
