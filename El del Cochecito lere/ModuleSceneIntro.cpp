#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "ModulePlayer.h"
#include "Color.h"
#include "p2DynArray.h"
#include "PhysVehicle3d.h"
#include <math.h>

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	for (int i = 0; i < MAX_PILES; i++) {
		mapPiles[i] = nullptr;
	}
	checkpointPos = vec3(24, -30, 26);
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

bool ModuleSceneIntro::Awake()
{

	bool ret = false;
	map_node = LoadMap(map_file);
	if (map_file.empty() == false)
	{
		ret = true;
	}

	pugi::xml_node pile;
	int pilesAdded = 0;
	int colorIterator = 0;

	for (pile = map_node.child("pile"); pile && ret; pile = pile.next_sibling("pile"))
	{
		//LOG("%i %i %i", pile.attribute("x").as_int(), pile.attribute("y").as_int(), pile.attribute("z").as_int());
		
		Cube* c = new Cube(1.2f, 1.7f, 1.2f);
		float Yoffset = c->size.y / 2;
		if (colorIterator % 2 == 0) { c->color = Red; }
		else { c->color = White; }
		c->SetPos(pile.attribute("x").as_int(), pile.attribute("y").as_int() + Yoffset, pile.attribute("z").as_int());
		mapPiles[pilesAdded] = c;

		pilesAdded++;
		colorIterator++;
	}

	coinSound = App->audio->LoadFx("Audio/Coin.wav");
	winSound = App->audio->LoadFx("Audio/Win.wav");
	App->audio->PlayMusic("Audio/Aficion.ogg");

	return ret;
}


// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	p = new Plane(0, 1, 0, 0);
	p->axis = true;

	base = Cube(0.7f, 1, 0.7f);
	float Yoffset = base.size.y / 2;
	base.SetPos(19.5f, Yoffset, -45);
	base.color = Grey;

	bar = Cube(15, 0.7f, 0.7f);
	bar.SetPos(11.5f, 1.0f, -45);
	bar.color = Red;

	stands1 = new Cube(3, 3, 90);
	Yoffset = stands1->size.y / 2;
	stands1->SetPos(-100, Yoffset, 10);
	stands1->color = Red;

	stands2 = new Cube(3, 3, 90);
	stands2->SetPos(-105, Yoffset + 3, 10);
	stands2->color = Red;

	stands3 = new Cube(3, 3, 90);
	stands3->SetPos(-110, Yoffset + 6, 10);
	stands3->color = Red;

	int cYoffset;
	int rColor;
	int spectCount = 0;
	int spectAdded = 0;
	for (int i = 0; i < MAX_SPECTATORS; i++)
	{
		Cube* c = new Cube(1.2f, 1.7f, 1.2f);
		cYoffset = c->size.y / 2;
		spectYpos = cYoffset + Yoffset;
		
		rColor = (int)(std::rand() % 6);
		switch (rColor)
		{
			case 0:
				c->color = Blue;
				break;
			case 1:
				c->color = Black;
				break;
			case 2:
				c->color = White;
				break;
			case 3:
				c->color = Yellow;
				break;
			case 4:
				c->color = LightBlue;
				break;
			case 5:
				c->color = Grey;
				break;
			case 6:
				c->color = NavyBlue;
				break;
		}
		spectYmov[i] = (float)(std::rand() % 2);
		if (spectCount < 45) {
			c->SetPos(-100, cYoffset + Yoffset + 2 + spectYmov[i], 54 - spectCount * 2);
		}
		else if (spectCount >= 45 && spectCount < 90) {
			c->SetPos(-105, cYoffset + Yoffset + 5 + spectYmov[i], 54 - (spectCount - 45) * 2);
		}
		else if (spectCount >= 90) {
			c->SetPos(-110, cYoffset + Yoffset + 8 + spectYmov[i], 54 - (spectCount - 90) * 2);
		}
		spectUp[i] = false;
		spectators[spectAdded] = c;
		spectAdded++;
		spectCount++;
	}

	for (int i = 0; i < MAX_PILES; i++)
	{
		if (mapPiles[i] != nullptr)
		{
			Cube cnp = *mapPiles[i];
			App->physics->AddBody(cnp, 0.0f);
		}
	}
	
	pBase = App->physics->AddBody(base, 0.f);
	pBar = App->physics->AddBody(bar, 500);
	const vec3 a(0, 0.7f, 0);
	const vec3 b(8, 0, 0);
	const vec3 c(0, 0, 1);
	hinge = App->physics->AddConstraintHinge(*pBase, *pBar, a, b, c, c, false);
	hinge->setLimit(0, 89);
	hinge->enableAngularMotor(true, 0, INFINITE);

	//WIP
	checkpoint = Cube(1, 3, 1);
	checkpoint.color = Blue;
	checkpoint.SetPos(25, 1.5, -29);
	lap = 0;
	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	for (int i = 0; i < MAX_PILES; i++) {
		if (mapPiles[i] != nullptr) {
			delete mapPiles[i];
			mapPiles[i] = nullptr;
		}
	}
	for (int i = 0; i < MAX_SPECTATORS; i++) {
		if (spectators[i] != nullptr) {
			delete spectators[i];
			spectators[i] = nullptr;
		}
	}
	delete stands1;
	delete stands2;
	delete stands3;
	delete p;

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	SpectAnimation();

	pBar->GetTransform(bar.transform.M);

	char title[80];
	int MaxTime = 40 - lap*4;
	currentTime = LapTimer.Read() / 1000;
	if (App->player->vehicle->Scuderia == Renault) { sprintf_s(title, "%.1f Km/h  ||  Gear: %i  ||  Laps: %i  || Time Left: %i || Scuderia: Reanult", 
	App->player->vehicle->GetKmh(), App->player->gear, lap, MaxTime - currentTime); }
	if (App->player->vehicle->Scuderia == Ferrari) { sprintf_s(title, "%.1f Km/h  ||  Gear: %i  ||  Laps: %i  || Time Left: %i || Scuderia: Ferrari", 
	App->player->vehicle->GetKmh(), App->player->gear, lap, MaxTime - currentTime); }
	if (App->player->vehicle->Scuderia == McLaren) {sprintf_s(title, "%.1f Km/h  ||  Gear: %i  ||  Laps: %i  || Time Left: %i || Scuderia: McLaren",
	App->player->vehicle->GetKmh(), App->player->gear, lap, MaxTime - currentTime);}
	if (App->player->vehicle->Scuderia == RedBull) {sprintf_s(title, "%.1f Km/h  ||  Gear: %i  ||  Laps: %i  || Time Left: %i || Scuderia: Red Bull",
	App->player->vehicle->GetKmh(), App->player->gear, lap, MaxTime - currentTime);}
	
	App->window->SetTitle(title);

	if (currentTime > MaxTime) { App->player->Restart(); }

	if (App->player->VehiclePos.z < checkpointPos.y) {
		if (App->player->VehiclePos.x > checkpointPos.x && App->player->VehiclePos.x < checkpointPos.z) {
			if (!lap_) {
				lap++;
				App->audio->PlayFx(coinSound);
				lap_ = true;
				LapTimer.Start();
				if (lap == 8) {
					Win();
				}
			}
		}
	}
	else {
		lap_ = false;
	}

	return UPDATE_CONTINUE;
}

update_status ModuleSceneIntro::PostUpdate(float dt)
{
	for (int i = 0; i < MAX_PILES; i++) {
		if (mapPiles[i] != nullptr) {
			mapPiles[i]->Render();
		}
	}
	for (int i = 0; i < MAX_SPECTATORS; i++) {
		if (spectators[i] != nullptr) {
			spectators[i]->Render();
		}
	}
	base.Render();
	bar.Render();
	stands1->Render();
	stands2->Render();
	stands3->Render();
	p->Render();
	checkpoint.Render();

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
	LOG("COLLISION");
	//if (body2 == App->player->vehicleBody && body1 == checkpointBody)lap++;
}

