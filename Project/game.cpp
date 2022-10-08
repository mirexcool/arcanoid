#include "inc/Framework.h"
#include <algorithm>
#include <iostream>
#include <cmath>
#include<string.h>
#include <sstream>
using namespace std;

class MySprite {

public:
    int x;
    int y;
    Sprite *sprite;
    int live;
};

class BallSprite {

public:
    int x;
    int y;
    Sprite *sprite;
    int vX;
    int vY;
};

/* Test Framework realization */
class MyFramework : public Framework {

    const int SIZE_X = 10;
    const int SIZE_Y = 6;

    int RACKET_WIDTH = 150;
    int RACKET_HEIGHT = 15;

    const int BRICK_SEP = 25;
    const int BRICK_Y_OFFSET = 50;
    const int RACKET_Y_OFFSET = RACKET_HEIGHT + 25;

    int BRICK_WIDTH;
    int BRICK_HEIGHT = 25;

    int WINDOW_WIDTH, WINDOW_HEIGHT;
    //racket
    Sprite *racket;
    int xRacket;
    int yRacket;
    bool moveRight = false;
    bool moveLeft = false;

    //ball
    const int BALL_RADIUS = 10;
    const double BALL_SPEED = 5;
    bool ballIsMoving = false;
    //mouse
    int mouseX, mouseY;
    int ballToMove;

    BallSprite *ball;

    MySprite *bricks;

    const char *START_BRICK_SPRITE = "../Project/data/01-Breakout-Tiles.png";
    const char *DAMAGED_BRICK_SPRITE = "../Project/data/02-Breakout-Tiles.png";

    const char *RACKET_SPRITE = "../Project/data/07-Breakout-Tiles.png";
    const char *BALL_BRICK_SPRITE = "../Project/data/58-Breakout-Tiles.png";

    const char *BONUS_SPRITE = "../Project/data/59-Breakout-Tiles.png";

    MySprite *bonus;


public:

    MyFramework(char *str) {
        string token;
        istringstream tokenStream(str);
        int i = 0;
        while (std::getline(tokenStream, token, 'x')) {
            if (i == 0)
                WINDOW_WIDTH = stoi(token);
            if (i == 1)
                WINDOW_HEIGHT = stoi(token);
            i++;
        }
    }

    MyFramework() {
        WINDOW_WIDTH = 600;
        WINDOW_HEIGHT = 800;
    }

    virtual void PreInit(int &width, int &height, bool &fullscreen) {
        width = WINDOW_WIDTH;
        height = WINDOW_HEIGHT;
        mouseX = 0;
        mouseY = 0;

        xRacket = WINDOW_WIDTH / 2;
        yRacket = WINDOW_HEIGHT - RACKET_Y_OFFSET;

        BRICK_WIDTH = (WINDOW_WIDTH - BRICK_SEP * SIZE_X) / SIZE_X;

        bricks = new MySprite[SIZE_X * SIZE_Y];
        bonus = new MySprite;

        ball = new BallSprite[3];
        fullscreen = false;
    }

    virtual void BuildRacket() {
        racket = createSprite(RACKET_SPRITE);
        setSpriteSize(racket, RACKET_WIDTH, RACKET_HEIGHT);
    }

    virtual void BuildBall(int index) {

        ball[index].sprite = createSprite(BALL_BRICK_SPRITE);
        setSpriteSize(ball[index].sprite, 2 * BALL_RADIUS, 2 * BALL_RADIUS);
        if (index == 0) {
            ball[index].x = WINDOW_WIDTH / 2;
            ball[index].y = WINDOW_HEIGHT - RACKET_Y_OFFSET - 2 * (BALL_RADIUS + 1);
        } else {
            ball[index].x = WINDOW_WIDTH / 2;
            ball[index].y = WINDOW_HEIGHT / 2;
        }
        ballToMove = 0;
    }

    virtual void BuildBricks() {
        int n = 0;
        for (int i = 0; i < SIZE_X; ++i) {
            for (int j = SIZE_Y - 1; j >= 0; j--) {
                double xPos = BRICK_SEP + i * BRICK_WIDTH + i * BRICK_SEP / 2.0;
                double yPos = BRICK_Y_OFFSET + j * BRICK_WIDTH + j * BRICK_SEP;
                bricks[n].sprite = BuildEachBrick(xPos, yPos, n);
                n++;
            }
        }
    }

    virtual Sprite *BuildEachBrick(int i, int j, int n) {
        Sprite *brick = createSprite(START_BRICK_SPRITE);
        setSpriteSize(brick, BRICK_WIDTH, BRICK_HEIGHT);
        int w, h;
        getSpriteSize(brick, w, h);
        bricks[n].x = i;
        bricks[n].y = j;
        bricks[n].live = 2;

        return brick;
    }

    virtual bool Init() {
        BuildRacket();
        BuildBall(0);
        BuildBricks();
        return true;
    }

