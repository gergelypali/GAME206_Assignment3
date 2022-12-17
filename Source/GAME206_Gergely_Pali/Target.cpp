// Fill out your copyright notice in the Description page of Project Settings.

#include "Target.h"
#include "Components/PointLightComponent.h"
#include "Components/SphereComponent.h"
#include "Components/BillboardComponent.h"
#include "GameFramework/Actor.h"
#include "NetworkGameState.h"
#include "GAME206_Gergely_PaliCharacter.h"

ATarget::ATarget()
{
    DesiredIntensity = 3000.0f;

    // light component to make it easier to find
    PointLight1 = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLight1"));
    PointLight1->Intensity = DesiredIntensity;
    PointLight1->SetVisibility(true);
    RootComponent = PointLight1;

    // sphere collider to register the collision with the bullets
    SphereCollider1 = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollider1"));
    SphereCollider1->InitSphereRadius(95.0f);
    SphereCollider1->SetVisibility(false);
    SphereCollider1->BodyInstance.SetCollisionProfileName("PhysicsActor");
    SphereCollider1->SetupAttachment(RootComponent);
    SphereCollider1->OnComponentHit.AddDynamic(this, &ATarget::OnHit);

    // a billboard to display an image to the player that is always facing it so it is much easier to see and find
    BillBoard1 = CreateDefaultSubobject<UBillboardComponent>(TEXT("BillBoard1"));
    BillBoard1->SetVisibility(true);
    static ConstructorHelpers::FObjectFinder<UTexture2D> BillboardTexture(TEXT("Texture2D'/Game/FirstPersonArms/Character/Textures/UE4_LOGO_CARD.UE4_LOGO_CARD'"));
    if (BillboardTexture.Succeeded())
    {
        BillBoard1->SetSprite(BillboardTexture.Object);
    }
    BillBoard1->SetHiddenInGame(false);
    BillBoard1->SetupAttachment(RootComponent);

    bReplicates = 1;
}

void ATarget::OnHit_Implementation(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
    {
        AGAME206_Gergely_PaliCharacter* OtherRealOwner = Cast<AGAME206_Gergely_PaliCharacter>(OtherActor->GetInstigator());
        if (OtherRealOwner)
        {
            OtherRealOwner->HandleTargetDied();
        }
        OtherActor->Destroy();
        Destroy();
    }
}
