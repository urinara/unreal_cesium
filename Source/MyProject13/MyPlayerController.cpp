// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPlayerController.h"
#include "MyGlobeAwarePawn.h"
#include "CesiumGeoreference.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/PlayerInput.h"
#include "GameFramework/PawnMovementComponent.h"


AMyPlayerController::AMyPlayerController()
{
}

void AMyPlayerController::BeginPlay()
{
    Super::BeginPlay();
}

// Borrowed from Engine/DefaultPawn.cpp
void InitializeDefaultPawnInputBindingsEx()
{
    static bool bBindingsAdded = false;
    if (!bBindingsAdded)
    {
        bBindingsAdded = true;

        // WSAD
        UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("Default_MoveForward", EKeys::W, 1.f));
        UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("Default_MoveForward", EKeys::S, -1.f));
        UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("Default_MoveForward", EKeys::MouseWheelAxis, 1.f));
        UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("Default_MoveRight", EKeys::A, -1.f));
        UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("Default_MoveRight", EKeys::D, 1.f));
        // Up/Down/Left/Right
        UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("Default_MoveForward", EKeys::Up, 1.f));
        UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("Default_MoveForward", EKeys::Down, -1.f));
        UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("Default_MoveRight", EKeys::Left, -1.f));
        UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("Default_MoveRight", EKeys::Right, 1.f));
        // E/C, PageDown/PageUp
        UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("Default_MoveUp", EKeys::E, 1.f));
        UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("Default_MoveUp", EKeys::C, -1.f));
        UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("Default_MoveUp", EKeys::PageUp, 1.f));
        UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("Default_MoveUp", EKeys::PageDown, -1.f));
        // Mouse X/Y/2D
        UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("Default_Turn", EKeys::MouseX, 1.f));
        //UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("Default_TurnRate", EKeys::Left, -1.f));
        //UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("Default_TurnRate", EKeys::Right, 1.f));
        UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("Default_LookUp", EKeys::MouseY, -1.f));
        //UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("Default_LookUpRate", EKeys::Up, 1.f));
        //UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("Default_LookUpRate", EKeys::Down, -1.f));
        UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("Default_MouseMove", EKeys::Mouse2D));

        UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("Default_LeftMouseButton", EKeys::LeftMouseButton));
        UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("Default_RightMouseButton", EKeys::RightMouseButton));
    }
}

void AMyPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    InitializeDefaultPawnInputBindingsEx();

    InputComponent->BindAxis("Default_MoveForward", this, &AMyPlayerController::MoveForward);
    InputComponent->BindAxis("Default_MoveRight", this, &AMyPlayerController::MoveRight);
    InputComponent->BindAxis("Default_MoveUp", this, &AMyPlayerController::MoveUp);
    InputComponent->BindAxis("Default_Turn", this, &AMyPlayerController::Turn);
    //InputComponent->BindAxis("Default_TurnRate", this, &AMyPlayerController::TurnAtRate);
    InputComponent->BindAxis("Default_LookUp", this, &AMyPlayerController::LookUp);
    //InputComponent->BindAxis("Default_LookUpRate", this, &AMyPlayerController::LookUpAtRate);
    InputComponent->BindAxis("Default_MouseMove", this, &AMyPlayerController::MouseMove);

    InputComponent->BindAction("Default_LeftMouseButton", IE_Pressed, this, &AMyPlayerController::HandleLeftMouseButtonPressed);
    InputComponent->BindAction("Default_LeftMouseButton", IE_Released, this, &AMyPlayerController::HandleLeftMouseButtonReleased);
    InputComponent->BindAction("Default_LeftMouseButton", IE_DoubleClick, this, &AMyPlayerController::HandleLeftMouseDoubleClk);
    InputComponent->BindAction("Default_RightMouseButton", IE_Pressed, this, &AMyPlayerController::HandleRightMouseButtonPressed);
    InputComponent->BindAction("Default_RightMouseButton", IE_Released, this, &AMyPlayerController::HandleRightMouseButtonReleased);
}

void AMyPlayerController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    if (InPawn->IsA<AMyGlobeAwarePawn>()) {
        MyGlobeAwarePawn = Cast<AMyGlobeAwarePawn>(InPawn);
        SetShowMouseCursor(true);
    }
}

void AMyPlayerController::MoveRight(float Val) {
    MyGlobeAwarePawn->MoveRight(Val);
}

void AMyPlayerController::MoveForward(float Val) {
    MyGlobeAwarePawn->MoveForward(Val);
}

void AMyPlayerController::MoveUp(float Val) {
    MyGlobeAwarePawn->MoveUp_World(Val);
}

