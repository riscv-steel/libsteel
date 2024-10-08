// ----------------------------------------------------------------------------
// Copyright (c) 2020-2024 RISC-V Steel contributors
//
// This work is licensed under the MIT License, see LICENSE file for details.
// SPDX-License-Identifier: MIT
// ----------------------------------------------------------------------------

#ifndef __LIBSTEEL_GPIO__
#define __LIBSTEEL_GPIO__

#include "globals.h"

// Struct providing access to RISC-V Steel GPIO Controller registers
typedef struct
{
  // IN (Input) Register. Address offset: 0x00.
  volatile uint32_t IN;
  // OE (Output Enable) Register. Address offset: 0x04.
  volatile uint32_t OE;
  // OUT (Output) Register. Address offset: 0x08.
  volatile uint32_t OUT;
  // CLR (Clear) Register. Address offset: 0x0c.
  volatile uint32_t CLR;
  // SET (Set) Register. Address offset: 0x10.
  volatile uint32_t SET;
} GpioController;

// Enumeration with the possible logic values of a GPIO pin (`LOW` or `HIGH`)
enum GpioLogicValue
{
  // Logic `LOW` (0)
  LOW = 0,
  // Logic `HIGH` (1)
  HIGH = 1
};

/**
 * @brief Set a GPIO pin to work as an output.
 *
 * @param gpio Pointer to the GpioController
 * @param pin_id The ID of the GPIO pin to set as an output. Note that IDs start at 0.
 */
static inline void gpio_set_output(GpioController *gpio, const uint32_t pin_id)
{
  SET_BIT(gpio->OE, pin_id);
}

/**
 * @brief Set a GPIO pin to work as an input.
 *
 * @param gpio Pointer to the GpioController
 * @param pin_id The ID of the GPIO pin to set as an input. Note that IDs start at 0.
 */
static inline void gpio_set_input(GpioController *gpio, const uint32_t pin_id)
{
  CLR_BIT(gpio->OE, pin_id);
}

/**
 * @brief Return the current logic state of a GPIO pin, either 0 or 1.
 *
 * @param gpio Pointer to the GpioController
 * @param pin_id The ID of the GPIO pin to read. Note that IDs start at 0.
 * @return uint32_t
 */
static inline uint32_t gpio_read(GpioController *gpio, const uint32_t pin_id)
{
  return READ_BIT(gpio->IN, pin_id);
}

/**
 * @brief Set the logic value for a GPIO pin. The value can be either LOW (0) or HIGH (1). An
 * attempt to write to a pin set as input, or a value other than LOW or HIGH, is gracefully ignored
 * (no errors are given).
 *
 * @param gpio Pointer to the GpioController
 * @param pin_id The ID of the GPIO pin to write. Note that IDs start at 0.
 * @param value Either LOW (0) or HIGH (1)
 */
static inline void gpio_write(GpioController *gpio, const uint32_t pin_id,
                              enum GpioLogicValue value)
{
  if (value == LOW)
    gpio->CLR = 0x1U << pin_id;
  else if (value == HIGH)
    gpio->SET = 0x1U << pin_id;
}

/**
 * @brief Set the value of a GPIO pin to logic 1. An attempt to set a pin configured as input is
 * gracefully ignored (no errors are given).
 *
 * @param gpio Pointer to the GpioController
 * @param pin_id The ID of the GPIO pin to set. Note that IDs start at 0.
 */
static inline void gpio_set(GpioController *gpio, const uint32_t pin_id)
{
  gpio->SET = 0x1U << pin_id;
}

/**
 * @brief Set the value of a GPIO pin to logic 0. An attempt to clear a pin configured as input is
 * gracefully ignored (no errors are given).
 *
 * @param gpio Pointer to the GpioController
 * @param pin_id The ID of the GPIO pin to clear. Note that IDs start at 0.
 */
static inline void gpio_clear(GpioController *gpio, const uint32_t pin_id)
{
  gpio->CLR = 0x1U << pin_id;
}

/**
 * @brief Toggle the value of a GPIO pin. An attempt to toggle a pin configured as input is
 * gracefully ignored (no errors are given).
 *
 * @param gpio Pointer to the GpioController
 * @param pin_id The ID of the GPIO pin to toggle. Note that IDs start at 0.
 */
static inline void gpio_toggle(GpioController *gpio, const uint32_t pin_id)
{
  INV_BIT(gpio->OUT, pin_id);
}

