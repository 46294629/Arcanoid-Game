#pragma once

#include <stdint.h>

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

// backbuffer
extern uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH];

#ifndef VK_ESCAPE
#  define VK_ESCAPE 0x1B
#  define VK_SPACE  0x20
#  define VK_LEFT   0x25
#  define VK_UP     0x26
#  define VK_RIGHT  0x27
#  define VK_DOWN   0x28
#  define VK_RETURN 0x0D
#endif

// VK_SPACE, VK_RIGHT, VK_LEFT, VK_UP, VK_DOWN, 'A', 'B' ...
bool is_key_pressed(int button_vk_code);

// check if mouse button is pressed (0 - left button, 1 - right button)
bool is_mouse_button_pressed(int button);

// get mouse cursor position
int get_cursor_x();
int get_cursor_y();

// returns true if window is active
bool is_window_active();

// set all pixels in buffer to 'black'
void clear_buffer();

void initialize();
void finalize();

void act(float dt);
void draw();

// quit game after act()
void schedule_quit_game();