void AMyPlayerController::Turn(float Val) {
    if (!rightMousePressed) return; // turn if right clicked
    MyGlobeAwarePawn->AddControllerYawInput(Val);
}

void AMyPlayerController::LookUp(float Val) {
    if (!rightMousePressed) return; // looup if right clicked
    MyGlobeAwarePawn->AddControllerPitchInput(Val);
}

bool AMyPlayerController::GetMouseHitResult(FHitResult& hitResult)
{
    FVector hitLocation, hitDirection;
    DeprojectMousePositionToWorld(hitLocation, hitDirection);

    FVector hitEndLocation = hitLocation + (hitDirection * 100000000000000.f);
    FCollisionQueryParams queryParams;
    queryParams.bTraceComplex = true;
    queryParams.bReturnFaceIndex = true;
    bool hasHit = GetWorld()->LineTraceSingleByChannel(hitResult, hitLocation, hitEndLocation, ECollisionChannel::ECC_Visibility, queryParams);
    UE_LOG(LogTemp, Warning, TEXT("hitLoc=%s, hitEnd=%s, hasHit=%s"), *hitLocation.ToString(), *hitEndLocation.ToString(), hasHit ? TEXT("true") : TEXT("false"));
    //DrawDebugLine(GetWorld(), hitResult.TraceStart, hitResult.Location, FColor(0, 0, 255), true, 20.f);

    return hasHit;
}

void AMyPlayerController::HandleLeftMouseButtonPressed()
{
    leftMousePressed = true;

    if (MyGlobeAwarePawn == nullptr) {
        MyGlobeAwarePawn = Cast<AMyGlobeAwarePawn>(GetPawn());
    }

    if (MyGlobeAwarePawn == nullptr) {
        UE_LOG(LogTemp, Error, TEXT("MyGlobeAwarePawn is NULL."));
        return;
    }

    // cache mouse screen and hit positions
    GetMousePosition(leftMouseDownPos.X, leftMouseDownPos.Y);
    leftMouseMovePos = leftMouseDownPos;

    bool hasHit = GetMouseHitResult(panHitResult);
    if (hasHit) {
        //MyGlobeAwarePawn->GetGeoreference()->KeepWorldOriginNearCamera = false;
        MyGlobeAwarePawn->Pan(panHitResult, 0.f, 0.f, true);
    }

    UE_LOG(LogTemp, Warning, TEXT("Handle left mouse single clicked. %s"), *leftMouseDownPos.ToString());
}

void AMyPlayerController::HandleLeftMouseButtonReleased()
{
    leftMousePressed = false;
    //MyGlobeAwarePawn->GetGeoreference()->KeepWorldOriginNearCamera = true;

    bool hasHit = GetMouseHitResult(panHitResult);
    if (hasHit) {
        //MyGlobeAwarePawn->GetGeoreference()->KeepWorldOriginNearCamera = false;
        //MyGlobeAwarePawn->Pan(panHitResult, 0.f, 0.f, false);
    }
}

void AMyPlayerController::HandleLeftMouseDoubleClk()
{
    if (LeftMouseDoubleClick.IsBound())
    {
        GetMousePosition(leftMouseDownPos.X, leftMouseDownPos.Y);
        //panHitResult = GetMouseHitResult();
        //pawnLLH = MyGlobeAwarePawn->GlobeAnchor->GetLongitudeLatitudeHeight();
        UE_LOG(LogTemp, Warning, TEXT("Handle left mouse double clicked. hitResult=%s"), *panHitResult.ToString());
        LeftMouseDoubleClick.Broadcast(leftMouseDownPos, panHitResult);
    }
}

void AMyPlayerController::HandleRightMouseButtonPressed()
{
    rightMousePressed = true;
}

void AMyPlayerController::HandleRightMouseButtonReleased()
{
    rightMousePressed = false;
}

void AMyPlayerController::MouseMove(float Value)
{
    if (!leftMousePressed) return;

    int viewPortSizeX, viewPortSizeY;
    GetViewportSize(viewPortSizeX, viewPortSizeY);
    FVector2D newMouseMovePos;
    GetMousePosition(newMouseMovePos.X, newMouseMovePos.Y);

    if (FVector2D::Distance(leftMouseMovePos, newMouseMovePos) < 2.f) return;

    float delX = (newMouseMovePos.X - leftMouseDownPos.X) / (viewPortSizeX - 1.f);
    float delY = (newMouseMovePos.Y - leftMouseDownPos.Y) / (viewPortSizeY - 1.f);

    FHitResult newPanHitResult;
    bool hasHit = GetMouseHitResult(newPanHitResult);
    if (hasHit) {
        MyGlobeAwarePawn->Pan(newPanHitResult, delX, delY, false);
    }

    leftMouseMovePos = newMouseMovePos;
}
