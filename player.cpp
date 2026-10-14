#include "player.h"
#include <sgg/graphics.h>
#include "gamestate.h"
#include <iostream>
#include "util.h"
#include <chrono>


void Player::drawLives(int m_lives) {
	float startX = 10.0f;
	float startY = 10.0f;

	m_brush_lives.texture = m_state->getFullAssetPath("lives3.png");
	m_brush_lives.fill_opacity = 1.0f;
	m_brush_lives.outline_opacity = 0.0f;


	std::string lifeImage;
	if (m_lives > 0) {
		lifeImage = "lives" + std::to_string(m_lives) + ".png";
	}
	
	m_brush_lives.texture = m_state->getFullAssetPath(lifeImage);
	graphics::drawRect(m_pos_lives, m_pos_y - 0.8f, 1.0f, 1.0f, m_brush_lives);
}

void Player::movePlayer(float dt)
{
	float delta_time = dt / 1000.0f;
	bool keyPressed = false;

	if (m_name == "Player1")
		keyPressed = graphics::getKeyState(graphics::SCANCODE_W);
	if (m_name == "Player2")
		keyPressed = graphics::getKeyState(graphics::SCANCODE_UP);

	if (keyPressed && m_pos_y >= m_state->getCanvasHeight() - m_size / 2.0f) {
		m_vy = -m_accel_vertical;
	}

	m_vy += delta_time * m_gravity;
	m_pos_y += 1.5* m_vy * delta_time;

	float ground_level = m_state->getCanvasHeight() - m_size / 2.0f;
	if (m_pos_y > ground_level) {
		m_pos_y = ground_level;
		m_vy = 0.0f;
	}
	if (m_pos_y < 0) {
		m_pos_y = 0; // Αν ο παίκτης προσπαθεί να ανέβει πάνω από την κορυφή, τον επαναφέρουμε
		m_vy = 0.0f; // Μηδενίζουμε την ταχύτητα
	}

}


bool Player::checkCollisionsWithOpponent(const Laser& laser)
{
	Box laserBox(laser.x, laser.y, 0.5f, 0.05f);

	Box opponentBox(
		m_opponent->getPosX(),
		m_opponent->getPosY(),
		m_opponent->getSize()/2,
		m_opponent->getSize()/2
	);

	return laserBox.intersect(opponentBox);
}


void Player::update(float dt)
{
	movePlayer(dt);

	if (m_name == "Player1" && graphics::getKeyState(graphics::SCANCODE_D)) {
		shoot();
	}
	if (m_name == "Player2" && graphics::getKeyState(graphics::SCANCODE_LEFT)) {
		shoot();
	}

	// Ενημέρωση της θέσης των λέιζερ
	for (auto& laser : m_lasers) {
		if (laser.active) {
			laser.x += laser.vx * dt / 1000.0f;

			// Αν το λέιζερ βγει εκτός οθόνης, το απενεργοποιούμε
			if (laser.x < 0 || laser.x > m_state->getCanvasWidth()) {
				laser.active = false;
			}

			// Έλεγχος αν το λέιζερ χτύπησε τον αντίπαλο
			if (checkCollisionsWithOpponent(laser)) {
				laser.active = false;
				m_opponent->decreaseLife();
			}
		}
	}


	m_lasers.erase(
		std::remove_if(m_lasers.begin(), m_lasers.end(), [](const Laser& laser) {
			return !laser.active;
			}),
		m_lasers.end());
}

void Player::init()
{
	m_pos_y = m_state->getCanvasHeight() - m_size / 2.0f;

	if (m_name == "Player1")
	{
		m_brush_player.texture = m_state->getFullAssetPath("char1.png");
		drawLives(m_lives);
		m_pos_x = 3.0f;
		m_pos_lives = m_pos_x - 0.1f;
		m_opponent = m_state->getPlayer2();


	}
	if (m_name == "Player2")
	{
		m_brush_player.texture = m_state->getFullAssetPath("char2.png");
		drawLives(m_lives);
		m_pos_x = 8.0f;
		m_pos_lives = m_pos_x + 0.1f;
		m_opponent = m_state->getPlayer1();


	}


	m_brush_player.fill_opacity = 1.0f;
	m_brush_player.outline_opacity = 0.0f;

}

void Player::draw()
{
	graphics::drawRect(m_pos_x, m_pos_y, m_size, m_size, m_brush_player);

	drawLives(m_lives);
	graphics::Brush laserBrush;
	SETCOLOR(laserBrush.fill_color, 1.1f, 0.5f, 0.2f);
	laserBrush.fill_opacity = 1.0f;
	laserBrush.outline_opacity = 0.0f;

	for (const auto& laser : m_lasers) {
		if (laser.active) {
			graphics::drawRect(laser.x, laser.y, 0.5f, 0.05f, laserBrush);
		}
	}

	if (m_state->m_debugging)
		debugDraw();


}



void Player::shoot() {

	float direction = (m_name == "Player1") ? 1.0f : -1.0f;

	float laserY = 0.0f;
	float laserX = 0.0f;

	static const int cooldownMs = 200;
	static auto lastShootTime = std::chrono::steady_clock::now();

	auto now = std::chrono::steady_clock::now();
	auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastShootTime).count();

	if (elapsedTime < cooldownMs) {
		return;
	}

	lastShootTime = now;



	if (m_name == "Player1")
	{
		laserX = m_pos_x - 0.1f + (direction * (m_size / 2.0f));
		laserY = m_pos_y - 0.1f;
	}
	if (m_name == "Player2")
	{
		laserX = m_pos_x + 0.1f + (direction * (m_size / 2.0f));
		laserY = m_pos_y;
	}

	Laser new_laser = { laserX, laserY, 8.0f * direction, true, m_name };
	m_lasers.emplace_back(new_laser);

}



void Player::decreaseLife()
{
	if (m_lives > 1) {
		m_lives--; drawLives(m_lives);
	}else if(m_lives == 1 ){
		setActive(false);
		GameState::getInstance()->setGameOver();
		GameState::getInstance()->setWinner(m_opponent->getName());
		
	}
}

void Player::debugDraw()
{
	graphics::Brush brush;
	SETCOLOR(brush.fill_color, 1.0f, 0.0f, 0.0f);
	brush.outline_opacity = 1.0f;
	brush.fill_opacity = 0.0f;

	graphics::drawRect(m_pos_x, m_pos_y, m_size, m_size, brush);
	for (const auto& laser : m_lasers) {
		if (laser.active) {
			graphics::drawRect(laser.x, laser.y, 0.5f, 0.05f, brush);
		}
	}
}




