// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"

void ATower::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle FireTimerHandle;
	GetWorldTimerManager().SetTimer(FireTimerHandle, this, &ATower::CheckFireCondition, FireRate, true);
}

void ATower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsInFireRange())
	{
		RotateTurret(Tank->GetActorLocation());
	}


}

void ATower::CheckFireCondition()
{
	if (IsInFireRange() && !Tank->GetIsAlive())
	{
		Fire();
	}
}

bool ATower::IsInFireRange()
{
	if (Tank)
	{
		float DistanceToTank = FVector::Dist(GetActorLocation(), Tank->GetActorLocation());

		return DistanceToTank <= FireRange;
	}

	return false;
}

void ATower::HandleDestruction()
{
	Super::HandleDestruction();

	Destroy();
};