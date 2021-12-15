#ifndef __SNAKE_H__
#define __SNAKE_H__

#define HEAD '*'
#define BODY '+'
#define FOOD 'o'

#include <cstdlib>
#include <ctime>

#include "cpoint.h"
#include "winsys.h"
#include "stack.h"
#include "screen.h"

enum direction_t {
    UP = 8,
    DOWN = 2,
    LEFT = 4,
    RIGHT = 6,
};

class CSnake : public CFramedWindow {
private:
    PointStack *SnakeBody;
    CPoint Head;

    enum direction_t new_direction;
    enum direction_t direction;

    bool pause;
    bool help;
    bool death;

    int level;
    int speed;
    int speed_counter;

    CPoint foodPoint;

    void generateStartingConditions() {
        help = true;
        pause = true;
        death = false;
        delete SnakeBody;
        SnakeBody = new PointStack;
        Head = CPoint(rand() % (geom.size.x - 5) + 2, rand() % (geom.size.y - 5) + 2);
        *SnakeBody += CPoint(Head.x - 1, Head.y);
        *SnakeBody += CPoint(Head.x - 2, Head.y);
        level = 1;
        speed = 5;
        speed_counter = 0;
        direction = new_direction = RIGHT;
        makeFood();
        paint();
    }

    void makeFood() {
        bool canBePlaced = false;
        int rand_x;
        int rand_y;
        while (!canBePlaced) {
            rand_x = rand() % (geom.size.x - 2) + 1;
            rand_y = rand() % (geom.size.y - 2) + 1;
            canBePlaced = true;
            if (Head == CPoint(rand_x, rand_y)) {
                canBePlaced = false;
            }
            for (int i = 0; i < (*SnakeBody).getSize(); i++) {
                if ((*SnakeBody)[i] == CPoint(rand_x, rand_y)) {
                    canBePlaced = false;
                    break;
                }
            }
        }
        foodPoint = CPoint(rand_x, rand_y);
    }

    bool justAte() {
        if (Head == foodPoint) {
            makeFood();
            level++;
            if (!((level + 1) % 5) && speed > 1)
                speed--;
            return true;
        } else {
            return false;
        }
    }

    using CFramedWindow::move;
    bool move() {
        if (help || pause)
            return true;

        switch (new_direction) {
            case UP:
                if (direction != DOWN) {
                    direction = UP;
                }
                break;
            case DOWN:
                if (direction != UP) {
                    direction = DOWN;
                }
                break;
            case LEFT:
                if (direction != RIGHT) {
                    direction = LEFT;
                }
                break;
            case RIGHT:
                if (direction != LEFT) {
                    direction = RIGHT;
                }
                break;
        }

        (*SnakeBody) += Head;

        switch (direction) {
            case UP:
                Head += CPoint(0, -1);
                break;
            case DOWN:
                Head += CPoint(0, 1);
                break;
            case LEFT:
                Head += CPoint(-1, 0);
                break;
            case RIGHT:
                Head += CPoint(1, 0);
                break;
        }

        if (Head.x == 0) {
            Head.x = geom.size.x - 2;
        } else if (Head.x == geom.size.x - 1) {
            Head.x = 1;
        }
        if (Head.y == 0) {
            Head.y = geom.size.y - 2;
        } else if (Head.y == geom.size.y - 1) {
            Head.y = 1;
        }

        for (int i = 0; i < (*SnakeBody).getSize(); i++) {
            if ((*SnakeBody)[i] == Head) {
                death = true;
                paint_dead();
                return false;
            }
        }

        if (!justAte()) {
            (*SnakeBody).pop();
        }

        return true;
    }

    void paint_regular() {
        speed_counter++;
        if (!(speed_counter % speed)) {
            if (!move()) {
                death = true;
            }
        }

        gotoyx(geom.topleft.y, geom.topleft.x);
        printl("LEVEL %d", level);

        attron(COLOR_PAIR(COLOR_SNAKE));
        gotoyx(geom.topleft.y + Head.y, geom.topleft.x + Head.x);
        printc(HEAD);

        for (int i = 0; i < (*SnakeBody).getSize(); i++) {
            gotoyx(geom.topleft.y + (*SnakeBody)[i].y, geom.topleft.x + (*SnakeBody)[i].x);
            printc(BODY);
        }
        attroff(COLOR_PAIR(COLOR_SNAKE));

        attron(COLOR_PAIR(COLOR_FOOD));
        gotoyx(geom.topleft.y + foodPoint.y, geom.topleft.x + foodPoint.x);
        printc(FOOD);
        attroff(COLOR_PAIR(COLOR_FOOD));
    }

    void paint_dead() {
        gotoyx(geom.topleft.y + 2, geom.topleft.x + 2);
        printl("You lost, you got to level %d.", level);
        gotoyx(geom.topleft.y + 3, geom.topleft.x + 2);
        printl("Press r to restart game.", level);
    }

    void paint_help() {
        int y = geom.topleft.y, x = geom.topleft.x;
        gotoyx(y + 2, x + 2);
        printl("h - pauses game and shows this message");
        gotoyx(y + 3, x + 2);
        printl("p - pause/play");
        gotoyx(y + 4, x + 2);
        printl("r - restart game");
        gotoyx(y + 5, x + 2);
        printl("q - quit");
        gotoyx(y + 6, x + 2);
        printl("arrows - move around");
    }

    void paint_pause() {
        int y = geom.topleft.y, x = geom.topleft.x;
        gotoyx(y + 2, x + 2);
        printl("Game paused. Press p to unpause.");
    }

public:
    explicit CSnake(CRect r, char _c = ' ') : CFramedWindow(r, _c) {
        srand(time(nullptr));
        generateStartingConditions();
    };

    bool handleEvent(int key) override {
        if (tolower(key) == 'p' && !death) {
            paint_pause();
            pause = !pause;
            help = false;
        }
        if (tolower(key) == 'h' && !death) {
            if (help) {
                help = false;
                pause = false;
            } else {
                help = true;
                pause = true;
            }
            paint_help();
        }
        if (tolower(key) == 'r') {
            generateStartingConditions();
        }
        if (key == '\t' && !pause && !death) {
            pause = true;
        }

        if (!pause && !death) {
            if (key == KEY_UP || key == KEY_DOWN || key == KEY_LEFT || key == KEY_RIGHT) {
                switch (key) {
                    case KEY_UP:
                        new_direction = UP;
                        break;
                    case KEY_DOWN:
                        new_direction = DOWN;
                        break;
                    case KEY_RIGHT:
                        new_direction = RIGHT;
                        break;
                    case KEY_LEFT:
                        new_direction = LEFT;
                        break;
                }
            }
            return true;
        }

        return CFramedWindow::handleEvent(key);
    }


    void paint() override {
        CFramedWindow::paint();
        if (help) {
            paint_help();
        } else if (pause) {
            paint_pause();
        } else if (death) {
            paint_dead();
        } else {
            paint_regular();
            if (death)
                paint_dead();
        }

    }
};

#endif
