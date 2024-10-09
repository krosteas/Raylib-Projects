#include <iostream>
#include "raylib.h"

Color Green = Color{ 38,185,154,255 };
Color Dark_Green = Color{ 20,160,133,255 };
Color Light_Green = Color{ 129,204,184,255 };
Color Yellow = Color{ 243,213,91,255 };

const int screen_width = 1920;
const int screen_height = 1080;
const int rectangle_height = 120;
const int rectangle_width = 25;
int playerScore = 0, cpuScore = 0,player2Score = 0;
bool isInMenu = true;
bool PvP = false;
bool PvE = false;

void CheckWinner() {
    if (PvP) {
        if (playerScore >= 5 || player2Score >= 5)
        {
            PvP = false;
            isInMenu = true;
        }
    }else if (PvE) {
        if (playerScore >= 5|| cpuScore >= 5)
        {
            PvE = false;
            isInMenu = true;
        }
    }
}
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
        if (x + radius >= GetScreenWidth() && PvE)
        {
            cpuScore++;
            resetBall();
        }
        if (x + radius >= GetScreenWidth() && PvP)
        {
            player2Score++;
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
        DrawRectangleRounded(Rectangle{ x, y, width, height }, 0.8, 0, WHITE);
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
class Player2Paddle : public Paddle {
public:
    void UpdateP2() {

        if (IsKeyDown(KEY_W) && y > 10)
        {
            y -= speed;
        }
        if (IsKeyDown(KEY_S) && y + height < GetScreenHeight() - 10)
        {
            y += speed;
        }
    }
};

Ball ball;
Paddle player;
CpuPaddle cpu;
Player2Paddle player2;

int main() {
    std::cout << "Starting the Game" << std::endl;

    InitWindow(screen_width, screen_height, "Pong Game");
    SetTargetFPS(60);

    ball.radius = 20;
    ball.x = screen_width / 2;
    ball.y = screen_height / 2;
    ball.speed_x = 7;
    ball.speed_y = 7;

    player.height = rectangle_height;
    player.width = rectangle_width;
    player.x = screen_width - 35;
    player.y = screen_height / 2 - player.height / 2;
    player.speed = 6;

    player2.height = rectangle_height;
    player2.width = rectangle_width;
    player2.x = 10;
    player2.y = screen_height / 2 - player2.height / 2;
    player2.speed = 6;

    cpu.height = rectangle_height;
    cpu.width = rectangle_width;
    cpu.x = 10;
    cpu.y = screen_height / 2 - cpu.height / 2;
    cpu.speed = 6;

    while (WindowShouldClose() == false)
    {
        BeginDrawing();

        if (isInMenu)
        {
            ClearBackground(Green);
            DrawText("PONG THE GAME", screen_width / 2 - MeasureText("PONG THE GAME", 80) / 2, screen_height / 4 - 20, 80, WHITE);
            DrawText("1.PLAY PVP", screen_width / 2 - MeasureText("1.PLAY PVE", 80) / 2, screen_height / 2 , 80, WHITE);
            DrawText("2.PLAY PVE", screen_width / 2 - MeasureText("1. PLAY PVE", 80) / 2, screen_height / 2 + 80, 80, WHITE);

            if (IsKeyDown(KEY_ONE))
            {
                PvP = true;
                isInMenu = false;
            }
            if (IsKeyDown(KEY_TWO))
            {
                PvE = true;
                isInMenu = false;
            }
        }
        if (PvP)
        {
            ball.Update();
            player.Update();
            player2.UpdateP2();
            CheckWinner();

            if (CheckCollisionCircleRec(Vector2{ ball.x,ball.y }, ball.radius, Rectangle{ player.x,player.y,player.width,player.height }))
            {
                ball.speed_x *= -1;
            }
            if (CheckCollisionCircleRec(Vector2{ ball.x,ball.y }, ball.radius, Rectangle{ player2.x,player2.y,player2.width,player2.height }))
            {
                ball.speed_x *= -1;
            }
            ClearBackground(Dark_Green);
            DrawRectangle(screen_width / 2, 0, screen_width / 2, screen_height, Green);
            DrawCircle(screen_width / 2, screen_height / 2, 150, Light_Green);
            DrawLine(screen_width / 2, 0, screen_width / 2, screen_height, WHITE);
            ball.Draw();
            player.Draw();
            player2.Draw();
            DrawText(TextFormat("%i", player2Score), screen_width / 4 - 20, 20, 80, WHITE);
            DrawText(TextFormat("%i", playerScore), 3 * screen_width / 4 - 20, 20, 80, WHITE);
        }
        if (PvE)
        {
            ball.Update();
            player.Update();
            cpu.UpdateCpu(ball.y);
            CheckWinner();
            if (CheckCollisionCircleRec(Vector2{ ball.x,ball.y }, ball.radius, Rectangle{ player.x,player.y,player.width,player.height }))
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
            DrawText(TextFormat("%i", playerScore), 3 * screen_width / 4 - 20, 20, 80, WHITE);
        }
        
        EndDrawing();
    }

    CloseWindow();
    return 0;
}