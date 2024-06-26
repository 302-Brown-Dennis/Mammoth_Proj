// Bullets fired from guns
// Author: Jake Bottenberg


#include "Projectiles.h"
#include "Kismet/GameplayStatics.h"
#include "Mammoth/Enemies/EnemyAI.h"
#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"
#include "Mammoth/PlayerCharacter_cpp.h"
#include "GameFramework/Actor.h"


AProjectiles::AProjectiles()
{
	
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	SetRootComponent(CollisionBox);
	CollisionBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	//CollisionBox->SetCollisionResponseToChannel(ECC_Skel)


}


void AProjectiles::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		CollisionBox->OnComponentHit.AddDynamic(this, &AProjectiles::OnHit);
	}
	
}
void AProjectiles::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	
	
}

void AProjectiles::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectiles::SetOwningPlayer(APlayerCharacter_cpp* OwningPlayer)
{
	
}

