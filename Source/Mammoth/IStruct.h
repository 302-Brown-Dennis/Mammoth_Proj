// Used for items in game
// Author: Cristian Ruelas

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "IStruct.generated.h"

USTRUCT(BlueprintType)
struct FItemStruct : public FTableRowBase
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    int32 ItemID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FName ItemName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FText ItemDescription;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    UTexture2D* ItemTB;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    int32 Stack;
};
