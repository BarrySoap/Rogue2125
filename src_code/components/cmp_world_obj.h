#pragma once

#include "cmp_sprite.h"
#include "../general/world_objs.h"
#include "../general/data_shapes.h"
#include "cmp_actor_stats.h"
#include "cmp_tilemap.h"

class WorldObjComp : public AnimComponent
{
public:
	WorldObjComp() = delete;
	WorldObjComp(Entity* p, std::shared_ptr<WorldObj> obj);
	~WorldObjComp() = default;

	int GetObjID();
	//std::shared_ptr<WorldObj>

	void update(double dt) override;

	void UseObj(std::shared_ptr<ActorStatsComponent> actor);
protected:

private:
	std::shared_ptr<Entity> m_map;

	std::shared_ptr<WorldObj> m_obj;
	DataShapes::Circle m_useCircle;
};