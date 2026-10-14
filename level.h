#pragma once
#include "gameobject.h"
#include "sgg/graphics.h"
#include "player.h"
#include <list>
#include <vector>

class Level : public GameObject {
private:
	float m_center_x = 5.5f;
	float m_center_y = 3.0f;
	graphics::Brush m_brush_background;
	std::list<Player*> m_dynamic_object;
	std::vector<Player*> m_static_object;
	

	void drawlaser();
	void checkCollisions();

public:
	void update(float dt) override;
	void init() override;
	void draw() override;
	Level(const std::string& name = "Level0");
	~Level();
};

