/******************************************************************************
* File Name: network_credentials.h
*
* Description: This file is the public interface for Wi-Fi credentials and
* TCP server certificate.
*
* Related Document: See README.md
*
*******************************************************************************
* Copyright 2019-2021, Cypress Semiconductor Corporation (an Infineon company) or
* an affiliate of Cypress Semiconductor Corporation.  All rights reserved.
*
* This software, including source code, documentation and related
* materials ("Software") is owned by Cypress Semiconductor Corporation
* or one of its affiliates ("Cypress") and is protected by and subject to
* worldwide patent protection (United States and foreign),
* United States copyright laws and international treaty provisions.
* Therefore, you may use this Software only as provided in the license
* agreement accompanying the software package from which you
* obtained this Software ("EULA").
* If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
* non-transferable license to copy, modify, and compile the Software
* source code solely for use in connection with Cypress's
* integrated circuit products.  Any reproduction, modification, translation,
* compilation, or representation of this Software except as specified
* above is prohibited without the express written permission of Cypress.
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
* including Cypress's product in a High Risk Product, the manufacturer
* of such system or application assumes all risk of such use and in doing
* so agrees to indemnify Cypress against all liability.
*******************************************************************************/

/*******************************************************************************
* Include guard
*******************************************************************************/
#ifndef NETWORK_CREDENTIALS_H_
#define NETWORK_CREDENTIALS_H_

#include "cy_wcm.h"

/*******************************************************************************
* Macros
********************************************************************************/
#define INITIALISER_IPV4_ADDRESS(addr_var, addr_val)   addr_var = { CY_WCM_IP_VER_V4, \
                                                       { .v4 = (uint32_t)(addr_val) } }

#define MAKE_IPV4_ADDRESS(a, b, c, d)                  ((((uint32_t) d) << 24) | \
                                                       (((uint32_t) c) << 16) | \
                                                       (((uint32_t) b) << 8) | \
                                                       ((uint32_t) a))

/* To use the Wi-Fi device in AP interface mode, set this macro as '1' */
#define USE_AP_INTERFACE                               (0)

#if(USE_AP_INTERFACE)
    #define WIFI_INTERFACE_TYPE                        CY_WCM_INTERFACE_TYPE_AP
    /* SoftAP Credentials: Modify SOFTAP_SSID and SOFTAP_PASSWORD as required */
    #define SOFTAP_SSID                                "MY_SOFT_AP"
    #define SOFTAP_PASSWORD                            "psoc1234"

    /* Security type of the SoftAP. See 'cy_wcm_security_t' structure
     * in "cy_wcm.h" for more details.
     */
    #define SOFTAP_SECURITY_TYPE                       CY_WCM_SECURITY_WPA2_AES_PSK

    #define SOFTAP_IP_ADDRESS_COUNT                    (2u)

    #define SOFTAP_IP_ADDRESS                          MAKE_IPV4_ADDRESS(192, 168, 10, 1)
    #define SOFTAP_NETMASK                             MAKE_IPV4_ADDRESS(255, 255, 255, 0)
    #define SOFTAP_GATEWAY                             MAKE_IPV4_ADDRESS(192, 168, 10, 1)
    #define SOFTAP_RADIO_CHANNEL                       (1u)
#else
    #define WIFI_INTERFACE_TYPE                        CY_WCM_INTERFACE_TYPE_STA
    /* Wi-Fi Credentials: Modify WIFI_SSID, WIFI_PASSWORD, and WIFI_SECURITY_TYPE
     * to match your Wi-Fi network credentials.
     * Note: Maximum length of the Wi-Fi SSID and password is set to
     * CY_WCM_MAX_SSID_LEN and CY_WCM_MAX_PASSPHRASE_LEN as defined in cy_wcm.h file.
     */
    #define WIFI_SSID                                  "MY_WIFI_SSID"
    #define WIFI_PASSWORD                              "MY_WIFI_PASSWORD"

    /* Security type of the Wi-Fi access point. See 'cy_wcm_security_t' structure
     * in "cy_wcm.h" for more details.
     */
    #define WIFI_SECURITY_TYPE                         CY_WCM_SECURITY_WPA2_AES_PSK

    /* Maximum number of connection retries to a Wi-Fi network. */
    #define MAX_WIFI_CONN_RETRIES                      (10u)

    /* Wi-Fi re-connection time interval in milliseconds */
    #define WIFI_CONN_RETRY_INTERVAL_MSEC              (1000u)