    virtual void initBallSpeed(int index, bool onMouseWay) {
        if (onMouseWay && ball[index].vX == 0 && ball[index].vY == 0) {
            int xVec = -(ball[index].x - mouseX);
            int yVec = -(ball[index].y - mouseY);
            double mod = sqrt(xVec * xVec + yVec * yVec);
            ball[index].vX = xVec * BALL_SPEED / mod;
            ball[index].vY = yVec * BALL_SPEED / mod;
        } else {
            if (randomBool() && !onMouseWay)
                ball[index].vX = rand() % (int) BALL_SPEED + 1;
            else ball[index].vX = -(rand() % (int) BALL_SPEED + 1);
            ball[index].vY = BALL_SPEED;
        }
    }


    virtual bool randomBool() {
        return 0 + (std::rand() % (1 - 0 + 1)) == 1;
    }

    virtual void Close() {

    }

    virtual bool Tick() {
        if (rand() % 500 == 0) {
            if (bonus->sprite == nullptr)
                CreateBonus();
        }
        drawTestBackground();

        DrawBricks();
        if (racket != nullptr)
            drawSprite(racket, xRacket, yRacket);
        for (int i = 0; i < 3; i++) {
            if (ball[i].sprite != nullptr) {
                drawSprite(ball[i].sprite, ball[i].x, ball[i].y);
                //CheckBallSpeed(i);
                if (ballToMove % 5 == 0)
                    BallMoving(i);
            }
        }
        RacketMoving();


        MoveBricks();
        if (ballToMove % 5 == 0)
            ballToMove = 0;
        ballToMove++;
        if (CheckGameOver())
            return true;
        return false;
    }

    virtual void CreateBonus() {
        bonus->sprite = createSprite(BONUS_SPRITE);
        setSpriteSize(bonus->sprite, BALL_RADIUS * 4, BALL_RADIUS * 4);

        bonus->x = rand() % (WINDOW_WIDTH + 1);
        bonus->y = rand() % (WINDOW_HEIGHT / 2 + 1);

    }

    virtual bool CheckGameOver() {
        bool gameNotFinished = true;

        for (int i = 0; i < 3; i++) {
            if (ball[i].sprite != nullptr) {
                gameNotFinished = false;
            }
        }
        if (gameNotFinished)
            return true;
        for (int i = 0; i < SIZE_X * SIZE_Y; i++) {
            if (bricks[i].sprite != nullptr) {
                return false;
            }
        }

        return true;
    }

    virtual void MoveBricks() {
        for (int i = 0; i < SIZE_X * SIZE_Y; i++) {
            if (bricks[i].sprite != nullptr) {
                if (i % 2 == 0) {
                    bricks[i].x++;
                    if (bricks[i].x > WINDOW_WIDTH)
                        bricks[i].x = 0;
                } else {
                    bricks[i].x--;
                    if (bricks[i].x < 0)
                        bricks[i].x = WINDOW_WIDTH;
                }
            }
        }
        if (bonus->sprite != nullptr) {
            bonus->y++;
            if (bonus->y >= yRacket && bonus->y <= yRacket + RACKET_HEIGHT) {
                if (bonus->x >= xRacket && bonus->x <= xRacket + RACKET_WIDTH) {
                    destroySprite(bonus->sprite);
                    bonus->sprite = nullptr;
                    makeBonus();
                }
            }
            if (bonus->sprite != nullptr) {
                if (bonus->y > WINDOW_HEIGHT) {
                    destroySprite(bonus->sprite);
                    bonus->sprite = nullptr;
                }
            }
        }
    }

    virtual void makeBonus() {
        if (randomBool()) {
            for (int i = 0; i < 3; i++) {
                if (ball[i].sprite == nullptr) {
                    BuildBall(i);
                    initBallSpeed(i, false);
                }
            }
        } else {
            for (int i = 0; i < 3; i++) {
                if (ball[i].sprite != nullptr) {
                    ball[i].x = rand() % WINDOW_WIDTH;
                    ball[i].y = rand() % WINDOW_HEIGHT / 2;
                }
            }
        }
    }

    virtual void DrawBricks() {
        for (int i = 0; i < SIZE_X * SIZE_Y; i++) {
            if (bricks[i].sprite != nullptr) {
                drawSprite(bricks[i].sprite, bricks[i].x, bricks[i].y);
            }
        }
        if (bonus->sprite != nullptr)
            drawSprite(bonus->sprite, bonus->x, bonus->y);
    }

    virtual void onMouseMove(int x, int y, int xrelative, int yrelative) {
        mouseX = x;
        mouseY = y;
    }

    virtual void onMouseButtonClick(FRMouseButton button, bool isReleased) {
        if (button == FRMouseButton::LEFT && isReleased && !ballIsMoving) {
            initBallSpeed(0, true);
            ballIsMoving = true;
        }

    }

    virtual void onKeyPressed(FRKey k) {

        if (k == FRKey::RIGHT) {
            moveRight = true;
        }
        if (k == FRKey::LEFT) {

            moveLeft = true;
        }
    }

