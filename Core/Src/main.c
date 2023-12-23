/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "dma2d.h"
#include "fatfs.h"
#include "jpeg.h"
#include "sdmmc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "usb_device.h"
#include "gpio.h"
#include "fmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include "usbd_cdc_if.h"
#include "ff.h"
#include "ffconf.h"
//#include "ST7789_Driver.h"
#include "lvgl.h"
#include "lv_port_disp.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define one_clk_time	0.00416666f
#define SDRAM_Size 		16*1024*1024
#define SDRAM_BASEADDRESS	0xC0000000
#define DTCM_BASEADDRESS	0x20000000
#define CHUNK_SIZE_IN  ((uint32_t)(64 * 1024))  // µ¥´Î½âÂëÊäÈëÊý¾Ý×î´ó³¤¶È
#define CHUNK_SIZE_OUT ((uint32_t)(64 * 1024))  // µ¥´Î½âÂëÊä³öÊý¾Ý×î´ó³¤¶È

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void PeriphCommonClock_Config(void);
static void MPU_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#define	p_buf

#ifdef	p_buf
	uint16_t *buf = (uint16_t *)0xC0000000;
#else
	SDRAM uint16_t buf[SDRAM_Size/2] = {0};
#endif
uint32_t	tim_start,tim_end;
uint32_t	tim_long;
float		time_us;
uint32_t Get_TIM_CNT(TIM_HandleTypeDef htim)
{
	return	htim.Instance->CNT;
}


uint8_t SDRAM_Test(void)
{
	volatile uint32_t  i = 0;			// ¼ÆÊý±äÁ¿
	uint32_t k = 0;
	uint16_t ReadData = 0; 	// ¶ÁÈ¡µ½µÄÊý¾Ý
	uint32_t Err_Num = 0;

	float    ExecutionSpeed;			// Ö´ÐÐËÙ¶È
	htim2.Instance->CNT = 0;
	tim_start = Get_TIM_CNT(htim2);
	HAL_TIM_Base_Start(&htim2);
	HAL_Delay(100);
	tim_end = Get_TIM_CNT(htim2);
	HAL_TIM_Base_Stop(&htim2);
	tim_long = tim_end - tim_start;
	time_us = tim_long * one_clk_time;
	usb_printf("\r\n delay %.2f us!\r\n", time_us);
	HAL_Delay(10);

	usb_printf("\r\n SDRAM Write an Read test start!\r\n");
	HAL_Delay(10);

	htim2.Instance->CNT = 0;
	tim_start = Get_TIM_CNT(htim2);
	HAL_TIM_Base_Start(&htim2);
	for (k = 0; k < SDRAM_Size/2; k++)
	{
		buf[k] = (uint16_t)k;
	}
	tim_end = Get_TIM_CNT(htim2);
	HAL_TIM_Base_Stop(&htim2);
	tim_long = tim_end - tim_start;
	time_us = tim_long * one_clk_time;
	ExecutionSpeed = (float)SDRAM_Size /1024/1024 /time_us*1000*1000 ;

	usb_printf("\r\nWrite Speed is %.2f MB/s\r\n",ExecutionSpeed);
	HAL_Delay(10);


	htim2.Instance->CNT = 0;
	tim_start = Get_TIM_CNT(htim2);
	HAL_TIM_Base_Start(&htim2);
	for(i = 0; i < SDRAM_Size/2;i++ )
	{
		ReadData = buf[i];  // ´ÓSDRAM¶Á³öÊý¾Ý
	}
	tim_end = Get_TIM_CNT(htim2);
	HAL_TIM_Base_Stop(&htim2);
	tim_long = tim_end - tim_start;
	time_us = tim_long * one_clk_time;


	ExecutionSpeed = (float)SDRAM_Size /1024/1024 /time_us*1000*1000 ;

	usb_printf("\r\nRead Speed is %.2f MB/s\r\n",ExecutionSpeed);
	HAL_Delay(10);


	usb_printf("\r\nstart 16bits test\r\n");
	HAL_Delay(10);
	for(i = 0; i < SDRAM_Size/2;i++ )
	{
		ReadData = buf[i];
		if( ReadData != (uint16_t)i )
		{
			Err_Num++;
		}
	}
	if(Err_Num != 0){
		usb_printf("\r\nSDRAM test failed , %d\r\n", Err_Num);
		HAL_Delay(10);
		return ERROR;
	}

	usb_printf("SDRAM OK\r\n");
	return SUCCESS;
}


