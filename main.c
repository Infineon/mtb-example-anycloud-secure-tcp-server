/******************************************************************************
* File Name:   main.c
*
* Description: This is the source code for TCP Secure Server Example in
* ModusToolbox.
*
* Related Document: See Readme.md
*
*******************************************************************************
* (c) 2019, Cypress Semiconductor Corporation. All rights reserved.
*******************************************************************************
* This software, including source code, documentation and related materials
* ("Software"), is owned by Cypress Semiconductor Corporation or one of its
* subsidiaries ("Cypress") and is protected by and subject to worldwide patent
* protection (United States and foreign), United States copyright laws and
* international treaty provisions. Therefore, you may use this Software only
* as provided in the license agreement accompanying the software package from
* which you obtained this Software ("EULA").
*
* If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
* non-transferable license to copy, modify, and compile the Software source
* code solely for use in connection with Cypress's integrated circuit products.
* Any reproduction, modification, translation, compilation, or representation
* of this Software except as specified above is prohibited without the express
* written permission of Cypress.
*
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
* reserves the right to make changes to the Software without notice. Cypress
* does not assume any liability arising out of the application or use of the
* Software or any product or circuit described in the Software. Cypress does
* not authorize its products for use in any products where a malfunction or
* failure of the Cypress product may reasonably be expected to result in
* significant property damage, injury or death ("High Risk Product"). By
* including Cypress's product in a High Risk Product, the manufacturer of such
* system or application assumes all risk of such use and in doing so agrees to
* indemnify Cypress against all liability.
*******************************************************************************/

/* Header file includes */
#include "cyhal.h"
#include "cybsp.h"
#include "cybsp_wifi.h"
#include "cy_retarget_io.h"
#include "string.h"

/* FreeRTOS header file */
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>

/* lwIP header files */
#include <lwip/tcpip.h>
#include <lwip/api.h>
#include <lwipinit.h>
#include <mbedtlsinit.h>
#include "ip4_addr.h"

/* mbedTLS header files */
#include "mbedtls_net_sockets.h"
#include "mbedtls/debug.h"
#include "mbedtls/ssl.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/error.h"
#include "mbedtls/certs.h"
#include "mbedtls/config.h"
#include "network_credentials.h"

/*******************************************************************************
* Macros
********************************************************************************/
#define MAX_CONNECTION_RETRIES            (10u)

#define TCP_SERVER_PORT                   "50007"

/* 32-bit task notification value for the tcp_server_task */
#define TCP_SERVER_LISTEN_OFF             (0x00lu)
#define TCP_SERVER_LISTEN_ON              (0x01lu)

#define USER_BTN1_INTR_PRIORITY           (5)

#define TCP_SERVER_TASK_STACK_SIZE        (1024*5)
#define TCP_SERVER_TASK_PRIORITY          (1)
#define TCP_SERVER_TASK_QUEUE_LEN         (10)
#define RTOS_TASK_TICKS_TO_WAIT           (100)
#define MAX_TCP_DATA_SIZE                 (1024)

/*******************************************************************************
* Function Prototypes
********************************************************************************/
void isr_button_press( void *callback_arg, cyhal_gpio_event_t event);
void tcp_server_task(void *arg);

/*******************************************************************************
* Global Variables
********************************************************************************/

/* The primary WIFI driver  */
whd_interface_t iface;

/* 32-bit task notification value to notify TCP server task to start/stop
 * listening on a TCP port */
uint32_t server_state = TCP_SERVER_LISTEN_OFF;

/* TCP Sever task handle */
TaskHandle_t tcp_server_task_handle;

/* This enables RTOS aware debugging */
volatile int uxTopUsedPriority;

const size_t tcp_server_cert_len = sizeof( tcp_server_cert );
const size_t pkey_len = sizeof( server_private_key );
struct netif *net;

/* Data buffer for TCP data */
 unsigned char data_buffer[MAX_TCP_DATA_SIZE];

/*******************************************************************************
 * Function Name: main
 ********************************************************************************
 * Summary:
 *  System entrance point. This function sets up user tasks and then starts
 *  the RTOS scheduler.
 *
 * Parameters:
 *  void
 *
 * Return:
 *  int
 *
 *******************************************************************************/
