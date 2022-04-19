#include "MovingPlatform.h"

AMovingPlatform::AMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true; // enable tick function
	
	SetMobility(EComponentMobility::Movable); // allow actor to move
}

AMovingPlatform::~AMovingPlatform()
{

}

void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	if(HasAuthority()) // replication is only allowed on the server!!!
	{
		SetReplicates(true); // allow replication
		SetReplicateMovement(true); // allow movement to be replicated
	}
}

void AMovingPlatform::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	//should only change on server to than replicate to clients
	if(HasAuthority()) // is this the server?
	{
		FVector Location = GetActorLocation();
		Location += FVector(MoveSpeed * DeltaSeconds,0,0);
		SetActorLocation(Location);
	}
}