static FATFS fatfs;
char wr_buf[256] = "Hello, World!\r\n你干嘛哎哟\r\n";
char rd_buf[256];

FATFS 	SD_FatFs; 		// ÎÄ¼þÏµÍ³¶ÔÏó
FRESULT 	MyFile_Res;    // ²Ù×÷½á¹û




int FATFS_Init()
{
	FRESULT status;
	//TCHAR *Path = "0:/";
	unsigned char work[512];

	status = f_mount(&fatfs, "0:", 1);  //
	if(status != FR_OK){
		status = f_mkfs("0:", FM_FAT32, 0, work, sizeof work);
		//status = f_mkfs(0, 0, 4096);
		if(status != FR_OK){
			return HAL_ERROR;
		}

		status = f_mount(&fatfs, "0:", 1);
		if(status != FR_OK){
			return HAL_ERROR;
		}
	}

	return HAL_OK;
}

int SD_WriteByte(char *file_name, void *pdata, uint32_t byte_len)
{
	FIL fil; //
	UINT bw; //
	FRESULT status;

	status = f_open(&fil, file_name, FA_CREATE_ALWAYS | FA_WRITE);

	f_lseek(&fil, 0);

	f_write(&fil, pdata, byte_len, &bw);

	f_close(&fil);

	return status;
}



int SD_ReadByte(char *file_name, void *pdata, uint32_t byte_len)
{
	FIL fil; //
	UINT br; //
	FRESULT status;

	status = f_open(&fil, file_name, FA_OPEN_EXISTING | FA_READ);

	f_lseek(&fil, 0);

	status = f_read(&fil, pdata, byte_len, &br);

	f_close(&fil);

	return status;
}




int SD_ReadFile(char *file_name, void *pdata, uint32_t *byte_len)
{
	FIL fil; //
	UINT br; //

	f_open(&fil, file_name, FA_OPEN_EXISTING | FA_READ);

	f_lseek(&fil, 0);

	f_read(&fil, pdata, fil.obj.objsize, &br);

	f_close(&fil);
	*byte_len = br;//fil.obj.objsize;
	return HAL_OK;
}


int SD_CreateDir(char *dir_name)
{
	FRESULT status;
	status = f_mkdir(dir_name);
	if(status != FR_OK)
		return HAL_ERROR;

	return HAL_OK;
}

uint32_t SD_CardCapacity = 0;		//SD¿¨µÄ×ÜÈÝÁ¿
uint32_t SD_FreeCapacity = 0;		//SD¿¨¿ÕÏÐÈÝÁ¿
void FatFs_GetVolume(void)	// ¼ÆËãÉè±¸ÈÝÁ¿
{
	FATFS *fs;		//¶¨Òå½á¹¹ÌåÖ¸Õë

	DWORD fre_clust, fre_sect, tot_sect; 	//

	f_getfree("0:",&fre_clust,&fs);			//

	tot_sect = (fs->n_fatent-2) * fs->csize;	//
	fre_sect = fre_clust * fs->csize;			//

	SD_CardCapacity = tot_sect / 2048 ;	//
	SD_FreeCapacity = fre_sect / 2048 ;	//
}

FRESULT status;
uint8_t pic_buf[100*1024];
uint8_t *pic_decode_buf = (uint8_t *)SDRAM_BASEADDRESS;
uint8_t *pic_rgb888 = (uint8_t *)(SDRAM_BASEADDRESS + 3*1024*1024);
uint32_t pic_size;
uint8_t x_offset=0;
int		slider1_value = 0;
uint8_t r=0,g=0,b=0;
//uint16_t frame[LCD_Pixel];
//uint16_t *frame = (uint16_t *)SDRAM_BASEADDRESS;

