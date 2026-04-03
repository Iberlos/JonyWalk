#include "JonyWalk_5_7GameState.h"

AJonyWalk_5_7GameState::AJonyWalk_5_7GameState()
{
    MaxScore = 0;
    CurrentScore = 0;
    bCompleted = false;
}

void AJonyWalk_5_7GameState::AddToMaxScore(int Amount)
{
    if (Amount <= 0) return;

    MaxScore += Amount;
}

void AJonyWalk_5_7GameState::AddScore(int Amount)
{
    if (Amount <= 0 || bCompleted) return;

    CurrentScore += Amount;

    CheckCompletion();
}

void AJonyWalk_5_7GameState::CheckCompletion()
{
    if (CurrentScore >= MaxScore && MaxScore > 0)
    {
        bCompleted = true;
        //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, "Completed!");
    }
}

int AJonyWalk_5_7GameState::GetCurrentScore()
{
    return CurrentScore;
}
