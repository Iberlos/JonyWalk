#include "Obstacle.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "JonyWalk_5_7Character.h"
#include "JonyWalk_5_7GameState.h"


AObstacle::AObstacle()
{
    PrimaryActorTick.bCanEverTick = false;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    RootComponent = Mesh;
    Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
    CollisionBox->SetupAttachment(Mesh);
    CollisionBox->SetCollisionProfileName(TEXT("BlockAll"));

    TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
    TriggerBox->SetupAttachment(Mesh);
    TriggerBox->SetCollisionProfileName(TEXT("Trigger"));
    TriggerBox->SetGenerateOverlapEvents(true);

    // Bind overlap
    TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AObstacle::OnTriggerOverlap);
}

void AObstacle::BeginPlay()
{
    Super::BeginPlay();

    // Get mesh bounds (local space)
    FVector Origin;
    FVector BoxExtent;
    Mesh->GetLocalBounds(Origin, BoxExtent);

    // Fit main collision box
    CollisionBox->SetBoxExtent(BoxExtent);

    // Slightly smaller in X/Y
    FVector TriggerExtent = BoxExtent;
    TriggerExtent.X *= 0.9f;
    TriggerExtent.Y *= 0.9f;

    // Set height to 1 meter
    TriggerExtent.Z = 50.f;

    TriggerBox->SetBoxExtent(TriggerExtent);

    // Position above main box
    float TopOfMesh = BoxExtent.Z;
    float TriggerCenterZ = TopOfMesh + TriggerExtent.Z;

    FVector TriggerLocation = FVector::ZeroVector;
    TriggerLocation.Z = TriggerCenterZ;

    TriggerBox->SetRelativeLocation(TriggerLocation);

    AJonyWalk_5_7GameState* GS = GetWorld()->GetGameState<AJonyWalk_5_7GameState>();
    if (GS)
    {
        GS->AddToMaxScore(Score);
    }
}

void AObstacle::OnTriggerOverlap(
    UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult
)
{
    AJonyWalk_5_7Character* player = Cast<AJonyWalk_5_7Character>(OtherActor);
    if (player && player->IsSkating())
    {
        AJonyWalk_5_7GameState* GS = GetWorld()->GetGameState<AJonyWalk_5_7GameState>();
        if (GS)
        {
            TriggerBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            GS->AddScore(Score);
            //GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, "Score!");
        }
    }
}