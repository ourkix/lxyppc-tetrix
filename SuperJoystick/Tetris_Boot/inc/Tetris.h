#ifndef     TETRIS_H
#define     TETRIS_H


#define     KEY_LEFT            1
#define     KEY_Left            1
#define     KEY_RIGHT           2
#define     KEY_Right           2
#define     KEY_UP              3
#define     KEY_Up              3
#define     KEY_DOWN            4
#define     KEY_Down            4
#define     KEY_PAUSE           5
#define     KEY_Pause           5
#define     TIME_50MS           6
#define     KEY_DROP            7
#define     KEY_Drop            7
#define     MSG_SWITCH          8
#define     KEY_1               9
#define     KEY_2               10
#define     KEY_3               11
#define     KEY_4               12
#define     KEY_L1              13
#define     KEY_L2              14
#define     KEY_R1              15
#define     KEY_R2              16
#define     KEY_1_UP            17
#define     KEY_1_DOWN          18
#define     KEY_2_UP            19
#define     KEY_2_DOWN          20
#define     KEY_3_UP            21
#define     KEY_3_DOWN          22
#define     KEY_4_UP            23
#define     KEY_4_DOWN          24
#define     KEY_PAUSE_UP        25
#define     KEY_PAUSE_DOWN      26
#define     KEY_SELECT_UP       27
#define     KEY_SELECT_DOWN     28
#define     MSG_ARR_LEFT        29
#define     MSG_ARR_Left        29
#define     MSG_ARR_RIGHT       30
#define     MSG_ARR_Right       30
#define     MSG_ARR_UP          31
#define     MSG_ARR_Up          31
#define     MSG_ARR_DOWN        32
#define     MSG_ARR_Down        32
#define     MSG_ARR_RELEASE     33

#define     MSG_DUMMY           0xFF


#ifdef      WIN32

#define      GetKey()               param //4 //(getchar(),4)

// Get a random value less than 16
#define     Rand32()          (rand()%32)

#else
#ifdef    USE_STM3210E_EVAL

#define     IsKeyLeft()         (!(GPIOA->IDR & GPIO_Pin_0))
#define     IsKeyUp()           (!(GPIOD->IDR & GPIO_Pin_3))
#define     IsKeyRight()        (!(GPIOA->IDR & GPIO_Pin_8))
#define     IsKeyDown()         (!(GPIOC->IDR & GPIO_Pin_13))
#define     IsKeyPause()        (0)     // No pause
#define     IsKeySelect()       (0)     // No select

#define     IsKeyL1()           (0)
#define     IsKeyL2()           (0)
#define     IsKeyR1()           (0)
#define     IsKeyR2()           (0)

#define     IsKey1()            (0)
#define     IsKey2()            (0)
#define     IsKey3()            (0)
#define     IsKey4()            (0)


#define     Led1On()            GPIOF->BRR = GPIO_Pin_6
#define     Led1Off()           GPIOF->BSRR = GPIO_Pin_6
#define     Led2On()            GPIOF->BRR = GPIO_Pin_7
#define     Led2Off()           GPIOF->BSRR = GPIO_Pin_7
#define     Led3On()            GPIOF->BRR = GPIO_Pin_8
#define     Led3Off()           GPIOF->BSRR = GPIO_Pin_8
#define     Led4On()            GPIOF->BRR = GPIO_Pin_9
#define     Led4Off()           GPIOF->BSRR = GPIO_Pin_9
#define     Led5On()            GPIOF->BRR = GPIO_Pin_10
#define     Led5Off()           GPIOF->BSRR = GPIO_Pin_10


#elif defined  (USE_STM3210B_EVAL)

#define     IsKeyLeft()         (!(GPIOD->IDR & GPIO_Pin_13))
#define     IsKeyUp()           (!(GPIOD->IDR & GPIO_Pin_14))
#define     IsKeyRight()        (!(GPIOD->IDR & GPIO_Pin_12))
#define     IsKeyDown()         (!(GPIOD->IDR & GPIO_Pin_15))
#define     IsKeyPause()        (!(GPIOD->IDR & GPIO_Pin_3))
#define     IsKeySelect()       (!(GPIOD->IDR & GPIO_Pin_4))

#define     Led1On()            GPIOC->BSRR = GPIO_Pin_7
#define     Led1Off()           GPIOC->BRR = GPIO_Pin_7
#define     Led2On()            GPIOC->BSRR = GPIO_Pin_6
#define     Led2Off()           GPIOC->BRR = GPIO_Pin_6

#elif defined (JOYSTICK)
#define     IsKeyL1()           (!(GPIOA->IDR & GPIO_Pin_6))
#define     IsKeyL2()           (!(GPIOA->IDR & GPIO_Pin_7))
#define     IsKeyR1()           (!(GPIOB->IDR & GPIO_Pin_6))
#define     IsKeyR2()           (!(GPIOA->IDR & GPIO_Pin_0))

#define     IsKey1()            (!(GPIOB->IDR & GPIO_Pin_5))
#define     IsKey2()            (!(GPIOB->IDR & GPIO_Pin_4))
#define     IsKey3()            (!(GPIOA->IDR & GPIO_Pin_15))
#define     IsKey4()            (!(GPIOA->IDR & GPIO_Pin_4))

#define     IsKeyLeft()         (!(GPIOB->IDR & GPIO_Pin_1))
#define     IsKeyUp()           (!(GPIOC->IDR & GPIO_Pin_14))
#define     IsKeyRight()        (!(GPIOB->IDR & GPIO_Pin_0))
#define     IsKeyDown()         (!(GPIOB->IDR & GPIO_Pin_10))
#define     IsKeyPause()        (!(GPIOC->IDR & GPIO_Pin_15))
#define     IsKeySelect()       (!(GPIOB->IDR & GPIO_Pin_7))

#define     Led1On()
#define     Led1Off()
#define     Led2On()
#define     Led2Off()
#define     Led3On()
#define     Led3Off()
#define     Led4On()
#define     Led4Off()
#define     Led5On()
#define     Led5Off()

#else 
#error Unknown device
#endif


#define      GetKey()               param //4 //(getchar(),4)

#include "stm32f10x_lib.h"
// Get a random value less than 16
#define     Rand32()          (SysTick->VAL &0x1F) //rand()%32

#endif

void    DebugDump();

typedef   enum
{
  GR_Update,
  GR_Score,
  GR_Over,
  GR_Move,
  GR_Pause,
  GR_Normal,
  GR_NoChange,
  GR_Init,
}GameResult;

typedef enum
{
  GS_Over,
  GS_Pause,
  GS_Normal,
}GameState;

GameResult     TetrisPlay(int param);

void  UpdateUI(GameResult result);

void    DisplayScoreLevel(void);
void    DisplayGameOver();
void    DisplayGamePause();

#endif
