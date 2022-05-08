// Copyright Pedro Matos. All Rights Reserved

#include "SpaceshipGenMovementComponent.h"
#include "RotationHelpers.h"
#include "Kismet/KismetMathLibrary.h"

// Called when the game starts
void USpaceshipGenMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	IsForwardAccelerationCurveValid = ForwardAccelerationCurve.GetRichCurveConst()->Keys.Num() > 0;
	IsReverseAccelerationCurveValid = ReverseAccelerationCurve.GetRichCurveConst()->Keys.Num() > 0;
	IsStrafeAccelerationCurveValid = StrafeAccelerationCurve.GetRichCurveConst()->Keys.Num() > 0;
	IsVerticalAccelerationCurveValid = VerticalAccelerationCurve.GetRichCurveConst()->Keys.Num() > 0;

	if (GetGenPawnOwner())
	{
		GetGenPawnOwner()->SetInputMode(EInputMode::AllAbsolute);
	}
}

void USpaceshipGenMovementComponent::PostInitProperties()
{
	Super::PostInitProperties();

	// Settings for Reducing Corrections due to the Speed of Movement of the Spaceship
	NetworkPreset = ENetworkPreset::Custom;
	MaxVelocityError = 5.0f;
}

void USpaceshipGenMovementComponent::BindReplicationData_Implementation()
{
	Super::BindReplicationData_Implementation();

	BindFloat(CurrentYaw, true, false, false);
	BindFloat(DesiredYaw, true, false, false);
	BindFloat(YawControlPower, true, false, false);

	BindFloat(CurrentPitch, true, false, false);
	BindFloat(DesiredPitch, true, false, false);
	BindFloat(PitchControlPower, true, false, false);

	BindFloat(CurrentRoll, true, false, false);
	BindFloat(DesiredRoll, true, false, false);
	BindFloat(RollControlPower, true, false, false);

	BindFloat(ForwardInputTotalDeltaTime, true, false, false);
	BindFloat(ReverseInputTotalDeltaTime, true, false, false);
	BindFloat(StrafeInputTotalDeltaTime, true, false, false);
	BindFloat(VerticalInputTotalDeltaTime, true, false, false);
}

void USpaceshipGenMovementComponent::GenReplicatedTick_Implementation(float DeltaTime)
{
	Super::GenReplicatedTick_Implementation(DeltaTime);

	// GMC_CLOG(!IsReplaying(), Error, TEXT("START: T: %f - Rotation = %s"), GetMoveTimestamp(), *GetRootCollisionRotation().ToString());
	
	const float CurrentDeltaTime = GetPhysDeltaTime();
	// const float CurrentDeltaTime = RoundFloatTwoDecimals(GetPhysDeltaTime());

	const auto RotationInputVal = GetMoveRotationInput();
	SetYawInput(RotationInputVal.Yaw, CurrentDeltaTime);
	SetPitchInput(RotationInputVal.Pitch, CurrentDeltaTime);
	SetRollInput(RotationInputVal.Roll, CurrentDeltaTime);

	SetMovementInput(GetMoveInputVector());

	CurrentPitch = FMath::FInterpConstantTo(CurrentPitch, DesiredPitch, CurrentDeltaTime, PitchSpeed);
	CurrentYaw = FMath::FInterpConstantTo(CurrentYaw, DesiredYaw, CurrentDeltaTime, YawSpeed);

	HandleRollDecay(CurrentDeltaTime);
	HandleYawPitchDecay(CurrentDeltaTime);

	HandleDecelerationFlags();
	HandleAccelerationCurves(CurrentDeltaTime);
	
	HandleAcceleration(CurrentDeltaTime);
	HandleDeceleration(CurrentDeltaTime);

	const FVector CurrentVelocity = GetVelocity();

	UpdateVelocity(
		CurrentVelocity.GetClampedToSize(-MaxVelocity, MaxVelocity)
	);

	ApplyAngularFilters(DeltaTime);
	
	if (AutoMoveUpdatedComponent)
	{
		FHitResult Hit;
		
		MoveUpdatedComponent(
			CurrentVelocity * CurrentDeltaTime,
			GetRootCollisionRotation(),
			true,
			&Hit,
			ETeleportType::None
		);

		if (Hit.IsValidBlockingHit())
		{
			AdjustVelocityFromHit(Hit, CurrentDeltaTime);

			MoveUpdatedComponent(
				GetVelocity() * CurrentDeltaTime,
				GetRootCollisionRotation(),
				true,
				nullptr,
				ETeleportType::None
			);
		}
	}
}

