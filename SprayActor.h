// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SprayActor.generated.h"

UCLASS()
class SPRAY_SYSTEM_API ASprayActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASprayActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(Server, Reliable)
	void Server_RandomSpray();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_RandomSpray(UTexture2D* NewTexture);

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components", meta = (AllowPrivateAccess = "true"))
	UDecalComponent* SprayDecal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components", meta = (AllowPrivateAccess = "true"))
	TArray<UTexture2D*> SprayTexture;

	UFUNCTION()
	UTexture2D* GetRandomSpray() const;
};
