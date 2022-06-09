// Copyright 2020-2021 CesiumGS, Inc. and Contributors

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DefaultPawn.h"
#include "GlobeAwareDefaultPawn.h"
#include "CesiumGlobeAnchorComponent.h"
#include <glm/vec3.hpp>
#include <vector>
#include "MyGlobeAwarePawn.generated.h"


UCLASS()
class MYPROJECT13_API AMyGlobeAwarePawn : public AGlobeAwareDefaultPawn {
    GENERATED_BODY()

public:
    AMyGlobeAwarePawn();

    virtual void BeginPlay() override;

    virtual void Pan(FHitResult hitResult, float XVal, float YVal, bool start);

    UPROPERTY(EditAnywhere, Category = "Eleventh")
    double PanningSpeed = 250.0;

    UPROPERTY(EditAnywhere, Category = "Eleventh")
    double SkewCorrection = 4.0;

private:

    double timeLastPanned = 0.0;
    double speedAtHeight;
    glm::dvec3 hitLLH;
    glm::dvec3 pawnStartLLH;
    glm::dvec3 pawnStartUE;
    glm::dvec3 startWorldOrigin;

};
