#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled), vehicle(NULL)
{
	turn = acceleration = brake = 0.0f;
	DynamicCamera = true;
	carSensor = Cube(1, 1, 1);
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	gear = 1;
	barUp = false;
	lap = false;

	// Camera initial point of view --------------------------
	view = THIRD_PERSON;

	// Car properties ----------------------------------------
	car.chassis_size.Set(2.5, 0.9f, 4);
	car.chassis_offset.Set(0, 1.5, -1.5);
	car.glass_size.Set(0.75, 0.7f, 3.4);
	car.glass_offset.Set(0, 1.95, -1.75);
	car.prow_size.Set(1, 0.7f, 3);
	car.prow_offset.Set(0, 1.5, 2);
	car.aileron_size.Set(2.5, 0.5f, 0.75);
	car.aileron_offset.Set(0, 2.25, -3.8);
	car.light_size.Set(0.2, 0.2f, 0.2);
	car.light_offset.Set(0, 1.4, -3.5);
	car.mass = 500.0f;
	car.suspensionStiffness = 15.0f;
	car.suspensionCompression = 0.83f;
	car.suspensionDamping = 0.9f;
	car.maxSuspensionTravelCm = 800.0f;
	car.frictionSlip = 20.5;
	car.maxSuspensionForce = 6000.0f;

	car.car_size = car.chassis_size + car.prow_size;

	// Wheel properties ---------------------------------------
	float connection_height = 1.8f;
	float wheel_radius = 0.6f;
	float wheel_width = 0.6f;
	float suspensionRestLength = 0.5f;

	// Don't change anything below this line ------------------

	float half_width = car.car_size.x*0.5f;
	float half_length = car.car_size.z*0.5f;
	
	vec3 direction(0,-1,0);
	vec3 axis(-1,0,0);
	
	car.num_wheels = 4;
	car.wheels = new Wheel[4];

	// FRONT-LEFT ------------------------
	car.wheels[0].connection.Set(half_width - 0.25 - 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[0].direction = direction;
	car.wheels[0].axis = axis;
	car.wheels[0].suspensionRestLength = suspensionRestLength;
	car.wheels[0].radius = wheel_radius;
	car.wheels[0].width = wheel_width;
	car.wheels[0].front = true;
	car.wheels[0].drive = true;
	car.wheels[0].brake = false;
	car.wheels[0].steering = true;

	// FRONT-RIGHT ------------------------
	car.wheels[1].connection.Set(-half_width + 0.25 + 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[1].direction = direction;
	car.wheels[1].axis = axis;
	car.wheels[1].suspensionRestLength = suspensionRestLength;
	car.wheels[1].radius = wheel_radius;
	car.wheels[1].width = wheel_width;
	car.wheels[1].front = true;
	car.wheels[1].drive = true;
	car.wheels[1].brake = false;
	car.wheels[1].steering = true;

	// REAR-LEFT ------------------------
	car.wheels[2].connection.Set(half_width - 0.2 - 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[2].direction = direction;
	car.wheels[2].axis = axis;
	car.wheels[2].suspensionRestLength = suspensionRestLength;
	car.wheels[2].radius = wheel_radius;
	car.wheels[2].width = wheel_width;
	car.wheels[2].front = false;
	car.wheels[2].drive = true;
	car.wheels[2].brake = true;
	car.wheels[2].steering = false;

	// REAR-RIGHT ------------------------
	car.wheels[3].connection.Set(-half_width + 0.2 + 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[3].direction = direction;
	car.wheels[3].axis = axis;
	car.wheels[3].suspensionRestLength = suspensionRestLength;
	car.wheels[3].radius = wheel_radius;
	car.wheels[3].width = wheel_width;
	car.wheels[3].front = false;
	car.wheels[3].drive = true;
	car.wheels[3].brake = true;
	car.wheels[3].steering = false;

	vehicle = App->physics->AddVehicle(car);
	vehicle->SetPos(spawnPoint.x, spawnPoint.y, spawnPoint.z);
	vehicle->info.revolutions = false;
	
	VehiclePos = vehicle->GetPos();
	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update(float dt)
{
	turn = acceleration = brake = 0.0f;
	//carSensor.SetPos(VehiclePos.x, VehiclePos.y + 2, VehiclePos.z);
	//vehicleBody = App->physics->AddBody(carSensor, this, 0.0f, true);
	//carSensor.Render();

	//VEHICLE MOVEMENT
	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
	{
		if (vehicle->GetKmh() > 300) //Limit acceleration when velocity is bigger than 300 km/h
			acceleration = MAX_ACCELERATION / (vehicle->GetKmh() / 200);
		else {
			if (acceleration < 10) acceleration = 1500;
			else if (acceleration < MAX_ACCELERATION) acceleration = MAX_ACCELERATION * (vehicle->GetKmh() / 5); //Progresive acceleration for reality sensation
			else acceleration = MAX_ACCELERATION;
		}
	}
	else if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		if (vehicle->GetKmh() > 0)
			brake = BRAKE_POWER;
		else acceleration = -MAX_ACCELERATION;
	}
	else brake = 10.0f;

	if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		if (turn < TURN_DEGREES) {
			if (vehicle->GetKmh() < 60) {
				turn += TURN_DEGREES;
			}
			else turn += TURN_DEGREES / (vehicle->GetKmh() / 80);
		}
	}

	if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		if (turn > -TURN_DEGREES) {
			if (vehicle->GetKmh() < 60) {
				turn -= TURN_DEGREES;
			}
			else turn -= TURN_DEGREES / (vehicle->GetKmh() / 80);

		}
	}

	//BAR INPUT
	if (barUp == true)
	{
		App->scene_intro->hinge->enableAngularMotor(false, 0, INFINITE);
		App->scene_intro->hinge->enableAngularMotor(true, -1, INFINITE);
		App->scene_intro->bar.color = Red;
		barUp = false;
	}
	if (VehiclePos.z > -60 && VehiclePos.z < -40 && barUp == false) {
		App->scene_intro->hinge->enableAngularMotor(true, 1, INFINITE);
		App->scene_intro->bar.color = Green;
		barUp = true;
	}

	//VEHICLE GEARS
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && gear <= 7) { gear += 1; }

	//RESTART
	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN) { Restart(); }
	if (App->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN) { App->scene_intro->lap = 8; App->scene_intro->Win(); }

	//CHANGE CAR
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		if (vehicle->Scuderia == Renault) { vehicle->Scuderia = Ferrari; App->scene_intro->SpectColorChange(1); }
		else if (vehicle->Scuderia == Ferrari) { vehicle->Scuderia = McLaren; App->scene_intro->SpectColorChange(2); }
		else if (vehicle->Scuderia == McLaren) { vehicle->Scuderia = RedBull; App->scene_intro->SpectColorChange(3); }
		else if (vehicle->Scuderia == RedBull) { vehicle->Scuderia = Renault; App->scene_intro->SpectColorChange(0); }
	}
	
	switch (gear)
	{
		case 1:
			if (vehicle->GetKmh() > 30) { acceleration = 0; vehicle->info.revolutions = true; }
			else { vehicle->info.revolutions = false; }
			break;
		case 2:
			if (vehicle->GetKmh() > 60) { acceleration = 0; vehicle->info.revolutions = true; }
			else { vehicle->info.revolutions = false; }
			if (vehicle->GetKmh() < 10) { gear -= 1; }
			break;
		case 3:
			if (vehicle->GetKmh() > 90) { acceleration = 0; vehicle->info.revolutions = true; }
			else { vehicle->info.revolutions = false; }
			if (vehicle->GetKmh() < 30) { gear -= 1; }
			break;
		case 4:
			if (vehicle->GetKmh() > 120) { acceleration = 0; vehicle->info.revolutions = true; }
			else { vehicle->info.revolutions = false; }
			if (vehicle->GetKmh() < 80) { gear -= 1; }
			break;
		case 5:
			if (vehicle->GetKmh() > 160) { acceleration = 0; vehicle->info.revolutions = true; }
			else { vehicle->info.revolutions = false; }
			if (vehicle->GetKmh() < 110) { gear -= 1; }
			break;
		case 6:
			if (vehicle->GetKmh() > 200) { acceleration = 0; vehicle->info.revolutions = true; }
			else { vehicle->info.revolutions = false; }
			if (vehicle->GetKmh() < 150) { gear -= 1; }
			break;
		case 7:
			if (vehicle->GetKmh() > 250) { acceleration = 0; vehicle->info.revolutions = true; }
			else { vehicle->info.revolutions = false; }
			if (vehicle->GetKmh() < 190) { gear -= 1; }
			break;
		case 8:
			vehicle->info.revolutions = false;
			if (vehicle->GetKmh() < 240) { gear -= 1; }
			break;
	}

	VehiclePos = vehicle->GetPos();
	vehicle->ApplyEngineForce(acceleration);
	vehicle->Turn(turn);
	vehicle->Brake(brake);

	vehicle->Render();

	if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
	{
		DynamicCamera = !DynamicCamera;
	}

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		DynamicCamera = true;
		if (view == THIRD_PERSON) { view = FIRST_PERSON; }
		else if (view == FIRST_PERSON) { view = UP_VIEW; }
		else if (view == UP_VIEW) { view = LOOK_BEHIND; }
		else if (view == LOOK_BEHIND) { view = THIRD_PERSON; }
	}

	if (DynamicCamera)
	{
		//the camera follows the car
		CameraFollowingPlayer();
	}


	return UPDATE_CONTINUE;
}

