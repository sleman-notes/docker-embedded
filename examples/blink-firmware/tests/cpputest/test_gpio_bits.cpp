#include "CppUTest/TestHarness.h"

extern "C"
{
	#include "gpio_bits.h"
}

#define LED_PIN		13

TEST_GROUP(GpioBits)
{
};

TEST(GpioBits, ModerPutsThePinInOutputMode)
{
	UNSIGNED_LONGS_EQUAL(0x04000000, gpio_moder_set(0x00000000, LED_PIN, GPIO_MODE_OUT));
}

TEST(GpioBits, ModerClearsThePreviousMode)
{
	UNSIGNED_LONGS_EQUAL(0x04000000, gpio_moder_set(0x0C000000, LED_PIN, GPIO_MODE_OUT));
}

TEST(GpioBits, OtyperSelectsPushPull)
{
	UNSIGNED_LONGS_EQUAL(0xFFFFDFFF, gpio_otyper_set(0xFFFFFFFF, LED_PIN, GPIO_OP_TYPE_PP));
}

TEST(GpioBits, OdrTogglesOnlyTheLedPin)
{
	UNSIGNED_LONGS_EQUAL(0x00002000, gpio_odr_toggle(0x00000000, LED_PIN));
	UNSIGNED_LONGS_EQUAL(0x00000000, gpio_odr_toggle(0x00002000, LED_PIN));
}