static lv_obj_t * scr1;
static lv_obj_t * scr2;
static lv_obj_t * scr3;
static lv_style_t style1, style2, style3;  //创建style
lv_obj_t * label;
lv_obj_t * label2;
lv_obj_t * label3;
lv_obj_t * label5;
lv_obj_t * slider1;
static lv_color_t bg_color;

void lv_scr_init(void)
{
	scr1 = lv_obj_create(NULL);//创建屏幕
	scr2 = lv_obj_create(NULL);
	scr3 = lv_obj_create(NULL);



	lv_style_init(&style1);                          //初始化style
	lv_style_set_radius(&style1, 5);                  //设置样式的圆角
	lv_style_set_bg_opa(&style1, LV_OPA_COVER);       //设置样式透明度
	bg_color = lv_color_make(255, 255, 255);
	lv_style_set_bg_color(&style1, bg_color);  //设置主背景颜色 调色板减轻，后面参数越高则也淡
	lv_obj_add_style(scr1, &style1,0);

	lv_style_init(&style2);                          //初始化style
	lv_style_set_radius(&style2, 5);                  //设置样式的圆角
	lv_style_set_bg_opa(&style2, LV_OPA_COVER);       //设置样式透明度
	bg_color = lv_color_make(0, 255, 0);
	lv_style_set_bg_color(&style2, bg_color);  //设置主背景颜色 调色板减轻，后面参数越高则也淡
	lv_obj_add_style(scr2, &style2, 0);

	lv_style_init(&style3);                          //初始化style
	lv_style_set_radius(&style3, 5);                  //设置样式的圆角
	lv_style_set_bg_opa(&style3, LV_OPA_COVER);       //设置样式透明度
	bg_color = lv_color_make(0, 0, 255);
	lv_style_set_bg_color(&style3, bg_color);  //设置主背景颜色 调色板减轻，后面参数越高则也淡
	lv_obj_add_style(scr3, &style3, 0);

}