void  ModulePlayer::CameraPointOfView()
{
	switch (view)
	{
		case THIRD_PERSON: CameraDistance = { -12.0f, 5.0f, -10.0f }; break;
		case FIRST_PERSON: CameraDistance = { -6.f, 3.f, -5.f }; break;
		case UP_VIEW: CameraDistance = { -18.0f, 10.0f, -14.0f }; break;
		case LOOK_BEHIND: CameraDistance = { 12.0f, 6.0f, 10.0f }; break;
	}
}

void  ModulePlayer::CameraFollowingPlayer()
{
	CameraPointOfView();

	vec3 forwardVector = vehicle->GetForward(); //the vector that looks forward respect the car position
	vec3 NewCameraPosition = { VehiclePos.x + (forwardVector.x * CameraDistance.x), VehiclePos.y + (forwardVector.y + CameraDistance.y), VehiclePos.z + (forwardVector.z * CameraDistance.z) };
	vec3 CamPos = App->camera->Position + (NewCameraPosition - App->camera->Position);

	App->camera->Look(CamPos, VehiclePos);
}

void ModulePlayer::Restart()
{
	vehicle->SetPos(spawnPoint.x, spawnPoint.y, spawnPoint.z);

	vehicle->vehicle->getRigidBody()->setAngularVelocity({ 0, 0, 0 });
	vehicle->vehicle->getRigidBody()->setLinearVelocity({ 0, 0, 0 });

	btQuaternion SpawnOrientation = { 0, 0, 0, 1 };
	vehicle->SetRotation(SpawnOrientation);

	App->scene_intro->lap = 0;
	App->scene_intro->LapTimer.Start();
}