#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include <math.h>
#include "arm_math.h"
#include "arm_const_structs.h"
#include "stm32f429i_discovery_lcd.h"
#include "lvgl.h"

#include "main.h"
#include "usart.h"


SemaphoreHandle_t UartSemaphore;


void SystemClock_Config(void);
HAL_StatusTypeDef os_printf(char *tag, char *str);
void lcd_init(void);

static void lv_tick_task(void * arg);
void gui_task(void *arg);
static void disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p);

void lv_ex_chart_1(void);


void IDEL_TASK(void *arg)
{
  while(1)
  {
    os_printf("IDEL" ,"task running");
    vTaskDelay(1000 / portTICK_RATE_MS);
  }
}


int main(void)
{
  HAL_Init();
  
  SystemClock_Config();
  MX_USART1_UART_Init();
  lcd_init();
  UartSemaphore = xSemaphoreCreateMutex();
  
  //xTaskCreate(showstr_task, "showstr_task", 2 * 1024, NULL, 6, NULL);
  // xTaskCreate(showgrap_task, "showgrap_task", 1 * 1024, NULL, 5, NULL);
  xTaskCreate(IDEL_TASK, "IDEL_TASK", 1 * 1024, NULL, 1, NULL);
  xTaskCreate(gui_task, "gui_task", 2*1024, NULL, 5, NULL);
  vTaskStartScheduler();
  while(1)
  {
    
  }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /**Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
  PeriphClkInitStruct.PLLSAI.PLLSAIN = 100;
  PeriphClkInitStruct.PLLSAI.PLLSAIR = 5;
  PeriphClkInitStruct.PLLSAIDivR = RCC_PLLSAIDIVR_4;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}



void lcd_init(void)
{
  BSP_LCD_Init();
  BSP_LCD_LayerDefaultInit(LCD_BACKGROUND_LAYER, LCD_FRAME_BUFFER);
  BSP_LCD_LayerDefaultInit(LCD_FOREGROUND_LAYER, LCD_FRAME_BUFFER);
  BSP_LCD_SelectLayer(LCD_FOREGROUND_LAYER);
  BSP_LCD_DisplayOn();
  //BSP_LCD_Clear(LCD_COLOR_WHITE);
  //HAL_Delay(100);
}

static void disp_init(void)
{
    lcd_init();
}


void gui_task(void *arg)
{
  HAL_UART_Transmit(&huart1, (uint8_t *)"lvgl start", sizeof("lvgl start"), 0xffff);
  //xTaskCreate(lv_tick_task, "lv_tick_task", 1 * 1024, NULL, 1, NULL);
  lv_init();
  //disp_init();
  static lv_disp_buf_t disp_buf_2;
  static lv_color_t buf2_1[LV_HOR_RES_MAX * 10];                         /*A buffer for 10 rows*/
  static lv_color_t buf2_2[LV_HOR_RES_MAX * 10];                        /*An other buffer for 10 rows*/
  lv_disp_buf_init(&disp_buf_2, buf2_1, buf2_2, LV_HOR_RES_MAX * 10);   /*Initialize the display buffer*/
    
  lv_disp_drv_t disp_drv;                         /*Descriptor of a display driver*/
  lv_disp_drv_init(&disp_drv);                    /*Basic initialization*/
  /*Set up the functions to access to your display*/
  /*Set the resolution of the display*/
  disp_drv.hor_res = LV_HOR_RES_MAX;
  disp_drv.ver_res = LV_HOR_RES_MAX;
  /*Used to copy the buffer's content to the display*/
  disp_drv.flush_cb = disp_flush;
  /*Set a display buffer*/
  disp_drv.buffer = &disp_buf_2;

  lv_disp_drv_register(&disp_drv);
  //xTaskCreate(lv_tick_task, "lv_tick_task", 1 * 1024, NULL, 2, NULL);

  lv_ex_chart_1();
  HAL_UART_Transmit(&huart1, (uint8_t *)"lvgl running", sizeof("lvgl running"), 0xffff);
  while(1)
  {
    lv_task_handler();
    vTaskDelay(5 / portTICK_RATE_MS);
  }
}



static void lv_tick_task(void * arg)
{
    while(1) 
    {
        lv_tick_inc(10);
        vTaskDelay(10 / portTICK_RATE_MS);
    }
}


static void disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p)
{
    /*The most simple case (but also the slowest) to put all pixels to the screen one-by-one*/

    int32_t x;
    int32_t y;
    for(y = area->y1; y <= area->y2; y++)
    {
      for(x = area->x1; x <= area->x2; x++)
      {
          BSP_LCD_DrawPixel(x, y, (uint32_t)color_p->full);
          color_p++;
      }
    }
    //os_printf("disp_flush", "flushing");
    lv_disp_flush_ready(disp_drv);
}



void lv_ex_chart_1(void)
{
    /*Create a chart*/
    lv_obj_t * chart;
    chart = lv_chart_create(lv_scr_act(), NULL);
    lv_obj_set_size(chart, 200, 150);
    lv_obj_align(chart, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_chart_set_type(chart, LV_CHART_TYPE_POINT | LV_CHART_TYPE_LINE);   /*Show lines and points too*/
    lv_chart_set_series_opa(chart, LV_OPA_70);                            /*Opacity of the data series*/
    lv_chart_set_series_width(chart, 4);                                  /*Line width and point radious*/

    lv_chart_set_range(chart, 0, 100);

    /*Add two data series*/
    lv_chart_series_t * ser1 = lv_chart_add_series(chart, LV_COLOR_RED);
    lv_chart_series_t * ser2 = lv_chart_add_series(chart, LV_COLOR_GREEN);

    /*Set the next points on 'dl1'*/
    lv_chart_set_next(chart, ser1, 10);
    lv_chart_set_next(chart, ser1, 10);
    lv_chart_set_next(chart, ser1, 10);
    lv_chart_set_next(chart, ser1, 10);
    lv_chart_set_next(chart, ser1, 10);
    lv_chart_set_next(chart, ser1, 10);
    lv_chart_set_next(chart, ser1, 10);
    lv_chart_set_next(chart, ser1, 30);
    lv_chart_set_next(chart, ser1, 70);
    lv_chart_set_next(chart, ser1, 90);

    /*Directly set points on 'dl2'*/
    ser2->points[0] = 90;
    ser2->points[1] = 70;
    ser2->points[2] = 65;
    ser2->points[3] = 65;
    ser2->points[4] = 65;
    ser2->points[5] = 65;
    ser2->points[6] = 65;
    ser2->points[7] = 65;
    ser2->points[8] = 65;
    ser2->points[9] = 65;

    lv_chart_refresh(chart); /*Required after direct set*/
}




HAL_StatusTypeDef os_printf(char *tag, char *str)
{
  static char uart_buf[100];
  if((strlen(tag) + strlen(str)) > 100)
  {
    HAL_UART_Transmit(&huart1, (uint8_t *)"stack overflow\n", strlen("stack overflow\n"), 0xffff);
    return HAL_ERROR;
  }
  else
  {
    xSemaphoreTake(UartSemaphore, portMAX_DELAY);
    uint8_t buf_cnt = sprintf(uart_buf, "%s:%s\n", tag, str);
    uint8_t ret  = HAL_UART_Transmit(&huart1, (uint8_t *)uart_buf, buf_cnt, 0xffff);
    xSemaphoreGive(UartSemaphore);
    return ret; 
  }
}



void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM6)
  {
    HAL_IncTick();
    lv_tick_inc(1);
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
