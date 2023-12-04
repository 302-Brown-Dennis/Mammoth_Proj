// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyTriggerVolume.generated.h"


UCLASS()
class MAMMOTH_API AEnemyTriggerVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyTriggerVolume();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* TriggerBox;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AEnemyAI> EnemyBlueprint;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, Category = "SpawnLocations")
	TArray<FVector> SpawnLocations;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION(Server, Reliable)
	void ServerSpawnEnemy(const FVector& SpawnLocation, class APlayerCharacter_cpp* Target);

	bool bHasTriggered = false;

};
