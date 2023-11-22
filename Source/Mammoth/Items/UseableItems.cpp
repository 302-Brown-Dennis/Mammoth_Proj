// Fill out your copyright notice in the Description page of Project Settings.


#include "UseableItems.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Mammoth/PlayerCharacter_cpp.h"
// Sets default values
AUseableItems::AUseableItems()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	CollisionVolume = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionVolume"));
	RootComponent = CollisionVolume;

	ItemUseWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("UseWidget"));
	ItemUseWidget->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AUseableItems::BeginPlay()
{
	Super::BeginPlay();
	if (ItemUseWidget)
	{
		ItemUseWidget->SetVisibility(false);
	}
	if (HasAuthority())
	{
		CollisionVolume->OnComponentBeginOverlap.AddDynamic(this, &AUseableItems::OnOverlapBegin);
		CollisionVolume->OnComponentEndOverlap.AddDynamic(this, &AUseableItems::OnOverlapEnd);

		CollisionVolume->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	}
	
}

// Called every frame
void AUseableItems::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AUseableItems::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter_cpp* playerCharacter = Cast<APlayerCharacter_cpp>(OtherActor);
	if (playerCharacter && ItemUseWidget)
	{
		ItemUseWidget->SetVisibility(true);
	}
	UE_LOG(LogTemp, Warning, TEXT("Over lap begin"));
}
void AUseableItems::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APlayerCharacter_cpp* playerCharacter = Cast<APlayerCharacter_cpp>(OtherActor);
	if (playerCharacter && ItemUseWidget)
	{
		ItemUseWidget->SetVisibility(false);
	}
	UE_LOG(LogTemp, Warning, TEXT("Over lap end"));
}

void AUseableItems::ShowItemUseWidget(bool bShowWidget)
{
	if (ItemUseWidget)
	{
		ItemUseWidget->SetVisibility(bShowWidget);
	}
}

