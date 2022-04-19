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
	
	Original = GetTransform();
	if(TargetLocations.Num() > 0)
		GlobalStartLocation = GetTransform().TransformPosition(TargetLocations[LocationIndex]);
		GlobalTarget = GetTransform().TransformPosition(TargetLocations[LocationIndex + LocationIncrement]);
}

void AMovingPlatform::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	//should only change on server to than replicate to clients
	if(HasAuthority()) // is this the server?
	{
		FVector Location = GetActorLocation();

		float JourneyLength = (GlobalTarget - GlobalStartLocation).Length();
		float JourneyTravelled = (Location - GlobalStartLocation).Length();
		
		if(JourneyTravelled > JourneyLength)
		{
			LocationIndex += LocationIncrement;

			if(LocationIndex == TargetLocations.Num() - 1)
			{
				if(bShouldLoop == true)
				{
					LocationIndex = -1;
					LocationIncrement = 1;
					GlobalStartLocation = Original.TransformPosition(TargetLocations[TargetLocations.Num() - 1]);
					GlobalTarget = Original.TransformPosition(TargetLocations[0]);
				}
				else
				{
					LocationIncrement = -1;
					GlobalStartLocation = Original.TransformPosition(TargetLocations.Last());
					GlobalTarget = Original.TransformPosition(TargetLocations[TargetLocations.Num() - 2]);
				}
			}
			else if(LocationIndex == 0)
			{
				LocationIncrement = 1;
				GlobalStartLocation = Original.TransformPosition(TargetLocations[0]);
				GlobalTarget = Original.TransformPosition(TargetLocations[1]);
			}
			else
			{
				GlobalStartLocation = Original.TransformPosition(TargetLocations[LocationIndex]);
				GlobalTarget = Original.TransformPosition(TargetLocations[LocationIndex + LocationIncrement]);
			}
		}
		
		const FVector Direction = (GlobalTarget - GlobalStartLocation).GetSafeNormal();
		Location += MoveSpeed * DeltaSeconds * Direction;
		SetActorLocation(Location);
	}
}
