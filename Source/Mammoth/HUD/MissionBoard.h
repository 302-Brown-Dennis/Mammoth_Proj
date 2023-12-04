// Fill out your copyright notice in the Description page of Project Settings.

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



protected:
	virtual bool Initialize() override;
	
private:
	UPROPERTY(meta = (BindWidget))
	class UButton* CloseButton;

	UPROPERTY(meta = (BindWidget))
	UButton* StartMissionButton;

	UPROPERTY(meta = (BindWidget))
	class UCheckBox* ReadyBox1;

	UPROPERTY(meta = (BindWidget))
	UCheckBox* ReadyBox2;

	UPROPERTY(meta = (BindWidget))
	UCheckBox* ReadyBox3;

	UPROPERTY(meta = (BindWidget))
	UCheckBox* ReadyBox4;

	UFUNCTION()
	void CloseButtonClicked();

	UFUNCTION()
	void StartMissionButtonClicked();

	bool bStartMissionWasClicked = false;
};
