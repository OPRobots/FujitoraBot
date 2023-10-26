#include "leds.h"

uint32_t lastTicksRainbow = 0;
uint32_t rainbowRGB[3] = {LEDS_MAX_PWM, 0, 0};
int rainbowColorDesc = 0;
int rainbowColorAsc = 1;

uint32_t lastTicksHeartbeat = 0;
uint32_t lastTicksWarning = 0;

uint32_t lastTicksWarningRGB = 0;
bool lastWarningRGBState = false;

/**
 * @brief Establece el estado del led de estado
 *
 * @param state Estado del led
 */
void set_status_led(bool state) {
  if (state) {
    gpio_set(GPIOA, GPIO12);
  } else {
    gpio_clear(GPIOA, GPIO12);
  }
}

/**
 * @brief Aleterna el estado del led de estado
 *
 */
void toggle_status_led(void) {
  gpio_toggle(GPIOA, GPIO12);
}

/**
 * @brief Activa el modo warning del led de estado
 *
 * @param ms Tiempo entre cambios de estado del modo warning
 */
void warning_status_led(uint16_t ms) {
  if (get_clock_ticks() > lastTicksWarning + ms) {
    toggle_status_led();
    lastTicksWarning = get_clock_ticks();
  }
}

/**
 * @brief Set the RGB color object
 *
 * @param r
 * @param g
 * @param b
 */
void set_RGB_color(uint32_t r, uint32_t g, uint32_t b) {
  timer_set_oc_value(TIM1, TIM_OC4, r);
  timer_set_oc_value(TIM1, TIM_OC3, g);
  timer_set_oc_value(TIM1, TIM_OC2, b);
}

void warning_RGB_color(uint32_t r, uint32_t g, uint32_t b, uint16_t ms) {
  if (get_clock_ticks() > lastTicksWarningRGB + ms) {
    if (lastWarningRGBState) {
      set_RGB_color(0, 0, 0);
    } else {
      set_RGB_color(r, g, b);
    }
    lastTicksWarningRGB = get_clock_ticks();
    lastWarningRGBState = !lastWarningRGBState;
  }
}

void set_RGB_rainbow(void) {
  if (get_clock_ticks() > lastTicksRainbow + 10) {
    lastTicksRainbow = get_clock_ticks();
    rainbowRGB[rainbowColorDesc] -= 20;
    rainbowRGB[rainbowColorAsc] += 20;
    set_RGB_color(rainbowRGB[0], rainbowRGB[1], rainbowRGB[2]);
    if (rainbowRGB[rainbowColorDesc] <= 0 || rainbowRGB[rainbowColorAsc] >= LEDS_MAX_PWM) {
      rainbowRGB[rainbowColorDesc] = 0;
      rainbowRGB[rainbowColorAsc] = LEDS_MAX_PWM;
      set_RGB_color(rainbowRGB[0], rainbowRGB[1], rainbowRGB[2]);
      rainbowColorDesc++;
      if (rainbowColorDesc > 2) {
        rainbowColorDesc = 0;
      }
      rainbowColorAsc = rainbowColorDesc == 2 ? 0 : rainbowColorDesc + 1;
    }
  }
}

void set_neon_fade(uint32_t n) {
  timer_set_oc_value(TIM1, TIM_OC1, n);
}

int32_t fade = 0;
int32_t increment = 20;
void set_neon_heartbeat(void) {
  if (get_clock_ticks() > lastTicksHeartbeat + 10) {
    lastTicksHeartbeat = get_clock_ticks();
    fade += increment;
    if (fade > LEDS_MAX_PWM) {
      fade = LEDS_MAX_PWM;
      increment = -increment;
    }
    if (fade <= 0) {
      fade = 0;
      increment = -increment;
    }
    set_neon_fade(fade);
  }
}

void all_leds_clear(void) {
  set_neon_fade(0);
  set_RGB_color(0, 0, 0);
  set_status_led(false);
}