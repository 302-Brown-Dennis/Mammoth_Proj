// Fill out your copyright notice in the Description page of Project Settings.


#include "UseableItems.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/TextBlock.h"
#include "Mammoth/PlayerCharacter_cpp.h"
#include "Mammoth/HUD/ObjectUseWidget.h"
// Sets default values
AUseableItems::AUseableItems()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;


	CollisionVolume = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionVolume"));
	RootComponent = CollisionVolume;

	ObjectUseWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("ObjectUseWidget"));
	ObjectUseWidget->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AUseableItems::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		CollisionVolume->OnComponentBeginOverlap.AddDynamic(this, &AUseableItems::OnOverlapBegin);
		CollisionVolume->OnComponentEndOverlap.AddDynamic(this, &AUseableItems::OnOverlapEnd);

		CollisionVolume->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	}
	if (ObjectUseWidget)
	{
		UObjectUseWidget* CastObjectUseWidget = Cast<UObjectUseWidget>(ObjectUseWidget->GetUserWidgetObject());

		CastObjectUseWidget->SetDisplayText(NewString);
		ObjectUseWidget->SetVisibility(false);
	}
	
}

// Called every frame
void AUseableItems::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AUseableItems::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter_cpp* PlayerCharacter = Cast<APlayerCharacter_cpp>(OtherActor);
	if (PlayerCharacter)
	{
		PlayerCharacter->SetOverlappingObject(this);
	}
	//UE_LOG(LogTemp, Warning, TEXT("Super::Over lap begin, in UseableItems class!"));
}
void AUseableItems::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APlayerCharacter_cpp* PlayerCharacter = Cast<APlayerCharacter_cpp>(OtherActor);
	if (PlayerCharacter)
	{
		PlayerCharacter->SetOverlappingObject(nullptr);
	}
	//UE_LOG(LogTemp, Warning, TEXT("Super::Over lap end, in UseableItems class!"));
}

void AUseableItems::ShowItemUseWidget(bool bShowWidget)
{
	if (ObjectUseWidget)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Setting visibilty!"));
		ObjectUseWidget->SetVisibility(bShowWidget);
	}
}

