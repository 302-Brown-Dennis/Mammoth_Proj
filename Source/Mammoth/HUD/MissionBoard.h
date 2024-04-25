// Class for handling starting missions
// Author: Dennis Brown

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MissionBoard.generated.h"

/**
 * 
 */
UCLASS()
class MAMMOTH_API UMissionBoard : public UUserWidget
{
	GENERATED_BODY()

public:
	void MenuSetup();
	void MenuTearDown();

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* CloseButton;


	UFUNCTION()
	void CloseButtonClicked();

};
