#ifndef SPRITZ_WINDOW_H
#define SPRITZ_WINDOW_H

#include "spritz/renderer.h"
#include <stdbool.h>
#include <stdint.h>

#include <spritz/graphics_api.h>

typedef struct SpritzWindow* SpritzWindow_t;

SpritzWindow_t spritzWindowCreate(int32_t width, int32_t height,
                                  const char* name,
                                  SpritzGraphicsAPIID_t apiPreference);

void spritzWindowDestroy(SpritzWindow_t* window);

bool spritzWindowShouldClose(SpritzWindow_t window);

bool spritzWindowIsKeyPressed(SpritzWindow_t window, int32_t key);

bool spritzWindowClear(SpritzWindow_t window);
bool spritzWindowSetClearColor(SpritzWindow_t window, float r, float g, float b,
                               float a);

void spritzUpdateWindows(void);

void spritzWindowSwapBuffers(SpritzWindow_t window);


bool spritzQueueQuad(SpritzWindow_t window, SpritzRendererQuadInfo_t quadInfo);

bool spritzFlush(SpritzWindow_t window);

#define SPRITZ_KEY_UNKNOWN -1

/* Printable keys */
#define SPRITZ_KEY_SPACE 32
#define SPRITZ_KEY_APOSTROPHE 39 /* ' */
#define SPRITZ_KEY_COMMA 44      /* , */
#define SPRITZ_KEY_MINUS 45      /* - */
#define SPRITZ_KEY_PERIOD 46     /* . */
#define SPRITZ_KEY_SLASH 47      /* / */
#define SPRITZ_KEY_0 48
#define SPRITZ_KEY_1 49
#define SPRITZ_KEY_2 50
#define SPRITZ_KEY_3 51
#define SPRITZ_KEY_4 52
#define SPRITZ_KEY_5 53
#define SPRITZ_KEY_6 54
#define SPRITZ_KEY_7 55
#define SPRITZ_KEY_8 56
#define SPRITZ_KEY_9 57
#define SPRITZ_KEY_SEMICOLON 59 /* ; */
#define SPRITZ_KEY_EQUAL 61     /* = */
#define SPRITZ_KEY_A 65
#define SPRITZ_KEY_B 66
#define SPRITZ_KEY_C 67
#define SPRITZ_KEY_D 68
#define SPRITZ_KEY_E 69
#define SPRITZ_KEY_F 70
#define SPRITZ_KEY_G 71
#define SPRITZ_KEY_H 72
#define SPRITZ_KEY_I 73
#define SPRITZ_KEY_J 74
#define SPRITZ_KEY_K 75
#define SPRITZ_KEY_L 76
#define SPRITZ_KEY_M 77
#define SPRITZ_KEY_N 78
#define SPRITZ_KEY_O 79
#define SPRITZ_KEY_P 80
#define SPRITZ_KEY_Q 81
#define SPRITZ_KEY_R 82
#define SPRITZ_KEY_S 83
#define SPRITZ_KEY_T 84
#define SPRITZ_KEY_U 85
#define SPRITZ_KEY_V 86
#define SPRITZ_KEY_W 87
#define SPRITZ_KEY_X 88
#define SPRITZ_KEY_Y 89
#define SPRITZ_KEY_Z 90
#define SPRITZ_KEY_LEFT_BRACKET 91  /* [ */
#define SPRITZ_KEY_BACKSLASH 92     /* \ */
#define SPRITZ_KEY_RIGHT_BRACKET 93 /* ] */
#define SPRITZ_KEY_GRAVE_ACCENT 96  /* ` */
#define SPRITZ_KEY_WORLD_1 161      /* non-US #1 */
#define SPRITZ_KEY_WORLD_2 162      /* non-US #2 */

