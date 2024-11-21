// Fill out your copyright notice in the Description page of Project Settings.


#include "SprayActor.h"

#include "Components/DecalComponent.h"

// Sets default values
ASprayActor::ASprayActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SprayDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("SprayDecal"));

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SprayDecal->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ASprayActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASprayActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UTexture2D* ASprayActor::GetRandomSpray() const
{
	if (SprayTexture.Num() > 0)
	{
		int32 RandomIndex = FMath::RandRange(0, SprayTexture.Num() - 1);
		return SprayTexture[RandomIndex];
	}
	return nullptr;
}

void ASprayActor::Server_RandomSpray_Implementation()
{
	if (UTexture2D* RandomTexture = GetRandomSpray())
	{
		Multicast_RandomSpray(RandomTexture);
	}
}

void ASprayActor::Multicast_RandomSpray_Implementation(UTexture2D* NewTexture)
{
	if (NewTexture && SprayDecal)
	{
		if (UMaterialInstanceDynamic* DynamicInstance = SprayDecal->CreateDynamicMaterialInstance())
		{
			DynamicInstance->SetTextureParameterValue("SprayTexture", NewTexture);
		}
	}
}

