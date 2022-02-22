// Copyright Pedro Matos. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GeneralMovement/Public/Framework/Components/GenMovementComponent.h"
#include "SpaceshipGenMovementComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RTSFPS_API USpaceshipGenMovementComponent : public UGenMovementComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Spaceship|Inputs")
	float PitchInput = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Spaceship|Inputs")
	float YawInput = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Spaceship|Inputs")
	float RollInput = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Spaceship|Inputs")
	int ForwardInput = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Spaceship|Inputs")
	int StrafeInput = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Spaceship|Inputs")
	int VerticalInput = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spaceship|Settings")
	float YawSpeed = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spaceship|Settings")
	float PitchSpeed = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spaceship|Settings")
	float RollSpeed = 30.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spaceship|Settings")
	float YawInputSpeed = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spaceship|Settings")
	float PitchInputSpeed = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spaceship|Settings")
	float RollInputSpeed = 100.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spaceship|Settings|Acceleration")
	float ForwardAcceleration = 11760.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spaceship|Settings|Acceleration")
	float ReverseAcceleration = 5880.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spaceship|Settings|Acceleration")
	float StrafeAcceleration = 3920.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spaceship|Settings|Acceleration")
	float VerticalAcceleration = 3920.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spaceship|Settings|Acceleration|Curves")
	FRuntimeFloatCurve ForwardAccelerationCurve;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spaceship|Settings|Acceleration|Curves")
	FRuntimeFloatCurve ReverseAccelerationCurve;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spaceship|Settings|Acceleration|Curves")
	FRuntimeFloatCurve StrafeAccelerationCurve;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spaceship|Settings|Acceleration|Curves")
	FRuntimeFloatCurve VerticalAccelerationCurve;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spaceship|Settings|Acceleration|Curves")
	bool UseAccelerationCurves = false;
	
	// Common Behavior on Space Sims, also called coupled mode, decelerates when no inputs on each axis
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spaceship|Settings|Acceleration")
	bool DecelerateWhenNoInputOnAxis = true;
	// This to clamp Acceleration and Velocity
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spaceship|Settings|Acceleration")
	bool UseAccelerationLimiter = true;
	
	// Automatically Calls MoveUpdatedComponent
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spaceship|Settings")
	bool AutoMoveUpdatedComponent = true;
	
	// Yaw Amount within the Radius of ControlLimitRadius 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Spaceship|Output")
	float CurrentYaw = 0;
	// Pitch Amount within the Radius of ControlLimitRadius 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Spaceship|Output")
	float CurrentPitch = 0;
	// Pitch Amount 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Spaceship|Output")
	float CurrentRoll = 0;

	// Yaw Amount within the Radius of ControlLimitRadius 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Spaceship|Output")
	float DesiredYaw = 0;
	// Pitch Amount within the Radius of ControlLimitRadius 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Spaceship|Output")
	float DesiredPitch = 0;
	// Pitch Amount 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Spaceship|Output")
	float DesiredRoll = 0;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Spaceship|Output")
	float CurrentForward;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Spaceship|Output")
	float CurrentStrafe;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Spaceship|Output")
	float CurrentVertical;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Spaceship|Output")
	float ForwardAccelerationAlpha = 1;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Spaceship|Output")
	float ReverseAccelerationAlpha = 1;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Spaceship|Output")
	float StrafeAccelerationAlpha = 1;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Spaceship|Output")
	float VerticalAccelerationAlpha = 1;

	// 0 to 1 Yaw Amount within the Radius of ControlLimitRadius 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Spaceship|Output")
	float YawControlPower = 0;
	// 0 to 1 Pitch Amount within the Radius of ControlLimitRadius 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Spaceship|Output")
	float PitchControlPower = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Spaceship|Output")
	float RollControlPower = 0;

	// When angular acceleration hits this value it's zeroed - not allowing it to increase
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spaceship|Settings|Limits|Angular")
	float MaxYawAcceleration = 20;
	// When angular acceleration hits this value it's zeroed - not allowing it to increase
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spaceship|Settings|Limits|Angular")
	float MaxPitchAcceleration = 20;
	// When angular acceleration hits this value it's zeroed - not allowing it to increase
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spaceship|Settings|Limits|Angular")
	float MaxRollAcceleration = 20;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spaceship|Settings|Limits|Movement")
	float MaxVelocity = 120000.0f;
	// 0-to-1 Will be Multiplied to the Max Velocity - Useful for Speed Limiters
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spaceship|Settings|Limits|Movement")
	float MaxVelocityAlpha = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spaceship|Settings|Limits|Angular")
	float YawPitchControlLimitRadius = 150.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spaceship|Settings|Limits|Angular")
	float RollControlLimit = 60.f;

	// True if you want Roll to decelerate when there's no input
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spaceship|Settings|Behavior|Roll")
	bool RollPowerDecays = true;
	// True = Uses the Curve, False = Uses Constant Interpolation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spaceship|Settings|Behavior|Roll")
	bool UseRollDecayCurve = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spaceship|Settings|Behavior|Roll")
	FRuntimeFloatCurve RollDecayCurve;

	// True if you want Yaw and Pitch to decelerate (they will move to the center of the Yaw/Pitch Power Circle when there's no input
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spaceship|Settings|Behavior|Pitch and Yaw")
	bool YawPitchPowerDecays = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spaceship|Settings|Behavior|Pitch and Yaw")
	UCurveFloat* YawPitchDecayCurve;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Spaceship|State")
	float ForwardInputTotalDeltaTime;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Spaceship|State")
	float ReverseInputTotalDeltaTime;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Spaceship|State")
	float StrafeInputTotalDeltaTime;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Spaceship|State")
	float VerticalInputTotalDeltaTime;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Spaceship|State")
	bool IsForwardAcceleration;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Spaceship|State")
	bool IsReverseAcceleration;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Spaceship|State")
	bool IsStrafeAcceleration;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Spaceship|State")
	bool IsVerticalAcceleration;

	UPROPERTY()
	bool IsForwardAccelerationCurveValid;
	UPROPERTY()
	bool IsReverseAccelerationCurveValid;
	UPROPERTY()
	bool IsStrafeAccelerationCurveValid;
	UPROPERTY()
	bool IsVerticalAccelerationCurveValid;
	
	UFUNCTION()
	static float RoundFloatTwoDecimals(float Value);
	UFUNCTION()
	static float RoundFloatOneDecimal(float Value);
	UFUNCTION()
	static int RoundToInt(float Value);

	UFUNCTION()
	static float EvaluateCurve(FRuntimeFloatCurve Curve, const float InTime);
	UFUNCTION()
	static float GetCurveMaxTime(FRuntimeFloatCurve Curve);
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	virtual void PostInitProperties() override;
protected:
	virtual void BindReplicationData_Implementation() override;

	virtual void GenReplicatedTick_Implementation(float DeltaTime) override;
