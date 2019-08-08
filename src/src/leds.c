#include <leds.h>

int lastTicksRainbow = 0;
uint32_t rainbowRGB[3] = {1024, 0, 0};
int rainbowColorDesc = 0;
int rainbowColorAsc = 1;

int lastTicksHeartbeat = 0;

void set_status_led(bool state) {
  if (state) {
    gpio_set(GPIOA, GPIO12);
  } else {
    gpio_clear(GPIOA, GPIO12);
  }
}
void toggle_status_led() {
  gpio_toggle(GPIOA, GPIO12);
}

void set_RGB_color(uint32_t r, uint32_t g, uint32_t b) {
  timer_set_oc_value(TIM1, TIM_OC4, r);
  timer_set_oc_value(TIM1, TIM_OC3, g);
  timer_set_oc_value(TIM1, TIM_OC2, b);
}

void set_RGB_rainbow() {
  if (get_clock_ticks() > lastTicksRainbow + 10) {
    lastTicksRainbow = get_clock_ticks();
    rainbowRGB[rainbowColorDesc] -= 20;
    rainbowRGB[rainbowColorAsc] += 20;
    set_RGB_color(rainbowRGB[0], rainbowRGB[1], rainbowRGB[2]);
    if (rainbowRGB[rainbowColorDesc] <= 0 || rainbowRGB[rainbowColorAsc] >= 1024) {
      rainbowRGB[rainbowColorDesc] = 0;
      rainbowRGB[rainbowColorAsc] = 1024;
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
void set_neon_heartbeat() {
  if (get_clock_ticks() > lastTicksHeartbeat + 10) {
    lastTicksHeartbeat = get_clock_ticks();
    fade += increment;
    if (fade > 1024) {
      fade = 1024;
      increment = -increment;
    }
    if (fade <= 0) {
      fade = 0;
      increment = -increment;
    }
    set_neon_fade(fade);
  }
}