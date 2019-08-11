#include <control.h>

void pid_timer_custom_isr() {
}

void speed_timer_custom_isr() {
}

void resume_pid_timer() {
  timer_enable_irq(TIM5, TIM_DIER_CC1IE);
}

void pause_pid_timer() {
  timer_disable_irq(TIM5, TIM_DIER_CC1IE);
}

void resume_speed_timer() {
  timer_enable_irq(TIM2, TIM_DIER_CC1IE);
}

void pause_speed_timer() {
  timer_disable_irq(TIM2, TIM_DIER_CC1IE);
}
