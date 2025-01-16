#pragma once

// ##############################################################################################
// MCU Select:

#include "mcu_select.h"

/*
    If there is not exist mcu_select.h at beside of this header file, Create it and put this bellow following content. 
    Then select your desired MCU that want work with.
*/
// ----------------------------------------------------------------
// mcu_select.h file:

// Define the target MCU family here
// Uncomment the desired MCU family definition below:

// #define STM32F1
// #define STM32F4
// #define STM32H7

// ----------------------------------------------------------------
// ##################################################################
// Include libraaries:

#if defined(STM32F1)
#include "stm32f1xx_hal.h"      // HAL library for STM32F1 series
#elif defined(STM32F4)
#include "stm32f4xx_hal.h"      // HAL library for STM32F4 series
#elif defined(STM32H7)
#include "stm32h7xx_hal.h"      // HAL library for STM32H7 series
#else
#error "Unsupported MCU family. Please define a valid target (e.g., STM32F1, STM32F4, STM32H7)."
#endif

#include <string>               // Include the standard string library for error handling and messages

// ####################################################################
// LimitSwitch object

/**
 * @class LimitSwitch
 * @brief The class for limitswitch/microswitch management.
 * @note - For interrupt mode, it is recomended Config external interrupt outside of this class.
 */
class LimitSwitch
{
    public:

        /// @brief Last error occurred for the object.
        std::string errorMessage;

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

    private:

        /// @brief The init state for init successful or not.
        bool _initState;

        /**
         * @brief GPIO pin state.
         */
        volatile bool _state;
        
        /**
         * @brief GPIO port. 
         */
        GPIO_TypeDef *_GPIO_PORT;

        /**
         * @brief GPIO pin. It can be GPIO_PIN_0, GPIO_PIN_1, ... 
         */
        uint16_t _GPIO_PIN;

        /// @brief Sensor active mode. 0: Active low (Default), 1: Active high. 
        uint8_t _ACTIVE_MODE;

        /// @brief Sensor pullup/pulldown mode. PUD_OFF:0 (Default), PUD_UP:1, PUD_DOWN:2 
        uint8_t _PUD_MODE;

        /// @brief External interupt enabled/disabled. true: enabled.
        bool _interruptMode;

        /**
         * @brief Enable RCC GPIO PORT for certain port.
         */
        bool _RCC_GPIO_CLK_ENABLE(GPIO_TypeDef *GPIO_PORT);
};