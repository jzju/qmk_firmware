/*
Copyright 2011 Jun Wako <wakojun@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdint.h>
#include "keycode.h"
#include "host.h"
#include "timer.h"
#include "print.h"
#include "debug.h"
#include "mousekey.h"

#define CLAMP(x, low, high)  (((x) > (high)) ? (high) : (((x) < (low)) ? (low) : (x)))


static report_mouse_t mouse_report = {};
static uint8_t mousekey_repeat =  0;
static uint8_t mousekey_accel = 0;

static void mousekey_debug(void);


/*
 * Mouse keys  acceleration algorithm
 *  http://en.wikipedia.org/wiki/Mouse_keys
 *
 *  speed = delta * max_speed * (repeat / time_to_max)**((1000+curve)/1000)
 */
/* milliseconds between the initial key press and first repeated motion event (0-2550) */
uint8_t mk_delay = MOUSEKEY_DELAY/10;
/* milliseconds between repeated motion events (0-255) */
uint8_t mk_interval = MOUSEKEY_INTERVAL;
/* steady speed (in action_delta units) applied each event (0-255) */
uint8_t mk_max_speed = MOUSEKEY_MAX_SPEED * MOUSEKEY_MOVE_DELTA;
uint8_t mk_slow_speed = MOUSEKEY_MAX_SPEED * MOUSEKEY_MOVE_DELTA / 4;
/* number of events (count) accelerating to steady speed (0-255) */
uint8_t mk_time_to_max = MOUSEKEY_TIME_TO_MAX;
/* ramp used to reach maximum pointer speed (NOT SUPPORTED) */
uint8_t acc_per_int = ((uint16_t) MOUSEKEY_MAX_SPEED) * MOUSEKEY_INTERVAL / MOUSEKEY_TIME_TO_MAX;
//int8_t mk_curve = 0;
/* wheel params */
uint8_t mk_wheel_max_speed = MOUSEKEY_WHEEL_MAX_SPEED;
uint8_t mk_wheel_time_to_max = MOUSEKEY_WHEEL_TIME_TO_MAX;

// x positive right, y positive down
// +- 1=forward, 2=slow
int8_t x_state = 0;
int8_t y_state = 0;


static uint16_t last_timer = 0;

static uint8_t wheel_unit(void)
{
    uint16_t unit;
    if (mousekey_accel & (1<<0)) {
        unit = (MOUSEKEY_WHEEL_DELTA * mk_wheel_max_speed)/4;
    } else if (mousekey_accel & (1<<1)) {
        unit = (MOUSEKEY_WHEEL_DELTA * mk_wheel_max_speed)/2;
    } else if (mousekey_accel & (1<<2)) {
        unit = (MOUSEKEY_WHEEL_DELTA * mk_wheel_max_speed);
    } else if (mousekey_repeat == 0) {
        unit = MOUSEKEY_WHEEL_DELTA;
    } else if (mousekey_repeat >= mk_wheel_time_to_max) {
        unit = MOUSEKEY_WHEEL_DELTA * mk_wheel_max_speed;
    } else {
        unit = (MOUSEKEY_WHEEL_DELTA * mk_wheel_max_speed * mousekey_repeat) / mk_wheel_time_to_max;
    }
    return (unit > MOUSEKEY_WHEEL_MAX ? MOUSEKEY_WHEEL_MAX : (unit == 0 ? 1 : unit));
}

void calc_v(int8_t *state, int8_t *rep)
{
    if (!*state)  *rep = 0;
    else if (*state > 0) *rep += acc_per_int;
    else if (*state < 0) *rep -= acc_per_int;
    if (*state == 1 || *state == -1) *rep = CLAMP(*rep, -mk_max_speed, mk_max_speed);
    else *rep = CLAMP(*rep, -mk_slow_speed, mk_slow_speed);
}

void mousekey_task(void)
{
    if (timer_elapsed(last_timer) < (mousekey_repeat ? mk_interval : mk_delay*10))
        return;

    if (x_state == 0 && y_state == 0 && mouse_report.v == 0 && mouse_report.h == 0)
        return;

    if (mousekey_repeat != UINT8_MAX)
        mousekey_repeat++;

    calc_v(&x_state, &mouse_report.x);
    calc_v(&y_state, &mouse_report.y);

    if (mouse_report.v > 0) mouse_report.v = wheel_unit();
    if (mouse_report.v < 0) mouse_report.v = wheel_unit() * -1;
    if (mouse_report.h > 0) mouse_report.h = wheel_unit();
    if (mouse_report.h < 0) mouse_report.h = wheel_unit() * -1;

    mousekey_send();
}