/**
 * @brief Test whether a GPIO pin is logic HIGH (1). Both input and output pins can be tested.
 *
 * @param gpio Pointer to the GpioController
 * @param pin_id
 * @return true
 * @return false
 */
static inline bool gpio_is_set(GpioController *gpio, const uint32_t pin_id)
{
  return READ_BIT(gpio->IN, pin_id) != 0;
}

/**
 * @brief Test whether a GPIO pin is logic LOW (0). Both input and output pins can be tested.
 *
 * @param gpio Pointer to the GpioController
 * @param pin_id
 * @return true
 * @return false
 */
static inline bool gpio_is_clear(GpioController *gpio, const uint32_t pin_id)
{
  return READ_BIT(gpio->IN, pin_id) == 0;
}

/**
 * @brief Set a group of GPIO pins to work as outputs at once. A bit mask is used to select
 * the appropriate pins.
 *
 * @param gpio Pointer to the GpioController
 * @param bit_mask A bit mask indicating which GPIO pins to set as outputs. Example:
 * 0b00010010 sets gpio[1] and gpio[4] as outputs.
 */
static inline void gpio_set_output_group(GpioController *gpio, const uint32_t bit_mask)
{
  SET_FLAG(gpio->OE, bit_mask);
}

/**
 * @brief Set a group of GPIO pins to work as inputs at once. A bit mask is used to select the
 * appropriate pins.
 *
 * @param gpio Pointer to the GpioController
 * @param bit_mask A bit mask indicating which GPIO pins to set as inputs. Example:
 * 0b00001100 sets gpio[2] and gpio[3] as inputs.
 */
static inline void gpio_set_input_group(GpioController *gpio, const uint32_t bit_mask)
{
  CLR_FLAG(gpio->OE, bit_mask);
}

/**
 * @brief Return a bit vector with the current logic state of all GPIO pins.
 *
 * @param gpio Pointer to the GpioController
 * @return uint32_t
 */
static inline uint32_t gpio_read_all(GpioController *gpio)
{
  return gpio->IN;
}

/**
 * @brief Set the logic values for a group of GPIO pins. The values for the output pins are
 * set at once from the value mask provided. Pins set as inputs will hold their values and are not
 * affected.
 *
 * @param gpio Pointer to the GpioController
 * @param value_mask A bit mask with the logic values for the GPIO pins. Example: 0b00010000
 * sets gpio[4] to 1 and all remaining pins to 0 (assuming GPIO pins 0 to 7 were previously set as
 * outputs).
 */
static inline void gpio_write_group(GpioController *gpio, const uint32_t value_mask)
{
  gpio->OUT = value_mask;
}

/**
 * @brief Set a group of GPIO pins to logic 1. The output pins are set to 1 at once and selected
 * from the bit mask provided. Pins set as inputs will hold their values and are not affected.
 *
 * @param gpio Pointer to the GpioController
 * @param bit_mask A bit mask indicating which GPIO pins must have their values set to logic 1.
 * Example: 0b00010000 sets gpio[4] to 1, all remaining pins keep their current values.
 */
static inline void gpio_set_group(GpioController *gpio, const uint32_t bit_mask)
{
  gpio->SET = bit_mask;
}

/**
 * @brief Set a group of GPIO pins to logic 0. The output pins are set to 0 at once and selected
 * from the bit mask provided. Pins set as inputs will hold their values and are not affected.
 *
 * @param gpio Pointer to the GpioController
 * @param bit_mask A bit mask indicating which GPIO pins must have their values set to logic 0.
 * Example: 0b00010000 sets gpio[4] to 0, all remaining pins keep their current values.
 */
static inline void gpio_clear_group(GpioController *gpio, const uint32_t bit_mask)
{
  gpio->CLR = bit_mask;
}

/**
 * @brief Toggle the value of a group of GPIO pins. The output pins are toggled at once and selected
 * from the bit mask provided. Pins set as inputs will hold their values and are not affected.
 *
 * @param gpio Pointer to the GpioController
 * @param bit_mask A bit mask indicating which GPIO pins must have their values toggled.
 * Example: 0b00010000 toggles gpio[4] from 0->1 or 1->0, depending on its current value. All
 * remaining pins keep their current values.
 */
static inline void gpio_toggle_group(GpioController *gpio, const uint32_t bit_mask)
{
  INV_FLAG(gpio->OUT, bit_mask);
}

#endif // __LIBSTEEL_GPIO__
