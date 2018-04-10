#include "scene_menu.h"
#include "system_player_controls.h"

#include "../general/equipment.h"

#include "../components/cmp_text.h"
#include "../components/cmp_sprite.h"
#include "../components/cmp_tilemap.h"
#include "../components/cmp_actor_stats.h"

#include "../game.h"
#include <SFML/Window/Keyboard.hpp>
#include <iostream>

using namespace std;
using namespace sf;
using namespace controls;
using namespace input;

View view;
float zoom;
double delay;

void MenuScene::Load() 
{
	/*counter++;
	cout << "Test: " << counter << "\n";*/
	zoom = 1.0f;
	view = View(FloatRect(0, 0, 1920, 1080));
	Engine::GetWindow().setView(view);
	cout << "Menu Load \n";
	{
		auto txt = makeEntity();
		auto t = txt->addComponent<TextComponent>(
			"Platformer\nPress Space to Start");
		
		auto tm = makeEntity();
		auto tmC = tm->addComponent<TileMapComponent>(txrMgr, counter);

		auto a = makeEntity();
		auto aC = a->addComponent<ActorStatsComponent>();
	}

	counter++;
	cout << "Test: " << counter << "\n";

	Useable g = Useable();
	g.GenerateItem();

	vector<string> data = g.GetDataForSave();

	for (int i = 0; i < data.size(); i++)
	{
		cout << data[i] << "\n";
	}

	g.CreateFromData(data);

	data = g.GetDataForSave();

	for (int i = 0; i < data.size(); i++)
	{
		cout << data[i] << "\n";
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	setLoaded(true);
}

void MenuScene::Update(const double& dt) {
  // cout << "Menu Update "<<dt<<"\n";

	txrMgr->UpdateAnims(dt);
	delay += dt;
	CheckCameraInput(view, zoom, dt);
	GetMouseClick();

	// Function not working
	//GetMousePos(Engine::GetWindow());

	// DEBUG
	/*if (input::GetKeyPressed(Keyboard::E, delay) == true) {
		cout << "pressed" << endl;
	}*/
	if (input::GetMousePressed(Mouse::Button::Left, delay) == true) {
		cout << "pressed" << endl;
	}

  if (sf::Keyboard::isKeyPressed(Keyboard::Space)) {
	  Engine::ChangeScene(&menu);
  }

  Scene::Update(dt);
}
