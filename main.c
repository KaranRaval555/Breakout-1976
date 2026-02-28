#include "base.h"
#include <raylib.h>

int main() {
  int score = 0;
  int lives = 3;
  int hits = 0;
  const int paddleScale = 5;
  const int ballScale = 5;
  float paddleSpeed = 800;
  bool isHorizontal;
  bool isPlaying = true;
  Vector2 brick_positions[brickRows][brickCols];

  InitWindow(screenWidth, screenHeight, "Breakout Game");
  InitAudioDevice();
  SetTargetFPS(60);

  const Texture2D background =  LoadTexture("./assets/background.png");
  const Texture2D ball =  LoadTexture("./assets/ball.png");
  const Texture2D paddle =  LoadTexture("./assets/paddle.png");
  const Font fontStyle =  LoadFont("./assets/rainyhearts.ttf");
  const Texture2D blue_brick = LoadTexture("./assets/blue_brick.png");
  const Texture2D green_brick = LoadTexture("./assets/green_brick.png");
  const Texture2D yello_brick = LoadTexture("./assets/yellow_brick.png");
  const Texture2D red_brick = LoadTexture("./assets/red_brick.png");
  const Sound lifeLost = LoadSound("./sounds/lifeLost.wav");
  const Sound batHit = LoadSound("./sounds/BatHit.mp3");
  const Sound wall = LoadSound("./sounds/WallHit.mp3");
  const Sound brick_sound = LoadSound("./sounds/brick.wav");



  Texture2D brickColors[8] = {blue_brick, blue_brick,green_brick, green_brick,yello_brick, yello_brick, red_brick, red_brick};

  Rectangle lineRec = {
    0.0f,
    0.0f,
    (float)screenWidth,
    (float)screenHeight + 10
  };

  Vector2 score_pos = {50.0f, 30.0f};
  Vector2 lives_pos = {screenWidth - 100.0f, 30.0f};
  Vector2 paddle_pos = {screenWidth / 2.0f - paddle.width * 2.0f, screenHeight - 100.0f};
  Vector2 ball_pos = {screenWidth / 2.0f - paddle.width * 2.0f + 70, screenHeight - 130.0f};
  Vector2 ball_speed = {200.0f, -600.0f};
  Rectangle ballArea = {ball_pos.x, ball_pos.y, ball.width * ballScale, ball.height * ballScale};
  Rectangle paddleArea = {paddle_pos.x, paddle_pos.y, paddle.width * paddleScale, paddle.height * paddleScale};
  Vector2 initialBrickPos = {120.0f, 100.0f};


  const int brickOffset = blue_brick.width * 2 + 4;

  for (int i = 0; i < brickRows; i++) {
    initialize_brick_positions(brick_positions, &initialBrickPos, brickOffset, blue_brick.width, i);
  }

  SetSoundVolume(brick_sound, 0.5);

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLACK);
    if (isPlaying) {
      DrawRectangleLinesEx(lineRec, 10, WHITE);
      DrawTextEx(fontStyle,TextFormat("%d", score),  score_pos, 90, 0, WHITE);
      DrawTextEx(fontStyle,TextFormat("%d", lives),  lives_pos, 90, 0, WHITE);
      DrawTextureEx(paddle, paddle_pos, 0,paddleScale, WHITE);
      DrawTextureEx(ball, ball_pos, 0, ballScale, WHITE);

      drawBricks(brickColors, brick_positions);

      const float dt = GetFrameTime();
      isHorizontal = true;
      ball_pos.x += ball_speed.x * dt;
      updateRecs(&ball_pos,&ballArea, &paddle_pos, &paddleArea);
      handleBricksCollision(brick_positions, ballArea, &ball_speed, blue_brick.width, blue_brick.height, brick_sound, isHorizontal, &score, &hits, &paddleSpeed);

      isHorizontal = false;
      ball_pos.y += ball_speed.y * dt;
      updateRecs(&ball_pos,&ballArea, &paddle_pos, &paddleArea);
      handleBricksCollision(brick_positions, ballArea, &ball_speed, blue_brick.width, blue_brick.height, brick_sound, isHorizontal, &score, &hits, &paddleSpeed);

      if (CheckCollisionRecs(ballArea, paddleArea)) {
        PlaySound(batHit);

        float paddleCenter = paddleArea.x + paddleArea.width / 2.0f;

        // Normalize hit position (-1 left, 0 center, 1 right)
        float hitPoint = (ball_pos.x + ballArea.width / 2.0f - paddleCenter) 
          / (paddleArea.width / 2.0f);

        // Clamp just in case
        if (hitPoint < -1.0f) hitPoint = -1.0f;
        if (hitPoint >  1.0f) hitPoint =  1.0f;

        float maxAngle = 75.0f * DEG2RAD;
        float newAngle = hitPoint * maxAngle;

        float speed = sqrtf(ball_speed.x * ball_speed.x +
            ball_speed.y * ball_speed.y);

        // generate new direction vector
        ball_speed.x = speed * sinf(newAngle);
        ball_speed.y = -speed * cosf(newAngle);  // negative because ball goes upward
        ball_pos.y = paddleArea.y - ballArea.height;
      }


      if(ball_pos.y >= screenHeight) {
        lives--;
        PlaySound(lifeLost);
        if(lives == 0) {
          isPlaying = false;
        }
        float paddleCenter = paddleArea.x + paddleArea.width / 2.0f;
        paddleSpeed = 800;
        resetBall(&ball_pos, paddleCenter, &ball_speed);
        updateRecs(&ball_pos,&ballArea, &paddle_pos, &paddleArea);
      }

      if(ball_pos.y <= 0) {
        ball_speed.y *= -1.0f;
        PlaySound(wall);
      }

      if(ball_pos.x <= 0 || ball_pos.x + ballArea.width >= screenWidth) {
        ball_speed.x *= -1.0f;
        PlaySound(wall);
      }

      if(IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_H) || IsKeyDown(KEY_A)) {
        if (paddle_pos.x - 17 >= 0) {
          paddle_pos.x -= paddleSpeed * GetFrameTime();
          paddleArea.x = paddle_pos.x;
        }
      }

      if(IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_L) || IsKeyDown(KEY_D)) {
        if (paddle_pos.x + 180 <= screenWidth) {
          paddle_pos.x += paddleSpeed * GetFrameTime();
          paddleArea.x = paddle_pos.x;
        }
      }

    } else {
      const char* txt = "GAME OVER";
      const int fontSize = 100;
      ball_speed.x = 200.0f;
      ball_speed.y = -600.0f;
      paddleSpeed = 800;
      lives = 3;
      score = 0;
      DrawText(txt, screenWidth / 2 - MeasureText(txt, fontSize) / 2, screenHeight / 2 - fontSize / 2, fontSize, RED);
    }
    if(IsKeyPressed(KEY_SPACE)) {
      isPlaying = true;
    }
    EndDrawing();
  }

  UnloadTexture(background);
  UnloadTexture(ball);
  UnloadTexture(paddle);
  UnloadTexture(blue_brick);
  UnloadTexture(green_brick);
  UnloadTexture(yello_brick);
  UnloadTexture(red_brick);
  CloseWindow();
  return 0;
}

