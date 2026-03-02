#include "base.h"
#include <math.h>

#define screenWidth 1920
#define screenHeight 1080
#define brickRows 8
#define brickCols 50

void resetBall(Vector2 *ball, float paddleCenter, Vector2 *ball_speed);
void updateRecs(Vector2 *ballPos, Rectangle *ballA, Vector2 *paddlePos, Rectangle *paddleA);
void drawBricks(Texture2D *brickColors, Vector2 brick_positions[brickRows][brickCols]);
void initialize_brick_positions(Vector2 brickPositions[brickRows][brickCols], Vector2 *initialPos, u8 brickOffset, u8 brickHeight , u8 row);
void handleBricksCollision(Vector2 brick_positions[brickRows][brickCols],Rectangle ball, Vector2 *ball_speed, u8 width, u8 height, Sound brick_sound,bool isHorizontal, u16* score, u16 *hits, float *paddleSpeed);
