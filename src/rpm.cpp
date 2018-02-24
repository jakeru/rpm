// rpm.c

// 2018-02-22 22:33 (Jakob Ruhe): created

#include <Arduino.h>
#include <Print.h>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

#define PIN_WEMOS_D4 2
#define PIN_WEMOS_D7 13
#define PIN_WEMOS_D8 15

#define PIN_GND1 PIN_WEMOS_D8
#define PIN_IN1 PIN_WEMOS_D7
#define PIN_LED PIN_WEMOS_D4

typedef struct {
  int32_t value;
  bool state;
  bool lastBlinkDetected;
  uint32_t blinks;
  uint32_t lastReportAt;
} MeterState;

static MeterState s_meterState;

static void ledSet(bool state)
{
  digitalWrite(PIN_LED, !state);
}

static bool timeAtOrAfter(uint32_t t, uint32_t now)
{
  return (int32_t)(now - t) >= 0;
}

static int32_t timeDiff(uint32_t t1, uint32_t t2)
{
  return t2 - t1;
}

static void loopMeter(MeterState* s, bool state)
{
  const int32_t limit = 10;
  bool newState = false;
  if (state) {
    if (s->value < limit) {
      s->value++;
    }
    else if (!s->state) {
      newState = true;
    }
  }
  else {
    if (s->value > -limit) {
      s->value--;
    }
    else if (s->state) {
      newState = true;
    }
  }

  uint32_t now = micros();

  if (!newState) {
    if (s->lastBlinkDetected) {
      if (timeAtOrAfter(s->lastReportAt + 5e6, now)) {
        ledSet(false);
        s->lastBlinkDetected = false;
        s->blinks = 0;
      }
    }
    return;
  }

  s->state = state;

  if (!state) {
    return;
  }

//  Serial.printf("%5u", s->blinks);
//  Serial.println();

  if (!s->lastBlinkDetected) {
    ledSet(true);
    s->lastReportAt = now;
  }

  s->lastBlinkDetected = true;
  s->blinks++;

  int32_t t = timeDiff(s->lastReportAt, now);
  if (t >= 1.0e6) {
    float rpm = 1.0e6 * 60 * s->blinks / t;
    Serial.printf("blinks: %5u, last: %.3f s, BPM: %.1f ", s->blinks,
      t / 1.0e6, rpm);
    Serial.println();
    s->blinks = 0;
    s->lastReportAt = now;
  }
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  ledSet(false);

  pinMode(PIN_GND1, OUTPUT);
  digitalWrite(PIN_GND1, LOW);

  pinMode(PIN_IN1, INPUT);

  Serial.begin(115200);
  Serial.println("Booting");
}

void loop() {
  loopMeter(&s_meterState, digitalRead(PIN_IN1));
}
