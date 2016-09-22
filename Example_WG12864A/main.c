#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
#include "misc.h"

#include "tim2_delay.h"
#include "WG12864A.h"

/***************************************************************************//**
 * @brief  Selects HSE as System clock source and configure HCLK, PCLK2 and PCLK1 prescalers.
 ******************************************************************************/
void SetSysClockToHSE(void)
{
	ErrorStatus HSEStartUpStatus;

	/* SYSCLK, HCLK, PCLK2 and PCLK1 configuration -----------------------------*/
    /* RCC system reset(for debug purpose) */
    RCC_DeInit();

    /* Enable HSE */
    RCC_HSEConfig( RCC_HSE_ON);

    /* Wait till HSE is ready */
    HSEStartUpStatus = RCC_WaitForHSEStartUp();

    if (HSEStartUpStatus == SUCCESS)
    {
        /* Enable Prefetch Buffer */
        //FLASH_PrefetchBufferCmd( FLASH_PrefetchBuffer_Enable);

        /* Flash 0 wait state */
        //FLASH_SetLatency( FLASH_Latency_0);

        /* HCLK = SYSCLK */
        RCC_HCLKConfig( RCC_SYSCLK_Div1);

        /* PCLK2 = HCLK */
        RCC_PCLK2Config( RCC_HCLK_Div1);

        /* PCLK1 = HCLK */
        RCC_PCLK1Config(RCC_HCLK_Div1);

        /* Select HSE as system clock source */
        RCC_SYSCLKConfig( RCC_SYSCLKSource_HSE);

        /* Wait till PLL is used as system clock source */
        while (RCC_GetSYSCLKSource() != 0x04)
        {
        }
    }
    else
    { /* If HSE fails to start-up, the application will have wrong clock configuration.
     User can add here some code to deal with this error */

        /* Go to infinite loop */
        while (1)
        {
        }
    }
}