float USpaceshipGenMovementComponent::GetCurveMaxTime(const FRuntimeFloatCurve Curve)
{
	float MinTime;
	float MaxTime;
	
	Curve.GetRichCurveConst()->GetTimeRange(MinTime, MaxTime);

	return MaxTime;
}

float USpaceshipGenMovementComponent::EvaluateCurve(const FRuntimeFloatCurve Curve, const float InTime)
{
	return Curve.GetRichCurveConst()->Eval(InTime);
}

void USpaceshipGenMovementComponent::HandleAccelerationCurves(const float DeltaTime)
{
	ForwardAccelerationAlpha = 1;
	ReverseAccelerationAlpha = 1;
	StrafeAccelerationAlpha = 1;
	VerticalAccelerationAlpha = 1;
	
	if (!UseAccelerationCurves)
	{
		return;
	}

	const float ForwardCurveMaxTime = GetCurveMaxTime(ForwardAccelerationCurve);
	const float ReverseCurveMaxTime = GetCurveMaxTime(ForwardAccelerationCurve);
	const float StrafeCurveMaxTime = GetCurveMaxTime(ForwardAccelerationCurve);
	const float VerticalCurveMaxTime = GetCurveMaxTime(ForwardAccelerationCurve);

	if (IsForwardAccelerationCurveValid)
	{
		if (IsForwardAcceleration)
		{
			ForwardInputTotalDeltaTime = FMath::Clamp(ForwardInputTotalDeltaTime + DeltaTime, 0.f, ForwardCurveMaxTime);
		}
		else
		{
			ForwardInputTotalDeltaTime = 1 - FMath::Clamp(ForwardInputTotalDeltaTime - DeltaTime, 0.f, ForwardCurveMaxTime);;
		}

		ForwardAccelerationAlpha = EvaluateCurve(ForwardAccelerationCurve, ForwardInputTotalDeltaTime);
	}

	if(IsReverseAccelerationCurveValid)
	{
		if (IsReverseAcceleration)
		{
			ReverseInputTotalDeltaTime = FMath::Clamp(ReverseInputTotalDeltaTime + DeltaTime, 0.f, ReverseCurveMaxTime);
		}
		else
		{
			ReverseInputTotalDeltaTime = 1 - FMath::Clamp(ReverseInputTotalDeltaTime - DeltaTime, 0.f, ReverseCurveMaxTime);;
		}

		ReverseAccelerationAlpha = EvaluateCurve(ReverseAccelerationCurve, ReverseInputTotalDeltaTime);
	}

	if (IsStrafeAccelerationCurveValid)
	{
		if (IsStrafeAcceleration)
		{
			StrafeInputTotalDeltaTime = FMath::Clamp(StrafeInputTotalDeltaTime + DeltaTime, 0.f, StrafeCurveMaxTime);
		}
		else
		{
			StrafeInputTotalDeltaTime = 1 - FMath::Clamp(StrafeInputTotalDeltaTime - DeltaTime, 0.f, StrafeCurveMaxTime);;
		}
	
		StrafeAccelerationAlpha = EvaluateCurve(StrafeAccelerationCurve, StrafeInputTotalDeltaTime);
	}

	if (IsVerticalAccelerationCurveValid)
	{
		if (IsVerticalAcceleration)
		{
			VerticalInputTotalDeltaTime = FMath::Clamp(VerticalInputTotalDeltaTime + DeltaTime, 0.f, VerticalCurveMaxTime);
		}
		else
		{
			VerticalInputTotalDeltaTime = 1 - FMath::Clamp(VerticalInputTotalDeltaTime - DeltaTime, 0.f, VerticalCurveMaxTime);;
		}
	
		VerticalAccelerationAlpha = EvaluateCurve(VerticalAccelerationCurve, VerticalInputTotalDeltaTime);
	}
}