int main()
{
    cy_rslt_t result ;

    /* This enables RTOS aware debugging in OpenOCD */
    uxTopUsedPriority = configMAX_PRIORITIES - 1 ;

    /* Initialize the board support package */
    result = cybsp_init() ;
    CY_ASSERT(result == CY_RSLT_SUCCESS) ;

    /* Initialize User button 1 and register interrupt on falling edge */
    cyhal_gpio_init(CYBSP_USER_BTN1, CYHAL_GPIO_DIR_INPUT,
                    CYHAL_GPIO_DRIVE_PULLUP, 1);
    cyhal_gpio_register_callback(CYBSP_USER_BTN1, isr_button_press, NULL);
    cyhal_gpio_enable_event(CYBSP_USER_BTN1, CYHAL_GPIO_IRQ_FALL,
                            USER_BTN1_INTR_PRIORITY, 1);

     /* Initialize the User LED */
    cyhal_gpio_init((cyhal_gpio_t) CYBSP_USER_LED, CYHAL_GPIO_DIR_OUTPUT,
                    CYHAL_GPIO_DRIVE_PULLUP, CYBSP_LED_STATE_OFF);

    /* Enable global interrupts */
    __enable_irq();

    /* Initialize retarget-io to use the debug UART port */
    cy_retarget_io_init(CYBSP_DEBUG_UART_TX, CYBSP_DEBUG_UART_RX,
                        CY_RETARGET_IO_BAUDRATE);

    /* \x1b[2J\x1b[;H - ANSI ESC sequence to clear screen */
    printf("\x1b[2J\x1b[;H");
    printf("===============================================================\n");
    printf("CE229254 - ModusToolbox Connectivity Example: Secure TCP Server\n");
    printf("===============================================================\n\n");

    /* Create the tasks */
    xTaskCreate(tcp_server_task, "Network Task", TCP_SERVER_TASK_STACK_SIZE, NULL,
                TCP_SERVER_TASK_PRIORITY, &tcp_server_task_handle) ;

    /* Start the FreeRTOS scheduler */
    vTaskStartScheduler() ;

    /* Should never get here */
    CY_ASSERT(0) ;
}

/*******************************************************************************
 * Function Name: isr_button_press
 *******************************************************************************
 *
 * Summary:
 *  GPIO interrupt service routine. This function detects button presses and
 *  sends task notifications to the TCP server task.
 *
 * Parameters:
 *  void *callback_arg : pointer to variable passed to the ISR
 *  cyhal_gpio_event_t event : GPIO event type
 *
 * Return:
 *  None
 *
 *******************************************************************************/
void isr_button_press( void *callback_arg, cyhal_gpio_event_t event)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    if(server_state == TCP_SERVER_LISTEN_OFF)
    {
        server_state = TCP_SERVER_LISTEN_ON;

        /* Notify the TCP server task to start listening on the port */
        xTaskNotifyFromISR(tcp_server_task_handle, server_state,
                           eSetValueWithoutOverwrite,&xHigherPriorityTaskWoken);

        portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
    }
}

/*******************************************************************************
 * Function Name: tcp_server_task
 *******************************************************************************
 * Summary:
 *  Task used to establish a connection to a remote TCP client.
 *
 * Parameters:
 *  void *args : Task parameter defined during task creation (unused)
 *
 * Return:
 *  void
 *
 *******************************************************************************/
