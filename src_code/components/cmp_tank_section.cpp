#include "cmp_tank_section.h"
#include "cmp_actor_stats.h"
#include <AllMgrs.h>
#include "engine.h"

using namespace std;

TankSection::TankSection(Entity* p, shared_ptr<Equipment> item, shared_ptr<InventoryComponent> inv, shared_ptr<SkillsComponent> skills)
	: AnimComponent(p)
{
	m_item = item;
	m_inv = inv;
	m_skills = skills;

	SetScale(0.75f);
	SetTexture(TextureMgr::GetInstance()->sheet_Items[m_item->GetPriID()][m_item->GetSecID()], 32);
	SetFPS(30.0f);

	SetColor(m_item->GetColor());
	SetOrigin(Vector2f(0.5f, 0.5f));
}

void TankSection::update(double dt)
{
	//SetTexture(TextureMgr::GetInstance()->sheet_Items[m_item->GetPriID()][m_item->GetSecID()], 32);
	//SetColor(m_item->GetColor());

	AnimComponent::basicUpdate(dt);

	m_sprite->setPosition(_parent->getPosition());

	if (m_item->GetPriID() == 0 && m_item->GetSecID() == 0)
	{
		shared_ptr<TankGun> gun = static_pointer_cast<TankGun>(m_item);

		gun->update(dt, m_skills->GetTotalSkill(m_inv->GetEquipped(), 1));

		if (gun->GetFired())
		{
			DmgData data = gun->GetShotData(m_skills->GetTotalSkill(m_inv->GetEquipped(), 0), m_skills->GetTotalSkill(m_inv->GetEquipped(), 1));
			data.actorID = _parent->GetCompatibleComponent<ActorStatsComponent>()[0]->GetID();

			data.vel = Vector2f(sin(GetSpriteAng() / 57.295f), -cos(GetSpriteAng() / 57.295f)) * 10.0f;
			data.angle = GetSpriteAng();

			Vector2f offset = normalize(data.vel) * 16.0f;


			shared_ptr<Entity> a = Engine::GetActiveScene()->makeEntity();
			a->setPosition(_parent->getPosition() + offset);
			a->addComponent<AttackComponent>(data);
		}
	}
}