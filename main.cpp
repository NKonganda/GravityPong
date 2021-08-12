#define SDL_MAIN_HANDLED
#include <ctime>
#include <iostream>
#include <sstream>
#include <SDL2/SDL.h>
#include <cmath>
#include <SDL2/SDL_ttf.h>
#include <limits.h>
#include "draw_utils.h"

SDL_Renderer* renderer;
TTF_Font* consola;
int mouseX;
int mouseY;
int WindowWidth = 500;
int maxScore = 10;
int SDL_RenderFillCircle(SDL_Renderer*, int, int, int);
float delta = 0;


class Paddle {
public:
    SDL_Rect rect;
    int player;
    float y;
    Paddle (int x, int y, int w, int h) {
        this->y = y;
        rect.x = x;
        rect.y = y;
        rect.w = w;
        rect.h = h;

        static int player = 1;
        this->player = player;
        player++;
    }

void draw(SDL_Renderer* renderer) {
        SDL_SetRenderDrawColor(renderer,200,0,0,255);
        SDL_RenderFillRect(renderer, &rect);
        SDL_SetRenderDrawColor(renderer, 0,0,255,255);
        SDL_RenderDrawRect(renderer,&rect);
        std::stringstream ss;
        ss << "P" << player;
        renderText(ss.str().c_str(), rect.x, rect.y, consola, renderer);
    }
    void update(int mousex, int mouseY) {
        rect.y = mouseY - rect.h /2;
    }
    void update(bool up, bool down) {
        if(up) {
            y -= 1;
        }
        if(down) {
            y += 1;
        }
        if (y < 0) {
            y = 0;
        }
        if (y  > WindowWidth - rect.h) {
            y = WindowWidth - rect.h;
        }
        rect.y = y;
        
    }
};

class Ball {
    public:
    int totalSpeed;
    float x;
    float y;
    float xSpeed;
    float ySpeed;
    float size;
    int startSpeed;

    Ball() {
        totalSpeed = 250;
        x = 250;
        y = 250;
        xSpeed = 150;
        ySpeed = 150;
        size = 15;
        startSpeed = 250;
    }

    virtual void draw(SDL_Renderer* renderer) {
        SDL_SetRenderDrawColor(renderer,200,0,0,255);
        SDL_RenderFillCircle(renderer,x,y,size);
    }

    virtual void update(float delta) {
        x += xSpeed * delta;
        y += ySpeed * delta;

        // if(x > WindowWidth) {
        //     x = WindowWidth;
        //     xSpeed = -xSpeed;
        // }
        // else if(x<0) {
        //     x = 0;
        //     xSpeed = -xSpeed;
        // }

        if(y > WindowWidth) {
            y = WindowWidth;
            ySpeed = -ySpeed;
        }
        else if(y<0) {
            y = 0;
            ySpeed = -ySpeed;
        }

    }
    void checkPaddleCollision(Paddle* p) {
        int leftBallSide = x - size;
        int rightBallSide = x + size;
        int topBallSide = y - size;
        int bottomBallSide = y + size;

        int leftPaddleSide = p->rect.x;
        int rightPaddleSide = p->rect.x + p->rect.w;
        int topPaddleSide = p->rect.y;
        int bottomPaddleSide = p->rect.y + p->rect.h;

        if(leftBallSide < rightPaddleSide && rightBallSide > leftPaddleSide && bottomBallSide > topPaddleSide && topBallSide < bottomPaddleSide) {
            float newX = x - (p->rect.x + p->rect.w / 2);
            float newY = y - (p->rect.y + p->rect.h / 2);
            float c = sqrt(newX * newX + newY * newY);
            newX /= c;
            newY /= c;

            xSpeed = newX * totalSpeed;
            ySpeed = newY * totalSpeed;
            printf("%i\n", totalSpeed);
            // if (totalSpeed < 1000) {
            //     totalSpeed += 30;
            // }

        }
    }
    int reset() {
        x = WindowWidth / 2;
        y = WindowWidth / 2;
        totalSpeed = startSpeed;
        xSpeed = (rand() % startSpeed) + totalSpeed;
        if (rand() % 2 == 0) {
            xSpeed = -xSpeed;
        }
        ySpeed = startSpeed - xSpeed;
        if (rand() % 2 == 0) {
            ySpeed = -ySpeed;
        }
    }
};

class GravityBall : public Ball {
public:
    void draw(SDL_Renderer* renderer) {
        SDL_SetRenderDrawColor(renderer,0,0,255,255);
        SDL_RenderFillCircle(renderer,x,y,size);
    }