void USpaceshipGenMovementComponent::AddDirectionAcceleration(const float InAcceleration, const float MaxAcceleration, const FVector Direction, const float DeltaTime)
{
	float ResultAcceleration = InAcceleration;

	if (UseAccelerationLimiter)
	{
		ResultAcceleration = FMath::Clamp(InAcceleration, -MaxAcceleration, MaxAcceleration);
	}
	
	const FVector AccelValue = Direction * RoundFloatTwoDecimals(ResultAcceleration * ReverseAccelerationAlpha);

	AddAcceleration(AccelValue, DeltaTime);
}


void USpaceshipGenMovementComponent::AddForwardAcceleration(const float InAcceleration, const float DeltaTime)
{
	if (!GetOwner())
	{
		return;
	}
	
	const FVector ForwardVector = GetOwner()->GetActorForwardVector();

	AddDirectionAcceleration(FMath::Abs(InAcceleration), ForwardAcceleration, ForwardVector, DeltaTime);
}

void USpaceshipGenMovementComponent::AddReverseAcceleration(const float InAcceleration, const float DeltaTime)
{
	if (!GetOwner())
	{
		return;
	}
	
	const FVector ReverseVector = -GetOwner()->GetActorForwardVector();

	AddDirectionAcceleration(FMath::Abs(InAcceleration), ReverseAcceleration, ReverseVector, DeltaTime);
}

void USpaceshipGenMovementComponent::AddStrafeAcceleration(const float InAcceleration, const float DeltaTime)
{
	if (!GetOwner())
	{
		return;
	}

	const FVector RightVector = GetOwner()->GetActorRightVector();

	AddDirectionAcceleration(InAcceleration, StrafeAcceleration, RightVector, DeltaTime);
}

void USpaceshipGenMovementComponent::AddVerticalAcceleration(const float InAcceleration, const float DeltaTime)
{
	if (!GetOwner())
	{
		return;
	}

	const FVector UpVector = GetOwner()->GetActorUpVector();

	AddDirectionAcceleration(InAcceleration, VerticalAcceleration, UpVector, DeltaTime);
}

void USpaceshipGenMovementComponent::HandleAcceleration(const float DeltaTime)
{
	if (ForwardInput > 0)
	{
		AddForwardAcceleration(ForwardAcceleration, DeltaTime);
	}
	else if (ForwardInput < 0)
	{
		AddReverseAcceleration(ReverseAcceleration, DeltaTime);
	}

	if (StrafeInput != 0)
	{
		AddStrafeAcceleration(StrafeInput * StrafeAcceleration, DeltaTime);
	}

	if (VerticalInput != 0)
	{
		AddVerticalAcceleration(VerticalInput * VerticalAcceleration, DeltaTime);
	}
}

void USpaceshipGenMovementComponent::HandleDecelerationFlags()
{
	if (!GetOwner())
	{
		return;
	}
	
	if (!UseAccelerationCurves)
	{
		return;
	}

	const FVector ForwardVector = GetOwner()->GetActorForwardVector();

	const FVector CurrentVelocity = GetVelocity();

	const FVector ForwardVel = CurrentVelocity.ProjectOnToNormal(ForwardVector);

	if (ForwardInput == 0 && CurrentVelocity.Size() > 0)
	{
		const bool IsBackwards = UKismetMathLibrary::Dot_VectorVector(ForwardVector, ForwardVel) < 0;

		if (IsBackwards)
		{
			IsForwardAcceleration = true;
		}
		else
		{
			IsReverseAcceleration = true;
		}
	}
}

