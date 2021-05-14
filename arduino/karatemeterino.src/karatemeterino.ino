/**
 * v-1.0.3 Karatemeter 
 * 2021-05-14. Read and calculate pythagorean acceleration in place.
 * 
 */
 #include "SparkFun_LIS331.h"
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include "karatemeterino.h"

// ----------------------------------------------------------------- //
// Settings panel

bool sensitivity_hard;
bool display_mode_record;

void init_panel() {
  pinMode(SWITCH_1_OUTPUT_PIN, OUTPUT);
  pinMode(SWITCH_1_PIN, INPUT);
  pinMode(SWITCH_2_OUTPUT_PIN, OUTPUT);
  pinMode(SWITCH_2_PIN, INPUT);
  digitalWrite(SWITCH_1_OUTPUT_PIN, LOW);
  digitalWrite(SWITCH_2_OUTPUT_PIN, LOW);
}

void start_listening_panel() {
  digitalWrite(SWITCH_1_OUTPUT_PIN, HIGH);
  digitalWrite(SWITCH_2_OUTPUT_PIN, HIGH);
}

void stop_listening_panel() {
  digitalWrite(SWITCH_1_OUTPUT_PIN, LOW);
  digitalWrite(SWITCH_2_OUTPUT_PIN, LOW);
}

bool check_sensitivity_is_hard() {
  return digitalRead(SWITCH_1_PIN);
}

bool check_display_mode_is_record() {
  return digitalRead(SWITCH_2_PIN);
}

void set_panel_state() {
  start_listening_panel();
  delay(10);
  sensitivity_hard = check_sensitivity_is_hard();
  display_mode_record = check_display_mode_is_record();
  stop_listening_panel();
}


// ----------------------------------------------------------------- //
// Debug

void debug_print(char* content) {
  if (DEBUG) {
    Serial.print(content);
    Serial.println();
  }
}

void debug_print(int content) {
  if (DEBUG) {
    Serial.print(content);
    Serial.println();
  }
}

// ----------------------------------------------------------------- //
// Beat allowance

void stop_buzz() {  
  noTone(BUZZER_PIN);
}

void start_buzz(unsigned int frequency) {  
  if (!SHUT_UP_HIT_SOUND) tone(BUZZER_PIN, frequency);
}

void init_buzzer() {  
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
}

void beat_not_allowed() {
  start_buzz(BUZZER_FREQUENCY);
}

void beat_allowed() {
  stop_buzz();
}

void buzz_note(unsigned int frequency, unsigned long duration_ms) {
  noTone(BUZZER_PIN);
  tone(BUZZER_PIN, frequency, duration_ms);
}

// ----------------------------------------------------------------- //
// Accelerometer

void set_accel_sensitivity() {
  if (sensitivity_hard) {
    accel_precision = LIS331::MED_RANGE;
    accel_precision_max = 200;
    sensitive_min = SENSITIVE_MIN_HARD;
  } else {
    accel_precision = LIS331::LOW_RANGE;
    accel_precision_max = 100;
    sensitive_min = SENSITIVE_MIN_SOFT;
  }
}

void turn_off_accelerometer() {
  xl.setPowerMode(LIS331::POWER_DOWN);
}

void turn_on_accelerometer() {  
  xl.begin(LIS331::USE_I2C);
  xl.intSrcConfig(LIS331::INT_SRC, 1);
  xl.setIntDuration(16, 1);
  xl.setIntThreshold(2, 1);
  xl.enableInterrupt(LIS331::X_AXIS, LIS331::TRIG_ON_HIGH, 1, false);
  xl.setFullScale(accel_precision);
}

void init_accelerometer() 
{  
  pinMode(9,INPUT);
  Wire.begin();
  xl.setI2CAddr(0x19);
}

void read_axes_acceleration(int* acceleration) {
  int x, y, z;
  xl.readAxes(x, y, z);
  *acceleration = (int)round(pythagorean_acceleration(x, y, z)) - calibration;
}

float convert_to_g(int value) {
  return xl.convertToG(accel_precision_max, value);
}

int check_accelerometer_timeout() {
  return millis() > accelerometer_timeout_at;
}

void reset_accelerometer_timeout() {
  accelerometer_timeout_at = millis() + ACCELERATOR_TIMEOUT_MS;
}

