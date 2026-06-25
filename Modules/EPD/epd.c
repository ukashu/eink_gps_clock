/*
 * epd.c
 *
 *  Created on: Jan 30, 2025
 *      Author: ukashu
 */

#include "epd.h"
#include "spi.h"

// waveform full refresh
unsigned char WF_Full_1IN54[159] =
{
0x80,	0x48,	0x40,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
0x40,	0x48,	0x80,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
0x80,	0x48,	0x40,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
0x40,	0x48,	0x80,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
0xA,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
0x8,	0x1,	0x0,	0x8,	0x1,	0x0,	0x2,
0xA,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
0x22,	0x22,	0x22,	0x22,	0x22,	0x22,	0x0,	0x0,	0x0,
0x22,	0x17,	0x41,	0x0,	0x32,	0x20
};

// waveform partial refresh(fast)
unsigned char WF_PARTIAL_1IN54_0[159] =
{
0x0,0x40,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x80,0x80,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x40,0x40,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x80,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0xF,0x0,0x0,0x0,0x0,0x0,0x0,
0x1,0x1,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x22,0x22,0x22,0x22,0x22,0x22,0x0,0x0,0x0,
0x02,0x17,0x41,0xB0,0x32,0x28,
};

/******************************************************************************
function :	Software reset
parameter:
******************************************************************************/
static void EPD_1IN54_V2_Reset(void)
{
    HAL_GPIO_WritePin(EPD_RST_GPIO_Port, EPD_RST_Pin, GPIO_PIN_SET);
    HAL_Delay(50);
    HAL_GPIO_WritePin(EPD_RST_GPIO_Port, EPD_RST_Pin, GPIO_PIN_RESET);
    HAL_Delay(2);
    HAL_GPIO_WritePin(EPD_RST_GPIO_Port, EPD_RST_Pin, GPIO_PIN_SET);
    HAL_Delay(50);
}