void USpaceshipGenMovementComponent::HandleDeceleration(const float DeltaTime)
{
	// GMC_CLOG(!IsReplaying(), Error, TEXT("START: T: %f - Velocity = %f, %f, %f"), GetMoveTimestamp(), GetVelocity().X, GetVelocity().Y, GetVelocity().Z);
	
	if (!GetOwner())
	{
		return;
	}

	if (!DecelerateWhenNoInputOnAxis)
	{
		return;
	}

	const FVector ForwardVector = GetOwner()->GetActorForwardVector();
	const FVector RightVector = GetOwner()->GetActorRightVector();
	const FVector UpVector = GetOwner()->GetActorUpVector();

	const FVector CurrentVelocity = GetVelocity();

	FVector ForwardVel = CurrentVelocity.ProjectOnToNormal(ForwardVector);
	FVector StrafeVel = CurrentVelocity.ProjectOnToNormal(RightVector);
	FVector VerticalVel = CurrentVelocity.ProjectOnToNormal(UpVector);

	if (ForwardInput == 0)
	{
		const bool IsBackwards = UKismetMathLibrary::Dot_VectorVector(ForwardVector, ForwardVel) < 0;

		if (IsBackwards)
		{
			if (ForwardVel.Size() <= ForwardAcceleration * DeltaTime)
			{
				ForwardVel = FMath::VInterpConstantTo(ForwardVel, FVector::ZeroVector, DeltaTime, ForwardAcceleration);
				// ForwardVel = FVector::ZeroVector;
			}
			else
			{
				ForwardVel = ForwardVel + ForwardVector * ForwardAcceleration * DeltaTime;
			}
		}
		else
		{
			if (ForwardVel.Size() <= ReverseAcceleration * DeltaTime)
			{
				ForwardVel = FMath::VInterpConstantTo(ForwardVel, FVector::ZeroVector, DeltaTime, ReverseAcceleration);
				// ForwardVel = FVector::ZeroVector;
			}
			else
			{
				ForwardVel = ForwardVel + ForwardVector * ReverseAcceleration * DeltaTime * -1;
			}
		}
	}

	if (StrafeInput == 0)
	{
		const bool IsNegative = UKismetMathLibrary::Dot_VectorVector(RightVector, StrafeVel) < 0;

		if (StrafeVel.Size() <= StrafeAcceleration * DeltaTime)
		{
			StrafeVel = FMath::VInterpConstantTo(StrafeVel, FVector::ZeroVector, DeltaTime, StrafeAcceleration);
			// StrafeVel = FVector::ZeroVector;
		}
		else
		{
			StrafeVel = StrafeVel + RightVector * StrafeAcceleration * (IsNegative ? 1 : -1) * DeltaTime;
		}
	}

	if (VerticalInput == 0)
	{
		const bool IsNegative = UKismetMathLibrary::Dot_VectorVector(UpVector, VerticalVel) < 0;

		if (VerticalVel.Size() <= VerticalAcceleration)
		{
			VerticalVel = FMath::VInterpConstantTo(VerticalVel, FVector::ZeroVector, DeltaTime, VerticalAcceleration);
			// VerticalVel = FVector::ZeroVector;
		}
		else
		{
			VerticalVel = VerticalVel + UpVector * VerticalAcceleration * (IsNegative ? 1 : -1) * DeltaTime;
		}
	}

	UpdateVelocity(ForwardVel + StrafeVel + VerticalVel);

	// GMC_CLOG(!IsReplaying(), Error, TEXT("END  : T: %f - Velocity = %f, %f, %f"), GetMoveTimestamp(), GetVelocity().X, GetVelocity().Y, GetVelocity().Z);
}

float USpaceshipGenMovementComponent::AddToMaxVelocityAlpha(const float Value)
{
	MaxVelocityAlpha = RoundFloatTwoDecimals(FMath::Clamp(MaxVelocityAlpha + Value, 0.0f, 1.0f));

	return MaxVelocityAlpha;
}

