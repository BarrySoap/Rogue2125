#include "engine.h"
#include "maths.h"
#include "system_physics.h"
#include "system_renderer.h"
#include "system_resources.h"
#include <SFML/Graphics.hpp>
#include <future>
#include <iostream>
#include <stdexcept>

using namespace sf;
using namespace std;
Scene* Engine::_activeScene = nullptr;
std::string Engine::_gameName;
SoundBuffer musicBuffer;
Sound music;
bool wait;

static bool loading = false;
static float loadingspinner = 0.f;
static float loadingTime;
static RenderWindow* _window;

void Loading_update(float dt, const Scene* const scn) {
	//  cout << "Eng: Loading Screen\n";
	if (scn->isLoaded()) {
		cout << "Eng: Exiting Loading Screen\n";
		loading = false;
	}
	else {
		loadingspinner += 220.0f * dt;
		loadingTime += dt;
	}
}
void Loading_render() {
	// cout << "Eng: Loading Screen Render\n";
	static CircleShape octagon(80, 8);
	octagon.setOrigin(80, 80);
	octagon.setRotation(loadingspinner);
	octagon.setPosition(Vcast<float>(Engine::getWindowSize()) * .5f);
	octagon.setFillColor(Color(255, 255, 255, (Uint8)min(255.f, 40.f*loadingTime)));
	static Text t("Loading", *Resources::get<sf::Font>("RobotoMono-Regular.ttf"));
	t.setFillColor(Color(255, 255, 255, (Uint8)min(255.f, 40.f*loadingTime)));
	t.setPosition(Vcast<float>(Engine::getWindowSize()) * Vector2f(0.4f, 0.3f));
	Renderer::queue(&t);
	Renderer::queue(&octagon);
}

float frametimes[256] = {};
uint8_t ftc = 0;

void Engine::Update() {
	static sf::Clock clock;
	float dt = clock.restart().asSeconds();
	{
		frametimes[++ftc] = dt;
		static string avg = _gameName + " FPS:";
		if (ftc % 60 == 0) {
			double davg = 0;
			for (const auto t : frametimes) {
				davg += t;
			}
			davg = 1.0 / (davg / 255.0);
			_window->setTitle(avg + toStrDecPt(2, davg));
		}
	}

	if (loading) {
		Loading_update(dt, _activeScene);
	}
	else if (_activeScene != nullptr) {
		Physics::update(dt);
		_activeScene->Update(dt);
	}
}

void Engine::Render(RenderWindow& window) {
	if (loading) {
		Loading_render();
	}
	else if (_activeScene != nullptr) {
		_activeScene->Render();
	}

	Renderer::render();
}

void Engine::Start(unsigned int width, unsigned int height,
	const std::string& gameName, Scene* scn) {
	RenderWindow window(VideoMode(width, height), gameName, sf::Style::Resize);
	_gameName = gameName;
	_window = &window;
	Renderer::initialise(window);
	Physics::initialise();

	ChangeScene(scn, gameName);
	if (!musicBuffer.loadFromFile("res/sounds/music.wav"))
	{
		std::cout << "Can't find music file!" << std::endl;
	}
	music.setBuffer(musicBuffer);
	music.setVolume(10.0f);
	music.setLoop(true);
	music.play();

	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			switch (event.type) {
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::KeyPressed:
				if (event.key.code == sf::Keyboard::Escape) {
					window.close();
				}
				else if (event.key.code == sf::Keyboard::F1) {
					window.create(sf::VideoMode(width, height, 32), gameName, sf::Style::Fullscreen);
				}
				else if (event.key.code == sf::Keyboard::F2) {
					window.create(sf::VideoMode(width, height, 32), gameName, sf::Style::Resize);
				}
			}
		}
		if (Keyboard::isKeyPressed(Keyboard::Escape)) {
			window.close();
		}

		if (wait == false) {
			window.clear();
			Update();
			Render(window);
			window.display();
		}
	}
	if (_activeScene != nullptr) {
		_activeScene->UnLoad();
		_activeScene = nullptr;
	}
	window.close();
	Physics::shutdown();
	// Render::shutdown();
}

std::shared_ptr<Entity> Scene::makeEntity() {
	auto e = make_shared<Entity>(this);
	ents.list.push_back(e);
	return std::move(e);
}

void Engine::setVsync(bool b) { _window->setVerticalSyncEnabled(b); }

void Engine::ChangeScene(Scene* s, std::string name) {
	cout << "Eng: changing scene: " << s << endl;
	auto old = _activeScene;
	_activeScene = s;

	if (old != nullptr) {
		old->UnLoad(); // todo: Unload Async
	}
	if (!s->isLoaded()) {
		cout << "Eng: Entering Loading Screen\n";
		loadingTime = 0;
		_activeScene->Load();
		loading = true;
	}
}

sf::Vector2u Engine::getWindowSize() { return _window->getSize(); }

sf::RenderWindow& Engine::GetWindow() { return *_window; }

Scene* Engine::GetActiveScene()
{
	return _activeScene;
}

void Scene::Update(const double& dt) { ents.update(dt); }

void Scene::Render() { ents.render(); }

bool Scene::isLoaded() const
{
	std::lock_guard<std::mutex> lck(_loaded_mtx);
	// Are we already loading asynchronously?
	if (_loaded_future.valid() // yes
		&&                     // Has it finished?
		_loaded_future.wait_for(chrono::seconds(0)) ==
		future_status::ready) {
		// Yes
		_loaded_future.get();
		//getFromFuture(_loaded_future);
		_loaded = true;
	}
	//_loaded = true;
	return _loaded;
}

void Scene::setLoaded(bool b) {
	{
		std::lock_guard<std::mutex> lck(_loaded_mtx);
		_loaded = b;
	}
}

void Scene::UnLoad() {
	ents.list.clear();
	setLoaded(false);
}

void Scene::LoadAsync() { _loaded_future = std::async(&Scene::Load, this); }

namespace timing {
	// Return time since Epoc
	long long now() {
		return std::chrono::high_resolution_clock::now()
			.time_since_epoch()
			.count();
	}
	// Return time since last() was last called.
	long long last() {
		auto n = now();
		static auto then = now();
		auto dt = n - then;
		then = n;
		return dt;
	}
} // namespace timing

// Return time since Epoc
long long now() {
  return std::chrono::high_resolution_clock::now()
      .time_since_epoch()
      .count();
}
// Return time since last() was last called.
long long last() {
  auto n = now();
  static auto then = now();
  auto dt = n - then;
  then = n;
  return dt;
}

Scene::~Scene() { UnLoad(); }