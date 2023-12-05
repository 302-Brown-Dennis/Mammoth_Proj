// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimInstance.h"
#include "EnemyAI.h"

void UEnemyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	Enemy = Cast<AEnemyAI>(TryGetPawnOwner());
	Pawn = TryGetPawnOwner();
}

void UEnemyAnimInstance::UpdateAnimationProperties()
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
	}
}
