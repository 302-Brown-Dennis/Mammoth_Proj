// Main player class
// Author: All


#include "PlayerCharacter_cpp.h"
#include "Kismet/GameplayStatics.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"
#include "Mammoth/Items/UseableItems.h"
#include "Mammoth/MammothGameComponents/MissionComponents.h"
#include "Mammoth/HUD/MissionBoard.h"
#include "Mammoth/GameModes/LobbyGameMode.h"
#include "Mammoth/GameState/MammothGameState.h"
#include "Mammoth/PlayerState/MammothPlayerState.h"
#include "Mammoth/PlayerController/MammothPlayerController.h"
#include "GameFramework/PlayerController.h"
#include "TimerManager.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
APlayerCharacter_cpp::APlayerCharacter_cpp()
{
	
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	NumOfPlayersReady = 0;
	


	bIsSprinting = false;
	MaxStamina = 100.f;
	Stamina = MaxStamina;
	//UE_LOG(LogTemp, Warning, TEXT("Character Constructor called!!"));
	
	// Accses steam online sub-system and check if valid
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (OnlineSubsystem)
	{
		OnlineSessionInterface = OnlineSubsystem->GetSessionInterface();

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Blue, FString::Printf(TEXT("Found SubSystem %s"), *OnlineSubsystem->GetSubsystemName().ToString()));
		}
	}

	
	/*
	* Multiplayer settings stuff
	*/
	// Net update frequency
	NetUpdateFrequency = 66.f;
	MinNetUpdateFrequency = 33.f;

	bReplicates = true;
	bAlwaysRelevant = true;
	// Testing widget for displaying players role on server
	// Can be toggled in WPB_OverHeadWidget Blueprint
	OverHeadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverHeadWidget"));
	OverHeadWidget->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void APlayerCharacter_cpp::BeginPlay()
{
	Super::BeginPlay();
	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController)
		{
			FInputModeGameOnly InputModeData;
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(false);

		}
	}
	UpdateHUDHealth();
	UpdateHUDStamina();
	if (HasAuthority())
	{
		OnTakeAnyDamage.AddDynamic(this, &APlayerCharacter_cpp::ReceiveDamage);
	}
}

void APlayerCharacter_cpp::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(APlayerCharacter_cpp, UseableItems, COND_OwnerOnly);
	DOREPLIFETIME(APlayerCharacter_cpp, MatchState);
	DOREPLIFETIME(APlayerCharacter_cpp, NumOfPlayersReady);
    DOREPLIFETIME(APlayerCharacter_cpp, Health);
	DOREPLIFETIME(APlayerCharacter_cpp, Stamina);
	//UE_LOG(LogTemp, Warning, TEXT("Character begin play!"));

	//for player health and stamina
	
}

// Called every frame
void APlayerCharacter_cpp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UE_LOG(LogTemp, Warning, TEXT("Character tick!"));

	//Sprinting Functionality
	/*
	if (bIsSprinting) {
		Stamina -= StaminaDrainRate * DeltaTime;
	}
	else {
		Stamina += StaminaRegenRate * DeltaTime;
	}

	Stamina = FMath::Clamp(Stamina, 0.0f, MaxStamina);
	*/
}

// Called to bind functionality to input
void APlayerCharacter_cpp::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Menu", IE_Pressed, this, &APlayerCharacter_cpp::UseKeyPressed);
	// Sprint functionality 
	//PlayerInputComponent->BindAction("SprintButton", IE_Pressed, this, &APlayerCharacter_cpp::StartSprint);
	//PlayerInputComponent->BindAction("Sprint", IE_Released, this, &APlayerCharacter_cpp::StopSprint);
}


void APlayerCharacter_cpp::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (Missions)
	{
		//Missions->PlayerCharacter = this;
	}
}

void APlayerCharacter_cpp::PlayerHasReadyUp()
{
	
	
}
void APlayerCharacter_cpp::GetReadyAmount_Implementation()
{
	int32 curval = NumOfPlayersReady;
	UE_LOG(LogTemp, Warning, TEXT("NUM PLAYERS READY CALLED FROM SERVER READY AMOUNT: %d"), NumOfPlayersReady);
	UE_LOG(LogTemp, Warning, TEXT("CUR VALUE CALLED FROM SERVER READY AMOUNT: %d"), curval);
}


void APlayerCharacter_cpp::SetOverlappingObject(AUseableItems* OverlappedObject)
{
	if (UseableItems)
	{
		UseableItems->ShowItemUseWidget(false);
	}
	UseableItems = OverlappedObject;
	if (IsLocallyControlled())
	{
		if (UseableItems)
		{
			UseableItems->ShowItemUseWidget(true);
		}
	}
}

void APlayerCharacter_cpp::OnRep_UseableItems(AUseableItems* LastObject)
{
	if (UseableItems)
	{
		UseableItems->ShowItemUseWidget(true);
	}
	if (LastObject)
	{
		LastObject->ShowItemUseWidget(false);
	}
}

void APlayerCharacter_cpp::UseKeyPressed()
{
	
	if (MissionBoardWidget == nullptr) return;
	
	if (MissionBoard == nullptr && UseableItems)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Menu key pressed! Creating Widget!!"));
		MissionBoard = CreateWidget<UMissionBoard>(GetWorld(), MissionBoardWidget);
	}
	if (MissionBoard)
	{
		MissionBoard->MenuSetup();
	}
		
		//Missions->ShowMissionBoardWidget();
	
}
void APlayerCharacter_cpp::OnMatchStateSet(FName State)
{
	//MatchState = State;
}
void APlayerCharacter_cpp::HandleCooldown()
{
}

