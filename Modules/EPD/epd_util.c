#include "epd_util.h"

#include "epd.h"
#include "GUI_Paint.h"

#include <stdio.h>
#include <stdlib.h>

#include "main.h" // this feels WRONG

int DEV_Module_Init(void)
{
    HAL_GPIO_WritePin(EPD_DC_GPIO_Port, EPD_DC_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(EPD_CS_GPIO_Port, EPD_CS_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(EPD_RST_GPIO_Port, EPD_RST_Pin, GPIO_PIN_SET);

    return 0;
}

void DEV_Module_Exit(void)
{

    HAL_GPIO_WritePin(EPD_DC_GPIO_Port, EPD_DC_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(EPD_CS_GPIO_Port, EPD_CS_Pin, GPIO_PIN_RESET);

    //close 5V
    HAL_GPIO_WritePin(EPD_RST_GPIO_Port, EPD_RST_Pin, GPIO_PIN_RESET);
}

#define IMAGE_SIZE (((EPD_1IN54_V2_WIDTH + 7) / 8) * EPD_1IN54_V2_HEIGHT)

static uint8_t BlackImage[IMAGE_SIZE];

int EPD_PrintDateTime(RTC_TimeTypeDef *time, RTC_DateTypeDef *date, char *message) {

    //printf("Drawing\r\n");
    //1.Select Image
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);

    char buffer[32];

    snprintf(buffer, sizeof(buffer), "%02d-%02d-%02dm %02d:%02d:%02d\n", time->Hours, time->Minutes, time->Seconds, date->Date, date->Month, date->Year);

    // 2.Drawing on the image
    Paint_DrawString_EN_4x4Blocks(5, 85, buffer, &Font8, BLACK, WHITE);

    if (message) {
        Paint_DrawString_EN(5, 180, message, &Font8, BLACK, WHITE);
    }

    EPD_1IN54_V2_Display(BlackImage);

    return 0;
}

void EPD_SleepNoClear() {
    //printf("Goto Sleep...\r\n");
    EPD_1IN54_V2_Sleep();

    //printf("Setting RST pin to low...\r\n");
    DEV_Module_Exit();
}

void EPD_Init() {
    //printf("HW initializing e-ink display\r\n");
    DEV_Module_Init();

    //printf("e-Paper Init and Clear...\r\n");
    EPD_1IN54_V2_Init();
    EPD_1IN54_V2_Clear();
    HAL_Delay(500);

    //printf("Paint_NewImage\r\n");
    Paint_NewImage(BlackImage,
                   EPD_1IN54_V2_WIDTH,
                   EPD_1IN54_V2_HEIGHT,
                   270,
                   WHITE);
}

void EPD_Reinit() {
    //printf("Reinitializing e-ink display\r\n");
    DEV_Module_Init();

    //printf("e-Paper Init and Clear...\r\n");
    EPD_1IN54_V2_Init();
    EPD_1IN54_V2_Clear();
    HAL_Delay(500);
}


int EPD_Test(void)
{
    //printf("EPD_1in54_V2_test Demo\r\n");
    DEV_Module_Init();

    //printf("e-Paper Init and Clear...\r\n");
    EPD_1IN54_V2_Init();
    EPD_1IN54_V2_Clear();
    HAL_Delay(500);

    /* you have to edit the startup_stm32fxxx.s file and set a big enough heap size */
    //printf("Paint_NewImage\r\n");
    Paint_NewImage(BlackImage,
                   EPD_1IN54_V2_WIDTH,
                   EPD_1IN54_V2_HEIGHT,
                   270,
                   WHITE);

#if 0   //print image
    printf("show image for array\r\n");
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);

    EPD_1IN54_V2_Display(BlackImage);
    HAL_Delay(2000);
#endif

#if 1   //Draw on screen
    //printf("Drawing\r\n");
    //1.Select Image
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);

    // 2.Drawing on the image
    Paint_DrawString_EN_4x4Blocks(5, 85, "waveshare", &Font8, BLACK, WHITE);

    EPD_1IN54_V2_Display(BlackImage);
    HAL_Delay(2000);

#endif

    //printf("Clear...\r\n");
    EPD_1IN54_V2_Init();
    EPD_1IN54_V2_Clear();

    //printf("Goto Sleep...\r\n");
    EPD_1IN54_V2_Sleep();

    // close 5V
    //printf("close 5V, Module enters 0 power consumption ...\r\n");
    DEV_Module_Exit();

    return 0;
}