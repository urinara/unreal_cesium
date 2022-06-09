// 11thD

#include "EleventhLibrary.h"
#include "HttpModule.h"

void UEleventhLibrary::AsyncExec(FExecDelegate delegate, const FString& command, const FString& arguments)
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

void UEleventhLibrary::AsyncHttp(FExecDelegate delegate, const FString& restApi)
{
    UE_LOG(LogTemp, Warning, TEXT("RestApi: %s"), *restApi);

    AsyncTask(ENamedThreads::AnyNormalThreadNormalTask, [delegate, restApi]()
    {
        FHttpModule* httpModule = &FHttpModule::Get();
        TSharedRef<IHttpRequest, ESPMode::ThreadSafe> request = httpModule->CreateRequest();
        request->SetURL(restApi);
        request->SetVerb("GET");
        request->SetHeader("User-Agent", "X-EleventhD-Agent");
        request->OnProcessRequestComplete().BindLambda([delegate](FHttpRequestPtr req, FHttpResponsePtr res, bool result) {
            AsyncTask(ENamedThreads::GameThread, [delegate]() {
                delegate.ExecuteIfBound(true);
            });
        });
        request->ProcessRequest();

    });

    //FProcHandle procHandle = FPlatformProcess::CreateProc(*command, *arguments, false, true, true, NULL, 0, NULL, NULL);
    //return true;
}