void lv_ex_label(void)
{

	/* 屏幕1控件 */
	label = lv_label_create(scr1);
    lv_label_set_recolor(label, true);
    lv_label_set_long_mode(label, LV_LABEL_LONG_CLIP); /*Circular scroll*/
    lv_obj_set_width(label, 240);
    lv_obj_set_height(label, 20);
    lv_label_set_text_fmt(label, "#00ff00 Hello, World!#");
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 40);


    LV_FONT_DECLARE(Font_16);//声明字库
    label2 = lv_label_create(scr1);
    lv_label_set_recolor(label2, true);
    lv_label_set_long_mode(label2, LV_LABEL_LONG_CLIP); /*Circular scroll*/
    lv_obj_set_width(label2, 240);
    lv_obj_set_height(label2, 20);
    lv_label_set_text_fmt(label2, "#ff0000 你好，世界！#");
    lv_obj_align(label2, LV_ALIGN_CENTER, 0, -40);
    lv_obj_set_style_text_font(label2, &Font_16, LV_PART_MAIN | LV_STATE_DEFAULT);

    LV_FONT_DECLARE(Font_japenese_16);
    label3 = lv_label_create(scr1);
    lv_label_set_recolor(label3, true);
    lv_label_set_long_mode(label3, LV_LABEL_LONG_CLIP); /*Circular scroll*/
    lv_obj_set_width(label3, 240);
    lv_obj_set_height(label3, 20);
    lv_label_set_text_fmt(label3, "#0000ff ハローワールド#");
    lv_obj_align(label3, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_text_font(label3, &Font_japenese_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    //lv_obj_set_pos(label3, 0, 0);

//    lv_obj_t * label4 = lv_label_create(lv_scr_act());
//    lv_obj_set_scrollbar_mode(label4, LV_SCROLLBAR_​​MODE_ON);
//    static lv_style_t style_red;
//    lv_style_init(&style_red);
//    lv_style_set_bg_color(&style_red, lv_color_red());
//    lv_obj_add_style(label4, &style_red, LV_PART_SCROLLBAR);


    /* 屏幕2控件 */
    slider1 = lv_slider_create(scr2);
    lv_obj_set_width(slider1, 100);
    lv_obj_set_height(slider1, 16);
    lv_slider_set_range(slider1, 0, 100);	   				/*Set the min. and max. values*/
    lv_slider_set_value(slider1, slider1_value, LV_ANIM_OFF);		/*Set the current value (position)*/
    //lv_obj_set_pos(slider1, 0, 40);
    lv_obj_align(slider1, LV_ALIGN_CENTER, 0, -50);



    LV_FONT_DECLARE(Font_16);//声明字库
    label5 = lv_label_create(scr2);
    lv_label_set_recolor(label5, true);
    lv_label_set_long_mode(label5, LV_LABEL_LONG_CLIP); /*Circular scroll*/
    lv_obj_set_width(label5, 240);
    lv_obj_set_height(label5, 20);
    lv_label_set_text_fmt(label5, "#ff0000 滚动文字#");
    lv_obj_align(label5, LV_ALIGN_CENTER, 0, -20);
    lv_obj_set_style_text_font(label5, &Font_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    static lv_style_t style_red;
    lv_style_init(&style_red);
	bg_color = lv_color_make(255, 0, 0);
    lv_style_set_bg_color(&style_red, bg_color);
    lv_obj_add_style(label5, &style_red, LV_PART_SCROLLBAR);
    lv_obj_set_scrollbar_mode(label5, LV_SCROLLBAR_MODE_ON);
    lv_obj_set_scroll_dir(label5, LV_DIR_VER);
    lv_obj_set_scroll_snap_x(label5, LV_SCROLL_SNAP_START);
    /* 屏幕3控件 */

}



uint32_t running_time_ms = 0;


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim == &htim2)
	{
		running_time_ms++;
		lv_tick_inc(1);
	}
}









/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MPU Configuration--------------------------------------------------------*/
  MPU_Config();

  /* Enable I-Cache---------------------------------------------------------*/
  SCB_EnableICache();

  /* Enable D-Cache---------------------------------------------------------*/
  SCB_EnableDCache();

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

/* Configure the peripherals common clocks */
  PeriphCommonClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_FMC_Init();
  MX_JPEG_Init();
  MX_SDMMC1_SD_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_USART1_UART_Init();
  MX_FATFS_Init();
  MX_DMA_Init();
  MX_SPI5_Init();
  MX_USB_DEVICE_Init();
  MX_DMA2D_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim2);
