#include <stdio.h>
#include <math.h>
#include <raylib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;


#define screenWidth 1920
#define screenHeight 1080
#define brickRows 8
#define brickCols 50

void resetBall(Vector2 *ball, float paddleCenter, Vector2 *ball_speed);
void updateRecs(Vector2 *ballPos, Rectangle *ballA, Vector2 *paddlePos, Rectangle *paddleA);
void drawBricks(Texture2D *brickColors, Vector2 brick_positions[brickRows][brickCols]);
void initialize_brick_positions(Vector2 brickPositions[brickRows][brickCols], Vector2 *initialPos, int brickOffset, int brickHeight , int row);
void handleBricksCollision(Vector2 brick_positions[brickRows][brickCols],Rectangle ball, Vector2 *ball_speed, int width, int height, Sound brick,bool isHorizontal, int* score, int *hits, float *paddleSpeed);