static void EPD_1IN54_V2_SendCommand(uint8_t cmd) {
	HAL_GPIO_WritePin(EPD_DC_GPIO_Port, EPD_DC_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(EPD_CS_GPIO_Port, EPD_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, &cmd, 1, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(EPD_CS_GPIO_Port, EPD_CS_Pin, GPIO_PIN_SET);
}

static void EPD_1IN54_V2_SendData(uint8_t data) {
	HAL_GPIO_WritePin(EPD_DC_GPIO_Port, EPD_DC_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(EPD_CS_GPIO_Port, EPD_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, &data, 1, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(EPD_CS_GPIO_Port, EPD_CS_Pin, GPIO_PIN_SET);
}

static void EPD_1IN54_V2_ReadBusy(void) {

	while (1) {
		if (HAL_GPIO_ReadPin(EPD_BUSY_GPIO_Port, EPD_BUSY_Pin) == GPIO_PIN_RESET) {
			break;
		}
		//HAL_Delay(200);
	}

	HAL_Delay(200);
}

/******************************************************************************
function :	Turn On Display full
parameter:
******************************************************************************/
static void EPD_1IN54_V2_TurnOnDisplay(void)
{
    EPD_1IN54_V2_SendCommand(0x22);
    EPD_1IN54_V2_SendData(0xc7);
    EPD_1IN54_V2_SendCommand(0x20);
    EPD_1IN54_V2_ReadBusy();
}

/******************************************************************************
function :	Turn On Display part
parameter:
******************************************************************************/
static void EPD_1IN54_V2_TurnOnDisplayPart(void)
{
    EPD_1IN54_V2_SendCommand(0x22);
    EPD_1IN54_V2_SendData(0xcF);
    EPD_1IN54_V2_SendCommand(0x20);
    EPD_1IN54_V2_ReadBusy();
}

static void EPD_1IN54_V2_Lut(UBYTE *lut)
{
	EPD_1IN54_V2_SendCommand(0x32);
	for(UBYTE i=0; i<153; i++)
		EPD_1IN54_V2_SendData(lut[i]);
	EPD_1IN54_V2_ReadBusy();
}

static void EPD_1IN54_V2_SetLut(UBYTE *lut)
{
	EPD_1IN54_V2_Lut(lut);

    EPD_1IN54_V2_SendCommand(0x3f);
    EPD_1IN54_V2_SendData(lut[153]);

    EPD_1IN54_V2_SendCommand(0x03);
    EPD_1IN54_V2_SendData(lut[154]);

    EPD_1IN54_V2_SendCommand(0x04);
    EPD_1IN54_V2_SendData(lut[155]);
	EPD_1IN54_V2_SendData(lut[156]);
	EPD_1IN54_V2_SendData(lut[157]);

	EPD_1IN54_V2_SendCommand(0x2c);
    EPD_1IN54_V2_SendData(lut[158]);
}

static void EPD_1IN54_V2_SetWindows(UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend)
{
    EPD_1IN54_V2_SendCommand(0x44); // SET_RAM_X_ADDRESS_START_END_POSITION
    EPD_1IN54_V2_SendData((Xstart>>3) & 0xFF);
    EPD_1IN54_V2_SendData((Xend>>3) & 0xFF);

    EPD_1IN54_V2_SendCommand(0x45); // SET_RAM_Y_ADDRESS_START_END_POSITION
    EPD_1IN54_V2_SendData(Ystart & 0xFF);
    EPD_1IN54_V2_SendData((Ystart >> 8) & 0xFF);
    EPD_1IN54_V2_SendData(Yend & 0xFF);
    EPD_1IN54_V2_SendData((Yend >> 8) & 0xFF);
}

static void EPD_1IN54_V2_SetCursor(UWORD Xstart, UWORD Ystart)
{
    EPD_1IN54_V2_SendCommand(0x4E); // SET_RAM_X_ADDRESS_COUNTER
    EPD_1IN54_V2_SendData(Xstart & 0xFF);

    EPD_1IN54_V2_SendCommand(0x4F); // SET_RAM_Y_ADDRESS_COUNTER
    EPD_1IN54_V2_SendData(Ystart & 0xFF);
    EPD_1IN54_V2_SendData((Ystart >> 8) & 0xFF);
}

/******************************************************************************
function :	Initialize the e-Paper register
parameter:
******************************************************************************/
void EPD_1IN54_V2_Init(void)
{
    EPD_1IN54_V2_Reset();

    EPD_1IN54_V2_ReadBusy();
    EPD_1IN54_V2_SendCommand(0x12);  //SWRESET
    EPD_1IN54_V2_ReadBusy();

    EPD_1IN54_V2_SendCommand(0x01); //Driver output control
    EPD_1IN54_V2_SendData(0xC7);
    EPD_1IN54_V2_SendData(0x00);
    EPD_1IN54_V2_SendData(0x01);

    EPD_1IN54_V2_SendCommand(0x11); //data entry mode
    EPD_1IN54_V2_SendData(0x01);

    EPD_1IN54_V2_SetWindows(0, EPD_1IN54_V2_HEIGHT-1, EPD_1IN54_V2_WIDTH-1, 0);

    EPD_1IN54_V2_SendCommand(0x3C); //BorderWavefrom
    EPD_1IN54_V2_SendData(0x01);

    EPD_1IN54_V2_SendCommand(0x18);
    EPD_1IN54_V2_SendData(0x80);

    EPD_1IN54_V2_SendCommand(0x22); // //Load Temperature and waveform setting.
    EPD_1IN54_V2_SendData(0XB1);
    EPD_1IN54_V2_SendCommand(0x20);

    EPD_1IN54_V2_SetCursor(0, EPD_1IN54_V2_HEIGHT-1);
    EPD_1IN54_V2_ReadBusy();

    EPD_1IN54_V2_SetLut(WF_Full_1IN54);
}

/******************************************************************************
function :	Initialize the e-Paper register (Partial display)
parameter:
******************************************************************************/
void EPD_1IN54_V2_Init_Partial(void)
{
	EPD_1IN54_V2_Reset();
	EPD_1IN54_V2_ReadBusy();

	EPD_1IN54_V2_SetLut(WF_PARTIAL_1IN54_0);
	EPD_1IN54_V2_SendCommand(0x37);
	EPD_1IN54_V2_SendData(0x00);
	EPD_1IN54_V2_SendData(0x00);
	EPD_1IN54_V2_SendData(0x00);
	EPD_1IN54_V2_SendData(0x00);
	EPD_1IN54_V2_SendData(0x00);
	EPD_1IN54_V2_SendData(0x40);
	EPD_1IN54_V2_SendData(0x00);
	EPD_1IN54_V2_SendData(0x00);
	EPD_1IN54_V2_SendData(0x00);
	EPD_1IN54_V2_SendData(0x00);

	EPD_1IN54_V2_SendCommand(0x3C); //BorderWavefrom
	EPD_1IN54_V2_SendData(0x80);

	EPD_1IN54_V2_SendCommand(0x22);
	EPD_1IN54_V2_SendData(0xc0);
	EPD_1IN54_V2_SendCommand(0x20);
	EPD_1IN54_V2_ReadBusy();
}

/******************************************************************************
function :	Clear screen
parameter:
******************************************************************************/
void EPD_1IN54_V2_Clear(void)
{
    UWORD Width, Height;
    Width = (EPD_1IN54_V2_WIDTH % 8 == 0)? (EPD_1IN54_V2_WIDTH / 8 ): (EPD_1IN54_V2_WIDTH / 8 + 1);
    Height = EPD_1IN54_V2_HEIGHT;

    EPD_1IN54_V2_SendCommand(0x24);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_1IN54_V2_SendData(0XFF);
        }
    }
    EPD_1IN54_V2_SendCommand(0x26);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_1IN54_V2_SendData(0XFF);
        }
    }
    EPD_1IN54_V2_TurnOnDisplay();
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void EPD_1IN54_V2_Display(UBYTE *Image)
{
    UWORD Width, Height;
    Width = (EPD_1IN54_V2_WIDTH % 8 == 0)? (EPD_1IN54_V2_WIDTH / 8 ): (EPD_1IN54_V2_WIDTH / 8 + 1);
    Height = EPD_1IN54_V2_HEIGHT;

    UDOUBLE Addr = 0;
    EPD_1IN54_V2_SendCommand(0x24);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            Addr = i + j * Width;
            EPD_1IN54_V2_SendData(Image[Addr]);
        }
    }
    EPD_1IN54_V2_TurnOnDisplay();
}

