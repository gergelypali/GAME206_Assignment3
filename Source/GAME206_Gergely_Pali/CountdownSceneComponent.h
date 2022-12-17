// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "CountdownSceneComponent.generated.h"

class UTextRenderComponent;
class AGAME206_Gergely_PaliGameMode;

// on screen visualization of the game timer
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAME206_GERGELY_PALI_API UCountdownSceneComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCountdownSceneComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	// we need the game mode to get the timer from it
	// GAME206_Gergely_PaliGameMode* GameMode;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UTextRenderComponent* CountdownText;

	void UpdateScoreDisplay(float NewScore);

	void AdvanceTimer();

	void CountdownHasFinished();

	FTimerHandle CountdownTimerHandle;

	void GameWonText();

};