// ---------------------------------------------
pugi::xml_node ModuleSceneIntro::LoadMap(pugi::xml_document& map_file) const
{
	pugi::xml_node ret;

	pugi::xml_parse_result result = map_file.load_file("map.xml");

	if (result == NULL) { LOG("Could not load map xml file map.xml. pugi error: %s", result.description()); }
	else ret = map_file.child("map");

	return ret;
}

void ModuleSceneIntro::SpectAnimation()
{
	for (int i = 0; i < MAX_SPECTATORS; i++)
	{
		if (spectUp[i] == true) { spectYmov[i] += 0.15; }
		else if (spectUp[i] == false) { spectYmov[i] -= 0.15; }

		if (i < 45) {
			spectators[i]->SetPos(-100, spectYpos + 2 + spectYmov[i], 54 - i * 2);
		}
		else if (i >= 45 && i < 90) {
			spectators[i]->SetPos(-105, spectYpos + 5 + spectYmov[i], 54 - (i - 45) * 2);
		}
		else if (i >= 90) {
			spectators[i]->SetPos(-110, spectYpos + 8 + spectYmov[i], 54 - (i - 90) * 2);
		}

		if (spectYmov[i] >= 2) { spectUp[i] = false; }
		if (spectYmov[i] <= 0) { spectUp[i] = true; }
	}
}

void ModuleSceneIntro::SpectColorChange(int scuderia) 
{
	int rColor;
	switch (scuderia)
	{
		case 0:
			for (int i = 0; i < MAX_SPECTATORS; i++)
			{
				rColor = (int)(std::rand() % 6);
				switch (rColor)
				{
				case 0:
					spectators[i]->color = Blue;
					break;
				case 1:
					spectators[i]->color = Black;
					break;
				case 2:
					spectators[i]->color = White;
					break;
				case 3:
					spectators[i]->color = Yellow;
					break;
				case 4:
					spectators[i]->color = LightBlue;
					break;
				case 5:
					spectators[i]->color = Grey;
					break;
				case 6:
					spectators[i]->color = NavyBlue;
					break;
				}
			}
			break;
		case 1:
			for (int i = 0; i < MAX_SPECTATORS; i++)
			{
				rColor = (int)(std::rand() % 6);
				switch (rColor)
				{
				case 0:
					spectators[i]->color = Red;
					break;
				case 1:
					spectators[i]->color = Red2;
					break;
				case 2:
					spectators[i]->color = White;
					break;
				case 3:
					spectators[i]->color = Yellow;
					break;
				case 4:
					spectators[i]->color = Red3;
					break;
				case 5:
					spectators[i]->color = Grey;
					break;
				case 6:
					spectators[i]->color = Yellow2;
					break;
				}
			}
			break;
		case 2:
			for (int i = 0; i < MAX_SPECTATORS; i++)
			{
				rColor = (int)(std::rand() % 6);
				switch (rColor)
				{
				case 0:
					spectators[i]->color = Blue;
					break;
				case 1:
					spectators[i]->color = Orange2;
					break;
				case 2:
					spectators[i]->color = White;
					break;
				case 3:
					spectators[i]->color = Orange;
					break;
				case 4:
					spectators[i]->color = LightBlue;
					break;
				case 5:
					spectators[i]->color = Grey;
					break;
				case 6:
					spectators[i]->color = NavyBlue;
					break;
				}
			}
			break;
		case 3:
			for (int i = 0; i < MAX_SPECTATORS; i++)
			{
				rColor = (int)(std::rand() % 6);
				switch (rColor)
				{
				case 0:
					spectators[i]->color = Blue;
					break;
				case 1:
					spectators[i]->color = Yellow2;
					break;
				case 2:
					spectators[i]->color = NavyBlue;
					break;
				case 3:
					spectators[i]->color = Yellow;
					break;
				case 4:
					spectators[i]->color = Red3;
					break;
				case 5:
					spectators[i]->color = Grey;
					break;
				case 6:
					spectators[i]->color = NavyBlue;
					break;
				}
			}
			break;
	}
}

void ModuleSceneIntro::Win() {
	App->audio->PlayFx(winSound);
}