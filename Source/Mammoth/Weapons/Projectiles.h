// Bullets fired from guns
// Author: Jake Bottenberg

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectiles.generated.h"

UCLASS()
class MAMMOTH_API AProjectiles : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AProjectiles();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetOwningPlayer(APlayerCharacter_cpp* OwningPlayer);

protected:
	
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(EditAnywhere)
	float Damage = 12.f;

private:

	UPROPERTY(EditAnywhere)
	class UBoxComponent* CollisionBox;

public:	

	class APlayerCharacter_cpp* PlayerCharacter;
	
	

};
