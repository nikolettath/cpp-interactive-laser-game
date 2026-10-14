#include "sgg/graphics.h"
#include <stdio.h>
#include "gamestate.h"
#include "util.h"
#include <string>

//ANNA TSAGKERA p3230209
//NIKOLETTA THEOCHARI p3230055

void draw()
{
	GameState::getInstance()->draw();
}



void update(float dt)
{
	GameState::getInstance()->update(dt);
}

void init()
{
	GameState::getInstance()->init();
}



int main(int argc, char** argv)
{
	graphics::createWindow(1792, 1024, "Space");

	graphics::setDrawFunction(draw);
	graphics::setUpdateFunction(update);

	init();

	graphics::setCanvasSize(GameState::getInstance()->getCanvasWidth(), GameState::getInstance()->getCanvasHeight());
	graphics::setCanvasScaleMode(graphics::CANVAS_SCALE_STRETCH);

	graphics::startMessageLoop();

	return 0;
}