void calibrate_accelerometer()
{
  buzz_note(2000, 100);
  display.setCursor(0,1);
  display.println(F("Calibrating..."));
  display.println(F(""));
  display.println(F("Don't move the sensor"));
  display_all();
  delay(2000);  
  int x, y, z;
  int x_calibrate = calculate_calibration(CALIBRATION_LOOPS, &x, &y, &z, &x);
  int y_calibrate = calculate_calibration(CALIBRATION_LOOPS, &x, &y, &z, &y);
  int z_calibrate = calculate_calibration(CALIBRATION_LOOPS, &x, &y, &z, &z);
  calibration = (int)round(pythagorean_acceleration(x_calibrate, y_calibrate, z_calibrate));
  display_clear();
  display.setCursor(0,1);
  display.println(F("Axes deviation: "));
  display.println(F(""));
  display.print(x_calibrate);
  display.print(F("x "));
  display.print(y_calibrate);
  display.print(F("y "));
  display.print(z_calibrate);
  display.println(F("z "));
  display.print(calibration);
  display.println(F(" total"));
  display_all();
  delay(1000);  
  display_clear();
}

int calculate_calibration(int loops, int* x, int* y, int* z, int* observed) {
  int readings = 0;
  int i;
  for (i = 0; i < loops; i ++) 
  {
    xl.readAxes(*x, *y, *z);
    readings += *observed;
    debug_print(*observed);
  }
  return abs(readings / loops);
}

// ----------------------------------------------------------------- //
// Display

void display_record_got() {
  buzz_note(2000, 200);
  delay(100);
  buzz_note(2500, 200);
  delay(100);
  buzz_note(4000, 300);
  
  display.setTextColor(BLACK); 
  display.setCursor(record_x_positions[TOP], 16);
  display.setTextSize(2);
  display.print(last_record);
  display.display();
  
  display.setTextColor(WHITE); 
  display.setCursor(record_x_positions[TOP], 16);
  display.setTextSize(2);
  display.print(last_record);
  display.display();
  
  display.setTextColor(BLACK); 
  display.setCursor(record_x_positions[TOP], 16);
  display.setTextSize(2);
  display.print(last_record);
  display.display();
  
  display.setTextColor(WHITE); 
  display.setCursor(record_x_positions[TOP], 16);
  display.setTextSize(2);
  display.print(last_record);
  display.display();
}

bool check_record_is_got(float current) {
  if (last_record >= current) return false;
  last_record = current;
  return true;
}

void set_display_mode() {
  if (display_mode_record) {
    display_mode = RECORD;
  } else {
    display_mode = BAR;
  }
}

void turn_off_display() {
  display.ssd1306_command(SSD1306_DISPLAYOFF);
}

void turn_on_display() 
{   
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextColor(WHITE);  
  display_clear();
}

void display_bar(
  const D_num start_x,
  const D_num start_y,
  const D_num end_x,
  const D_num end_y
) 
{
  int bar_x;
  int started_x;
  int end_y2;
  int start_y2 = DISPLAY_HEIGHT;
  if (0 == end_y) end_y2 = DISPLAY_HEIGHT - ((start_x + (int)alternate_x_default) % 2);
  else end_y2 = DISPLAY_HEIGHT - (int)end_y;
  bar_x = ((int)start_x * BAR_WIDTH);
  for (started_x = 0; started_x < BAR_WIDTH -1; started_x ++) {
    display.setCursor(bar_x, DISPLAY_HEIGHT);
    display.drawLine(bar_x, start_y2, bar_x, end_y2, SSD1306_WHITE);
    bar_x++;
  }
}

void display_number(
  const D_num start_x,
  const D_num start_y,
  const D_num number)
{
  display.setCursor(start_x, start_y);
  display.setTextSize(1);
  display.print(convert_to_g(number));
}

void display_number_with_caption(const D_num number, const char caption[], const int position) {
  float g = convert_to_g(number);
  display.setCursor(record_x_positions[position], 16);
  display.setTextSize(2);
  if (g < 100) display.print(g);
  else display.print((int)g);
  display.setCursor(record_x_positions[position], 1);
  display.setTextSize(1);
  display.print(caption);
  if (TOP == position) {
    if (check_record_is_got(g)) {
      display_record_got();
    }
    display.drawLine(
      record_x_positions[position] - 4, 
      0, 
      record_x_positions[position] - 4, 
      DISPLAY_HEIGHT / 2, 
      SSD1306_WHITE
      );
  }
}

void display_all() {
  display_battery();
  display.display();
}

void display_clear() {
  display.clearDisplay();
}

