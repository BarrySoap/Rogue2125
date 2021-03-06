#include "buttons.h"
#include "engine.h"
#include "components\cmp_sprite.h"
#include "components\cmp_text.h"
#include "components\cmp_button.h"

using namespace std;
using namespace sf;

shared_ptr<Entity> new_button(string text)
{
	auto btn = Engine::GetActiveScene()->makeEntity();
	btn->addTag("button");

	auto btn_shape = btn->addComponent<ShapeComponent>();
	btn_shape->setShape<RectangleShape>(Vector2f(300.0f, 50.0f));
	btn_shape->getShape().setOrigin(btn_shape->getShape().getLocalBounds().width / 2, btn_shape->getShape().getLocalBounds().height / 2);

	auto btn_text = btn->addComponent<TextComponent>(text);
	btn_text->SetOrigin(Vector2f(0.5, 0.5f));

	btn->addComponent<ButtonComponent>(btn_text, btn_shape);

	return btn;
}