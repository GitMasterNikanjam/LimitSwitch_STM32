
#include "LimitSwitch.h"


void LimitSwitch::clean(void)
{
    if(_initState == false)
    {
        return;
    }

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = _GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    GPIO_InitStruct.Pull = GPIO_NOPULL;

    HAL_GPIO_Init(_GPIO_PORT, &GPIO_InitStruct);

    _state = false;
    _GPIO_PORT = nullptr;
    _ACTIVE_MODE = 0;
    _PUD_MODE = 0;
    _initState = false;
}

bool LimitSwitch::read(void)
{
    if(_initState == false)
    {
        return false;
    }

    uint8_t pinState = HAL_GPIO_ReadPin(_GPIO_PORT, _GPIO_PIN);

    if( pinState == _ACTIVE_MODE)
    {
        _state = true;
        return true;
    }

    _state = false;
    return false;
}

LimitSwitch::LimitSwitch(GPIO_TypeDef *port, uint16_t pin, uint8_t activeMode, uint8_t pudMode, bool interruptMode)
{
    _GPIO_PORT = port;
    _GPIO_PIN = pin;
    _interruptMode = interruptMode;

    if(activeMode <= 1)
    {
        _ACTIVE_MODE = activeMode;
    }
    else
    {
        _ACTIVE_MODE = 0;
    }
    
    if(pudMode <= 2)
    {
        _PUD_MODE = pudMode;
    }
    else
    {
        _PUD_MODE = 0;
    }

    _state = false;
    _initState = false;
}

bool LimitSwitch::init(void)
{
    if( !((_GPIO_PIN == GPIO_PIN_0) || (_GPIO_PIN == GPIO_PIN_1) || (_GPIO_PIN == GPIO_PIN_2) || (_GPIO_PIN == GPIO_PIN_3) ||
         (_GPIO_PIN == GPIO_PIN_4) || (_GPIO_PIN == GPIO_PIN_5) || (_GPIO_PIN == GPIO_PIN_6) || (_GPIO_PIN == GPIO_PIN_7) ||
         (_GPIO_PIN == GPIO_PIN_8) || (_GPIO_PIN == GPIO_PIN_9) || (_GPIO_PIN == GPIO_PIN_10) || (_GPIO_PIN == GPIO_PIN_11) ||
         (_GPIO_PIN == GPIO_PIN_12) || (_GPIO_PIN == GPIO_PIN_13) || (_GPIO_PIN == GPIO_PIN_14) || (_GPIO_PIN == GPIO_PIN_15)) )
    {
        errorMessage = "Error LimitSwitch: GPIO pin number is not correct.";
        return false;
    }

    if(_GPIO_PORT != nullptr)
    {
        GPIO_InitTypeDef GPIO_InitStruct = {0};
        if(_RCC_GPIO_CLK_ENABLE(_GPIO_PORT) == false)
        {
            return false;
        }

        GPIO_InitStruct.Pin = _GPIO_PIN;

        if(_interruptMode == false)
        {
            GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        }
        else
        {
            GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
        }
        
        GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
        switch(_PUD_MODE)
        {
            case GPIO_NOPULL:
                GPIO_InitStruct.Pull = GPIO_NOPULL;
            break;
            case GPIO_PULLUP:
                GPIO_InitStruct.Pull = GPIO_PULLUP;
            break;
            case GPIO_PULLDOWN:
                GPIO_InitStruct.Pull = GPIO_PULLDOWN;
            break;
            default:
                errorMessage = "Error LimitSwitch: GPIO PUD_MODE is not correct.";
                return false;
        }
        
        HAL_GPIO_Init(_GPIO_PORT, &GPIO_InitStruct);
    }
    else
    {
        errorMessage = "Error LimitSwitch: GPIO port can not be nullptr.";
        return false;
    }

    /* EXTI interrupt init*/
    switch(_GPIO_PIN)
    {
        case GPIO_PIN_0:
            HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
            HAL_NVIC_EnableIRQ(EXTI0_IRQn);
        break;
        case GPIO_PIN_1:
            HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
            HAL_NVIC_EnableIRQ(EXTI1_IRQn);
        break;
        case GPIO_PIN_2:
            HAL_NVIC_SetPriority(EXTI2_IRQn, 0, 0);
            HAL_NVIC_EnableIRQ(EXTI2_IRQn);
        break;
        case GPIO_PIN_3:
            HAL_NVIC_SetPriority(EXTI3_IRQn, 0, 0);
            HAL_NVIC_EnableIRQ(EXTI3_IRQn);
        break;
        case GPIO_PIN_4:
            HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
            HAL_NVIC_EnableIRQ(EXTI4_IRQn);
        break;
        default:
            if( (_GPIO_PIN == GPIO_PIN_5) || (_GPIO_PIN == GPIO_PIN_6) || (_GPIO_PIN == GPIO_PIN_7) || (_GPIO_PIN == GPIO_PIN_8) || (_GPIO_PIN == GPIO_PIN_9) )
            {
                HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
                HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
            }
            else if( (_GPIO_PIN == GPIO_PIN_10) || (_GPIO_PIN == GPIO_PIN_11) || (_GPIO_PIN == GPIO_PIN_12) || (_GPIO_PIN == GPIO_PIN_13) || (_GPIO_PIN == GPIO_PIN_14) || (_GPIO_PIN == GPIO_PIN_15) )
            {
                HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
                HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
            }
    }

    _initState = true;

    return true;
}

void LimitSwitch::EXTI_Callback(void)
{
    read();
}

bool LimitSwitch::_RCC_GPIO_CLK_ENABLE(GPIO_TypeDef *GPIO_PORT)
{
    if(GPIO_PORT == nullptr)
    {
        errorMessage = "Error LimitSwitch: GPIO port can not be nullptr.";
        return false;
    }

    #ifdef GPIOA
    if(GPIO_PORT == GPIOA)
    {
        __HAL_RCC_GPIOA_CLK_ENABLE();
    }
    #endif
    #ifdef GPIOB
    else if (GPIO_PORT == GPIOB)
    {
        __HAL_RCC_GPIOB_CLK_ENABLE();
    }
    #endif
    #ifdef GPIOC
    else if (GPIO_PORT == GPIOC)
    {
        __HAL_RCC_GPIOC_CLK_ENABLE();
    }
    #endif
    #ifdef GPIOD
    else if (GPIO_PORT == GPIOD)
    {
        __HAL_RCC_GPIOD_CLK_ENABLE();
    }
    #endif
    #ifdef GPIOE
    else if (GPIO_PORT == GPIOE)
    {
        __HAL_RCC_GPIOE_CLK_ENABLE();
    }
    #endif
    #ifdef GPIOF
    else if (GPIO_PORT == GPIOF)
    {
        __HAL_RCC_GPIOF_CLK_ENABLE();
    }
    #endif
    #ifdef GPIOG
    else if (GPIO_PORT == GPIOG)
    {
        __HAL_RCC_GPIOG_CLK_ENABLE();
    }
    #endif
    #ifdef GPIOH
    else if (GPIO_PORT == GPIOH)
    {
        __HAL_RCC_GPIOH_CLK_ENABLE();
    }
    #endif
    #ifdef GPIOI
    else if (GPIO_PORT == GPIOI)
    {
        __HAL_RCC_GPIOI_CLK_ENABLE();
    }
    #endif
    else
    {
        errorMessage = "Error LimitSwitch: GPIO port is not correct.";
        return false;
    }

    return true;
}