#endif

/* TCP server certificate. Copy from the TCP server certificate
 * generated by OpenSSL (See Readme.md on how to generate a SSL certificate).
 */
#define keySERVER_CERTIFICATE_PEM \
"-----BEGIN CERTIFICATE-----\n"\
"MIIDXTCCAkWgAwIBAgIJAMqhDN/tUBXVMA0GCSqGSIb3DQEBCwUAMEUxCzAJBgNV\n"\
"BAYTAkFVMRMwEQYDVQQIDApTb21lLVN0YXRlMSEwHwYDVQQKDBhJbnRlcm5ldCBX\n"\
"aWRnaXRzIFB0eSBMdGQwHhcNMjEwODE5MTExOTU0WhcNMjIwODE5MTExOTU0WjBF\n"\
"MQswCQYDVQQGEwJBVTETMBEGA1UECAwKU29tZS1TdGF0ZTEhMB8GA1UECgwYSW50\n"\
"ZXJuZXQgV2lkZ2l0cyBQdHkgTHRkMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIB\n"\
"CgKCAQEAyIEP0iuIKQkBvDV58Xkv/Ia9mLOAL/5jrulfkDYLx7Hl8llBfCAtlAQd\n"\
"KQRbhmRv1prgRdvn0mkykr15BVPgBGC+27FW1+M1jtRxzpSvfknX8hRAA0Jh4zwZ\n"\
"2tF1vNq9JQWEuPDfawpFiShIg1C8DJ/GVeakbU12BA088xsCKWqvVWtZ9Vqespuc\n"\
"bLa27l4z0iVMUAECMU+sztrruYMQf8C2wIa3ZAclIt7tL4Nkm7S928YF77fMPdl8\n"\
"nv08G5OHzSJN9RNqf/UDoy1kBGSdFI6L7aTeqgyBPhzWBDXdIm6otUwfOeXsbYRy\n"\
"yxP5Am3c0dSvGCIiTEyuhzF0unxQHwIDAQABo1AwTjAdBgNVHQ4EFgQUY2NDL36s\n"\
"dGvkA4unf+0kHFKHevQwHwYDVR0jBBgwFoAUY2NDL36sdGvkA4unf+0kHFKHevQw\n"\
"DAYDVR0TBAUwAwEB/zANBgkqhkiG9w0BAQsFAAOCAQEAUcCHz74R/kB19V8oQmwl\n"\
"ahF1C9RpTlciTy0NhaMiBFS34WvC0I5iXLzY0tnZodfUUF8oI1AspLcTxfL/HrUh\n"\
"Xunus1DKek9ZMp/G5W7Qc3+A/53m266M6BVWaCrs7y5BJlzOkPMLRBhaQa7vBIBP\n"\
"pHC1mKvkBppLAUk4NV/hAGqVonoeFKpC3qE+tZmufEcygklgB+M347Le2crxUcdL\n"\
"/f0Fw082e96Sw3HVPJmGCdACaNeBWai4ydE9R1dLacajFB8KYD+Gt0qkvrHnlK7h\n"\
"oBZsbXfHZrZoBqjYzKY2s2Js0w19O2D2FVcjZEfF3HuEQI0c19VuAcjPL5Bd8Mw0\n"\
"VQ==\n"\
"-----END CERTIFICATE-----\n"

/* Private key of the TCP Server. Copy from the TCP server key 
 * generated by OpenSSL (See Readme.md on how to create a private key).
 */
