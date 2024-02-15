// Loosley based on Stephen Ulibarri Udemy course https://www.udemy.com/course/unreal-engine-the-ultimate-game-developer-course/
// Adapted to work with multiplayer
// Author: Dennis Brown


#include "EnemyAnimInstance.h"
#include "EnemyAI.h"
#include "Kismet/KismetMathLibrary.h"


void UEnemyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	Enemy = Cast<AEnemyAI>(TryGetPawnOwner());
	Pawn = TryGetPawnOwner();
}

void UEnemyAnimInstance::UpdateAnimationProperties(float DeltaTime)
{
	if (Enemy == nullptr)
	{
		Enemy = Cast<AEnemyAI>(TryGetPawnOwner());
		Pawn = TryGetPawnOwner();
		
	}
	if (Pawn)
	{
		FVector Speed = Pawn->GetVelocity();
		FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0.f);
		MoveSpeed = LateralSpeed.Size();

		Direction = CalculateDirection(Speed, Pawn->GetActorRotation());

		 
	}
}


