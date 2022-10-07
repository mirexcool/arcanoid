

#include "inc/Framework.h"

#include <algorithm>
#include <iostream>
#include <list>
#include "unistd.h"



/* Test Framework realization */
class MyFramework : public Framework {

    const int SIZE_X = 10;
    const int SIZE_Y = 10;

    const int BRICK_SEP = 2;
    const int BRICK_Y_OFFSET = 50;
    const int RACKET_Y_OFFSET = 25;

    int BRICK_WIDTH, BRICK_HEIGHT ;

    int WINDOW_WIDTH , WINDOW_HEIGHT;
    std::list<Sprite*> listObject;

    Sprite *racket;


public:

	virtual void PreInit(int& width, int& height, bool& fullscreen)
	{
		width = 600;
		height = 400;
        WINDOW_WIDTH = width;
        WINDOW_HEIGHT = height;

        BRICK_WIDTH = (WINDOW_WIDTH - BRICK_SEP * SIZE_X  )/ SIZE_X;
        BRICK_HEIGHT= WINDOW_HEIGHT / SIZE_Y;


        fullscreen = false;

	}

    virtual void BuildBricks(){
        //racket = createSprite("../Project/data/49-Breakout-Tiles.png");

        for (int i = 0; i < SIZE_X; ++i) {
            //for (int j = SIZE_Y - 1; j  >=0; j--){
                double xPos = BRICK_SEP + i * BRICK_WIDTH  + i * BRICK_SEP / 2.0;
                //double yPos = BRICK_Y_OFFSET + i * BRICK_WIDTH + i * BRICK_SEP;

                listObject.push_back(BuildEachBrick(xPos, 0));

            //}
        }
    }

    virtual Sprite* BuildEachBrick(int i, int j){
        Sprite *brick = createSprite("../Project/data/02-Breakout-Tiles.png");
        setSpriteSize(brick,BRICK_WIDTH, BRICK_HEIGHT);
        int w, h;
        getSpriteSize(brick,w, h);

        drawSprite(brick, i, j);

        return brick;
    }

	virtual bool Init() {


		return true;
	}

	virtual void Close() {

	}

	virtual bool Tick() {
        drawTestBackground();
        BuildBricks();
        DrawBricks();
        /*if (racket != nullptr)
            drawSprite(racket, WINDOW_WIDTH/2.0, WINDOW_HEIGHT - RACKET_Y_OFFSET);*/
        sleep(1);
        return false;
	}

    virtual void DrawBricks(){
        //std::list<Sprite*>::iterator it;
        Sprite* brick = createSprite("../01-Breakout-Tiles.png");
        for (auto it = listObject.begin(); it != listObject.end(); ++it){
            int w, h;
            //if (*it != nullptr)
                getSpriteSize(*it,w,h);
            //getSpriteSize(it,w,h);

            //drawSprite(it, 50, 50);
        }
    }

	virtual void onMouseMove(int x, int y, int xrelative, int yrelative) {

	}

	virtual void onMouseButtonClick(FRMouseButton button, bool isReleased) {

	}

	virtual void onKeyPressed(FRKey k) {

        if (k == FRKey::RIGHT) {
            drawSprite(createSprite("../Project/data/02-Breakout-Tiles.png"), 50, 50);
            std::cout << "PRESSED";
        }
	}

	virtual void onKeyReleased(FRKey k) {
	}
	
	virtual const char* GetTitle() override
	{
		return "Arcanoid";
	}
};

int main(int argc, char *argv[])
{

	return run(new MyFramework);
}