float USpaceshipGenMovementComponent::GetMaxVelocityWithAlpha() const
{
	return RoundFloatTwoDecimals(MaxVelocity * MaxVelocityAlpha);
}

void USpaceshipGenMovementComponent::HandleRollDecay(const float DeltaTime)
{
	if (RollPowerDecays == false || RollInput != 0)
	{
		return;
	}

	if (!UseRollDecayCurve)
	{
		CurrentRoll = FMath::FInterpConstantTo(CurrentRoll, 0, DeltaTime, RollSpeed);
	}
	else
	{
		// TODO
	}

	HandleRollControlLimits();
}

void USpaceshipGenMovementComponent::HandleYawPitchDecay(const float DeltaTime)
{
	if (YawPitchPowerDecays == false || YawInput != 0 || PitchInput != 0)
	{
		return;
	}

	if (!YawPitchDecayCurve)
	{
		CurrentYaw = FMath::FInterpConstantTo(CurrentYaw, 0, DeltaTime, YawSpeed);
		DesiredYaw = FMath::FInterpConstantTo(DesiredYaw, 0, DeltaTime, YawInputSpeed);
		CurrentPitch = FMath::FInterpConstantTo(CurrentPitch, 0, DeltaTime, PitchSpeed);
		DesiredPitch = FMath::FInterpConstantTo(DesiredPitch, 0, DeltaTime, PitchInputSpeed);
	}
	else
	{
		// TODO
	}

	HandleYawPitchControlLimits();
}

FVector USpaceshipGenMovementComponent::ClampYawPitch(const FVector YawPitchVector) const
{
	return YawPitchVector.GetClampedToSize(-YawPitchControlLimitRadius, YawPitchControlLimitRadius);
}

void USpaceshipGenMovementComponent::HandleYawPitchControlLimits()
{
	const FVector YawPitchVector = FVector(
		CurrentYaw,
		CurrentPitch,
		0
	);
	const FVector DesiredYawPitchVector = FVector(
		DesiredYaw,
		DesiredPitch,
		0
	);

	const FVector YawPitchLimited = ClampYawPitch(YawPitchVector);
	const FVector DesiredYawPitchLimited = ClampYawPitch(DesiredYawPitchVector);

	CurrentPitch = YawPitchLimited.Y;
	CurrentYaw = YawPitchLimited.X;
	DesiredPitch = DesiredYawPitchLimited.Y;
	DesiredYaw = DesiredYawPitchLimited.X;

	const FVector YawPitchPower = YawPitchLimited / YawPitchControlLimitRadius;

	YawControlPower = YawPitchPower.X;
	PitchControlPower = YawPitchPower.Y;
}

void USpaceshipGenMovementComponent::HandleRollControlLimits()
{
	const float ClampedRoll = FMath::Clamp(CurrentRoll, -RollControlLimit, RollControlLimit);

	CurrentRoll = ClampedRoll;

	RollControlPower = ClampedRoll / RollControlLimit;
}

float USpaceshipGenMovementComponent::GetYawWithSpeed(const float Speed, const float DeltaTime) const
{
	return YawInput * Speed * DeltaTime;
}

float USpaceshipGenMovementComponent::GetPitchWithSpeed(const float Speed, const float DeltaTime) const
{
	return PitchInput * Speed * DeltaTime;
}

float USpaceshipGenMovementComponent::GetRollWithSpeed(const float Speed, const float DeltaTime) const
{
	return RollInput * Speed * DeltaTime;
}

float USpaceshipGenMovementComponent::HandleYaw(const float DeltaTime)
{
	DesiredYaw = GetYawWithSpeed(YawInputSpeed, DeltaTime) + DesiredYaw;

	return DesiredYaw;
}

float USpaceshipGenMovementComponent::HandlePitch(const float DeltaTime)
{
	DesiredPitch = GetPitchWithSpeed(PitchInputSpeed, DeltaTime) + DesiredPitch;

	return DesiredPitch;
}

