// 11thD

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "EleventhLibrary.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_DELEGATE_OneParam(FExecDelegate, bool, result);

UCLASS()
class MYPROJECT13_API UEleventhLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable)
	static void AsyncExec(FExecDelegate delegate, const FString& command, const FString& arguments);

	UFUNCTION(BlueprintCallable)
	static void AsyncHttp(FExecDelegate delegate, const FString& restApi);
};

//RamaMeleeWeapon class .h

