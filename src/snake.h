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
        PointStack * SnakeBody;
        CPoint Head;

        enum direction_t new_direction;
        enum direction_t direction;

        bool pause;
        bool help;
        bool death;

        int level;
        int speed;
        
        CPoint foodPoint;
    
        void generateStartingConditions() {
            help = true;
            pause = true;
            SnakeBody = new PointStack;
            Head = CPoint(rand() % (geom.size.x - 5) + 5, rand() % (geom.size.y - 5) + 5);
            *SnakeBody += CPoint(Head.x - 1, Head.y);
            *SnakeBody += CPoint(Head.x - 2, Head.y);
            level = 1;
            speed = 1000;
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
                if (speed > 100)
                    speed -= 50;
                return true;
            } else {
                return false;
            }
        }

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
                    return false;
                }
            }

            if (!justAte()) {
                (*SnakeBody).pop();
            } 

            return true;
        }

        void paint_regular() {
            if (!move()) {
                death = true;                
            }

            gotoyx(geom.topleft.y, geom.topleft.x);
            printl("LEVEL %d", level);

            gotoyx(Head.y, Head.x);
            printc(HEAD);

            for (int i = 0; i < (*SnakeBody).getSize(); i++) {
                gotoyx((*SnakeBody)[i].y, (*SnakeBody)[i].x);
                printc(BODY);
            }

            gotoyx(foodPoint.y, foodPoint.x);
            printc(FOOD);
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
            printl("h - shows this message");
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
        CSnake(CRect r, char _c = ' ') : CFramedWindow(r, _c) {
            srand(time(NULL));
            generateStartingConditions();
        };

        bool handleEvent(int key) {
            if (tolower(key) == 'p') {
                pause = !pause;
                help = false;
            }
            if (tolower(key) == 'h') {
                if (help) {
                    help = false;
                    pause = false;
                } else {
                    help = true;
                    pause = true;
                }
            }
            if (tolower(key) == 'r') {
                generateStartingConditions();
            }
            if (key == '\t') {
                pause = true;
            }

            if (!pause && !death) {
                if (key == KEY_UP || key == KEY_DOWN || key == KEY_LEFT || key == KEY_RIGHT) {
                    switch (key) {
                        case KEY_UP:
                            new_direction = UP;
                        case KEY_DOWN:
                            new_direction = DOWN;
                        case KEY_RIGHT:
                            new_direction = RIGHT;
                        case KEY_LEFT:
                            new_direction = LEFT;
                    }
                }
                return true;
            }

            return CFramedWindow::handleEvent(key);
        }


        void paint() {
            if (help) {
                paint_help();               
            } else if (pause) {
                paint_pause();
            } else if (death) {
                paint_dead();
            } else {
                paint_regular();
            }
    
        }
};

#endif