//  FATFS_Init();
//  LCD_Init();
//  FatFs_GetVolume();
//  status = SD_WriteByte("0:ABC.txt", wr_buf, 256);
//  HAL_Delay(2);
//  status = SD_ReadByte("0:ABC.txt", rd_buf, 256);
//  status = SD_ReadFile("0:ying.jpg", pic_buf, &pic_size);
//
//  HAL_JPEG_Decode(&hjpeg, pic_buf, CHUNK_SIZE_IN, pic_decode_buf, CHUNK_SIZE_OUT, 2000);
//  HAL_JPEG_Decode(&hjpeg, pic_buf+CHUNK_SIZE_IN, pic_size-CHUNK_SIZE_IN, pic_decode_buf+CHUNK_SIZE_OUT, pic_size-CHUNK_SIZE_IN, 2000);
//  HAL_DMA2D_Start(&hdma2d, (uint32_t)pic_decode_buf, (uint32_t)pic_rgb888, 890, 988);
//  memcpy(rd_buf, pic_rgb888, 256);
  lv_init();
  lv_port_disp_init();
  lv_scr_init();

  lv_scr_load(scr1);
  lv_ex_label();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  //SDRAM_Test();

	  /*
	  for(int i=0; i<LCD_Pixel; i++)
		  frame[i] = RED_MASK;
	  LCD_Refresh(frame);
	  for(int i=0; i<LCD_Pixel; i++)
		  frame[i] = GREEN_MASK;
	  LCD_Refresh(frame);
	  for(int i=0; i<LCD_Pixel; i++)
		  frame[i] = BLUD_MASK;
	  LCD_Refresh(frame);

	  for(int i=0; i<LCD_Pixel/2; i++)
		  frame[i] = BLUD_MASK;
	  for(int i=LCD_Pixel/2; i<LCD_Pixel; i++)
		  frame[i] = RED_MASK;
	  LCD_Refresh(frame);*/
	  lv_label_set_text_fmt(label, "#00ff00 See you again!#");
	  lv_task_handler();//用来刷新控件的，不用的话控件就不更新
	  HAL_Delay(1000);
	  lv_label_set_text_fmt(label, "#00ff00 Hello, World!#");
	  lv_task_handler();
	  HAL_Delay(1000);
	  lv_label_set_text_fmt(label, "#00ff00 FUCK, World!#");
	  lv_task_handler();
	  HAL_Delay(1000);
	  //lv_obj_clean(scr1);
	  //lv_ex_label();

	  //lv_ex_label();
	  //lv_timer_handler();



  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);
  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}
  /** Macro to configure the PLL clock source
  */
  __HAL_RCC_PLL_PLLSOURCE_CONFIG(RCC_PLLSOURCE_HSE);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 5;
  RCC_OscInitStruct.PLL.PLLN = 192;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 48;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief Peripherals Common Clock Configuration
  * @retval None
  */
void PeriphCommonClock_Config(void)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Initializes the peripherals clock
  */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADC|RCC_PERIPHCLK_SPI5;
  PeriphClkInitStruct.PLL2.PLL2M = 5;
  PeriphClkInitStruct.PLL2.PLL2N = 120;
  PeriphClkInitStruct.PLL2.PLL2P = 10;
  PeriphClkInitStruct.PLL2.PLL2Q = 5;
  PeriphClkInitStruct.PLL2.PLL2R = 2;
  PeriphClkInitStruct.PLL2.PLL2RGE = RCC_PLL2VCIRANGE_2;
  PeriphClkInitStruct.PLL2.PLL2VCOSEL = RCC_PLL2VCOWIDE;
  PeriphClkInitStruct.PLL2.PLL2FRACN = 0;
  PeriphClkInitStruct.Spi45ClockSelection = RCC_SPI45CLKSOURCE_PLL2;
  PeriphClkInitStruct.AdcClockSelection = RCC_ADCCLKSOURCE_PLL2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* MPU Configuration */

void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct = {0};

  /* Disables the MPU */
  HAL_MPU_Disable();
  /** Initializes and configures the Region and the memory to be protected
  */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.BaseAddress = 0x24000000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_512KB;
  MPU_InitStruct.SubRegionDisable = 0x0;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL1;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  /** Initializes and configures the Region and the memory to be protected
  */
  MPU_InitStruct.Number = MPU_REGION_NUMBER1;
  MPU_InitStruct.BaseAddress = 0x30000000;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  /** Initializes and configures the Region and the memory to be protected
  */
  MPU_InitStruct.Number = MPU_REGION_NUMBER2;
  MPU_InitStruct.BaseAddress = 0x38000000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_64KB;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  /** Initializes and configures the Region and the memory to be protected
  */
  MPU_InitStruct.Number = MPU_REGION_NUMBER3;
  MPU_InitStruct.BaseAddress = 0xC0000000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_16MB;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  /** Initializes and configures the Region and the memory to be protected
  */
  MPU_InitStruct.Number = MPU_REGION_NUMBER4;
  MPU_InitStruct.BaseAddress = 0x90000000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_8MB;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  /* Enables the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);

}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

