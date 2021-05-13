#include "displayconfig.h"
#include "impactconfig.h"

extern "C" {
#include <MeasureAndDisplayByModeUseCase.h>
}

// ----------------------------------------------------------------- //
// Debug

#define DEBUG                     0       // TESTING AT 1
#define BYPASS_BATTERY_CHECK      0       // TESTING AT 1
#define SHUT_UP_HIT_SOUND         0       // TESTING AT 1

void debug_print(char* content);
void debug_print(int content);

// ----------------------------------------------------------------- //
// Settings Panel

#define SWITCH_1_OUTPUT_PIN 5
#define SWITCH_1_PIN        11

#define SWITCH_2_OUTPUT_PIN 4
#define SWITCH_2_PIN        12

void init_panel();

void start_listening_panel();
void stop_listening_panel();
bool check_sensitivity_is_hard();
bool check_display_mode_is_record();
void set_panel_state();

// ----------------------------------------------------------------- //
// Beat allowance

#define BUZZER_PIN                3
#define BUZZER_FREQUENCY          4000      // 31-4978

void beat_not_allowed();
void beat_allowed();

void init_buzzer();
void stop_buzz();
void start_buzz(unsigned int frequency);
void buzz_note(unsigned int frequency, unsigned long duration_ms);

// ----------------------------------------------------------------- //
// Display

#define CHAR_SIZE_1_HEIGHT  9

Mode display_mode;

float last_record;
const int record_x_positions[2] = {1, DISPLAY_WIDTH / 2};

Adafruit_SSD1306 display(4);

enum Position {
  LAST = 0,
  TOP = 1
};

void set_display_mode();
void turn_on_display();
void display_bar(const D_num x, const D_num y, const D_num x2, const D_num y2);
void display_number(const D_num x, const D_num y, const D_num number);
void display_number_with_caption(const D_num number, const char caption[], const int position);
void display_record_got();
bool check_record_is_got(float current);
void display_all();
void display_clear();
void display_battery();
void display_settings();
void display_cover();
void turn_off_display();

// ----------------------------------------------------------------- //
// Accelerometer

#define CALIBRATION_LOOPS         16
#define ACCELERATOR_TIMEOUT_MS    10000

#define SENSITIVE_MIN_SOFT        100
#define SENSITIVE_MIN_HARD        320
/*
 * LOW_RANGE - +/-6g for the LIS331HH or +/-100g for the H3LIS331DH.
 * MED_RANGE - +/-12g for the LIS331HH or +/-200g for the H3LIS331DH.
 * HIGH_RANGE - +/-24g for the LIS331HH or +/-400g for the H3LIS331DH.
 */

LIS331 xl;

unsigned long accelerometer_timeout_at;
int x_calibrate, y_calibrate, z_calibrate;

bool alternate_x_default = true;

LIS331::fs_range accel_precision;
int accel_precision_max;
int sensitive_min;

void set_accel_sensitivity();
void init_accelerometer();
void read_axes_acceleration(int xyz[]);
float convert_to_g(int value);
int check_accelerometer_timeout();
void reset_accelerometer_timeout();
void calibrate_accelerometer();
int calculate_calibration(int loops, int* x, int* y, int* z, int* observed);
void turn_off_accelerometer();
void turn_on_accelerometer();


// ----------------------------------------------------------------- //
// Karatemeter use case

#define BAR_WIDTH                 4

MeasureConfig measure_config;
DisplayConfig display_config;

void init_use_case();

// ----------------------------------------------------------------- //
// Battery

#define BATTERY_CHECK_PIN     A0
#define BATTERY_LEVEL_X       112
#define CHAR_WIDTH            4
#define BATTERY_MAX           321     // 321 (max charged with internal charger) = 4.3V;  325 (max charged with external charger) = 4.37V;  308(+usb?) = 4.136V;  267 = 3.8V; 305 (min) = 3.56V;   // not working: 3.43V
#define BATTERY_MIN           305

#define BATTERY_LEAK_LED_PIN  7

#define SLEEP_INTERVAL_SEC    7

bool all_is_off;

bool check_battery_is_critical();
int check_battery();
void init_battery_leak_led();
void battery_leak_sleep();
void turn_off_all();
void turn_on_all();
