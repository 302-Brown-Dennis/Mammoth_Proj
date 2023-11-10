#pragma once

#include "CoreMinimal.h"
#include "IStruct.h"
#include "RecipeStruct.generated.h"

USTRUCT(BlueprintType)
struct FCraftingIngredient
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crafting")
    FItemStruct Ingredient;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crafting")
    int32 Quantity;
};

USTRUCT(BlueprintType)
struct FCraftingRecipeStruct : public FTableRowBase
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crafting")
    FItemStruct ResultItem;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crafting")
    TArray<FCraftingIngredient> RequiredItems;  
};