#define keySERVER_PRIVATE_KEY_PEM \
"-----BEGIN PRIVATE KEY-----\n"\
"MIIEvAIBADANBgkqhkiG9w0BAQEFAASCBKYwggSiAgEAAoIBAQDIgQ/SK4gpCQG8\n"\
"NXnxeS/8hr2Ys4Av/mOu6V+QNgvHseXyWUF8IC2UBB0pBFuGZG/WmuBF2+fSaTKS\n"\
"vXkFU+AEYL7bsVbX4zWO1HHOlK9+SdfyFEADQmHjPBna0XW82r0lBYS48N9rCkWJ\n"\
"KEiDULwMn8ZV5qRtTXYEDTzzGwIpaq9Va1n1Wp6ym5xstrbuXjPSJUxQAQIxT6zO\n"\
"2uu5gxB/wLbAhrdkByUi3u0vg2SbtL3bxgXvt8w92Xye/Twbk4fNIk31E2p/9QOj\n"\
"LWQEZJ0UjovtpN6qDIE+HNYENd0ibqi1TB855exthHLLE/kCbdzR1K8YIiJMTK6H\n"\
"MXS6fFAfAgMBAAECggEAK4hB/BqxfF9VPhzKNhBex9AlWAMZANKVqANJWVEyq4/H\n"\
"Yh/Y5O9DlhBymFY23O3vjkOgD+rX4UovzcZPgUWdrLWomqBdP5A4L6OWdgi4NY4o\n"\
"0YluAIcoI6MH5gd2LaO8T05sa474ow/e2xGg9MnRmlgSQtJJvuWDiwuowQuLI5yA\n"\
"pFVZgTzDqYid/auzyP4eG+CWHs8dT1Jsba6r1y5bpzrLHe9q1VFZvSD717Oafies\n"\
"hIS2ZvcUo407DVwHrLFFPOh3/G+71l3yLMTtr4jO+QF5vLyLKqt+Fo8prBfRIAeC\n"\
"JwmAbM3kwANmTbRc8qhb4wDnt7y7NZuMq9Ys+COOOQKBgQDuhJ78V41I0r70Jjm0\n"\
"kWLOqrY/z9ENm6BpQQ2ykSr3V2DwTqHc0NkxDv+85pNmrGyO29CMnwDsyBYLOtwp\n"\
"7rIYr8YEyJ+18i/rfIkOLCJA+vmjlvI9MXiuDa68x2kURx3uRJONuiTwEpzvBDyZ\n"\
"1Luei7f7SPI4tgFHj8N5A0yoCwKBgQDXMyzyUq7cGmao/WUJVNVp1PdxHqIy5i7d\n"\
"EqYHTlak2b5ZkM/Foz4XOWgtBCFWw0ECypwzrSCratztCRvswhWg4DHmYmnOGYyX\n"\
"bWsYrv75BkTQemx3Rvswp1lQd3F0yRi8Vmx5a8cTK36HcfIdAXMn0GXg5vtTEJGQ\n"\
"UmWjhBTAvQKBgGkIxephs7np9GUXoDm//D7+NhuLrIfNjKpNF3cXLlCVjf7E4cqX\n"\
"vc04ya2p7Cc+rt+KNSB3tLSQuARh+yoMDymH3i+lGY/KAwg0YzZ2gO8UlnJPuoTw\n"\
"EDC1nclMFx3y419Vm5TB8Bo4WBt5RiaH48dBx9dEnEYVACM/T9kudsdZAoGAV6qn\n"\
"YshAJ7YxoCIqKNUAY664ZsK9/NI8wR65uDw3m8yAeOg/XBoWT0u5nq0SD9BXCZYU\n"\
"cBDVQD+2ZHHTiG3AddsfOOVaiPVriryuZk3T3QqquetFiInIBpgGEHu4IoVRqFfk\n"\
"tMotPSf40aCfqyVODigdeLNgKwSS6CJI+YXzFlkCgYAHmm8+yMOAYZRPKQCFX/3a\n"\
"x8/QBKFgaq0b57zBNxbULH1RwZpKWNx3T1pt8k664EAb6vqf9HMl6u0Po4LhSzA/\n"\
"ajTTut6XA9qb+K0HKEBJtnx859N6cSobYcM290+YnCKvp/NHO63uIahWOrTye0sI\n"\
"Yy1FGoDZ3Gtcby+0zUs6Ng==\n"\
"-----END PRIVATE KEY-----\n"


