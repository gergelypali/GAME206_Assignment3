// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GAME206_Gergely_PaliGameMode.h"
#include "Target.generated.h"

// target class that the player can shoot
UCLASS()
class GAME206_GERGELY_PALI_API ATarget : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, Category = "Target Variables")
	float DesiredIntensity;

	UFUNCTION(Server, Reliable)
	void OnHit(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(VisibleAnywhere, Category = "Target Component")
	class UPointLightComponent* PointLight1;

	UPROPERTY(VisibleAnywhere, Category = "Target Component")
	class UBillboardComponent* BillBoard1;

	UPROPERTY(VisibleAnywhere, Category = "Target Component")
	class USphereComponent* SphereCollider1;

	// Sets default values for this actor's properties
	ATarget();
};
