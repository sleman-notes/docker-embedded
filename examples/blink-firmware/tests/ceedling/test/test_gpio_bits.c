#include "unity.h"
#include "gpio_bits.h"

#define LED_PIN		13

void setUp(void)
{
}

void tearDown(void)
{
}

void test_moder_puts_the_pin_in_output_mode(void)
{
	TEST_ASSERT_EQUAL_HEX32(0x04000000, gpio_moder_set(0x00000000, LED_PIN, GPIO_MODE_OUT));
}

void test_moder_clears_the_previous_mode(void)
{
    TEST_ASSERT_EQUAL_HEX32(0x04000000, gpio_moder_set(0x0C000000, LED_PIN, GPIO_MODE_OUT));
}

void test_otyper_selects_push_pull(void)
{
    TEST_ASSERT_EQUAL_HEX32(0xFFFFDFFF, gpio_otyper_set(0xFFFFFFFF, LED_PIN, GPIO_OP_TYPE_PP));
}

void test_odr_toggles_only_the_led_pin(void)
{
    TEST_ASSERT_EQUAL_HEX32(0x00002000, gpio_odr_toggle(0x00000000, LED_PIN));
	TEST_ASSERT_EQUAL_HEX32(0x00000000, gpio_odr_toggle(0x00002000, LED_PIN));
}