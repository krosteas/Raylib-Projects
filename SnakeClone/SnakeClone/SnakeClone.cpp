#include <iostream>
#include "raylib.h"
#include <deque>
#include <raymath.h>

Color retro_green = Color{ 173, 204, 96 ,255 };
Color retro_dark_green = Color{ 43, 51, 24 ,255 };

int cellSize = 30;
int cellCountRows = 25;
int cellCountCols = 25;
double lastUpdateTime = 0;
float offset = 75;

bool ElementInDeque(Vector2 element, std::deque<Vector2> deque ) {

    for (unsigned int i = 0; i < deque.size(); i++) {

        if (Vector2Equals(deque[i],element))
        {
            return true;
        }

    }
    return false;
}
bool eventTriggered(double interval) {
    double currentTime = GetTime();

    if (currentTime - lastUpdateTime >= interval) 
    {
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}
class Snake {
public:
    std::deque<Vector2> body = { Vector2{6,9},Vector2{5,9},Vector2{4,9} };
    Vector2 direction = { 1,0 };
    bool addSegment = false;


    void Draw(){
        for (unsigned int i = 0; i < body.size(); i++)
        {
            float x = body[i].x;
            float y = body[i].y;
            Rectangle segment = Rectangle{ offset + x * cellSize,offset + y * cellSize, (float)cellSize, (float)cellSize };
            DrawRectangleRounded(segment,0.5,6, retro_dark_green);
        }
    }
    void Update() {        
        body.push_front(Vector2Add(body[0], direction));
        if (addSegment == true)
        {
            addSegment = false;
        }
        else 
        {
            body.pop_back();
            
        }
    }
    void reset() {
        body = { Vector2{ 6,9 }, Vector2{ 5,9 }, Vector2{ 4,9 } };
        direction = { 1,0 };
    }
};
class Food {

public:
    Vector2 position = {5,6};
    Texture2D texture;

    Food(std::deque<Vector2>SnakeBody) {
        Image image = LoadImage("Graphics/food.png");
        texture = LoadTextureFromImage(image);
        position = generateRandomPos(SnakeBody);
        UnloadImage(image);
    }
    ~Food() {
        UnloadTexture(texture);
    }
    void Draw() {
        DrawTexture(texture, offset + position.x * cellSize, offset + position.y * cellSize,WHITE);
    }
    Vector2 generateRandomCell() {
        float x = GetRandomValue(0, cellCountRows - 1);
        float y = GetRandomValue(0, cellCountCols - 1);

        return Vector2{ x,y };
    }
    Vector2 generateRandomPos(std::deque<Vector2>SnakeBody) {
        Vector2 position = generateRandomCell();
        while (ElementInDeque(position,SnakeBody))
        {
            position = generateRandomCell();
        }
        return position;
    }

};
class Game {
public:
    Snake snake = Snake();
    Food food = Food(snake.body);
    bool running = true;
    int score = 0;
    Sound eatSound;
    Sound wallSound;
    Music music;
    
    Game() {
        InitAudioDevice();
        eatSound = LoadSound("Sounds/eat.mp3");
        wallSound = LoadSound("Sounds/wall.mp3");
        music = LoadMusicStream("Sounds/Music.mp3");
    }
    ~Game() {
        UnloadSound(eatSound);
        UnloadSound(wallSound);
        UnloadMusicStream(music);
    }
    void Draw() {
        food.Draw();
        snake.Draw();
    }

    void Update(){
        if (running)
        {
            snake.Update();
            CheckCollisionWithFood(snake.body);
            CheckCollisionWithEdges();
            CheckcollisionWithItself();
        }
    }
    void CheckCollisionWithFood(std::deque<Vector2>SnakeBody) {
        if (Vector2Equals(snake.body[0], food.position))
        {
            food.position = food.generateRandomPos(SnakeBody);
            snake.addSegment = true;
            score++;
            PlaySound(eatSound);
        }

    }
    void CheckCollisionWithEdges() {
        if(snake.body[0].x == cellCountRows || snake.body[0].x == -1 )
        {
            GameOver();
        }
        if (snake.body[0].y == cellCountCols || snake.body[0].y == -1)
        {
            GameOver();
        }
    }
    void CheckcollisionWithItself() {
        std::deque<Vector2> headlessBody = snake.body;
        headlessBody.pop_front();

        if (ElementInDeque(snake.body[0],headlessBody))
        {
            GameOver();
        }
    }
    void GameOver() {
        snake.reset();
        food.position = food.generateRandomPos(snake.body);
        running = false;
        score = 0;
        PlaySound(wallSound);
    }
};

int main()
{
    int screen_width = 1920;
    int screen_height = 1080;
    std::cout << "Game is Starting..." << std::endl;
    InitWindow(2*offset + cellSize * cellCountRows, 2 * offset + cellSize * cellCountCols, "Snake Clone");
    SetTargetFPS(60);
    Game game = Game();
    SetMusicVolume(game.music, 0.5);
    PlayMusicStream(game.music);

    while (WindowShouldClose() == false)
    {
        BeginDrawing();
        UpdateMusicStream(game.music);

        if (eventTriggered(0.2) == true)
        {
            game.Update();
        }
        
        if (IsKeyPressed(KEY_UP) && game.snake.direction.y != 1)
        {
            game.snake.direction = { 0,-1 };
            game.running = true;
        }
        if (IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1)
        {
            game.snake.direction = { 0,1 };
            game.running = true;
        }
        if (IsKeyPressed(KEY_LEFT) && game.snake.direction.x != 1)
        {
            game.snake.direction = { -1,0 };
            game.running = true;
        }
        if (IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1)
        {
            game.snake.direction = { 1,0 };
            game.running = true;
        }
        ClearBackground(retro_green);
        DrawRectangleLinesEx(Rectangle{ offset,offset,(float)cellSize * cellCountRows,(float)cellSize * cellCountRows}, 2, retro_dark_green);
        DrawText("Retro Snake", offset, 20, 40, retro_dark_green);
        DrawText(TextFormat("Score: %i",game.score), cellSize * cellCountRows - offset - 20, 20, 40, retro_dark_green);
        game.Draw();
        EndDrawing();
    }
    CloseWindow();
}