/* TCP client certificate. In this example this is the RootCA
 * certificate so as to verify the TCP client's identity. */
#define keyCLIENT_ROOTCA_PEM \
"-----BEGIN CERTIFICATE-----\n"\
"MIIDXTCCAkWgAwIBAgIJANovoG8KgaO5MA0GCSqGSIb3DQEBCwUAMEUxCzAJBgNV\n"\
"BAYTAkFVMRMwEQYDVQQIDApTb21lLVN0YXRlMSEwHwYDVQQKDBhJbnRlcm5ldCBX\n"\
"aWRnaXRzIFB0eSBMdGQwHhcNMjEwODIwMDgwNTM2WhcNMjIwODIwMDgwNTM2WjBF\n"\
"MQswCQYDVQQGEwJBVTETMBEGA1UECAwKU29tZS1TdGF0ZTEhMB8GA1UECgwYSW50\n"\
"ZXJuZXQgV2lkZ2l0cyBQdHkgTHRkMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIB\n"\
"CgKCAQEAu85FvtjQqktjqepm0IE+Npj4Je/JylEJNPfEavvYn7LOCAjIh+n2fV9G\n"\
"rq4s5sO5DV1ctenRtIoMlT4HL6f2or/0hfPnG4IDNZRFi+NwGf67wFuvRPOc8lnY\n"\
"QrGMAKJx+l2SNby8SsjCwwmAM3iAuzPPGRn9whPJfy/D46oHN3yHfXpGRaGQCZbK\n"\
"1RJ31XOS6A4q80wef5DPz4YYLFROFuS+lC5oHsOjDDNHbhz6W9D1dPvV0MMLbMoL\n"\
"tRaeE8R2y/AkLm+2yum5xC2kMN4ZgBCOFbtmUbI92tDXTnV/yAu0GQ2DHZIIoHaQ\n"\
"rlJaZOz+RTBSJ6Ir3Sro0UkLWAkbsQIDAQABo1AwTjAdBgNVHQ4EFgQURJQssHJl\n"\
"GfLSHOE0uaWBTdn3YDwwHwYDVR0jBBgwFoAURJQssHJlGfLSHOE0uaWBTdn3YDww\n"\
"DAYDVR0TBAUwAwEB/zANBgkqhkiG9w0BAQsFAAOCAQEAWX7Iid2vJCzxY7ADMoFu\n"\
"SyigePc9A3EuM+1UbvIDqpiIpXvCGZwpbJ64Kf8D1rYCJxKz9l4f+7j/MgG+qaFE\n"\
"UuDsfEcnPh+SVgn2PrzOSL4I9tCOquIkUZ4RXoYbJqb912EKz2sU2RT3szE3d5Qx\n"\
"jNqtnAMwffGmgLNpyCP0DLZ8VgTjLE0fw8sXJvvnB0Ez0CS4JIV+lzFLYIIap7KB\n"\
"H3rWLlJmrPhenNkYkDLDPpC7kg1yxgzAH7VTuDjo37eosSjvMbjk2SKAFwe/aYvt\n"\
"8wSqH9jsWFKCGgxIAwoKDpydrF5jVoFZ9Ge87pUi+ayHf+zzEydbVYgyh33ytigP\n"\
"qw==\n"\
"-----END CERTIFICATE-----\n"

#endif /* NETWORK_CREDENTIALS_H_ */
