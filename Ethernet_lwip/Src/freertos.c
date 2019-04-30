/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */     

#include "api.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId LED_Blink_TaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
   
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void StartBlinkTask(void const * argument);

extern void MX_LWIP_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
       
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of LED_Blink_Task */
  osThreadDef(LED_Blink_Task, StartBlinkTask, osPriorityIdle, 0, 128);
  LED_Blink_TaskHandle = osThreadCreate(osThread(LED_Blink_Task), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */

const static char indexdata[] =
		"<html> \
		<head><title>A test page</title></head> \
		<body> \This is a small test page. By Justin\</body> \
		</html>";

const static char http_html_hdr[] ="Content-type: text/html\r\n\r\n";

static void tcpecho_thread(void *arg){
	err_t err;
	err_t accept_err;
	err_t recv_err;

	struct netconn *conn, *newconn;
	struct netbuf *buf;
	char *rq;
	uint16_t len;

	//create new connection identifier
	conn = netconn_new(NETCONN_TCP);

	if(conn!=NULL){

		//Bind connection to port 80
		err = netconn_bind(conn, NULL, 80);

		if(err == ERR_OK){
			//Tell connection to go into listening mode
			netconn_listen(conn);

			while(1){
				//Grab new connection
				accept_err = netconn_accept(conn, &newconn);

				//Process new connection
				if(accept_err == ERR_OK){
					while((recv_err = netconn_recv(newconn, &buf)) == ERR_OK){
						do
						{
							netbuf_data(buf, &rq, &len);
							//netconn_write(newconn, data, len, NETCONN_COPY);

							/* Check if the request was an HTTP "GET /\r\n". */
							if(rq[0] == 'G' && rq[1] == 'E' &&rq[2] == 'T' && rq[3] == ' ' &&rq[4] == '/') {

								/* Send the header. */
								//netconn_write(newconn, http_html_hdr, sizeof(http_html_hdr),NETCONN_NOCOPY);

								/* Send the actual web page. */
								netconn_write(newconn, indexdata, sizeof(indexdata),NETCONN_NOCOPY);

								/* Close the connection. */
								netconn_close(newconn);
							}

						}
						while(netbuf_next(buf) >= 0);

						netbuf_delete(buf);
					}
					netconn_close(newconn);
					netconn_delete(newconn);
				}
			}
		}
		else {
			netconn_delete(newconn);
		}
	}
}

void StartDefaultTask(void const * argument)
{
  /* init code for LWIP */
  MX_LWIP_Init();

  //tcpip_init(NULL, NULL);

  //Netif_Config();

  int TCPECHO_THREAD_PRIO = 60;
  sys_thread_new("tcpecho_thread", tcpecho_thread, NULL, DEFAULT_THREAD_STACKSIZE, TCPECHO_THREAD_PRIO);

  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}



/* USER CODE BEGIN Header_StartBlinkTask */
/**
* @brief Function implementing the LED_Blink_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartBlinkTask */
void StartBlinkTask(void const * argument)
{
  /* USER CODE BEGIN StartBlinkTask */
  /* Infinite loop */
  for(;;)
  {
	  HAL_GPIO_WritePin(LED_Indicator_GPIO_Port, LED_Indicator_Pin, GPIO_PIN_RESET);
	  osDelay(1000);
	  HAL_GPIO_WritePin(LED_Indicator_GPIO_Port, LED_Indicator_Pin, GPIO_PIN_SET);
	  osDelay(1000);
  }
  /* USER CODE END StartBlinkTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
     
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
