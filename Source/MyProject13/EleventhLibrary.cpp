// 11thD


#include "EleventhLibrary.h"

void UEleventhLibrary::AsyncExec(FExecDelegate delegate, const FString command, const FString arguments)
{
    UE_LOG(LogTemp, Warning, TEXT("Command: %s %s"), *command, *arguments);

    AsyncTask(ENamedThreads::AnyNormalThreadNormalTask, [delegate, command, arguments]()
    {
        int32 returnCode;
        FString outStdOut;
        FString outStdErr;
        bool result = FPlatformProcess::ExecProcess(*command, *arguments, &returnCode, &outStdOut, &outStdErr);
        UE_LOG(LogTemp, Warning, TEXT("StdOut: %s"), *outStdOut);
        UE_LOG(LogTemp, Warning, TEXT("StdOut: %s"), *outStdErr);

        AsyncTask(ENamedThreads::GameThread, [delegate, result]()
        {
            delegate.ExecuteIfBound(result);
        });

    });

    //FProcHandle procHandle = FPlatformProcess::CreateProc(*command, *arguments, false, true, true, NULL, 0, NULL, NULL);
    //return true;
}
