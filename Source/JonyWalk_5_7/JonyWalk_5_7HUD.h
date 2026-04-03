#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "JonyWalk_5_7HUD.generated.h"

UCLASS()
class JONYWALK_5_7_API AJonyWalkHUD : public AHUD
{
    GENERATED_BODY()

public:
    virtual void DrawHUD() override;

protected:
    FString GetScoreText() const;
};