void resetBall(Vector2 *ball, float paddleCenter, Vector2 *ball_speed){
  ball->x = paddleCenter;
  ball->y = screenHeight / 2.0f;
  ball_speed->y = -600;
  ball_speed->x = 0.0f;
}

void updateRecs(Vector2 *ballPos, Rectangle *ballA, Vector2 *paddlePos, Rectangle *paddleA) {
  ballA->x = ballPos->x;
  ballA->y = ballPos->y;
  paddleA->x = paddlePos->x;
  paddleA->y = paddlePos->y;
}

void drawBricks(Texture2D *brickColors, Vector2 brick_positions[brickRows][brickCols]) {
  for (int row = 0; row < brickRows; row++) {
    for (int col = 0; col < brickCols; col++) {
      DrawTextureEx(brickColors[row], brick_positions[row][col], 0, 2, WHITE);
    }
  }
}

void initialize_brick_positions(Vector2 brickPositions[brickRows][brickCols], Vector2 *initialPos, int brickOffset, int brickHeight , int row) {
  int rowGap = brickOffset;
  for (int col = 0; col < brickCols; col++) {
    Vector2 pos = {initialPos->x + rowGap, initialPos->y};
    rowGap += brickOffset;
    brickPositions[row][col] = pos;
  }
  initialPos->y = initialPos->y + brickHeight;
}

void handleBricksCollision(Vector2 brick_positions[brickRows][brickCols],Rectangle ball, Vector2 *ball_speed, int width, int height, Sound brick_sound,bool isHorizontal, int* score, int *hits, float *paddleSpeed){
  for (int row = 0; row < brickRows; row++) {
    for (int col = 0; col < brickCols; col++) {
      Rectangle temp = {brick_positions[row][col].x, brick_positions[row][col].y, width, height};
      if(CheckCollisionRecs(temp, ball)) {
        PlaySound(brick_sound);
        brick_positions[row][col] = (Vector2){-100,-100};
        *hits += 1;

        if(*hits == 4 || *hits == 12 || (row >= 0 && row < 4)) {
          ball_speed->x *= 1.05f;
          ball_speed->y *= 1.05f;
          *paddleSpeed *= 1.05f;
        }
        if(row < 2) {
          *score += 7;
        }
        else if(row == 2 || row == 3) {
          *score += 5;
        }
        else if(row == 4 || row == 5) {
          *score += 3;
        }
        else {
          *score += 1;
        }
        if(isHorizontal) {
          ball_speed->x *= -1.0f;
        }
        else {
          ball_speed->y *= -1.0f;
        }
        return;
      }
    }
  }
}
