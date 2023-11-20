// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "PlayerCharacter_cpp.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerAcceptanceDelegate);

UCLASS(Blueprintable, config=Game)
class MAMMOTH_API APlayerCharacter_cpp : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter_cpp();

	//FPlayerAcceptanceDelegate OnPlayerReady;

	//Daniel M Added GetLifeTimeReplicated
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void OpenLobby();

	UFUNCTION(BlueprintCallable)
	void CallOpenLevel(const FString& Address);

	UFUNCTION(BlueprintCallable)
	void CallClientTravel(const FString& Address);

	// My Blueprint callable func for accepting or starting a new mission
	UFUNCTION(BlueprintCallable)
	void OnAccpetLevel();

// Online sub-system controls
public:
	// Pointer to online session interface
	IOnlineSessionPtr OnlineSessionInterface;

	FPlayerAcceptanceDelegate PlayerAcceptanceDelegate;

// Protected controls for creating steam session
protected:
	UFUNCTION(BlueprintCallable)
	void CreateGameSession();

	UFUNCTION(BlueprintCallable)
	void JoinGameSession();

	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnFindSessionsComplete(bool bWasSuccessful);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	

// Create delegate
private:
	FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;
	FOnFindSessionsCompleteDelegate FindSessionsCompleteDelegate;
	TSharedPtr<FOnlineSessionSearch> SessionSearch;
	FOnJoinSessionCompleteDelegate JoinSessionCompleteDelegate;

	// MY CODE
	//UPROPERTY(BlueprintAssignable, Category = "Acceptance")
	//FPlayerAcceptanceDelegate OnPlayerAccepted;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* OverHeadWidget;


	/*
	* Player Health and Stamina
	*/

	UPROPERTY(EditAnywhere, Category = "Player Stats")
	float MaxHealth = 100.f;

	UPROPERTY(ReplicatedUsing = OnRep_Health, VisibleAnywhere, Category = "Player Stats")
	float Health = 100.f;

	UFUNCTION()
	void OnRep_Health();

	UPROPERTY(EditAnywhere, Category = "Player Stats")
	float MaxStamina = 100.f;

	UPROPERTY(ReplicatedUsing = OnRep_Health, VisibleAnywhere, Category = "Player Stats")
	float Stamina = 100.f;

	UFUNCTION()
	void OnRep_Stamina();

	class AMammothPlayerController* MammothPlayerController;
};
