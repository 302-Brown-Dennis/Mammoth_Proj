// Class for useable items
// Author: Dennis Brown

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UseableItems.generated.h"

UCLASS()
class MAMMOTH_API AUseableItems : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUseableItems();

	// Base shape for collision
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item | Collision")
	class USphereComponent* CollisionVolume;

	UPROPERTY(EditAnywhere, Category = "Text")
	FString NewString = "E - Use";
	//FText NewText = FText::FromString(TEXT("E - Use"));
	void ShowItemUseWidget(bool bShowWidget);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
		bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	

private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* ObjectUseWidget;
};