void mousekey_on(uint8_t code)
{
    if (code == KC_MS_UP) {
        if (y_state)  y_state = 2;
        else          y_state = -1;
    }
    else if (code == KC_MS_DOWN) {
        if (y_state)  y_state = -2;
        else          y_state = 1;
    }
    else if (code == KC_MS_LEFT) {
        if (x_state)  x_state = 2;
        else          x_state = -1;
    }
    else if (code == KC_MS_RIGHT) {
        if (x_state)  x_state = -2;
        else          x_state = 1;
    }
    else if (code == KC_MS_WH_UP)    mouse_report.v = wheel_unit();
    else if (code == KC_MS_WH_DOWN)  mouse_report.v = wheel_unit() * -1;
    else if (code == KC_MS_WH_LEFT)  mouse_report.h = wheel_unit() * -1;
    else if (code == KC_MS_WH_RIGHT) mouse_report.h = wheel_unit();
    else if (code == KC_MS_BTN1)     mouse_report.buttons |= MOUSE_BTN1;
    else if (code == KC_MS_BTN2)     mouse_report.buttons |= MOUSE_BTN2;
    else if (code == KC_MS_BTN3)     mouse_report.buttons |= MOUSE_BTN3;
    else if (code == KC_MS_BTN4)     mouse_report.buttons |= MOUSE_BTN4;
    else if (code == KC_MS_BTN5)     mouse_report.buttons |= MOUSE_BTN5;
    else if (code == KC_MS_ACCEL0)   mousekey_accel |= (1<<0);
    else if (code == KC_MS_ACCEL1)   mousekey_accel |= (1<<1);
    else if (code == KC_MS_ACCEL2)   mousekey_accel |= (1<<2);
}

void mousekey_off(uint8_t code)
{
    if (code == KC_MS_UP) {
        if (y_state == -1) {
            y_state = 0;
            mouse_report.y = 0;
        }
        else y_state = 1;
    }
    else if (code == KC_MS_DOWN) {
        if (y_state == 1) {
            y_state = 0;
            mouse_report.y = 0;
        }
        else y_state = -1;
    }
    else if (code == KC_MS_LEFT) {
        if (x_state == -1) {
            x_state = 0;
            mouse_report.x = 0;
        }
        else x_state = 1;
    }
    else if (code == KC_MS_RIGHT) {
        if (x_state == 1) {
            x_state = 0;
            mouse_report.x = 0;
        }
        else x_state = -1;
    }
    else if (code == KC_MS_WH_UP    && mouse_report.v > 0) mouse_report.v = 0;
    else if (code == KC_MS_WH_DOWN  && mouse_report.v < 0) mouse_report.v = 0;
    else if (code == KC_MS_WH_LEFT  && mouse_report.h < 0) mouse_report.h = 0;
    else if (code == KC_MS_WH_RIGHT && mouse_report.h > 0) mouse_report.h = 0;
    else if (code == KC_MS_BTN1) mouse_report.buttons &= ~MOUSE_BTN1;
    else if (code == KC_MS_BTN2) mouse_report.buttons &= ~MOUSE_BTN2;
    else if (code == KC_MS_BTN3) mouse_report.buttons &= ~MOUSE_BTN3;
    else if (code == KC_MS_BTN4) mouse_report.buttons &= ~MOUSE_BTN4;
    else if (code == KC_MS_BTN5) mouse_report.buttons &= ~MOUSE_BTN5;
    else if (code == KC_MS_ACCEL0) mousekey_accel &= ~(1<<0);
    else if (code == KC_MS_ACCEL1) mousekey_accel &= ~(1<<1);
    else if (code == KC_MS_ACCEL2) mousekey_accel &= ~(1<<2);

    if (mouse_report.x == 0 && mouse_report.y == 0 && mouse_report.v == 0 && mouse_report.h == 0)
        mousekey_repeat = 0;
}

void mousekey_send(void)
{
    mousekey_debug();
    host_mouse_send(&mouse_report);
    last_timer = timer_read();
}

void mousekey_clear(void)
{
    mouse_report = (report_mouse_t){};
    mousekey_repeat = 0;
    mousekey_accel = 0;
}

static void mousekey_debug(void)
{
    if (!debug_mouse) return;
    print("mousekey [btn|x y v h](rep/acl): [");
    phex(mouse_report.buttons); print("|");
    print_decs(mouse_report.x); print(" ");
    print_decs(mouse_report.y); print(" ");
    print_decs(mouse_report.v); print(" ");
    print_decs(mouse_report.h); print("](");
    print_dec(mousekey_repeat); print("/");
    print_dec(mousekey_accel); print(")\n");
}
