// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ReturnToMainMenu.generated.h"

/**
 * 
 */
UCLASS()
class MAMMOTH_API UReturnToMainMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void MenuSetup();
	void MenuTearDown();

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* ReturnButton;

	UFUNCTION()
	void ReturnButtonClicked();

protected:
	virtual bool Initialize() override;

};
