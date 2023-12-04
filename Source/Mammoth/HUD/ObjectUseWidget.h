// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ObjectUseWidget.generated.h"

/**
 * 
 */
UCLASS()
class MAMMOTH_API UObjectUseWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* UseText;

	void SetDisplayText(FString TextToDisplay);
};
