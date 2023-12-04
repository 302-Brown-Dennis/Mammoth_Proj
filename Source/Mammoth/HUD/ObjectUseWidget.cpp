// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectUseWidget.h"
#include "Components/TextBlock.h"

void UObjectUseWidget::SetDisplayText(FString TextToDisplay)
{
	if (UseText)
	{
		UseText->SetText(FText::FromString(TextToDisplay));
	}
}
