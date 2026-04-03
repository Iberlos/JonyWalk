#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "JonyWalk_5_7GameState.generated.h"

UCLASS()
class JONYWALK_5_7_API AJonyWalk_5_7GameState : public AGameStateBase
{
    GENERATED_BODY()

public:
    AJonyWalk_5_7GameState();

    UPROPERTY(BlueprintReadOnly, Category = "Score")
    int MaxScore;

    UPROPERTY(BlueprintReadOnly, Category = "Score")
    int CurrentScore;

    UPROPERTY(BlueprintReadOnly, Category = "Score")
    bool bCompleted;

    UFUNCTION(BlueprintCallable, Category = "Score")
    void AddToMaxScore(int Amount);

    UFUNCTION(BlueprintCallable, Category = "Score")
    void AddScore(int Amount);

protected:
    void CheckCompletion();
    int GetCurrentScore();
};