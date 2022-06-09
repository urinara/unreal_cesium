// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <glm/glm.hpp>
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

class AMyGlobeAwarePawn;
class ACesiumGeoreference;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FLeftMouseDoubleClick, const FVector2D&, ScreenPos, const FHitResult&, HitResult);

UCLASS()
class MYPROJECT13_API AMyPlayerController : public APlayerController
{
    GENERATED_BODY()

    AMyGlobeAwarePawn* MyGlobeAwarePawn;
    ACesiumGeoreference* CesiumGeoReference;

    bool leftMousePressed = false;
    bool rightMousePressed = false;
    FHitResult panHitResult;
    FVector2D leftMouseDownPos;
    FVector2D leftMouseMovePos;
    double timeLastPanned = 0.0;
    glm::dvec3 pawnLLH = glm::dvec3(0.0);

protected:
    virtual void BeginPlay() override;
    virtual void SetupInputComponent() override;
    virtual void OnPossess(APawn* InPawn) override;

public:
    AMyPlayerController();

    void MoveRight(float Val);
    void MoveForward(float Val);
    void MoveUp(float Val);
    void Turn(float Val);
    void LookUp(float Val);
    void MouseMove(float Val);

    void HandleLeftMouseButtonPressed();
    void HandleLeftMouseButtonReleased();
    void HandleRightMouseButtonPressed();
    void HandleRightMouseButtonReleased();
    void HandleLeftMouseDoubleClk();

    UFUNCTION(BlueprintCallable, Category = "Eleventh")
    bool GetMouseHitResult(FHitResult& hitResult);

    UPROPERTY(BlueprintAssignable, Category = "Eleventh");
    FLeftMouseDoubleClick LeftMouseDoubleClick;

};