void APlayerCharacter_cpp::Server_SetPlayerIsReady_Implementation()
{
	//NumOfPlayersReady++;
	//Multicast_UpdatePlayersReady(NumOfPlayersReady);
	// Get Lobby Game Mode
	LobbyGameMode = Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode());
	if (LobbyGameMode)
	{
		//NumOfPlayersReady++;
		//UE_LOG(LogTemp, Warning, TEXT("NUM PLAYERS READY IS: %d"), NumOfPlayersReady);
		// Make sure we have authority i.e. we are the server
		if (HasAuthority())
		{
			//Multicast_UpdatePlayersReady();
			UE_LOG(LogTemp, Warning, TEXT("wadawdawd: %d"), NumOfPlayersReady);
			//UE_LOG(LogTemp, Warning, TEXT("CALLING CHECK PLAYERS"));
			// Server checks all player states
			LobbyGameMode->CheckPlayersReady();
		}					
	}
}
void APlayerCharacter_cpp::OnRep_UpdatePlayersReady()
{
	UE_LOG(LogTemp, Warning, TEXT("BEFORE MULTICAST PLAYERS READY IS: %d"), NumOfPlayersReady);
	Multicast_UpdatePlayersReady();
	UE_LOG(LogTemp, Warning, TEXT("AFTERRRR MULTICAST PLAYERS READY IS: %d"), NumOfPlayersReady);
	
}
void APlayerCharacter_cpp::Multicast_UpdatePlayersReady_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("MULTICAST"));
	NumOfPlayersReady++;
	UE_LOG(LogTemp, Warning, TEXT("UYPDATING PLAYERS READY IS: %d"), NumOfPlayersReady);
}
void APlayerCharacter_cpp::Server_UpdatePlayersReady_Implementation()
{
	
		UE_LOG(LogTemp, Warning, TEXT("IN SERVER FUNC AND HAS AUTHORITY!!!!!"));
		//NumOfPlayersReady++;
		OnRep_UpdatePlayersReady();
	
	//Multicast_UpdatePlayersReady();
}
void APlayerCharacter_cpp::UpdatePlayerReady()
{
	NumOfPlayersReady++;
	Server_UpdatePlayersReady();
}

void APlayerCharacter_cpp::OnRep_MatchState()
{
}
//Player Health Rep Function
void APlayerCharacter_cpp::OnRep_Health(float LastHealth) 
{
	UpdateHUDHealth();
}
void APlayerCharacter_cpp::UpdateHUDHealth() {
	MammothPlayerController = MammothPlayerController == nullptr ? Cast<AMammothPlayerController>(Controller) : MammothPlayerController;
	if (MammothPlayerController) {
		MammothPlayerController->SetHUDHealth(Health, MaxHealth);
	}
}
//Player Stamina Rep Function
void APlayerCharacter_cpp::OnRep_Stamina() 
{
	UpdateHUDStamina();
}
void APlayerCharacter_cpp::UpdateHUDStamina() {
	MammothPlayerController = MammothPlayerController == nullptr ? Cast<AMammothPlayerController>(Controller) : MammothPlayerController;
	if (MammothPlayerController) {
		MammothPlayerController->SetHUDStamina(Stamina, MaxStamina);
	}
}
void APlayerCharacter_cpp::UpdateHUDAmmo(int32 Ammo)
{
	MammothPlayerController = MammothPlayerController == nullptr ? Cast<AMammothPlayerController>(Controller) : MammothPlayerController;
	if (MammothPlayerController) {
		MammothPlayerController->SetHUDAmmo(Ammo);
	}
}
// Following Functions implemented for Stamina/Sprinting

void APlayerCharacter_cpp::StartSprint() 
{

	SetSprinting(true);
}

void APlayerCharacter_cpp::DrainStamina()
{
	Stamina = FMath::Max(Stamina - StaminaDrainRate, 0.0f);
	UpdateHUDStamina();
	if (Stamina <= 0.0f)
	{
		GetWorldTimerManager().ClearTimer(StaminaRegenTimer);
		// Player is out of stamina, stop running
		SetSprinting(false);
		//UE_LOG(LogTemp, Warning, TEXT("DrainStamina SetSprinting False"));
	}
}
void APlayerCharacter_cpp::RegenStamina()
{
	if ((bIsSprinting == false) && (Stamina <= 100.0f)) {
		Stamina = FMath::Min(Stamina + StaminaRegenRate, MaxStamina);
		UpdateHUDStamina();
		//UE_LOG(LogTemp, Warning, TEXT("RegenStam Activated"));
	}
}
void APlayerCharacter_cpp::StopSprint() 
{
	SetSprinting(false);
}
void APlayerCharacter_cpp::SetSprinting(bool bNewSprintState)
{
	if (bIsSprinting != bNewSprintState)
	{
		bIsSprinting = bNewSprintState;
		OnSprintStateChangeBPEvent(bIsSprinting);
		if (bIsSprinting)
		{
			GetWorldTimerManager().SetTimer(StaminaDrainTimer, this, &APlayerCharacter_cpp::DrainStamina, 1.0f, true);
			GetWorldTimerManager().ClearTimer(StaminaRegenTimer);
		}
		else
		{
			GetWorldTimerManager().SetTimer(StaminaRegenTimer, this, &APlayerCharacter_cpp::RegenStamina, 1.0f, true);
			GetWorldTimerManager().ClearTimer(StaminaDrainTimer);
			//notifiy
		}
	}
}
void APlayerCharacter_cpp::ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* InstigatorController, AActor* DamageCauser)
{
	float DamageToHealth = Damage;
	Health = FMath::Clamp(Health - DamageToHealth, 0.f, MaxHealth);
	if (HasAuthority())
	{
		UpdateHUDHealth();
	}
	//UE_LOG(LogTemp, Warning, TEXT("MY health is!: %f"), Health);
}