    void update(float delta) {
        ySpeed += 1;
        x += xSpeed * delta;
        y += ySpeed * delta;

    if (y > WindowWidth) {
            y = WindowWidth;
            ySpeed = -ySpeed * 1.1;
            xSpeed *- 1.5;
        }
    else if (y<0) {
            y = 0;
            ySpeed = -ySpeed;
        }
    }
};



int main() {
    srand(time(0));
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("Window", 100, 100, WindowWidth, WindowWidth, SDL_WINDOW_RESIZABLE);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

    TTF_Init();
    consola = TTF_OpenFont("consola.ttf", 24);

    Paddle paddle1(10, 100, 30, 120);
    Paddle paddle2(460, 100, 30, 120);

    Ball* ball;
    Ball rBall;
    rBall.reset();
    GravityBall gBall;
    gBall.reset();


    clock_t start;
    clock_t end;

    std::stringstream p1ScoreText;
    std::stringstream p2ScoreText;
    int p1Score = 0;
    int p2Score = 0;
    bool gameStarted = false;
    start = clock();
    while(!SDL_QuitRequested()) {

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch(event.type) {
                case SDL_KEYDOWN: {
                    if(event.key.keysym.scancode == SDL_SCANCODE_BACKSPACE) {
                        if(textInput.length() > 0) {
                            textInput.pop_back();
                        }
                    }
                    else {
                        char c = event.key.keysym.sym;
                        if(c >= '0' && c <= '9') {
                            textInput += c;
                        }
                    }
                    break;
                }
                case SDL_KEYUP: {
                    break;
                }
                default: {
                    break;
                }
            }
        }
        SDL_GetMouseState(&mouseX,&mouseY);
        const unsigned char* keyState = SDL_GetKeyboardState(0);

        bool up;
        bool down;
        if (keyState[SDL_SCANCODE_W]) {
            up = true;
        }
        else {
            up = false;
        }
        if (keyState[SDL_SCANCODE_S]) {
            down = true;
        }
        else {
            down = false;
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        
        if (!gameStarted) {
            std::string result = renderTextInput(renderer, consola, 200, 350, 3, 1);
            renderText("Pong", 125, 100, consola, renderer, 0, 0, 0, 5);
            renderText("mph", 300, 360, consola, renderer, 0, 0, 0, 2);
            if (renderButton(renderer, "Regular", consola, 100,275, mouseX, mouseY)) {
                ball = &rBall;
                gameStarted = true;
                result = textInput;
            }

            else if (renderButton(renderer, "Gravity", consola, 300,275, mouseX, mouseY)) {
                ball = &gBall;
                gameStarted = true;
                result = textInput;
            }
            if (result.length() > 0) {
                int resInt = std::stoi(result.c_str(), 0);
                resInt *= 5;
                rBall.startSpeed = resInt;
                rBall.totalSpeed = resInt;
                rBall.xSpeed = resInt;
                rBall.ySpeed = resInt;
                gBall.startSpeed = resInt;
                gBall.totalSpeed = resInt;
                gBall.xSpeed = resInt;
                gBall.ySpeed = resInt;
                std::cout << result << '\n';
            }
        }
        else {
            if (p1Score < maxScore && p2Score < maxScore) {
                ball->checkPaddleCollision(&paddle1);
                ball->checkPaddleCollision(&paddle2);
                paddle1.update(mouseX,mouseY);
                paddle2.update(up,down);

                if(ball->x < 0) {
                    p2Score++;
                    ball->reset();

                }
                if(ball->x > WindowWidth) {
                    p1Score++;
                    ball->reset();

                }


                ball->update(delta);
                paddle1.draw(renderer);
                paddle2.draw(renderer);
                ball->draw(renderer);

                p1ScoreText.str("");
                p2ScoreText.str("");
                p1ScoreText << "P1: " << p1Score;
                p2ScoreText << "P2: " << p2Score;
                renderText(p1ScoreText.str().c_str(),50,50,consola,renderer,2);
                renderText(p2ScoreText.str().c_str(),300,50,consola,renderer,2);

            }
            else {
                if (p1Score >= maxScore) {
                    renderText("Player 1 is the winner!", 100, 150, consola, renderer,1);
                }
                else {
                    renderText("Player 2 is the winner!", 100, 150, consola, renderer, 1);
                }
                renderText("Press ENTER key to restart", 100, 300, consola, renderer, 1);

                if (keyState[SDL_SCANCODE_RETURN]) {
                p1Score = 0;
                p2Score = 0;
                ball->reset();

                }
            }
        }
        

        SDL_RenderPresent(renderer);

        end = clock();
        delta = (float)(end -start) / CLOCKS_PER_SEC;
        start = end;



    }
    return 0;
}