void display_battery()
{
  const int percent = check_battery();
  int start_x;
  int digits = 0;
  int counting_num = percent; 
  while (counting_num != 0) {
      counting_num /= 10;
      ++digits;
  }
  start_x = BATTERY_LEVEL_X - (CHAR_WIDTH * (digits - 1));
  display.setCursor(start_x, 1);
  display.setTextSize(1);
  display.print(percent);
  display.print("%");
}

void display_settings() {
  String sens_setting;
  String mode_setting;
  sens_setting = "Sensitivity ";
  if (sensitivity_hard) sens_setting += "HARD";
  else sens_setting += "SOFT";
  mode_setting = "Display Mode ";
  if (display_mode_record) mode_setting += "RECORD";
  else mode_setting += "BAR";
  display_clear();
  display.setTextSize(1);
  display.setCursor(0, 1);
  display.println(sens_setting);
  display.println();
  display.println(mode_setting);
  display_all();
  delay(2000);
}

void display_cover() {  
  display_clear();
  display.setCursor(0, 1);
  display.setTextSize(2);
  display.println(F("KARATE"));
  display.print(F("METER 1.0"));
  display_all();
}

void display_credits() {  
  display_clear();
  display.setCursor(0, 12);
  display.setTextSize(1);
  display.print(F("Westial presents..."));
  display_all();
  delay(2000);
}

// ----------------------------------------------------------------- //
// Karatemeter use case

void init_use_case() {  
  measure_config.read_acceleration_fn = (void *) read_axes_acceleration;
  measure_config.check_timeout_fn = (void *) check_accelerometer_timeout;
  measure_config.sensitive_min = sensitive_min;
  measure_config.sensible_range = PEAK_RANGE_DISTANCE;
  measure_config.memory_length = PEAK_VALUES_LENGTH;
  
  display_config.line_fn = (void *) display_bar;
  display_config.corner_number_fn = (void *) display_number;
  display_config.number_with_caption = (void *) display_number_with_caption;
  display_config.width = DISPLAY_WIDTH;
  display_config.bars = DISPLAY_BARS;
  display_config.height = DISPLAY_HEIGHT - CHAR_SIZE_1_HEIGHT;
  MeasureAndDisplayByModeUseCase_Create(&measure_config, &display_config);
}

// ----------------------------------------------------------------- //
// Battery

int check_battery()
{ 
  int percent = (100 * (analogRead(BATTERY_CHECK_PIN) - BATTERY_MIN)) / (BATTERY_MAX - BATTERY_MIN);
  return 100 < percent ? 100 : percent;
}

void init_battery_leak_led() {
  pinMode(BATTERY_LEAK_LED_PIN, OUTPUT);
  digitalWrite(BATTERY_LEAK_LED_PIN, LOW);
}

void battery_leak_sleep() {
  buzz_note(3500, 50);
  digitalWrite(BATTERY_LEAK_LED_PIN, HIGH);
  delay(150);
  digitalWrite(BATTERY_LEAK_LED_PIN, LOW);
  delay(SLEEP_INTERVAL_SEC * 1000);
}

bool check_battery_is_critical() {
  return !BYPASS_BATTERY_CHECK && 0 >= check_battery();
}

void turn_off_all() {
    if (all_is_off) return;
    turn_off_accelerometer();
    turn_off_display();
    all_is_off = true;  
}

void turn_on_all() {
    if (!all_is_off) return;
    init_panel();
    init_accelerometer();  
    turn_on_display();
    turn_on_accelerometer();  
    set_panel_state();
    set_display_mode();
    set_accel_sensitivity();
    init_use_case();    
    calibrate_accelerometer();
    display_settings();
    display_credits();
    display_cover();    
    all_is_off = false;  
}

// ----------------------------------------------------------------- //
// Arduino API

void setup() 
{
  all_is_off = true;
  Serial.begin(9600);
  init_battery_leak_led();
  init_buzzer();

  if (!check_battery_is_critical()) {
    turn_on_all();
  }  
}

void loop() 
{
  if (check_battery_is_critical()) {
    turn_off_all();
    battery_leak_sleep();
  } else {
    turn_on_all(); 
    alternate_x_default = !alternate_x_default;
    reset_accelerometer_timeout();
    display_clear();
    beat_allowed();
    MeasureAndDisplayByModeUseCase_Invoke(display_mode);
    beat_not_allowed();
    display_all();
  }
}
