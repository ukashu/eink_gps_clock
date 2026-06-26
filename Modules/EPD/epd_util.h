#pragma once

int DEV_Module_Init(void);
void DEV_Module_Exit(void);
int EPD_Test(void);
int EPD_PrintDateTime();
void EPD_Init();
void EPD_Reinit();
void EPD_SleepNoClear();