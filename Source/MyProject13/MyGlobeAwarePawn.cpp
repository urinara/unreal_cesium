
#include "MyGlobeAwarePawn.h"
#include "CesiumGeoreference.h"
#include "DrawDebugHelpers.h"
#include <glm/glm.hpp>


AMyGlobeAwarePawn::AMyGlobeAwarePawn() : AGlobeAwareDefaultPawn()
{
}

void AMyGlobeAwarePawn::BeginPlay()
{
    Super::BeginPlay();
}

void AMyGlobeAwarePawn::Pan(FHitResult hitResult, float XVal, float YVal, bool start)
{
    ACesiumGeoreference* geoRef = GetGeoreference();
    glm::dvec3 hitUE = glm::dvec3(hitResult.Location.X, hitResult.Location.Y, hitResult.Location.Z);

    if (start)
    {
        hitLLH = geoRef->TransformUnrealToLongitudeLatitudeHeight(hitUE);
        pawnStartLLH = GlobeAnchor->GetLongitudeLatitudeHeight();
        FVector actorPos = GetActorLocation();
        pawnStartUE = glm::dvec3(actorPos.X, actorPos.Y, actorPos.Z);
        FIntVector originLoc = GetWorld()->OriginLocation;
        startWorldOrigin = glm::dvec3(originLoc.X, originLoc.Y, originLoc.Z);

        speedAtHeight = -1.0 * PanningSpeed * pawnStartLLH.z;
        return;
    }

    double currentTime = FPlatformTime::Seconds();
    if (currentTime - timeLastPanned < 0.1) return; // give at least 150 ms interval

    // Update the caches
    timeLastPanned = FPlatformTime::Seconds();

    //glm::dvec3 newHitLLH = geoRef->TransformUnrealToLongitudeLatitudeHeight(hitUE);
    //glm::dvec3 newHitLLH = geoRef->GetGeoTransforms().TransformUnrealToLongitudeLatitudeHeight(startWorldOrigin, hitUE);
    //glm::dvec3 deltaLLH = hitLLH - newHitLLH;
    //glm::dvec3 newPawnLLH = pawnStartLLH + glm::dvec3(deltaLLH.x, deltaLLH.y, 0.0);
    //glm::dvec3 deltaLLH = pawnStartLLH - hitLLH;
    //glm::dvec3 newPawnLLH = glm::dvec3(newHitLLH.x, newHitLLH.y, pawnStartLLH.z) + glm::dvec3(deltaLLH.x, deltaLLH.y, 0.0);

    // right direction along the surface
    FVector side = FRotationMatrix(GetViewRotation()).GetScaledAxis(EAxis::Y);
    glm::dvec3 sideDir = glm::normalize(glm::dvec3(side.X, side.Y, side.Z));

    // surface normal
    glm::dvec4 upEcef(CesiumGeospatial::Ellipsoid::WGS84.geodeticSurfaceNormal(GlobeAnchor->GetECEF()), 0.0);
    glm::dvec4 upVec4 = upEcef * geoRef->GetGeoTransforms().GetEllipsoidCenteredToAbsoluteUnrealWorldTransform();
    glm::dvec3 upDir = glm::normalize(glm::dvec3(upVec4.x, upVec4.y, upVec4.z));

    // get front direction perpendicular to surface normal
    glm::dvec3 frontDir = glm::normalize(glm::cross(sideDir, upDir));

    // overall move direction
    glm::dvec3 moveDir = static_cast<double>(XVal) * sideDir + static_cast<double>(YVal) * frontDir * SkewCorrection;

    UE_LOG(LogTemp, Warning, TEXT("XVal=%f, YVal=%f, moveDir=%lf"),
        XVal, YVal, glm::length(moveDir));
    glm::dvec3 newPawnUE = speedAtHeight * moveDir + pawnStartUE;
    //glm::dvec3 newPawnLLH = geoRef->TransformUnrealToLongitudeLatitudeHeight(newPawnUE);
    glm::dvec3 newPawnLLH = geoRef->GetGeoTransforms().TransformUnrealToLongitudeLatitudeHeight(
        startWorldOrigin,
        newPawnUE);

    UE_LOG(LogTemp, Warning, TEXT("pawnStartUE=%lf,%lf,%lf, %lf,     %lf,%lf,%lf,    %s"),
        pawnStartUE.x, pawnStartUE.y, pawnStartUE.z,
        glm::distance(newPawnUE, pawnStartUE),
        newPawnLLH.x, newPawnLLH.y, newPawnLLH.z,
        *(GetWorld()->OriginLocation.ToString()));

    newPawnLLH.z = pawnStartLLH.z;

    GlobeAnchor->MoveToLongitudeLatitudeHeight(newPawnLLH);
}