void tcp_server_task(void *arg)
{
    cy_rslt_t result ;
    whd_ssid_t ssid_data ;
    const char *ssid = WIFI_SSID ;
    const char *key = WIFI_PASSWORD ;
    unsigned char * write_buff;

    /* Variable to track the number of connection retries to the Wi-Fi AP
     * specified by WIFI_SSID macro */
    int conn_retries = 0;

    /* mbed TLS structures */
    mbedtls_net_context client_fd, listen_fd;
    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;
    mbedtls_ssl_context ssl;
    mbedtls_ssl_config conf;
    mbedtls_x509_crt cacert;
    mbedtls_pk_context pkey;

    /* mbedTLS API results */
    int mbedtls_result = 1;
    size_t  len;

    /* Initialize and start the tcpip_thread */
    tcpip_init(NULL, NULL) ;
    printf("lwIP TCP/IP stack initialized\n") ;

    /* Initialize the Wi-Fi Driver */
    result = cybsp_wifi_init_primary(&iface) ;

    if(result == CY_RSLT_SUCCESS)
    {
        printf("Wi-Fi driver initialized \n") ;
    }
    else
    {
        printf("Wi-Fi Driver initialization failed!\n");
        CY_ASSERT(0);
    }

    /* Initialize RTC and set the time using mbedtls_platform_set_time
     * function.
     */
    mbedtls_init() ;

    /* Join the Wi-Fi AP */
    result = WHD_PENDING;
    ssid_data.length = strlen(ssid);
    memcpy(ssid_data.value, ssid, ssid_data.length);
    while(result != CY_RSLT_SUCCESS && conn_retries < MAX_CONNECTION_RETRIES)
    {
        result = whd_wifi_join(iface, &ssid_data, WHD_SECURITY_WPA2_AES_PSK,
                              (const uint8_t *)key, strlen(key));
        conn_retries++;
    }

    if(result == CY_RSLT_SUCCESS)
    {
        printf("Successfully joined Wi-Fi network '%s'\n", ssid);
    }
    else
    {
        printf("Failed to join Wi-Fi network '%s'\n", ssid);
        CY_ASSERT(0);
    }

    /* Add the Wi-Fi interface to the lwIP stack */
    result = add_interface_to_lwip(iface, NULL);
    if(result == CY_RSLT_SUCCESS)
    {
        printf("WiFi interface added to TCP/IP stack\n");
    }
    else
    {
        printf("Failed to add Wi-Fi interface to lwIP stack!\n");
        CY_ASSERT(0);
    }

    /* Fetch the IP address assigned based on the added Wi-Fi interface */
    net = get_lwip_interface() ;

    while(net->ip_addr.u_addr.ip4.addr == 0)
    {
         /* Wait till IP address is assigned */
         vTaskDelay(RTOS_TASK_TICKS_TO_WAIT) ;
    }
    printf("IP Address %s assigned\n", ip4addr_ntoa(&net->ip_addr.u_addr.ip4));

    /* Initialize mbed TLS structures required to setup a TLS session */
    mbedtls_net_init( &client_fd );
    mbedtls_net_init( &listen_fd );
    mbedtls_ssl_init( &ssl );
    mbedtls_ssl_config_init( &conf );
    mbedtls_x509_crt_init( &cacert );
    mbedtls_pk_init(&pkey);
    mbedtls_ctr_drbg_init( &ctr_drbg );

    /* Initialize and seed the entropy source */
    mbedtls_entropy_init( &entropy );
    if( ( mbedtls_result = mbedtls_ctr_drbg_seed( &ctr_drbg, mbedtls_entropy_func, &entropy,
                                NULL, 0 ) ) != 0 )
    {
        printf( "Failed! mbedtls_ctr_drbg_seed returned %d\n", mbedtls_result );
        CY_ASSERT(0);
    }
    else
    {
        printf( "Success: The random number generator has been seeded.\n");
    }

    /* Create and bind to a TCP socket */
    if( ( mbedtls_result = mbedtls_net_bind( &listen_fd, ip4addr_ntoa(&net->ip_addr.u_addr.ip4)
                                            , TCP_SERVER_PORT, MBEDTLS_NET_PROTO_TCP ) ) != 0 )
    {
      printf( "Failed! mbedtls_net_bind returned %d\n", mbedtls_result );
      CY_ASSERT(0);
    }

    printf("Loading Certificates...");
    mbedtls_result = mbedtls_x509_crt_parse( &cacert, (const unsigned char *) tcp_server_cert,
            tcp_server_cert_len);
    if( mbedtls_result != 0 )
    {
      printf( "Failed!  mbedtls_x509_crt_parse returned %d\n", mbedtls_result );
      CY_ASSERT(0);
    }

    mbedtls_result =  mbedtls_pk_parse_key( &pkey, (const unsigned char *) server_private_key,
            pkey_len, NULL, 0 );
    if( mbedtls_result != 0 )
    {
      printf( "Failed!  mbedtls_pk_parse_key returned %d\n\n", mbedtls_result );
      CY_ASSERT(0);
    }
    printf(" ok\n");

    /* Configure the TLS layer */
    printf( "Setting up the SSL data...." );
    if( ( mbedtls_result = mbedtls_ssl_config_defaults( &conf,
            MBEDTLS_SSL_IS_SERVER,
            MBEDTLS_SSL_TRANSPORT_STREAM,
            MBEDTLS_SSL_PRESET_DEFAULT ) ) != 0 )
    {
      printf( "Failed! mbedtls_ssl_config_defaults returned %d\n", mbedtls_result );
      CY_ASSERT(0);
    }
    printf("ok\n");

    /* Set the random engine to be used for random number generation */
    mbedtls_ssl_conf_rng( &conf, mbedtls_ctr_drbg_random, &ctr_drbg );

    /* Set the data required for certificate verification */
    mbedtls_ssl_conf_ca_chain( &conf, &cacert, NULL );
    if( ( mbedtls_result = mbedtls_ssl_conf_own_cert( &conf, &cacert, &pkey ) ) != 0 )
    {
        printf( "Failed! mbedtls_ssl_conf_own_cert returned %d\n", mbedtls_result );
        CY_ASSERT(0);
    }

    while(true)
    {
        printf("Press User Button 1 (USER BTN1) to start listening.\n");

        /* Block till USER_BNT1 is pressed */
        xTaskNotifyWait(0, 0, &server_state, portMAX_DELAY);

        /* Set up the SSL context for the TLS session */
        if( ( mbedtls_result = mbedtls_ssl_setup( &ssl, &conf ) ) != 0 )
        {
            printf( "Failed! mbedtls_ssl_setup returned %d\n", mbedtls_result );
            CY_ASSERT(0);
        }

        /* Turn ON the user LED to indicate that the server is listening */
        cyhal_gpio_write((cyhal_gpio_t) CYBSP_USER_LED, CYBSP_LED_STATE_ON);

        /* Accept new connections */
        printf("Waiting for connections...\n");
        if( (mbedtls_result=mbedtls_net_accept(&listen_fd, &client_fd, NULL, 0 , NULL)) == 0 )
        {
            printf("Info: Incoming Connection from client\n");

            /* Set the input and output functions to use for the network traffic */
            mbedtls_ssl_set_bio( &ssl, &client_fd, mbedtls_net_send, mbedtls_net_recv, NULL );

            printf( "Performing the SSL/TLS handshake..." );
          
            /* Perform the SSL handshake with the incoming TCP client */
            while( ( mbedtls_result = mbedtls_ssl_handshake( &ssl ) ) != 0 )
            {
                  if( mbedtls_result != MBEDTLS_ERR_SSL_WANT_READ && mbedtls_result != MBEDTLS_ERR_SSL_WANT_WRITE )
                  {
                      printf( "Failed! mbedtls_ssl_handshake returned %d\n", mbedtls_result );
                      CY_ASSERT(0);
                  }
            }
            printf( " ok\n" );

            /* Read the data from the TCP client */
            do
            {
                mbedtls_result = mbedtls_ssl_read( &ssl, data_buffer, MAX_TCP_DATA_SIZE );

                if( mbedtls_result == MBEDTLS_ERR_SSL_WANT_READ || mbedtls_result == MBEDTLS_ERR_SSL_WANT_WRITE )
                {
                    continue;
                }

                else if( mbedtls_result <= 0 )
                {
                    switch( mbedtls_result )
                    {
                        case MBEDTLS_ERR_SSL_PEER_CLOSE_NOTIFY:
                            printf( "Connection was closed gracefully\n" );
                            break;

                        case MBEDTLS_ERR_NET_CONN_RESET:
                            printf( "Connection was reset by peer\n" );
                            break;

                        default:
                            printf( "mbedtls_ssl_read returned -0x%x\n", -mbedtls_result );
                            CY_ASSERT(0);
                            break;
                    }
                    break;
                }
                else
                {
                    printf("===============================================================\n");
                    printf( "Message From Client: %s\n",(char *) data_buffer );
                    printf("===============================================================\n");
                    break;
                }
            }while(true);

            /* Write message to the TCP client */
            len = sprintf((char*)data_buffer, "Hello World!");
            write_buff = data_buffer;
            do
            {
                 mbedtls_result = mbedtls_ssl_write( &ssl, write_buff, len );

                 if( mbedtls_result == MBEDTLS_ERR_SSL_WANT_READ || mbedtls_result == MBEDTLS_ERR_SSL_WANT_WRITE )
                 {
                     continue;
                 }
                 else if( mbedtls_result < 0 )
                 {
                     printf( "Failed! mbedtls_ssl_write returned %d\n", mbedtls_result );
                     CY_ASSERT(0);
                 }
                 /* Continue the write operation if the data is partially sent.
                  * Update the data_buffer and len so as to write the remaining data.
                  */
                 else if ( mbedtls_result < len )
                 {
                     write_buff += mbedtls_result;
                     len -= mbedtls_result;
                     continue;
                 }                 
                 else
                 {
                     /* Break out of the loop once the write operation is successful. */
                     break;
                 }

            }while(true);

            /*Reset the SSL session for the next incoming TCP client connection */
            mbedtls_ssl_session_reset( &ssl );

            /* Set the TCP server to state as TCP_SERVER_LISTEN_OFF
             * Turn OFF the User LED to indicate that server has stopped listening on the port */
            server_state = TCP_SERVER_LISTEN_OFF;
            cyhal_gpio_write((cyhal_gpio_t) CYBSP_USER_LED, CYBSP_LED_STATE_OFF);
        }
    }
 }

/* [] END OF FILE */
