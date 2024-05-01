// Class for display text above items
// Author: Dennis Brown


#include "ObjectUseWidget.h"
#include "Components/TextBlock.h"

void UObjectUseWidget::SetDisplayText(FString TextToDisplay)
{
	if (UseText)
	{
		UseText->SetText(FText::FromString(TextToDisplay));
	}
}
