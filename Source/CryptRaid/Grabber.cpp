// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"



// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UPhysicsHandleComponent* PhyHandle = GetPhysicsHandle();

	if(PhyHandle && PhyHandle->GetGrabbedComponent()){
		FVector tarLoc = GetComponentLocation() + GetForwardVector() * HoldDist;
		PhyHandle->SetTargetLocationAndRotation(tarLoc, GetComponentRotation());
	}
}

void UGrabber::Release(){
	UPhysicsHandleComponent* PhyHandle = GetPhysicsHandle();
	
	if(PhyHandle && PhyHandle->GetGrabbedComponent()){
		AActor* GrabbedActor = PhyHandle->GetGrabbedComponent()->GetOwner();
		GrabbedActor->Tags.Remove("Grabbed");
		PhyHandle->ReleaseComponent();
	}

}

void UGrabber::Grab(){
	UPhysicsHandleComponent* PhyHandle = GetPhysicsHandle();

	if(PhyHandle == nullptr){
		return;
	}
	FHitResult HitResult;
	bool HasHit = GetGrabbableInReach(HitResult);
    if(HasHit){
		
		UPrimitiveComponent* HitComp = HitResult.GetComponent();
		HitComp->SetSimulatePhysics(true);
		HitComp->WakeAllRigidBodies();
		AActor* HitActor = HitResult.GetActor();
		HitActor->Tags.Add("Grabbed");
		HitActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		PhyHandle->GrabComponentAtLocationWithRotation(
			HitComp,
			NAME_None,
			HitResult.ImpactPoint,
			GetComponentRotation()
		);
	} 
}

UPhysicsHandleComponent* UGrabber::GetPhysicsHandle() const{
	UPhysicsHandleComponent* result = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if(result ==nullptr){
	   UE_LOG(LogTemp, Error, TEXT("Physics handle needed"));

	}
	return result;
}

bool UGrabber::GetGrabbableInReach(FHitResult& OutHitRes) const{
	FVector start = GetComponentLocation();
	FVector end = start + GetForwardVector()* MaxGrabDist;

	DrawDebugLine(GetWorld(), start, end, FColor::Red);
	DrawDebugSphere(GetWorld(), end, 0.1, 0.1, FColor::Blue, false, 1);
	FCollisionShape sphere = FCollisionShape::MakeSphere(GrabRad);
	return GetWorld()->SweepSingleByChannel(
		OutHitRes, 
		start, end, 
		FQuat::Identity, ECC_GameTraceChannel2, sphere);
}
