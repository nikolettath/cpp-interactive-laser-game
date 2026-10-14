#include "level.h"
#include"gamestate.h"
#include "sgg/graphics.h"
#include"box.h"
#include "player.h"
#include "util.h"


void Level::drawlaser()
{
	for (auto& player : m_dynamic_object) {
		player->draw();
	}
}

void Level::checkCollisions()
{
	for (auto player : m_dynamic_object) {
		if (!player->isActive()) continue;

		for (auto& laser : player->getLasers()) {
			if (!laser.active) continue;

			for (auto otherPlayer : m_dynamic_object) {
				if (player == otherPlayer || !otherPlayer->isActive()) continue;

				float dx = laser.x - otherPlayer->getPosX();
				float dy = laser.y - otherPlayer->getPosY();
				float distanceSquared = dx * dx + dy * dy;
				float radiusSquared = (otherPlayer->getSize() / 2.0f) * (otherPlayer->getSize() / 2.0f);

				if (distanceSquared < radiusSquared) {
					otherPlayer->decreaseLife();
					laser.active = false;
				}
			}
		}
	}
	
}

void Level::update(float dt)
{	
	checkCollisions();
	GameObject::update(dt);
}

void Level::init()
{
	m_brush_background.outline_opacity = 0.0f;
	m_brush_background.texture = m_state->getFullAssetPath("space.png");

}

void Level::draw()
{
	float w = m_state->getCanvasWidth();
	float h = m_state->getCanvasHeight();

	float offset_x = m_state->m_global_offset_x + w / 2.0f;
	float offset_y = m_state->m_global_offset_y + h / 2.0f;

	//draw background
	graphics::drawRect(m_center_x, m_center_y, 1.2f * w, 1.1f * h, m_brush_background);

	if (m_state->getPlayer1()->isActive())
		m_state->getPlayer1()->draw();

	if (m_state->getPlayer2()->isActive())
		m_state->getPlayer2()->draw();

}

Level::Level(const std::string& name) :GameObject(name)
{
	m_brush_background.outline_opacity = 0.0f;
	m_brush_background.texture = m_state->getFullAssetPath("space.png");
}

Level::~Level()
{

}
