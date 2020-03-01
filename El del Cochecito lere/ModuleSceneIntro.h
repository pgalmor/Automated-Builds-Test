#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"
#include "Bullet/include/btBulletDynamicsCommon.h"
#include "Timer.h"

#include "PugiXml\src\pugixml.hpp"

#define MAX_SNAKE 2
#define MAX_PILES 1000
#define MAX_SPECTATORS 135

struct PhysBody3D;
struct PhysMotor3D;
struct Timer;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Awake();
	bool Start();
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);
	void SpectAnimation();
	void SpectColorChange(int scuderia);
	void Win();

public:

	PhysBody3D* pb_chassis;
	Cube p_chassis;

	PhysBody3D* pb_wheel;
	Cylinder p_wheel;

	PhysBody3D* pb_wheel2;
	Cylinder p_wheel2;

	PhysMotor3D* left_wheel;
	PhysMotor3D* right_wheel;

	btHingeConstraint* hinge;

	Cube base;
	Cube bar;
	Cube* stands1;
	Cube* stands2;
	Cube* stands3;
	PhysBody3D* pBase;
	PhysBody3D* pBar;

	Cube* mapPiles[MAX_PILES];
	Cube* spectators[MAX_SPECTATORS];
	float spectYpos;
	float spectYmov[MAX_SPECTATORS];
	bool spectUp[MAX_SPECTATORS];

	//Laps
	Cube checkpoint;
	vec3 checkpointPos;
	int lap;
	bool lap_ = false;

	//Timers
	int currentTime;
	Timer LapTimer;

	int coinSound;
	int winSound;

private:
	pugi::xml_document map_file;
	pugi::xml_node map_node;

	pugi::xml_node LoadMap(pugi::xml_document&) const;

	Plane* p;
};