float USpaceshipGenMovementComponent::HandleRoll(const float DeltaTime)
{
	CurrentRoll = GetRollWithSpeed(RollSpeed, DeltaTime) + CurrentRoll;
	DesiredRoll = GetRollWithSpeed(RollInputSpeed, DeltaTime) + DesiredRoll;

	return DesiredRoll;
}

void USpaceshipGenMovementComponent::SetMovementInput(const FVector Input)
{
	SetForwardInput(Input.X);
	SetStrafeInput(Input.Y);
	SetVerticalInput(Input.Z);
}

float USpaceshipGenMovementComponent::RoundFloatTwoDecimals(const float Value)
{
	return FMath::RoundToFloat(Value * 100.f) / 100.f;
}

float USpaceshipGenMovementComponent::RoundFloatOneDecimal(const float Value)
{
	return FMath::RoundToFloat(Value * 10.f) / 10.f;
}

int USpaceshipGenMovementComponent::RoundToInt(const float Value)
{
	return FMath::RoundToInt(Value);
}

void USpaceshipGenMovementComponent::SetForwardInput(const float Input)
{
	IsForwardAcceleration = false;
	IsReverseAcceleration = false;

	const int IntInput = FMath::RoundToInt(Input);
	
	if (IntInput > 0)
	{
		IsForwardAcceleration = true;
	}
	else if (IntInput < 0)
	{
		IsReverseAcceleration = true;
	}
		
	ForwardInput = RoundToInt(Input);
}

void USpaceshipGenMovementComponent::SetStrafeInput(const float Input)
{
	IsStrafeAcceleration = false;

	const int IntInput = FMath::RoundToInt(Input);
	
	if (IntInput != 0)
	{
		IsStrafeAcceleration = true;
	}
	
	StrafeInput = RoundToInt(Input);
}

void USpaceshipGenMovementComponent::SetVerticalInput(const float Input)
{
	IsVerticalAcceleration = false;

	const int IntInput = FMath::RoundToInt(Input);
	
	if (IntInput != 0)
	{
		IsVerticalAcceleration = true;
	}
	
	VerticalInput = RoundToInt(Input);
}

void USpaceshipGenMovementComponent::SetPitchInput(const float Input, const float DeltaTime)
{
	PitchInput = Input;
	// PitchInput = RoundFloatTwoDecimals(Input);
	// PitchInput = RoundFloatOneDecimal(Input);

	HandlePitch(DeltaTime);
	HandleYawPitchControlLimits();
}

void USpaceshipGenMovementComponent::SetYawInput(const float Input, const float DeltaTime)
{
	YawInput = Input;
	// YawInput = RoundFloatTwoDecimals(Input);
	// YawInput = RoundFloatOneDecimal(Input);

	HandleYaw(DeltaTime);
	HandleYawPitchControlLimits();
}

void USpaceshipGenMovementComponent::SetRollInput(const float Input, const float DeltaTime)
{
	RollInput = Input;
	// RollInput = RoundFloatTwoDecimals(Input);
	// RollInput = RoundFloatOneDecimal(Input);

	HandleRoll(DeltaTime);
	HandleRollControlLimits();
}

void USpaceshipGenMovementComponent::ApplyAngularFilters(const float DeltaTime)
{
	if (!GetOwner())
	{
		return;
	}

	const auto Pitch = PitchControlPower * MaxPitchAcceleration * DeltaTime;
	const auto Yaw = YawControlPower * MaxYawAcceleration * DeltaTime;
	const auto Roll = RollControlPower * MaxRollAcceleration * DeltaTime;

	const auto QuaternionRotation = URotationHelpers::Euler_To_Quaternion(FRotator(Pitch, Yaw, Roll));
	
	GetOwner()->AddActorLocalRotation(QuaternionRotation);

	// GMC_CLOG(!IsReplaying(), Error, TEXT("END  : T: %f - Rotation = %s"), GetMoveTimestamp(), *GetRootCollisionRotation().ToString());
}
