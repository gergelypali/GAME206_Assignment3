// Copyright Epic Games, Inc. All Rights Reserved.

#include "GAME206_Gergely_PaliCharacter.h"
#include "GAME206_Gergely_PaliProjectile.h"
#include "GAME206_Gergely_PaliGameMode.h"
#include "GameFramework/PlayerState.h"
#include "NetworkGameState.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "CountdownSceneComponent.h"
#include "Target.h"
#include "Components/TextRenderComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Engine/Canvas.h"
#include "Components/CanvasPanel.h"


//////////////////////////////////////////////////////////////////////////
// AGAME206_Gergely_PaliCharacter

AGAME206_Gergely_PaliCharacter::AGAME206_Gergely_PaliCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	GetCapsuleComponent()->SetIsReplicated(true);

	// set our turn rates for input
	TurnRateGamepad = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->SetIsReplicated(true);
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetIsReplicated(true);
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

	static ConstructorHelpers::FClassFinder<UUserWidget> ScoreObj(TEXT("/Game/BP_HUD"));
	HUDWidgetClass = ScoreObj.Class;
}

void AGAME206_Gergely_PaliCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	TSoftClassPtr<AActor> ActorBpClass = TSoftClassPtr<AActor>(FSoftObjectPath(TEXT("Blueprint'/Game/FirstPerson/Blueprints/BP_FirstPersonProjectile.BP_FirstPersonProjectile_C'")));
	LoadedBpAsset = ActorBpClass.LoadSynchronous();

	GameState = (ANetworkGameState*)GetWorld()->GetGameState();
	GameState->GameWon.AddDynamic(this, &AGAME206_Gergely_PaliCharacter::GameWonHandler);

	if (IsLocallyControlled())
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);
		CurrentWidget->AddToViewport();
		ScoreTextBlock = (UTextBlock*)CurrentWidget->GetWidgetFromName(TEXT("ScoreNum"));
		ScoreCanvas = (UCanvasPanel*)CurrentWidget->GetWidgetFromName(TEXT("ScoreCanvas"));
		EndCanvas = (UCanvasPanel*)CurrentWidget->GetWidgetFromName(TEXT("GameOverCanvas"));

		EndCanvas->SetVisibility(ESlateVisibility::Hidden);

		InitScore();
	}

}

//////////////////////////////////////////////////////////////////////////// Input

void AGAME206_Gergely_PaliCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind fire event
	PlayerInputComponent->BindAction("PrimaryAction", IE_Pressed, this, &AGAME206_Gergely_PaliCharacter::OnPrimaryAction);

	// Enable touchscreen input
	EnableTouchscreenMovement(PlayerInputComponent);

	// Bind movement events
	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &AGAME206_Gergely_PaliCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &AGAME206_Gergely_PaliCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "Mouse" versions handle devices that provide an absolute delta, such as a mouse.
	// "Gamepad" versions are for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn Right / Left Gamepad", this, &AGAME206_Gergely_PaliCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("Look Up / Down Gamepad", this, &AGAME206_Gergely_PaliCharacter::LookUpAtRate);
}

void AGAME206_Gergely_PaliCharacter::OnPrimaryAction()
{
	// Trigger the OnItemUsed Event
	Fire();
	UpdateScore();
	OnUseItem.Broadcast();
}

void AGAME206_Gergely_PaliCharacter::Fire_Implementation()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
	// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
	FVector MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);
	const FVector SpawnLocation = GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);

	//Set Spawn Collision Handling Override
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	ActorSpawnParams.Owner = this;
	ActorSpawnParams.Instigator = this;

	GetWorld()->SpawnActor(LoadedBpAsset, &SpawnLocation, &SpawnRotation, ActorSpawnParams);
}

void AGAME206_Gergely_PaliCharacter::BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == true)
	{
		return;
	}
	if ((FingerIndex == TouchItem.FingerIndex) && (TouchItem.bMoved == false))
	{
		OnPrimaryAction();
	}
	TouchItem.bIsPressed = true;
	TouchItem.FingerIndex = FingerIndex;
	TouchItem.Location = Location;
	TouchItem.bMoved = false;
}

void AGAME206_Gergely_PaliCharacter::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == false)
	{
		return;
	}
	TouchItem.bIsPressed = false;
}

void AGAME206_Gergely_PaliCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AGAME206_Gergely_PaliCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AGAME206_Gergely_PaliCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void AGAME206_Gergely_PaliCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

bool AGAME206_Gergely_PaliCharacter::EnableTouchscreenMovement(class UInputComponent* PlayerInputComponent)
{
	if (FPlatformMisc::SupportsTouchInput() || GetDefault<UInputSettings>()->bUseMouseForTouch)
	{
		PlayerInputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AGAME206_Gergely_PaliCharacter::BeginTouch);
		PlayerInputComponent->BindTouch(EInputEvent::IE_Released, this, &AGAME206_Gergely_PaliCharacter::EndTouch);

		return true;
	}
	
	return false;
}

void AGAME206_Gergely_PaliCharacter::GameWonHandler_Implementation()
{
	ScoreCanvas->SetVisibility(ESlateVisibility::Hidden);
	EndCanvas->SetVisibility(ESlateVisibility::Visible);
	Cast<UTextBlock>((UTextBlock*)CurrentWidget->GetWidgetFromName(TEXT("PlayerFinalScore")))->SetText(FText::FromString(FString::SanitizeFloat(GetPlayerState()->GetScore())));
	Cast<UTextBlock>((UTextBlock*)CurrentWidget->GetWidgetFromName(TEXT("WinnerPlayer")))->SetText(FText::FromString(GameState->GetWinningPlayer()));
}

void AGAME206_Gergely_PaliCharacter::SpawnNewTarget_Implementation()
{
	SpawnLocationTarget = GenerateNewSpawnLocation();
	ATarget* NewTarget = (ATarget*)GetWorld()->SpawnActor(ATarget::StaticClass(), SpawnLocationTarget);
}

FTransform* AGAME206_Gergely_PaliCharacter::GenerateNewSpawnLocation()
{
	int x = FMath::RandRange(200, 2800);
	int y = FMath::RandRange(200, 3300);

	FTransform* newLocation = new FTransform(FVector3d(x, y, 1000));
	return newLocation;
}

void AGAME206_Gergely_PaliCharacter::HandleTargetDied()
{
	GameState->AddScore(GetPlayerState());
	UpdateScore();
	SpawnNewTarget();
}

void AGAME206_Gergely_PaliCharacter::InitScore_Implementation()
{
	ScoreTextBlock->SetText(FText::FromString(FString::SanitizeFloat(0.0f)));
}

void AGAME206_Gergely_PaliCharacter::UpdateScore_Implementation()
{
	ScoreTextBlock->SetText(FText::FromString(FString::SanitizeFloat(GameState->GetPlayerScore(GetPlayerState()))));
}
