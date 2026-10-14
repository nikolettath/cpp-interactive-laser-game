#pragma once
#include "player.h"
#include <string>
#include "gameobject.h"
#include "sgg/graphics.h"
#include <vector>

class GameState {
private:
	std::string m_asset_path = "assets\\";

	float m_canvas_width = 11.0f;
	float m_canvas_height = 6.0f;

	bool m_in_menu = true;
	bool m_game_over = false;
	std::string m_winner = "";

	static GameState* m_unique_instance;

	GameState();

	class Player* m_player1 = 0;
	class Player* m_player2 = 0;

	class Level* m_current_level = 0;


public:

	float m_player_offset_x = 0.0f;
	float m_player_offset_y = 0.0;

	float m_global_offset_x = 0.0f;
	float m_global_offset_y = 0.0f;

	bool m_debugging = false;



	void init();
	void draw();
	void update(float dt);


	static GameState* getInstance();
	~GameState();

	float getCanvasWidth() { return m_canvas_width; }
	float getCanvasHeight() { return m_canvas_height; }

	std::string getAssetDir();
	std::string getFullAssetPath(const std::string& asset);

	class Player* getPlayer1() const { return m_player1; }
	class Player* getPlayer2() const { return m_player2; }

	void startGame();
	void setGameOver();
	void setWinner(const std::string& winner);

};