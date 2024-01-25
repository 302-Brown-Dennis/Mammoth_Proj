// Class for triggering enemy spawns
// Author: Dennis Brown


#include "EnemyTriggerVolume.h"
#include "Components/BoxComponent.h"
#include "Mammoth/Enemies/EnemyAI.h"
#include "Mammoth/PlayerCharacter_cpp.h"
#include "Engine/TargetPoint.h"

// Sets default values
AEnemyTriggerVolume::AEnemyTriggerVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	RootComponent = TriggerBox;

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AEnemyTriggerVolume::OnOverlapBegin);

}

// Called when the game starts or when spawned
void AEnemyTriggerVolume::BeginPlay()
{
	Super::BeginPlay();
	
}
// Called by client when a client player overlaps
void AEnemyTriggerVolume::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && (OtherActor != this) && OtherComp && HasAuthority() && !bHasTriggered)
    {
        APlayerCharacter_cpp* PlayerCharacter_cpp = Cast<APlayerCharacter_cpp>(OtherActor);
        // Check if the overlapping actor is the player
        if (PlayerCharacter_cpp)
        {
            // Iterate through the array of spawn locations
            for (const FVector& SpawnLocation : SpawnLocations)
            {               
                // Spawn the enemy from the Blueprint
                if (EnemyBlueprint)
                {
                    //UE_LOG(LogTemp, Warning, TEXT("Calling server spawn!"));

                    // Clients calls the server function
                    ServerSpawnEnemy(SpawnLocation, PlayerCharacter_cpp);
                }
                
                // Set flag so enemies stop spawning
                bHasTriggered = true;
            }
        }
    }
}

// Called every frame
void AEnemyTriggerVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Server spawns the enemies 
void AEnemyTriggerVolume::ServerSpawnEnemy_Implementation(const FVector& SpawnLocation, APlayerCharacter_cpp* Target)
{
   //UE_LOG(LogTemp, Warning, TEXT("Server spawning enemies!"));
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    SpawnParams.Instigator = GetInstigator();

    AEnemyAI* NewEnemy = GetWorld()->SpawnActor<AEnemyAI>(EnemyBlueprint, SpawnLocation, FRotator::ZeroRotator, SpawnParams);
    if (NewEnemy)
    {
        NewEnemy->MoveToTarget(Target);
    }


}