/******************************************************************************
function :	 The image of the previous frame must be uploaded, otherwise the
		         first few seconds will display an exception.
parameter:
******************************************************************************/
void EPD_1IN54_V2_DisplayPartBaseImage(UBYTE *Image)
{
    UWORD Width, Height;
    Width = (EPD_1IN54_V2_WIDTH % 8 == 0)? (EPD_1IN54_V2_WIDTH / 8 ): (EPD_1IN54_V2_WIDTH / 8 + 1);
    Height = EPD_1IN54_V2_HEIGHT;

    UDOUBLE Addr = 0;
    EPD_1IN54_V2_SendCommand(0x24);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            Addr = i + j * Width;
            EPD_1IN54_V2_SendData(Image[Addr]);
        }
    }
    EPD_1IN54_V2_SendCommand(0x26);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            Addr = i + j * Width;
            EPD_1IN54_V2_SendData(Image[Addr]);
        }
    }
    EPD_1IN54_V2_TurnOnDisplayPart();
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void EPD_1IN54_V2_DisplayPart(UBYTE *Image)
{
    UWORD Width, Height;
    Width = (EPD_1IN54_V2_WIDTH % 8 == 0)? (EPD_1IN54_V2_WIDTH / 8 ): (EPD_1IN54_V2_WIDTH / 8 + 1);
    Height = EPD_1IN54_V2_HEIGHT;

    UDOUBLE Addr = 0;
    EPD_1IN54_V2_SendCommand(0x24);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            Addr = i + j * Width;
            EPD_1IN54_V2_SendData(Image[Addr]);
        }
    }
    EPD_1IN54_V2_TurnOnDisplayPart();
}
/******************************************************************************
function :	Enter sleep mode
parameter:
******************************************************************************/
void EPD_1IN54_V2_Sleep(void)
{
    EPD_1IN54_V2_SendCommand(0x10); //enter deep sleep
    EPD_1IN54_V2_SendData(0x01);
    HAL_Delay(100);
}


