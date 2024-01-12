// Currently unused
// Author: Dennis Brown

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MissionComponents.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MAMMOTH_API UMissionComponents : public UActorComponent
{
	GENERATED_BODY()

public:	

	UMissionComponents();

	void ShowMissionBoardWidget();

	//
	// Does the player character need to be a friend?
	// Does the player need to accsess stuff in here?
	// 
	// friend class APlayerCharacter_cpp;

protected:

	virtual void BeginPlay() override;

public:	

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	class APlayerCharacter_cpp* PlayerCharacter;

private:

	
	class UMissionBoard* MissionBoard;
		
};
