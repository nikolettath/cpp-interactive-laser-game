#include "gamestate.h"
#include "player.h"
#include "level.h"
#include "sgg/graphics.h"



GameState::GameState()
{
}


void GameState::init()
{
	m_current_level = new Level();
	m_current_level->init();

	m_player1 = new Player("Player1");
	m_player1->init();

	m_player2 = new Player("Player2");
	m_player2->init();

	m_player1->setOpponent(m_player2);
	m_player2->setOpponent(m_player1);

	graphics::preloadBitmaps(getAssetDir());
	


}

void GameState::draw()
{

	if (m_in_menu) {
		graphics::Brush brush;
		brush.texture = getFullAssetPath("enter.png");
		brush.outline_opacity = 0.0f;
		graphics::drawRect(getCanvasWidth() / 2.0f, getCanvasHeight() / 2.0f, 9.0f, 7.0f, brush);
		return;
	}

	if (m_game_over) {
		graphics::Brush brush;
		if (m_winner == "Player1") {
			brush.texture = getFullAssetPath("winner1.png");
		}
		else if (m_winner == "Player2") {
			brush.texture = getFullAssetPath("winner2.png");
		}
		brush.outline_opacity = 0.0f;
		graphics::drawRect(getCanvasWidth() / 2.0f, getCanvasHeight() / 2.0f, 9.0f, 7.0f, brush);
		return;
	}


	if (!m_current_level)
		return;

	m_current_level->draw();
	m_player1->draw();
	m_player2->draw();
	

}

void GameState::update(float dt)
{
	if (m_in_menu) {
		if (graphics::getKeyState(graphics::SCANCODE_RETURN)) { 
			startGame();
		}
		return;
	}
	if (m_game_over) {
		return;
	}

	m_player1->update(dt);
	m_player2->update(dt);

	if (dt > 500)
		return;

	if (!m_current_level)
		return;
	m_current_level->update(dt);

	m_debugging = graphics::getKeyState(graphics::SCANCODE_0);
}





GameState* GameState::getInstance()
{
	if (m_unique_instance == nullptr)
		m_unique_instance = new GameState();
	return m_unique_instance;
}

GameState::~GameState()
{

	if (m_player1)
		delete m_player1;
	if (m_player2)
		delete m_player2;
	if (m_current_level)
		delete m_current_level;
}


std::string GameState::getAssetDir()
{
	return m_asset_path;
}

std::string GameState::getFullAssetPath(const std::string& asset)
{
	return m_asset_path + asset;
}

void GameState::startGame()
{
	m_winner = ""; m_in_menu = false; m_game_over = false;
}

void GameState::setGameOver()
{
	m_game_over = true;
}

void GameState::setWinner(const std::string & winner) 
{ 
	m_winner = winner; 
}



GameState* GameState::m_unique_instance = nullptr;