/* Function keys */
#define SPRITZ_KEY_ESCAPE 256
#define SPRITZ_KEY_ENTER 257
#define SPRITZ_KEY_TAB 258
#define SPRITZ_KEY_BACKSPACE 259
#define SPRITZ_KEY_INSERT 260
#define SPRITZ_KEY_DELETE 261
#define SPRITZ_KEY_RIGHT 262
#define SPRITZ_KEY_LEFT 263
#define SPRITZ_KEY_DOWN 264
#define SPRITZ_KEY_UP 265
#define SPRITZ_KEY_PAGE_UP 266
#define SPRITZ_KEY_PAGE_DOWN 267
#define SPRITZ_KEY_HOME 268
#define SPRITZ_KEY_END 269
#define SPRITZ_KEY_CAPS_LOCK 280
#define SPRITZ_KEY_SCROLL_LOCK 281
#define SPRITZ_KEY_NUM_LOCK 282
#define SPRITZ_KEY_PRINT_SCREEN 283
#define SPRITZ_KEY_PAUSE 284
#define SPRITZ_KEY_F1 290
#define SPRITZ_KEY_F2 291
#define SPRITZ_KEY_F3 292
#define SPRITZ_KEY_F4 293
#define SPRITZ_KEY_F5 294
#define SPRITZ_KEY_F6 295
#define SPRITZ_KEY_F7 296
#define SPRITZ_KEY_F8 297
#define SPRITZ_KEY_F9 298
#define SPRITZ_KEY_F10 299
#define SPRITZ_KEY_F11 300
#define SPRITZ_KEY_F12 301
#define SPRITZ_KEY_F13 302
#define SPRITZ_KEY_F14 303
#define SPRITZ_KEY_F15 304
#define SPRITZ_KEY_F16 305
#define SPRITZ_KEY_F17 306
#define SPRITZ_KEY_F18 307
#define SPRITZ_KEY_F19 308
#define SPRITZ_KEY_F20 309
#define SPRITZ_KEY_F21 310
#define SPRITZ_KEY_F22 311
#define SPRITZ_KEY_F23 312
#define SPRITZ_KEY_F24 313
#define SPRITZ_KEY_F25 314
#define SPRITZ_KEY_KP_0 320
#define SPRITZ_KEY_KP_1 321
#define SPRITZ_KEY_KP_2 322
#define SPRITZ_KEY_KP_3 323
#define SPRITZ_KEY_KP_4 324
#define SPRITZ_KEY_KP_5 325
#define SPRITZ_KEY_KP_6 326
#define SPRITZ_KEY_KP_7 327
#define SPRITZ_KEY_KP_8 328
#define SPRITZ_KEY_KP_9 329
#define SPRITZ_KEY_KP_DECIMAL 330
#define SPRITZ_KEY_KP_DIVIDE 331
#define SPRITZ_KEY_KP_MULTIPLY 332
#define SPRITZ_KEY_KP_SUBTRACT 333
#define SPRITZ_KEY_KP_ADD 334
#define SPRITZ_KEY_KP_ENTER 335
#define SPRITZ_KEY_KP_EQUAL 336
#define SPRITZ_KEY_LEFT_SHIFT 340
#define SPRITZ_KEY_LEFT_CONTROL 341
#define SPRITZ_KEY_LEFT_ALT 342
#define SPRITZ_KEY_LEFT_SUPER 343
#define SPRITZ_KEY_RIGHT_SHIFT 344
#define SPRITZ_KEY_RIGHT_CONTROL 345
#define SPRITZ_KEY_RIGHT_ALT 346
#define SPRITZ_KEY_RIGHT_SUPER 347
#define SPRITZ_KEY_MENU 348

#define SPRITZ_KEY_LAST SPRITZ_KEY_MENU

/*! @} */

/*! @defgroup mods Modifier key flags
 *  @brief Modifier key flags.
 *
 *  See [key input](@ref input_key) for how these are used.
 *
 *  @ingroup input
 *  @{ */

/*! @brief If this bit is set one or more Shift keys were held down.
 *
 *  If this bit is set one or more Shift keys were held down.
 */
#define SPRITZ_MOD_SHIFT 0x0001
/*! @brief If this bit is set one or more Control keys were held down.
 *
 *  If this bit is set one or more Control keys were held down.
 */
#define SPRITZ_MOD_CONTROL 0x0002
/*! @brief If this bit is set one or more Alt keys were held down.
 *
 *  If this bit is set one or more Alt keys were held down.
 */
#define SPRITZ_MOD_ALT 0x0004
/*! @brief If this bit is set one or more Super keys were held down.
 *
 *  If this bit is set one or more Super keys were held down.
 */
#define SPRITZ_MOD_SUPER 0x0008
/*! @brief If this bit is set the Caps Lock key is enabled.
 *
 *  If this bit is set the Caps Lock key is enabled and the @ref
 *  SPRITZ_LOCK_KEY_MODS input mode is set.
 */
#define SPRITZ_MOD_CAPS_LOCK 0x0010
/*! @brief If this bit is set the Num Lock key is enabled.
 *
 *  If this bit is set the Num Lock key is enabled and the @ref
 *  SPRITZ_LOCK_KEY_MODS input mode is set.
 */
#define SPRITZ_MOD_NUM_LOCK 0x0020

/*! @} */

/*! @defgroup buttons Mouse buttons
 *  @brief Mouse button IDs.
 *
 *  See [mouse button input](@ref input_mouse_button) for how these are used.
 *
 *  @ingroup input
 *  @{ */
#define SPRITZ_MOUSE_BUTTON_1 0
#define SPRITZ_MOUSE_BUTTON_2 1
#define SPRITZ_MOUSE_BUTTON_3 2
#define SPRITZ_MOUSE_BUTTON_4 3
#define SPRITZ_MOUSE_BUTTON_5 4
#define SPRITZ_MOUSE_BUTTON_6 5
#define SPRITZ_MOUSE_BUTTON_7 6
#define SPRITZ_MOUSE_BUTTON_8 7
#define SPRITZ_MOUSE_BUTTON_LAST SPRITZ_MOUSE_BUTTON_8
#define SPRITZ_MOUSE_BUTTON_LEFT SPRITZ_MOUSE_BUTTON_1
#define SPRITZ_MOUSE_BUTTON_RIGHT SPRITZ_MOUSE_BUTTON_2
#define SPRITZ_MOUSE_BUTTON_MIDDLE SPRITZ_MOUSE_BUTTON_3

#endif
