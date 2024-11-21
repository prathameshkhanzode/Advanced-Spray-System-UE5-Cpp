// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SprayActor.h"
#include "Components/ActorComponent.h"
#include "SprayActorComponent.generated.h"

class ASprayActor;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPRAY_SYSTEM_API USprayActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USprayActorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category="Spray System")
	bool CanSpray(FHitResult& OutHitResult);

	UFUNCTION(BlueprintCallable, Category="Spray System")
	void RequestSpray();

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "Spray System")
	void Server_SpawnSpray(const FVector& SpawnLocation,const FRotator& SpawnRotation,const FVector& SpawnScale);

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "Spray System")
	void Multicast_SpawnSpray(const FVector& SpawnLocation,const FRotator& SpawnRotation,const FVector& SpawnScale);

private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spray System", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ASprayActor> SprayActorClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spray System", meta = (AllowPrivateAccess = "true"))
	int32 MaxSprayCount = 5;

	UPROPERTY(Replicated, meta = (AllowPrivateAccess = "true"))
	TArray<ASprayActor*> SprayArray;

	UFUNCTION(BlueprintCallable, Category="Spray System")
	void ManageSprayArray(ASprayActor* NewSpray);

	UFUNCTION(BlueprintCallable, Category="Spray System")
	void GetLineTraceStartandEnd(FVector& LineTraceStart, FVector& LineTraceEnd);

	UPROPERTY()
	ASprayActor* SpawnedSprayActor;
};
