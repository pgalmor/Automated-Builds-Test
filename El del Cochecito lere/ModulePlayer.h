#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"
#include "PhysVehicle3D.h"

struct PhysVehicle3D;
struct VehicleInfo;

#define MAX_ACCELERATION 200.0f
#define TURN_DEGREES 20.0f * DEGTORAD
#define BRAKE_POWER 100.0f

enum cameraTypes {
	THIRD_PERSON,
	FIRST_PERSON,
	UP_VIEW,
	LOOK_BEHIND
};
class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void Restart();

	//-----------Player_Camera-------------
	void CameraPointOfView();
	void CameraFollowingPlayer();

public:

	// Player spawn point --------------------------
	vec3 spawnPoint = { 20, 3, -70 };

	VehicleInfo car;

	bool DynamicCamera; 
	vec3 CameraDistance;
	vec3 VehiclePos;
	PhysVehicle3D* vehicle;
	PhysBody3D* vehicleBody;
	Cube carSensor;
	float turn;
	float acceleration;
	float brake;
	int gear;
	bool barUp;
	bool lap;

private:
	cameraTypes view;
};