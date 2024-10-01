#include <iostream>
#include "raylib.h"

Color Green = Color{ 38,185,154,255 };
Color Dark_Green = Color{ 20,160,133,255 };
Color Light_Green = Color{ 129,204,184,255 };
Color Yellow = Color{ 243,213,91,255 };

int playerScore = 0, cpuScore = 0;

class Ball {
public:
    float x, y;
    int speed_x, speed_y;
    int radius;

    void Draw() {
        DrawCircle(x, y, radius, Yellow);
    }
    void Update() {
        x += speed_x;
        y += speed_y;
        if (y - radius <= 0 || y + radius >= GetScreenHeight())
        {
            speed_y *= -1;
        }
        if (x - radius <= 0)
        {
            playerScore++;
            resetBall();
        }    
        if(x + radius >= GetScreenWidth())
        {
            cpuScore++;
            resetBall();
        }
    }
    void resetBall() {
        x = GetScreenWidth() / 2;
        y = GetScreenHeight() / 2;

        int speedchoices[2] = { -1,1 };
        speed_x *= speedchoices[GetRandomValue(0, 1)];
        speed_y *= speedchoices[GetRandomValue(0, 1)];
    }
};

class Paddle {

public:
    float x, y;
    float width, height;
    int speed;
    
    void Draw() {
        DrawRectangleRounded(Rectangle{ x, y, width, height }, 0.8,0 ,WHITE);
}
    void Update() {

        if (IsKeyDown(KEY_UP) && y > 10)
        {
            y -= speed;
        }        
        if (IsKeyDown(KEY_DOWN) && y + height < GetScreenHeight() - 10)
        {
            y += speed;
        }
    }
};

class CpuPaddle : public Paddle {
public:

    void UpdateCpu(int ball_y) {

        if (y + height / 2 > ball_y && y > 10) {
            y -= speed;
        }
        if (y + height / 2 <= ball_y && y + height < GetScreenHeight() - 10) {
            y += speed;
        }

    }
};

Ball ball;
Paddle player;
CpuPaddle cpu;

int main() {
    std::cout << "Starting the Game" << std::endl;
    const int screen_width = 1920;
    const int screen_height = 1080;
    const int rectangle_height = 120;
    const int rectangle_width = 25;
    InitWindow(screen_width, screen_height, "Pong Game"); 
    SetTargetFPS(60);
    
    ball.radius = 20;
    ball.x = screen_width / 2;
    ball.y = screen_height / 2;
    ball.speed_x = 7;
    ball.speed_y = 7; 

    player.height = 120;
    player.width = 25;
    player.x = screen_width - 35;
    player.y = screen_height / 2 - player.height / 2;
    player.speed = 6;

    cpu.height = 120;
    cpu.width = 25;
    cpu.x = 10;
    cpu.y = screen_height / 2 - cpu.height / 2;
    cpu.speed = 6;

    while (WindowShouldClose() == false)
    {
        BeginDrawing();

        ball.Update();
        player.Update();
        cpu.UpdateCpu(ball.y);

        if (CheckCollisionCircleRec(Vector2{ball.x,ball.y},ball.radius,Rectangle{player.x,player.y,player.width,player.height }))
        {
            ball.speed_x *= -1;
        }
        if (CheckCollisionCircleRec(Vector2{ ball.x,ball.y }, ball.radius, Rectangle{ cpu.x,cpu.y,cpu.width,cpu.height }))
        {
            ball.speed_x *= -1;
        }
        ClearBackground(Dark_Green);
        DrawRectangle(screen_width / 2, 0, screen_width / 2, screen_height, Green);
        DrawCircle(screen_width / 2, screen_height / 2, 150, Light_Green);
        DrawLine(screen_width / 2, 0, screen_width / 2, screen_height, WHITE);
        ball.Draw();
        player.Draw();
        cpu.Draw();
        DrawText(TextFormat("%i", cpuScore), screen_width / 4 - 20, 20, 80, WHITE);
        DrawText(TextFormat("%i",playerScore),3 * screen_width / 4 - 20, 20, 80, WHITE);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

