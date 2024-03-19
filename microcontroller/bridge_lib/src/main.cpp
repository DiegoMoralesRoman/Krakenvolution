#include "HWCDC.h"
#include "esp32-hal-gpio.h"
#include "esp32-hal.h"
#include <Arduino.h>
#include <bridge.hpp>

#include <messages/test.pb.h>

constexpr int LED_BUILTIN = 20;

bridge::Bridge pb = bridge::Bridge::create(Serial);

void on_led(const Led& msg) {
	static bool state = false;
	digitalWrite(LED_BUILTIN, state);
	state = !state;
	pb.publish("led", (void*)(&msg), Led_fields);
}

void setup() {
	Serial.begin(115200);
	pinMode(LED_BUILTIN, OUTPUT);
	pb.subscribe("led", on_led, Led_fields);
}

int cnt = 0;

void loop() {
	if (Serial.available() > 0) {
		delay(100);
		pb.read_buffer();
	}
}
