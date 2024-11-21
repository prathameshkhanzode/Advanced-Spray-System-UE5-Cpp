// Fill out your copyright notice in the Description page of Project Settings.


#include "SprayActorComponent.h"

#include "SprayActor.h"
#include "Spray_SystemCharacter.h"
#include "Camera/CameraComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
USprayActorComponent::USprayActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}

// Called when the game starts
void USprayActorComponent::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void USprayActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool USprayActorComponent::CanSpray(FHitResult& OutHitResult)
{
	FVector LineTraceStart, LineTraceEnd;
	GetLineTraceStartandEnd(LineTraceStart, LineTraceEnd);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner());

	bool bHit = GetWorld()->LineTraceSingleByChannel(OutHitResult, LineTraceStart, LineTraceEnd,
		ECC_Visibility, QueryParams);

	return bHit && OutHitResult.GetComponent()->IsA(UStaticMeshComponent::StaticClass());
}

void USprayActorComponent::RequestSpray()
{
	FHitResult OutHitResult;
	if (CanSpray(OutHitResult))
	{
		FVector& SpawnLocation = OutHitResult.ImpactPoint;
		FRotator SpawnRotation;
		FVector SpawnScale;
		FVector& ImpactNormalValues = OutHitResult.ImpactNormal;
		
		if (const ASpray_SystemCharacter* Character = Cast<ASpray_SystemCharacter>(GetOwner()))
		{
			const UCameraComponent* FPCamera = Character->GetComponentByClass<UCameraComponent>();
			constexpr float Tolerance = 0.001f;

			if (FMath::IsNearlyEqual(ImpactNormalValues.Z, 1.0f, Tolerance))
			{
				SpawnRotation = FRotator(90.0f, FPCamera->GetForwardVector().Rotation().Yaw, 180.0f);
				SpawnScale = FVector(0.01f, -0.2f, 0.2f);
			}
			else if (FMath::IsNearlyEqual(ImpactNormalValues.Z, -1.0f, Tolerance))
			{
				SpawnRotation = FRotator(-90.0f, FPCamera->GetForwardVector().Rotation().Yaw, 180.0f);
				SpawnScale = FVector(0.01f, -0.2f, 0.2f);
			}
			else if (FMath::IsNearlyEqual(ImpactNormalValues.Z, 0.0f, Tolerance))
			{
				SpawnRotation = FRotator(ImpactNormalValues.Rotation().Pitch, ImpactNormalValues.Rotation().Yaw + 180.0f,
					FPCamera->GetComponentRotation().Roll);
				SpawnScale = FVector(0.01f, 0.2f, 0.2f);
			}
			else
			{
				SpawnRotation = FRotator(ImpactNormalValues.Rotation().Pitch, ImpactNormalValues.Rotation().Yaw,
					OutHitResult.GetComponent()->GetRelativeRotation().Yaw - 90.f - FPCamera->GetComponentRotation().Yaw);
				SpawnScale = FVector(0.01f, -0.2f, 0.2f);
			}
		}
		Server_SpawnSpray(SpawnLocation, SpawnRotation, SpawnScale);
	}
}

void USprayActorComponent::Multicast_SpawnSpray_Implementation(const FVector& SpawnLocation,
	const FRotator& SpawnRotation, const FVector& SpawnScale)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();
	SpawnParams.Instigator = GetOwner()->GetInstigator();

	SpawnedSprayActor = GetWorld()->SpawnActor<ASprayActor>(SprayActorClass, SpawnLocation, SpawnRotation, SpawnParams);
	if (SpawnedSprayActor)
	{
		SpawnedSprayActor->Server_RandomSpray();
		SpawnedSprayActor->SetActorScale3D(SpawnScale);
		ManageSprayArray(SpawnedSprayActor);
	}
}

void USprayActorComponent::ManageSprayArray(ASprayActor* NewSpray)
{
	SprayArray.Add(NewSpray);
	if (SprayArray.Num() > MaxSprayCount)
	{
		if (ASprayActor* OldSpray = SprayArray[0])
		{
			OldSpray->Destroy();
		}
		SprayArray.RemoveAt(0);
	}
}

void USprayActorComponent::GetLineTraceStartandEnd(FVector& LineTraceStart, FVector& LineTraceEnd)
{
	if (ASpray_SystemCharacter* Character = Cast<ASpray_SystemCharacter>(GetOwner()))
	{
		if (UCameraComponent* FPCamera = Character->FindComponentByClass<UCameraComponent>())
		{
			FVector ViewLocation = FPCamera->GetComponentLocation();
			FRotator ViewRotation = FPCamera->GetComponentRotation();

			LineTraceStart = ViewLocation;
			LineTraceEnd = LineTraceStart + ViewRotation.Vector() * 500.0f;
		}
	}
}

void USprayActorComponent::Server_SpawnSpray_Implementation(const FVector& SpawnLocation,const FRotator& SpawnRotation,
	const FVector& SpawnScale)
{
	Multicast_SpawnSpray(SpawnLocation, SpawnRotation, SpawnScale);
}

bool USprayActorComponent::Server_SpawnSpray_Validate(const FVector& SpawnLocation,const FRotator& SpawnRotation,
	const FVector& SpawnScale)
{
	return true;
}

void USprayActorComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USprayActorComponent, SprayArray);
}