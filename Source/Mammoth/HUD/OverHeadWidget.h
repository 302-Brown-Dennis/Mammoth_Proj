// Based on Stephen Ulibarri Udemy course https://www.udemy.com/course/unreal-engine-5-cpp-multiplayer-shooter/
// Modified by: Dennis Brown

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OverHeadWidget.generated.h"

/**
 * 
 */
UCLASS()
class MAMMOTH_API UOverHeadWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* DisplayText;

	void SetDisplayText(FString TextToDisplay);

	UFUNCTION(BlueprintCallable)
	void ShowPlayerNetRole(APawn* InPawn);

	// True to show Remote role, False for local
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bShowRemoteOrLocalRole = false;

	// Show or hide roles
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bShowRoles = true;

protected:
	virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override;


};