public:
	UFUNCTION(BlueprintCallable)
	void HandleAccelerationCurves(float DeltaTime);
	UFUNCTION(BlueprintCallable)
	void AddDirectionAcceleration(float InAcceleration, float MaxAcceleration, FVector Direction, float DeltaTime);
	UFUNCTION(BlueprintCallable)
	void AddForwardAcceleration(float InAcceleration, float DeltaTime);
	UFUNCTION(BlueprintCallable)
	void AddReverseAcceleration(float InAcceleration, float DeltaTime);
	UFUNCTION(BlueprintCallable)
	void AddStrafeAcceleration(float InAcceleration, float DeltaTime);
	UFUNCTION(BlueprintCallable)
	void AddVerticalAcceleration(float InAcceleration, float DeltaTime);

	UFUNCTION(BlueprintCallable)
	void HandleAcceleration(float DeltaTime);
	UFUNCTION(BlueprintCallable)
	void HandleDecelerationFlags();
	UFUNCTION(BlueprintCallable)
	void HandleDeceleration(float DeltaTime);

	UFUNCTION(BlueprintCallable)
	float AddToMaxVelocityAlpha(float Value);
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetMaxVelocityWithAlpha() const;

	UFUNCTION(BlueprintCallable, Category="Spaceship|Input Handlers")
	FVector ClampYawPitch(const FVector YawPitchVector) const;
	
	UFUNCTION(BlueprintCallable, Category="Spaceship|Input Handlers")
	float HandleYaw(const float DeltaTime);
	UFUNCTION(BlueprintCallable, Category="Spaceship|Input Handlers")
	float HandlePitch(const float DeltaTime);
	UFUNCTION(BlueprintCallable, Category="Spaceship|Input Handlers")
	float HandleRoll(const float DeltaTime);

	UFUNCTION(BlueprintCallable, Category="Spaceship|Limiter")
	void HandleYawPitchControlLimits();
	UFUNCTION(BlueprintCallable, Category="Spaceship|Limiter")
	void HandleRollControlLimits();

	UFUNCTION(BlueprintCallable, Category="Spaceship|Behavior")
	void HandleRollDecay(const float DeltaTime);
	UFUNCTION(BlueprintCallable, Category="Spaceship|Behavior")
	void HandleYawPitchDecay(const float DeltaTime);

	UFUNCTION(BlueprintCallable, Category="Spaceship|Helpers")
	float GetYawWithSpeed(const float Speed, const float DeltaTime) const;
	UFUNCTION(BlueprintCallable, Category="Spaceship|Helpers")
	float GetPitchWithSpeed(const float Speed, const float DeltaTime) const;
	UFUNCTION(BlueprintCallable, Category="Spaceship|Helpers")
	float GetRollWithSpeed(const float Speed, const float DeltaTime) const;

	// Calls SetForwardInput, SetStrafeInput, SetVerticalInput in one pass. Useful to pass in a whole vector
	UFUNCTION(BlueprintCallable, Category="Spaceship|Input")
	void SetMovementInput(FVector Input);
	
	UFUNCTION(BlueprintCallable, Category="Spaceship|Input")
	void SetForwardInput(float Input = 1);
	UFUNCTION(BlueprintCallable, Category="Spaceship|Input")
	void SetStrafeInput(float Input = 1);
	UFUNCTION(BlueprintCallable, Category="Spaceship|Input")
	void SetVerticalInput(float Input = 1);
	UFUNCTION(BlueprintCallable, Category="Spaceship|Input")
	void SetPitchInput(const float Input, const float DeltaTime);
	UFUNCTION(BlueprintCallable, Category="Spaceship|Input")
	void SetYawInput(const float Input, const float DeltaTime);
	UFUNCTION(BlueprintCallable, Category="Spaceship|Input")
	void SetRollInput(const float Input, const float DeltaTime);

	UFUNCTION(BlueprintCallable, Category="Spaceship|Post Process")
	void ApplyAngularFilters(const float DeltaTime);
};
