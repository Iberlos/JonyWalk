#include "JonyWalk_5_7HUD.h"
#include "Engine/Canvas.h"
#include "JonyWalk_5_7GameState.h"

void AJonyWalkHUD::DrawHUD()
{
    Super::DrawHUD();

    if (!Canvas) return;

    AJonyWalk_5_7GameState* GS = GetWorld()->GetGameState<AJonyWalk_5_7GameState>();
    if (!GS) return;

    // --- Top-left score ---
    FString ScoreText = GetScoreText();

    DrawText(
        ScoreText,
        FLinearColor::White,
        50.f,
        50.f,
        nullptr,
        1.5f,
        false
    );

    // --- Center "Completed" ---
    if (GS->bCompleted)
    {
        FString CompletedText = TEXT("COMPLETED");

        float TextWidth, TextHeight;
        GetTextSize(CompletedText, TextWidth, TextHeight, nullptr, 2.5f);

        float CenterX = (Canvas->SizeX - TextWidth) * 0.5f;
        float CenterY = (Canvas->SizeY - TextHeight) * 0.5f;

        DrawText(
            CompletedText,
            FLinearColor::Green,
            CenterX,
            CenterY,
            nullptr,
            2.5f,
            false
        );
    }
}

FString AJonyWalkHUD::GetScoreText() const
{
    AJonyWalk_5_7GameState* GS = GetWorld()->GetGameState<AJonyWalk_5_7GameState>();
    if (!GS) return TEXT("Score: 0 / 0");

    return FString::Printf(TEXT("Score: %d / %d"), GS->CurrentScore, GS->MaxScore);
}