#include "Engine.h"
#include <stdlib.h>
#include <memory.h>
#include <algorithm>
#include <cmath>

#define BRICK_WIDTH 64
#define BRICK_HEIGTH 10
#define BRICK_COLUMN_NUM (SCREEN_WIDTH / BRICK_WIDTH)
#define BRICK_ROW_NUM 5

#define PLATFORM_WIDTH 10
#define PLATFORM_LENGTH 200

#define BALL_R 6

unsigned int bricks[BRICK_ROW_NUM][BRICK_COLUMN_NUM];
unsigned int brick_count = BRICK_ROW_NUM * BRICK_COLUMN_NUM;

void fill_color(const unsigned int &top, const unsigned int &left, const unsigned int &bottom, 
                const unsigned int &right, const unsigned int &color)
{
  for (unsigned int i = top; i < bottom; ++i)
  {
    for (unsigned int j = left; j < right; ++j)
    {
      buffer[i][j] = color;
    }
  }
}

unsigned int get_platform_position()
{
  unsigned int platform_position = get_cursor_x();
  if (platform_position < PLATFORM_LENGTH / 2)
  {
    platform_position = PLATFORM_LENGTH / 2;
  }
  if (platform_position + PLATFORM_LENGTH / 2 > SCREEN_WIDTH)
  {
    platform_position = SCREEN_WIDTH - PLATFORM_LENGTH / 2;
  }
  return platform_position;
}

class Ball
{
public:
    Ball()
    {
      ballx = get_platform_position();
      bally = SCREEN_HEIGHT - PLATFORM_WIDTH - BALL_R;
      last_ballx = ballx;
      last_bally = bally;
      addx = -1; 
      addy = -1; 
      started = false;
    }
    bool is_started()
    {
      return started;
    }
    void start()
    {
      started = true;
	    ballx = get_platform_position();
    }
    int move();
    void set_color();
    void clear_color();
private:
    int ballx, bally; 
    int last_ballx, last_bally;
    int addx, addy;  

    bool started; 
};

int Ball::move()
{
	last_ballx = ballx;
	last_bally = bally;

	ballx += addx;
	bally += addy;

  if (1 == addx && ballx + BALL_R >= SCREEN_WIDTH)
  {
    ballx = ballx - 2 * (ballx + BALL_R - SCREEN_WIDTH);
    addx *= -1;
  }
  else if (-1 == addx && ballx <= BALL_R)
  {
    ballx = 2 * abs(ballx);
    addx *= -1;
  }

  unsigned platform_postion = get_platform_position();
  if (-1 == addy && bally <= BALL_R)
  {
    bally = 2 * abs(bally);
    addy *= -1;
  }
  else if (1 == addy && bally + BALL_R + PLATFORM_WIDTH >= SCREEN_HEIGHT)
  {
    if (ballx <= platform_postion + PLATFORM_LENGTH / 2 &&
      ballx >= platform_postion - PLATFORM_LENGTH / 2)
    {
      bally = bally - 2 * (bally + BALL_R + PLATFORM_WIDTH - SCREEN_HEIGHT);
      addy *= -1;
    }
    else
    {
      return -1;
    }
  }

  bool hit = false;
  for (unsigned int i = 0; i < BRICK_ROW_NUM && false == hit; ++i)
  {
      for (unsigned int j = 0; j < BRICK_COLUMN_NUM; ++j)
      {
          if (1 == bricks[i][j])
          {
            continue;
          }
          if (ballx + BALL_R >= j * BRICK_WIDTH + 1 && ballx - BALL_R < (j + 1) * BRICK_WIDTH &&
              bally >= i * BRICK_HEIGTH + 1 && bally < (i + 1) * BRICK_HEIGTH)
          {
            addx *= -1;
          }
          else if (bally + BALL_R >= i * BRICK_HEIGTH + 1 && bally - BALL_R < (i + 1) * BRICK_HEIGTH &&
              ballx >= j * BRICK_WIDTH + 1 && ballx < (j + 1) * BRICK_WIDTH)
          {
            addy *= -1;
          }
          else
          {
            continue;
          }
          bricks[i][j] = 1;
          if (0 == --brick_count)
          {
            return 1;
          }
          hit = true;
          break;
      }
  }
  return 0;
}

void Ball::set_color()
{
  unsigned int green = (255 << 8);
  for (unsigned int i = bally - BALL_R; i < bally + BALL_R; ++i)
  {
    for (unsigned int j = ballx - BALL_R; j < ballx + BALL_R; ++j)
    {
      buffer[i][j] = green;
    }
  }
}

void Ball::clear_color()
{
  for (unsigned int i = last_bally - BALL_R; i < last_bally + BALL_R; ++i)
  {
    for (unsigned int j = last_ballx - BALL_R; j < last_ballx + BALL_R; ++j)
    {
      buffer[i][j] = 0;
    }
  }
}

Ball ball;

// initialize game data in this function
void initialize()
{
  memset(bricks, 0, sizeof(bricks)); 
}

// this function is called to update game data,
// dt - time elapsed since the previous update (in seconds)
void act(float dt)
{
  if (is_key_pressed(VK_ESCAPE))
  {
    schedule_quit_game();
  }
  
  if (false == ball.is_started() && is_mouse_button_pressed(0))
  {
    ball.start();
  }

  if (true == ball.is_started())
  {
	  if (0 != ball.move())
	  {
		  schedule_quit_game();
	  }
  }
  
}

// uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH] - is an array of 32-bit colors (8 bits per R, G, B)
void draw()
{
  // clear backbuffer
  memset(buffer, 0, SCREEN_HEIGHT * SCREEN_WIDTH * sizeof(uint32_t));

  unsigned int blue = 255, green = (255 << 8), red = (255 << 16);
  for (unsigned int i = 0; i < BRICK_ROW_NUM; ++i)
  {
    for (unsigned int j = 0; j < BRICK_COLUMN_NUM; ++j)
    {
      if (0 == bricks[i][j])
      {
        fill_color(1 + BRICK_HEIGTH * i, 1 + BRICK_WIDTH * j, BRICK_HEIGTH * (i + 1), BRICK_WIDTH * (j + 1), red);
      }
    }
  }

  unsigned int platform_position = get_platform_position();
  fill_color(SCREEN_HEIGHT - PLATFORM_WIDTH, platform_position - PLATFORM_LENGTH / 2, SCREEN_HEIGHT, platform_position + PLATFORM_LENGTH / 2, blue);
  
  if (false == ball.is_started())
  {
	  fill_color(SCREEN_HEIGHT - PLATFORM_WIDTH - 2 * BALL_R, platform_position - BALL_R, SCREEN_HEIGHT - PLATFORM_WIDTH, platform_position + BALL_R, green);
  }
  else
  {
	  ball.set_color();
  }
}

// free game data in this function
void finalize()
{
}