int main(void)
{
	const char *title ="WG12864";
	unsigned char test_text[] = "This is a test of";
	const unsigned char Picture[8][128] =
	{
	 {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xE0,0xE0,0xE0,0xE0,0xE0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xE0,0xE0,0xE0,0xE0,0xE0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xE0,0xE0,0xE0,0xE0,0xE0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0},
	 {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xC0,0xF8,0xFF,0xFF,0xFF,0xFF,0x1F,0x3,0x1F,0xFF,0xFF,0xFF,0xFF,0xF8,0xC0,0x0,0x0,0x0,0x4,0x3C,0xFC,0xFC,0xFC,0xF8,0x80,0x0,0x80,0xF8,0xFC,0xFC,0xFC,0x3C,0x4,0x0,0x0,0xFC,0xFC,0xFC,0xFC,0xFC,0x0,0x0,0x0,0xF0,0xF8,0xF8,0xFC,0xFC,0x9C,0x9C,0x9C,0x9C,0xBC,0x3C,0x38,0x30,0x20,0x0,0x0,0x0,0xFF,0xFF,0xFF,0xFF,0xFF,0x0,0x0,0x0,0x30,0x38,0x78,0x7C,0x7C,0x3C,0x1C,0x1C,0x9C,0xFC,0xFC,0xF8,0xF8,0xE0,0x0,0x0,0x0,0x0,0xFF,0xFF,0xFF,0xFF,0xFF,0x38,0x1C,0x1C,0x3C,0xFC,0xFC,0xF8,0xF0,0xC0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0},
	 {0x0,0x0,0x0,0x0,0x80,0xF0,0xFE,0xFF,0xFF,0xFF,0x7F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x7F,0xFF,0xFF,0xFF,0xFE,0xF0,0x80,0x0,0x0,0x1,0x7,0x3F,0xFF,0xFF,0xF8,0xFF,0xFF,0x3F,0x7,0x0,0x0,0x0,0x0,0x0,0xFF,0xFF,0xFF,0xFF,0xFF,0x0,0x0,0x10,0x70,0x79,0xFB,0xFB,0xF3,0xE3,0xE7,0xE7,0xE7,0xFF,0xFF,0x7F,0x3E,0x1C,0x0,0x0,0x0,0xFF,0xFF,0xFF,0xFF,0xFF,0x0,0x0,0x0,0x3C,0x7E,0xFE,0xFF,0xFF,0xE7,0xE7,0x63,0x73,0x7F,0xFF,0xFF,0xFF,0xFF,0x0,0x0,0x0,0x0,0xFF,0xFF,0xFF,0xFF,0xFF,0x70,0xE0,0xE0,0xF0,0xFF,0xFF,0x7F,0x3F,0xF,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x80,0x80,0xC0,0xE0,0xF0,0x70,0x38,0x38,0x10,0x0,0x0},
	 {0x0,0x0,0x0,0x0,0x0,0x80,0x80,0x0,0x0,0x0,0x0,0x80,0x80,0x80,0x0,0x0,0x80,0x30,0x20,0xB0,0x0,0x80,0x80,0x80,0x80,0x80,0x0,0x0,0x0,0x0,0x0,0x80,0x80,0x80,0x0,0x0,0x80,0x80,0x80,0x80,0x0,0x0,0x0,0x80,0x80,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x80,0xE0,0xF8,0xF8,0xFC,0xFC,0xFE,0xFF,0x3F,0x1F,0xF,0x3,0x1,0x0,0x0,0x0,0x0,0x0,0x0,0x0},
	 {0x0,0x0,0x0,0x0,0x7,0x8,0x8,0x5,0x0,0x0,0xC,0xA,0xA,0xF,0x0,0x0,0xF,0x6,0x1,0xF,0x0,0x0,0x0,0xF,0x0,0x0,0x0,0x0,0x0,0x0,0x1C,0xB,0x8,0xF,0x18,0x8,0x7,0x0,0x0,0xF,0x0,0x0,0xD,0x2,0xF,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x80,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x80,0xC0,0xE0,0xE0,0xF0,0x78,0x3C,0x1E,0xF,0x7,0x7,0x3,0x1,0x1,0x1,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0},
	 {0x0,0x0,0x0,0x0,0x3E,0x2,0x2,0x3E,0x0,0x0,0x30,0x2A,0x2A,0x3E,0x0,0x20,0x1E,0x2,0x2,0x3E,0x0,0x0,0x3E,0x1C,0x22,0x0,0x0,0x3E,0x18,0x4,0x3E,0x0,0x22,0x14,0x8,0x14,0x22,0x0,0x0,0x0,0x0,0x3E,0x2,0x2,0x3E,0x0,0x0,0x30,0x2A,0x2A,0x3E,0x0,0x0,0x34,0xA,0x3E,0x0,0x20,0x1E,0x2,0x2,0x3E,0x0,0x0,0x3E,0x28,0x28,0x10,0x0,0x0,0x3E,0x8,0x8,0x3E,0x0,0x0,0x3E,0x18,0x4,0x3E,0x0,0x0,0x3E,0x1C,0x22,0x0,0x0,0x3E,0x0,0x0,0x3E,0x2A,0x14,0x0,0x0,0x0,0x20,0x10,0x18,0xE,0x7,0x7,0x3,0x1,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0},
	 {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x80,0x0,0x0,0x0,0x0,0x0,0x0,0x80,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x80,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x80,0x0,0x80,0x0,0x0,0x0,0x0,0x0,0x80,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x80,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0},
	 {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1E,0x20,0x1C,0x2,0x1C,0x20,0x1E,0x1E,0x20,0x1C,0x2,0x1C,0x20,0x1E,0x1E,0x20,0x1C,0x2,0x1C,0x20,0x1E,0x0,0x20,0x0,0x0,0x30,0x2A,0x2A,0x3E,0x0,0x2,0x1C,0x20,0x1C,0x2,0x0,0x3E,0x0,0x24,0x2A,0x2A,0x12,0x0,0x3F,0x0,0x0,0x30,0x2A,0x2A,0x3E,0x0,0x0,0x3F,0x22,0x22,0x1C,0x0,0x0,0x20,0x0,0x0,0x1C,0x22,0x22,0x14,0x0,0x1C,0x22,0x22,0x1C,0x0,0x0,0x3E,0x2,0x2,0x3E,0x2,0x2,0x3C,0x0,0x30,0xE,0x1,0x0,0x3F,0x22,0x22,0x1C,0x0,0x0,0x3F,0x0,0x0,0x1C,0x22,0x22,0x1C,0x0,0x0,0x5C,0xA2,0xA2,0x7E,0x0,0x30,0xE,0x1,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}
	};

	SetSysClockToHSE();

	TIM2_init();
	LcdInit();
	LcdClear();
	LcdUpdate();
	delay_ms(100);

	// Pixels
	LcdPixel(127, 0, PIXEL_ON);
	LcdPixel(0, 63, PIXEL_ON);
	// Lines
	LcdLine(0, 0, 127, 63,  PIXEL_ON);
	// Circle
	LcdCircle(64, 32, 20, PIXEL_ON);
	// Rect
	LcdRect(40, 10, 88, 54,  PIXEL_ON);
	LcdUpdate();
	delay_ms(2000);
	LcdClear();
	LcdUpdate();

	// Text
	LcdGotoXYFont(2, 0);
	LcdStr(FONT_1X, test_text);
	LcdGotoXYFont(2, 1);
	LcdFStr(FONT_1X, (const unsigned char*)title);
	LcdIvertLineFragment(1, 2, 9);
	// Big Nimbers
	LcdGotoXYFont(9, 1);
	LcdStr(FONT_2X, (unsigned char*)"128x64");
	// Bigest Nimbers
	LcdGotoXYFont(0, 4);
	LcdStr(FONT_4X, (unsigned char*)"128x64");
	LcdUpdate();
	delay_ms(2000);

	// Picture
	LcdImage((const unsigned char *)Picture);
	LcdUpdate();

    while(1)
    {

    }
}