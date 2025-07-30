// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"

UTriggerComponent::UTriggerComponent(){
    PrimaryComponentTick.bCanEverTick = true;
    
}

void UTriggerComponent::BeginPlay(){
	Super::BeginPlay();

}

void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction){
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    if(Mover == nullptr){
        return;
    }
    AActor* Actor = GetAcceptActor();
    if(Actor != nullptr){
        UPrimitiveComponent* component = Cast<UPrimitiveComponent>(Actor->GetRootComponent());
        if(component != nullptr){
            component->SetSimulatePhysics(false);
        }
        Actor->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
        Mover->SetShouldMove(true);
    } else{
        Mover->SetShouldMove(false);
    }

}

void UTriggerComponent::setMover(UMover* NewMover){
    Mover = NewMover;
}

AActor* UTriggerComponent::GetAcceptActor() const{

    TArray<AActor*> Actors;
    GetOverlappingActors(Actors);
    
    for(AActor* Actor : Actors){

        bool HasAcceptTag = Actor->ActorHasTag(AcceptTagName);
        bool isGrabbed = Actor->ActorHasTag("Grabbed");
        if (HasAcceptTag && !isGrabbed){
            return Actor;
        }
    }
    return nullptr;
}