    virtual void RacketMoving() {
        if (moveRight && (xRacket + RACKET_WIDTH) < WINDOW_WIDTH)
            xRacket += 3;
        if (moveLeft && xRacket > 0)
            xRacket -= 3;
    }

    virtual void BallMoving(int i) {
        ball[i].x += ball[i].vX;
        ball[i].y += ball[i].vY;

        BallRacketReflecting(i);
        BallBrickContact(i);
        // reflecting
        if (ball[i].x <= 0) {
            ball[i].vX = -ball[i].vX;
        }
        if (ball[i].x >= WINDOW_WIDTH - 2 * BALL_RADIUS) {
            ball[i].vX = -ball[i].vX;
        }
        if (ball[i].y <= 0) {
            ball[i].vY = -ball[i].vY;
        }
        if (ball[i].y >= WINDOW_HEIGHT - 2 * BALL_RADIUS) {
            destroyBall(i);
        }
    }

    virtual void destroyBall(int i) {
        if (ball[i].sprite != nullptr) {
            destroySprite(ball[i].sprite);
            ball[i].sprite = nullptr;
        }
    }

    virtual void CheckBallSpeed(int i) {
        int xM = 1, yM = 1;
        if (ball[i].vX < 0)
            xM = -1;
        if (ball[i].vX < 0)
            yM = -1;
    }

    virtual void BallBrickContact(int index) {
        int ballContour[4][2] = {{ball[index].x,                   ball[index].y},
                                 {ball[index].x,                   ball[index].y + 2 * BALL_RADIUS},
                                 {ball[index].x + 2 * BALL_RADIUS, ball[index].y},
                                 {ball[index].x + 2 * BALL_RADIUS, ball[index].y + 2 * BALL_RADIUS}};

        for (int *element: ballContour) {
            for (int i = 0; i < SIZE_X * SIZE_Y; i++) {
                if (bricks[i].sprite != nullptr && bricks[i].live > 0) {
                    if (element[1] >= bricks[i].y && element[1] <= bricks[i].y + BRICK_HEIGHT) {
                        if (element[0] >= bricks[i].x && element[0] <= bricks[i].x + BRICK_WIDTH) {
                            ball[index].vY = -ball[index].vY;
                            if (bricks[i].sprite != nullptr) {
                                bricks[i].live--;
                                ball[index].y += ball[index].vY;
                                ball[index].x += ball[index].vX;
                                destroySprite(bricks[i].sprite);
                                bricks[i].sprite = nullptr;
                                if (bricks[i].live > 0) {
                                    Sprite *newBrick = createSprite(DAMAGED_BRICK_SPRITE);
                                    setSpriteSize(newBrick, BRICK_WIDTH, BRICK_HEIGHT);
                                    bricks[i].sprite = newBrick;
                                }
                            }
                            return;
                        }
                    }

                    if (element[0] >= bricks[i].x && element[0] <= bricks[i].x + BRICK_WIDTH) {
                        if (element[1] >= bricks[i].y && element[1] <= bricks[i].y + BRICK_HEIGHT) {
                            ball[index].vY = -ball[index].vY;
                            if (bricks[i].sprite != nullptr) {
                                bricks[i].live--;
                                ball[index].y += ball[index].vY;
                                ball[index].x += ball[index].vX;
                                destroySprite(bricks[i].sprite);
                                bricks[i].sprite = nullptr;
                                if (bricks[i].live > 0) {
                                    Sprite *newBrick = createSprite(DAMAGED_BRICK_SPRITE);
                                    setSpriteSize(newBrick, BRICK_WIDTH, BRICK_HEIGHT);
                                    bricks[i].sprite = newBrick;
                                }
                            }
                            return;
                        }
                    }
                }
            }
        }
    }

    virtual void BallRacketReflecting(int i) {
        int ballContour[4][2] = {{ball[i].x,                   ball[i].y},
                                 {ball[i].x,                   ball[i].y + 2 * BALL_RADIUS},
                                 {ball[i].x + 2 * BALL_RADIUS, ball[i].y},
                                 {ball[i].x + 2 * BALL_RADIUS, ball[i].y + 2 * BALL_RADIUS}};

        for (int *element: ballContour) {
            if (element[1] >= yRacket) {
                if (element[0] >= xRacket && element[0] <= xRacket + RACKET_WIDTH) {
                    ball[i].vY = -abs(ball[i].vY);
                    break;
                }
            }
        }
    }


    virtual void onKeyReleased(FRKey k) {
        if (k == FRKey::RIGHT) {
            moveRight = false;
        }
        if (k == FRKey::LEFT) {
            moveLeft = false;
        }
    }

    virtual const char *GetTitle() override {
        return "Arcanoid";
    }

    virtual void setWindowSize() {

    }
};


int main(int argc, char *argv[]) {
    MyFramework *game = nullptr;
    if (argc > 0) {
        if (strcmp(argv[0], "-window")) {
            if (argc > 1) {
                game = new MyFramework(argv[2]);
            }
        }
    }
    srand(time(NULL));
    if (game == nullptr)
        return run(new MyFramework());
    return run(game);
}
