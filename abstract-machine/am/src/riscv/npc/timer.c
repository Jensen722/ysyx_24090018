#include <am.h>
#include <riscv/riscv.h>
#include <stdio.h>
#define RTC_ADDR 0xa0000048

void __am_timer_init() {
}

void __am_timer_uptime(AM_TIMER_UPTIME_T *uptime) {
  uint32_t lo = inl(RTC_ADDR);       // 低32位
  uint32_t hi = inl(RTC_ADDR + 4);   // 高32位
  uptime->us = ((uint64_t)hi << 32) | lo;
}

void __am_timer_rtc(AM_TIMER_RTC_T *rtc) {
  rtc->second = 0;
  rtc->minute = 55;
  rtc->hour   = 14;
  rtc->day    = 18;
  rtc->month  = 3;
  rtc->year   = 2025;
}
