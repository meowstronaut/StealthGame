#include "FPSBlackHole.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"


AFPSBlackHole::AFPSBlackHole() {
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = MeshComp;

	AttractiveSphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("AttractiveSphereComp"));
	AttractiveSphereComp->SetSphereRadius(3000);
	AttractiveSphereComp->SetupAttachment(MeshComp);

	DestructiveSphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("DestructiveSphereComp"));
	DestructiveSphereComp->SetSphereRadius(100);
	DestructiveSphereComp->SetupAttachment(MeshComp);

	DestructiveSphereComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSBlackHole::OverlapDestructiveSphere);
}


void AFPSBlackHole::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	TArray<UPrimitiveComponent*> OverlappingComps;
	AttractiveSphereComp->GetOverlappingComponents(OverlappingComps);

	for (int32 i = 0; i < OverlappingComps.Num(); i++) {
		UPrimitiveComponent* PrimComp = OverlappingComps[i];
		if (PrimComp && PrimComp->IsSimulatingPhysics()) {
			const float SphereRadius = AttractiveSphereComp->GetScaledSphereRadius();
			const float ForceStrength = -2000;

			PrimComp->AddRadialForce(GetActorLocation(), SphereRadius, ForceStrength,
				ERadialImpulseFalloff::RIF_Constant, true);
		}
	}
}

void AFPSBlackHole::OverlapDestructiveSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

	if (OtherActor) {
		OtherActor->Destroy();
	}
}