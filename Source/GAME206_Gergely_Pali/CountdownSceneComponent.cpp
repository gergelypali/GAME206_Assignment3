// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/TextRenderComponent.h"
#include "GAME206_Gergely_PaliGameMode.h"
#include "CountdownSceneComponent.h"

// Sets default values for this component's properties
UCountdownSceneComponent::UCountdownSceneComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// create a text renderer to show the timer to the player
	CountdownText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("CountdownNumber"));
	CountdownText->SetTextRenderColor(FColor::MakeRandomColor());
	CountdownText->SetHorizontalAlignment(EHTA_Center);
	CountdownText->SetVerticalAlignment(EVRTA_TextTop);
	CountdownText->SetWorldSize(25.0f);
	// move to the desired location on the "HUD"
	CountdownText->SetRelativeLocation(FVector(300.0f, 0, 150.0f));
	CountdownText->SetRelativeRotation(FRotator(0, 180.0f, 0));
	CountdownText->SetIsReplicated(true);
	CountdownText->SetupAttachment(this);
}


// Called when the game starts
void UCountdownSceneComponent::BeginPlay()
{
	Super::BeginPlay();

	//GameMode = (AGAME206_Gergely_PaliGameMode*)GetWorld()->GetAuthGameMode();

	//UpdateTimerDisplay();
	//GetWorld()->GetTimerManager().SetTimer(CountdownTimerHandle, this, &UCountdownSceneComponent::AdvanceTimer, 1.0f, true);
}


// Called every frame
void UCountdownSceneComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCountdownSceneComponent::UpdateScoreDisplay(float NewScore)
{
	CountdownText->SetText(FText::AsNumber(NewScore));
	//CountdownText->SetText(FText::AsNumber(FMath::Max(GameMode->GameTimer, 0)));
}

void UCountdownSceneComponent::AdvanceTimer()
{
	/*
	--GameMode->GameTimer;
	UpdateTimerDisplay();
	if (GameMode->GameTimer < 1)
	{
		// We're done counting down, so stop running the timer.
		GetWorld()->GetTimerManager().ClearTimer(CountdownTimerHandle);
		//Perform any special actions we want to do when the timer ends.
		CountdownHasFinished();
	}
	*/
}

void UCountdownSceneComponent::CountdownHasFinished()
{
	/*
	//Change to the GAME over printout
	CountdownText->SetText(FText::FromString("GAME OVER!"));
	CountdownText->SetWorldSize(100.0f);
	CountdownText->SetRelativeLocation(FVector(300.0f, 0, 100.0f));
	GameMode->PauseAndDisplayResult();
	*/
}

void UCountdownSceneComponent::GameWonText()
{
	/*
	// stop the timer so it wont update the printout
	GetWorld()->GetTimerManager().ClearTimer(CountdownTimerHandle);
	//Change to the Game Won printout with the timer
	int time = GameMode->GameTimer;
	CountdownText->SetText(FText::Format(FText::FromString("YOU HAVE WON!\nTHE FINAL SCORE IS {0}"), time));
	CountdownText->SetWorldSize(50.0f);
	CountdownText->SetRelativeLocation(FVector(300.0f, 0, 100.0f));
	GameMode->PauseAndDisplayResult();
	*/
}
