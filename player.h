#pragma once
#include <vector>
#include <list>
#include"gamestate.h"
#include"gameobject.h"
#include "box.h"
#include <sgg/graphics.h>
#include "util.h"

struct Laser {
	float x, y;
	float vx;
	bool active = true;
	std::string owner;
};

class Player : public GameObject {
private:
	float m_pos_x, m_pos_y;
	float m_vy = 0.0f;
	float m_gravity = 9.8f;
	float m_accel_vertical = 7.5f;
	float m_size = 1.5f;
	int m_lives = 3;
	float m_pos_lives;
	graphics::Brush m_brush_player;
	graphics::Brush m_brush_lives;
	std::vector<Laser> m_lasers;

	void movePlayer(float dt);
	void drawLives(int m_lives);
	void debugDraw();
	Player* m_opponent = nullptr;
public:
	
	bool checkCollisionsWithOpponent(const Laser& laser);
	Player(const std::string name) : GameObject(name) {}
	void update(float dt) override;
	void init() override;
	void draw() override;
	void shoot();
	void decreaseLife();
	float getPosX() const { return m_pos_x; }
	float getPosY() const { return m_pos_y; }
	float getSize() const { return m_size; }
	void setOpponent(Player* opponent) { m_opponent = opponent; }
	std::vector<Laser>& getLasers() { return m_lasers; }
	std::string getName() { return m_name; }

};
