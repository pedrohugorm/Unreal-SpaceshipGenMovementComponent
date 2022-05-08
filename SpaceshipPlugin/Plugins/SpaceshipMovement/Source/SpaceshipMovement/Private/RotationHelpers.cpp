// Copyright Pedro Matos. All Rights Reserved


#include "RotationHelpers.h"

// Formula to convert a Euler angle in degrees to a quaternion rotation
FQuat URotationHelpers::Euler_To_Quaternion(const FRotator Current_Rotation)
{
	FQuat q; // Declare output quaternion
	const float Yaw = Current_Rotation.Yaw * PI / 180; // Convert degrees to radians 
	const float Roll = Current_Rotation.Roll * PI / 180;
	const float Pitch = Current_Rotation.Pitch * PI / 180;

	const double Cy = cos(Yaw * 0.5);
	const double Sy = sin(Yaw * 0.5);
	const double Cr = cos(Roll * 0.5);
	const double Sr = sin(Roll * 0.5);
	const double Cp = cos(Pitch * 0.5);
	const double Sp = sin(Pitch * 0.5);

	q.W = Cy * Cr * Cp + Sy * Sr * Sp;
	q.X = Cy * Sr * Cp - Sy * Cr * Sp;
	q.Y = Cy * Cr * Sp + Sy * Sr * Cp;
	q.Z = Sy * Cr * Cp - Cy * Sr * Sp;

	return q; // Return the quaternion of the input Euler rotation
}

// Set the scene component's world rotation to the input quaternion
void URotationHelpers::SetWorldRotationQuat(USceneComponent* SceneComponent, const FQuat& Desired_Rotation)
{
	if (SceneComponent)
	{
		SceneComponent->SetWorldRotation(Desired_Rotation);
	}
}

// Set the scene component's relative rotation to the input quaternion
void URotationHelpers::SetRelativeRotationQuat(USceneComponent* SceneComponent, const FQuat& Desired_Rotation)
{
	if (SceneComponent)
	{
		SceneComponent->SetRelativeRotation(Desired_Rotation);
	}
}

// Add the input delta rotation to the scene component's current local rotation
void URotationHelpers::AddLocalRotationQuat(USceneComponent* SceneComponent, const FQuat& Delta_Rotation)
{
	if (SceneComponent)
	{
		SceneComponent->AddLocalRotation(Delta_Rotation);
	}
}

// Set the Actor's world rotation to the input quaternion
void URotationHelpers::SetActorWorldRotationQuat(AActor* Actor, const FQuat& Desired_Rotation)
{
	if (Actor)
	{
		Actor->SetActorRotation(Desired_Rotation);
	}
}

// Set the Actor's relative rotation to the input quaternion
void URotationHelpers::SetActorRelativeRotationQuat(AActor* Actor, const FQuat& Desired_Rotation)
{
	if (Actor)
	{
		Actor->SetActorRelativeRotation(Desired_Rotation);
	}
}

// Add the input delta rotation to the Actor's current local rotation
void URotationHelpers::AddActorLocalRotationQuat(AActor* Actor, const FQuat& Delta_Rotation)
{
	if (Actor)
	{
		Actor->AddActorLocalRotation(Delta_Rotation);
	}
}
