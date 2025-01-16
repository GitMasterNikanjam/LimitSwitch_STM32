# LimitSwitch Library for STM32

- This library can be used for limitswitch/microswitch touch sensor management.
> <span style="color: yellow;"> Read library header file and select your MCU.

---------------------------------------------------------------------------------------

# Public Member Variables for LimitSwitch Class

```cpp
/// @brief Last error occurred for the object.
std::string errorMessage;
```

# Public Member Functions for LimitSwitch Class

```cpp

/**
* @brief Constructor. Set LimitSwitch port, pin and its input mode. Not apply setting on hardware.
* @note - init() method needs after this for apply setting on hardware.
* @param port: GPIO port. It can be GPIOA, GPIOB, ...
* @param pin: GPIO pin number. It can be GPIO_PIN_0, GPIO_PIN_1, ...
* @param activeMode: Sensor active mode. 0: Active low (Default), 1: Active high. 
* @param pudMode: Sensor pullup/pulldown mode. PUD_OFF:0 (Default), PUD_UP:1, PUD_DOWN:2 
* @param interruptMode: is the external interrupt mode enable/disable. The true value means it is enabled.
*/
LimitSwitch(GPIO_TypeDef *port, uint16_t pin, uint8_t activeMode = 0, uint8_t pudMode = 0, bool interruptMode = false);

/**
* @brief Apply setting on hardware. Start LimitSwitch action.
* @return true if successed.
*/
bool init(void);

/**
* @brief Clean setting on hardware. Stop  LimitSwitch action.
*/
void clean(void);

/**
 * @brief Read and return LimitSwitch sensor.
 * @note - Update sensor state value.
 * @return true if the sensor is touched or pressed.
 * @return false if the sensor is not touched or pressed.
 */
bool read(void);

/**
 * @brief Return last state of sensor limitswitch that updated.
 * @return true if the sensor is touched or pressed.
 * @return false if the sensor is not touched or pressed.
 * @note - This function useful when object is in interrupt mode.
 */
bool get(void) {return _state;};

/**
 * @brief External interrupt callback function.
 * @note - This funcition must implement in the **HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)** function.
 * @note - If object set in interrupt mode, then use this function, otherwise dont use.
 */
void EXTI_Callback(void);
```