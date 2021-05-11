# AnyCloud: secure TCP server

This code example demonstrates the implementation of a secure TCP server with PSoC™ 6 MCU with AIROC™ CYW43xxx Wi-Fi & Bluetooth® combo chips.

In this example, the TCP server establishes a secure connection with a TCP client through SSL handshake. Once the SSL handshake completes successfully, the server allows the user to send LED ON/OFF command to the TCP client; the client responds by sending an acknowledgement message to the server. The Wi-Fi device can be brought up in either STA interface or Soft AP interface mode. Additionally, this code example can be configured to work with IPv4 or link-local IPv6 addressing mode.

This example uses the [Wi-Fi middleware core](https://github.com/cypresssemiconductorco/wifi-mw-core) library of the AnyCloud SDK. This library enables application development based on Wi-Fi by bundling together various other libraries - FreeRTOS, Wi-Fi host driver (WHD), lwIP TCP/IP stack, mbed TLS, and Secure sockets. The Secure sockets library provides an easy-to-use API by abstracting the network stack (lwIP) and the security stack (mbed TLS).

[Provide feedback on this code example.](https://cypress.co1.qualtrics.com/jfe/form/SV_1NTns53sK2yiljn?Q_EED=eyJVbmlxdWUgRG9jIElkIjoiQ0UyMjkyNTQiLCJTcGVjIE51bWJlciI6IjAwMi0yOTI1NCIsIkRvYyBUaXRsZSI6IkFueUNsb3VkOiBzZWN1cmUgVENQIHNlcnZlciIsInJpZCI6InNkYWsiLCJEb2MgdmVyc2lvbiI6IjIuMC4wIiwiRG9jIExhbmd1YWdlIjoiRW5nbGlzaCIsIkRvYyBEaXZpc2lvbiI6Ik1DRCIsIkRvYyBCVSI6IklDVyIsIkRvYyBGYW1pbHkiOiJQU09DIn0=)

## Requirements

- [ModusToolbox&trade; software](https://www.cypress.com/products/modustoolbox-software-environment) v2.2 and later

   **Note:** This code example version requires ModusToolbox&trade; software version 2.2 or later and is not backward compatible with v2.1 or older versions. If you cannot move to ModusToolbox&trade; v2.2 or later, use the latest compatible version of this example: [latest-v1.X](https://github.com/cypresssemiconductorco/mtb-example-anycloud-secure-tcp-server/tree/latest-v1.X).
- Board support package (BSP) minimum required version: 2.0.0
- Programming language: C
- Associated parts: All [PSoC&trade; 6 MCU](http://www.cypress.com/PSoC6) parts, [PSoC™ 6 MCU with AIROC™ CYW43012 Wi-Fi & Bluetooth® combo chip](https://www.cypress.com/documentation/datasheets/cyw43012-single-chip-ultra-low-power-ieee-80211n-compliant-ieee-80211ac), [PSoC™ 6 MCU with AIROC™ CYW4343W Wi-Fi & Bluetooth® combo chip](https://www.cypress.com/documentation/datasheets/cyw4343w-single-chip-80211-bgn-macbasebandradio-bluetooth-51)

## Supported toolchains (make variable 'TOOLCHAIN')

- GNU Arm® embedded compiler v9.3.1 (`GCC_ARM`) - Default value of `TOOLCHAIN`
- Arm compiler v6.13 (`ARM`)
- IAR C/C++ compiler v8.42.2 (`IAR`)

## Supported kits (make variable 'TARGET')

- [PSoC&trade; 6 Wi-Fi Bluetooth&reg; prototyping kit](https://www.cypress.com/CY8CPROTO-062-4343W) (`CY8CPROTO-062-4343W`) - Default value of `TARGET`
- [PSoC&trade; 6 Wi-Fi Bluetooth&reg; pioneer kit](https://www.cypress.com/CY8CKIT-062-WiFi-BT) (`C8CKIT-062-WIFI-BT`)
- [PSoC&trade; 62S2 Wi-Fi Bluetooth&reg; pioneer kit](https://www.cypress.com/CY8CKIT-062S2-43012) (`CY8CKIT-062S2-43012`)
- [PSoC&trade; 62S1 Wi-Fi Bluetooth&reg; pioneer kit](https://www.cypress.com/CYW9P62S1-43438EVB-01) (`CYW9P62S1-43438EVB-01`)
- [PSoC&trade; 62S1 Wi-Fi Bluetooth&reg; pioneer kit](https://www.cypress.com/CYW9P62S1-43012EVB-01) (`CYW9P62S1-43012EVB-01`)
- Rapid IoT Connect platform RP01 feather kit (`CYSBSYSKIT-01`)
- Rapid IoT Connect developer kit (`CYSBSYSKIT-DEV-01`)

## Hardware setup

This example uses the board's default configuration. See the kit user guide to ensure that the board is configured correctly.

**Note:** The PSoC&trade; 6 Wi-Fi Bluetooth&reg; pioneer kit (CY8CKIT-062-WIFI-BT) ship with KitProg2 installed. The ModusToolbox&trade; software requires KitProg3. Before using this code example, make sure that the board is upgraded to KitProg3. The tool and instructions are available in the [Firmware Loader](https://github.com/cypresssemiconductorco/Firmware-loader) GitHub repository. If you do not upgrade, you will see an error like "unable to find CMSIS-DAP device" or "KitProg firmware is out of date".

## Software setup

- Install a terminal emulator if you don't have one. Instructions in this document use [Tera Term](https://ttssh2.osdn.jp/index.html.en).

- Install a Python Interpreter if you don't have one. This code example is tested using [Python 3.7.7](https://www.python.org/downloads/release/python-377/).


## Using the code example

Create the project and open it using one of the following:

<details><summary><b>In Eclipse IDE for ModusToolbox&trade;</b></summary>

1. Click the **New Application** link in the **Quick Panel** (or, use **File** > **New** > **ModusToolbox Application**). This launches the [Project Creator](http://www.cypress.com/ModusToolboxProjectCreator) tool.

2. Pick a kit supported by the code example from the list shown in the **Project Creator - Choose Board Support Package (BSP)** dialog.

   When you select a supported kit, the example is reconfigured automatically to work with the kit. To work with a different supported kit later, use the [Library Manager](https://www.cypress.com/ModusToolboxLibraryManager) to choose the BSP for the supported kit. You can use the Library Manager to select or update the BSP and firmware libraries used in this application. To access the Library Manager, click the link from the **Quick Panel**.

   You can also just start the application creation process again and select a different kit.

   If you want to use the application for a kit not listed here, you may need to update the source files. If the kit does not have the required resources, the application may not work.

3. In the **Project Creator - Select Application** dialog, choose the example by enabling the checkbox.

4. Optionally, change the suggested **New Application Name**.

5. Enter the local path in the **Application(s) Root Path** field to indicate where the application needs to be created.

   Applications that can share libraries can be placed in the same root path.

6. Click **Create** to complete the application creation process.

For more details, see the [Eclipse IDE for ModusToolbox&trade; User Guide](https://www.cypress.com/MTBEclipseIDEUserGuide) (locally available at *{ModusToolbox&trade; install directory}/ide_{version}/docs/mt_ide_user_guide.pdf*).

</details>

<details><summary><b>In command-line interface (CLI)</b></summary>

ModusToolbox&trade; provides the Project Creator as both a GUI tool and a command line tool to easily create one or more ModusToolbox&trade; applications. See the "Project Creator Tools" section of the [ModusToolbox&trade; User Guide](https://www.cypress.com/ModusToolboxUserGuide) for more details.

Alternatively, you can manually create the application using the following steps:

1. Download and unzip this repository onto your local machine, or clone the repository.

2. Open a CLI terminal and navigate to the application folder.

   On Windows, use the command line `modus-shell` program provided in the ModusToolbox&trade; installation instead of a standard Windows command line application. This shell provides access to all ModusToolbox&trade; tools. You can access it by typing `modus-shell` in the search box in the Windows menu.

   In Linux and macOS, you can use any terminal application.

   **Note:** The cloned application contains a default BSP file (*TARGET_xxx.mtb*) in the *deps* folder. Use the [Library Manager](https://www.cypress.com/ModusToolboxLibraryManager) (`make modlibs` command) to select and download a different BSP file, if required. If the selected kit does not have the required resources or is not [supported](#supported-kits-make-variable-target), the application may not work.

3. Import the required libraries by executing the `make getlibs` command.

Various CLI tools include a `-h` option that prints help information to the terminal screen about that tool. For more details, see the [ModusToolbox&trade; User Guide](https://www.cypress.com/ModusToolboxUserGuide) (locally available at *{ModusToolbox&trade; install directory}/docs_{version}/mtb_user_guide.pdf*).

</details>

<details><summary><b>In Third-party IDEs</b></summary>

1. Follow the instructions from the **In command-line interface (CLI)** section to create the application, and import the libraries using the `make getlibs` command.

2. Export the application to a supported IDE using the `make <ide>` command.

   For a list of supported IDEs and more details, see the "Exporting to IDEs" section of the [ModusToolbox&trade; User Guide](https://www.cypress.com/ModusToolboxUserGuide) (locally available at *{ModusToolbox&trade; install directory}/docs_{version}/mtb_user_guide.pdf*.

3. Follow the instructions displayed in the terminal to create or import the application as an IDE project.

</details>

## Operation

1. Connect the board to your PC using the provided USB cable through the KitProg3 USB connector.

2. The kit can be configured to run either as Wi-Fi STA interface mode or in AP interface mode. The interface mode is configured using  the `USE_AP_INTERFACE` macro defined in the *network_credentials.h* file. Based on the desired interface mode, do the following:

   **Kit in STA mode (default interface):**

   1. Set the `USE_AP_INTERFACE` macro to '0'. This is the default mode.

   2. Modify the `WIFI_SSID`, `WIFI_PASSWORD`, and `WIFI_SECURITY_TYPE` macros to match with that of the Wi-Fi network credentials that you want to connect. These macros are defined in the *network_credentials.h* file. Ensure that the Wi-Fi network that you are connecting to is configured as a private network for the proper functioning of this example.

   **Kit in AP mode:**

   1. Set the `USE_AP_INTERFACE` macro to '1'.

   2. Update the `SOFTAP_SSID`, `SOFTAP_PASSWORD`, and `SOFTAP_SECURITY_TYPE` as desired. This step is optional.



3. Configure the IP addressing mode. By default, IPv4-based addressing is used. If you want to use IPv6 addressing mode, set the `USE_IPV6_ADDRESS` macro defined in the *secure_tcp_server.h* file as shown below:

   ```
   #define USE_IPV6_ADDRESS               (1)
   ```

4. Open a terminal program and select the KitProg3 COM port. Set the serial port parameters to 8N1 and 115200 baud.

5. Program the board using one of the following:

   <details><summary><b>Using Eclipse IDE for ModusToolbox&trade;</b></summary>

      1. Select the application project in the Project Explorer.

      2. In the **Quick Panel**, scroll down, and click **\<Application Name> Program (KitProg3)**.

   </details>

   <details><summary><b>Using CLI</b></summary>

     From the terminal, execute the `make program` command to build and program the application using the default toolchain to the default target. You can specify a target and toolchain manually:

      ```
      make program TARGET=<BSP> TOOLCHAIN=<toolchain>
      ```

      Example:


      ```
      make program TARGET=CY8CPROTO-062-4343W TOOLCHAIN=GCC_ARM
      ```
   </details>
   After programming, the application starts automatically. Confirm that the text as shown in either one of the below Figures is displayed on the UART terminal. Note that the Wi-Fi SSID and the IP address assigned will be different based on the network that you have connected to; in AP mode, the AP credentials will be different based on your configuration in Step 2.

   **Figure 1. UART Terminal Showing the Wi-Fi Connection Status (IPv4 Address and STA Mode)**
   ![](images/wifi-conn-status-ipv4-sta-mode.png)


   **Figure 2. UART Terminal Showing the Wi-Fi Connection Status (IPv6 Address and STA Mode)**

   ![](images/wifi-conn-status-ipv6-sta-mode.png)

   <br>

   **Figure 3. UART Terminal Showing the Wi-Fi Connection Status (IPv4 Address and AP Mode)**

   ![](images/wifi-conn-status-ipv4-ap-mode.png)

   Similarly, when the CE is configured for IPv6 and AP mode, the IPv4 address displayed in the Figure 3 will be replaced by the IPv6 address.

6. Connect your computer to the Wi-Fi AP that you have configured in Step 2:

   - In STA mode: Connect the computer to the same AP to which the kit is connected.

   - In AP mode: Connect the computer to the kit's AP.

   Make a note of the IP address assigned to the kit. Note that the type of IP address (IPv4 or IPv6) assigned will be based on the IP addressing mode configured in step 3.

7. From the project directory (*{project directory}/python-tcp-secure-client* folder), open a command shell and run the Python TCP secure client (*tcp_secure_client.py*). In the command shell opened in the project directory, type in the following command based on the IP addressing mode configuration:

   **For IPv4-based addressing:**

   ```
   python tcp_secure_client.py ipv4 <IPv4 address of the kit>
   ```

   **For IPv6-based addressing:**

   ```
   python tcp_secure_client.py ipv6 <IPv6 address of the kit>
   ```

   **Note:** Ensure that the firewall settings of your computer allow access to the Python software so that it can communicate with the TCP server. For more details on enabling Python access, refer to this [community thread](https://community.cypress.com/thread/53662).


8. Press the user button (CYBSP_USER_BTN) to send LED ON/OFF command to the Python TCP client.

   Each user button press will issue the LED ON or LED OFF commands alternately. The client in turn sends an acknowledgement message back to the server.

   Figure 4 and Figure 5 show the TCP server output in IPv4 addressing mode, when the CE is configured in STA and AP mode respectively. Figure 6 shows the TCP client output in IPv4 addressing mode for both AP and STA mode. When the CE is configured in STA mode, Figure 7 and Figure 8 show the TCP server and TCP client outputs respectively in IPv6 addressing mode.

   When the CE is configured in AP and IPv6 mode, the only change from the Figure 4 will be the IPv6 address being displayed instead of IPv4.

   **Figure 4. TCP server output - STA mode (IPv4 addressing mode)**

   ![](images/tcp-server-ipv4-output-sta-mode.png)

   <br>

   **Figure 5. TCP server output - AP mode (IPv4 addressing mode)**

   ![](images/tcp-server-ipv4-output-ap-mode.png)

   <br>

   **Figure 6. TCP client output (IPv4 addressing mode)**

   ![](images/tcp-client-ipv4-output.png)

   <br>

   **Figure 7. TCP server output (IPv6 addressing mode)**

   ![](images/tcp-server-ipv6-output-sta-mode.png)

   <br>

   **Figure 8. TCP client output (IPv6 addressing mode)**

   ![](images/tcp-client-ipv6-output.png)

      **Note:** Instead of using the Python TCP client (*tcp_secure_client.py*), alternatively you can use the example [mtb-example-anycloud-secure-tcp-client](https://github.com/cypresssemiconductorco/mtb-example-anycloud-secure-tcp-client) to run as TCP client on a second kit. See the code example documentation to learn how to use the example.

## Debugging

You can debug the example to step through the code. In the IDE, use the **\<Application Name> Debug (KitProg3)** configuration in the **Quick Panel**. For more details, see the "Program and debug" section in the Eclipse IDE for ModusToolbox&trade; user guide: *{ModusToolbox&trade; install directory}/ide_{version}/docs/mt_ide_user_guide.pdf*.

## Design and implementation

### Resources and settings


**Table 1. Application resources**

| Resource  |  Alias/Object     |    Purpose     |
| :------- | :------------    | :------------ |
| SDIO (HAL) | sdio_obj | SDIO interface for Wi-Fi connectivity |
| UART (HAL) |cy_retarget_io_uart_obj| UART HAL object used by Retarget-IO for Debug UART port |
| BUTTON (BSP) | CYBSP_USER_BTN | User button used to send LED ON/OFF commands to the TCP client |

<br>

This example uses the Arm® Cortex®-M4 (CM4) CPU of PSoC&trade; 6 MCU to execute an RTOS task: TCP secure server task. At device reset, the default Cortex-M0+ (CM0+) application enables the CM4 CPU and configures the CM0+ CPU to go to sleep.

In this example, the TCP server establishes a secure connection with a TCP client through SSL handshake. During the SSL handshake, the server presents its SSL certificate for verification, and verifies the incoming client identity. The server's SSL certificate used in this example is a self-signed SSL certificate. See [Creating a self-signed certificate](#creating-a-self-signed-certificate) for more details.

Once the SSL handshake completes successfully, the server allows the user to send LED ON/OFF commands to the TCP client; the client responds by sending an acknowledgement message to the server.

**Note:** The CY8CPROTO-062-4343W board shares the same GPIO for the user button (CYBSP_USER_BTN) and the CYW4343W host wake pin. Because this example uses the GPIO for interfacing with the user button, the SDIO interrupt to wake up the host is disabled by setting `CY_WIFI_HOST_WAKE_SW_FORCE` to '0' in the Makefile through the `DEFINES` variable.

<br>

### Creating a self-signed SSL certificate

The TCP server demonstrated in this example uses a self-signed SSL certificate. This requires **OpenSSL** which is already preloaded in the ModusToolbox installation. Self-signed SSL certificate means that there is no third-party certificate issuing authority, commonly referred to as CA, involved in the authentication of the server. Clients connecting to the server must have an exact copy of the SSL certificate to verify the server's identity.
 
Do the following to generate a self-signed SSL certificate:

#### **Generate SSL certificate and private key**

1. Run the below commands with a CLI (on Windows, use the command line "modus-shell" program provided in the ModusToolbox installation instead of a standard Windows command-line application) to generate the SSL certificate and private key.

   ```
   openssl req -new -newkey rsa:2048 -days 365 -nodes -x509 -keyout server.key -out server.crt
   ```

2. Follow the instructions in the command window to provide the details required for creating the SSL certificate and private key.

   The *server.crt* file is your server's certificate and *server.key* is your server's private key.

<br>

## Related resources

| Application notes                                            |                                                              |
| :----------------------------------------------------------- | :----------------------------------------------------------- |
| [AN228571](https://www.cypress.com/AN228571) – Getting started with PSoC&trade; 6 MCU on ModusToolbox&trade; | Describes PSoC&trade; 6 MCU devices and how to build your first application with ModusToolbox&trade; |
| [AN221774](https://www.cypress.com/AN221774) – Getting started with PSoC&trade; 6 MCU on PSoC&trade; Creator&trade; | Describes PSoC&trade; 6 MCU devices and how to build your first application with PSoC&trade; Creator&trade; |
| [AN210781](https://www.cypress.com/AN210781) – Getting started with PSoC&trade; 6 MCU with Bluetooth&reg; Low Energy connectivity on PSoC&trade; Creator&trade; | Describes PSoC&trade; 6 MCU with Bluetooth&reg; LE connectivity devices and how to build your first application with PSoC&trade; Creator&trade; |
| [AN215656](https://www.cypress.com/AN215656) – PSoC&trade; 6 MCU: dual-CPU system design | Describes the dual-CPU architecture in PSoC&trade; 6 MCU, and shows how to build a simple dual-CPU design |
| **Code examples**                                            |                                                              |
| [Using ModusToolbox](https://github.com/cypresssemiconductorco/Code-Examples-for-ModusToolbox-Software) | [Using PSoC&trade; Creator&trade;](https://www.cypress.com/documentation/code-examples/psoc-6-mcu-code-examples) |
| **Device documentation**                                     |                                                              |
| [PSoC&trade; 6 MCU datasheets](https://www.cypress.com/search/all?f[0]=meta_type%3Atechnical_documents&f[1]=resource_meta_type%3A575&f[2]=field_related_products%3A114026) | [PSoC&trade; 6 technical reference manuals](https://www.cypress.com/search/all/PSoC%206%20Technical%20Reference%20Manual?f[0]=meta_type%3Atechnical_documents&f[1]=resource_meta_type%3A583) |
| **Development kits**                                         | Buy at www.cypress.com                                       |
| [CY8CKIT-062-BLE](https://www.cypress.com/CY8CKIT-062-BLE) PSoC&trade; 6 Bluetooth&reg; LE pioneer kit | [CY8CKIT-062-WiFi-BT](https://www.cypress.com/CY8CKIT-062-WiFi-BT) PSoC&trade; 6 Wi-Fi Bluetooth&reg; pioneer kit |
| [CY8CPROTO-063-BLE](https://www.cypress.com/CY8CPROTO-063-BLE) PSoC&trade; 6 Bluetooth&reg; LE prototyping kit | [CY8CPROTO-062-4343W](https://www.cypress.com/CY8CPROTO-062-4343W) PSoC&trade; 6 Wi-Fi Bluetooth&reg; prototyping kit |
| [CY8CKIT-062S2-43012](https://www.cypress.com/CY8CKIT-062S2-43012) PSoC&trade; 62S2 Wi-Fi Bluetooth&reg; pioneer kit | [CY8CPROTO-062S3-4343W](https://www.cypress.com/CY8CPROTO-062S3-4343W) PSoC&trade; 62S3 Wi-Fi Bluetooth&reg; prototyping kit |
| [CYW9P62S1-43438EVB-01](https://www.cypress.com/CYW9P62S1-43438EVB-01) PSoC&trade; 62S1 Wi-Fi Bluetooth&reg; pioneer kit | [CYW9P62S1-43012EVB-01](https://www.cypress.com/CYW9P62S1-43012EVB-01) PSoC&trade; 62S1 Wi-Fi Bluetooth&reg; pioneer kit |
| [CY8CKIT-064B0S2-4343W](http://www.cypress.com/CY8CKIT-064B0S2-4343W) PSoC&trade; 64 "Secure Boot" Wi-Fi Bluetooth&reg; pioneer kit |
| **Libraries**                                                |                                                              |
| PSoC&trade; 6 peripheral driver library (PDL) and docs  | [mtb-pdl-cat1](https://github.com/cypresssemiconductorco/mtb-pdl-cat1) on GitHub |
| Hardware abstraction layer (HAL) Library and docs    | [mtb-hal-cat1](https://github.com/cypresssemiconductorco/mtb-hal-cat1) on GitHub |
| Retarget IO - A utility library to retarget the standard input/output (STDIO) messages to a UART port | [retarget-io](https://github.com/cypresssemiconductorco/retarget-io) on GitHub |
| **Middleware**                                               |                                                              |
| CapSense&trade; library and documents                                   | [capsense](https://github.com/cypresssemiconductorco/capsense) on GitHub |
| Links to all PSoC&trade; 6 MCU middleware                           | [psoc6-middleware](https://github.com/cypresssemiconductorco/psoc6-middleware) on GitHub |
| **Tools**                                                    |                                                              |
| [Eclipse IDE for ModusToolbox&trade;](https://www.cypress.com/modustoolbox) | The cross-platform, Eclipse-based IDE for IoT designers that supports application configuration and development targeting converged MCU and wireless systems. |
| [PSoC&trade; Creator™](https://www.cypress.com/products/psoc-creator-integrated-design-environment-ide) | The legacy Cypress IDE for PSoC&trade; and FM0+ MCU development. |

## Other resources

Cypress provides a wealth of data at www.cypress.com to help you select the right device, and quickly and effectively integrate it into your design.

For PSoC&trade; 6 MCU devices, see [How to design with PSoC&trade; 6 MCU - KBA223067](https://community.cypress.com/docs/DOC-14644) in the Cypress community.

## Document history

Document title: *CE229254* - *AnyCloud: secure TCP server*

| Version | Description of Change |
| ------- | --------------------- |
| 1.0.0   | New code example      |
| 1.1.0   |  Updated for ModusToolbox&trade; 2.1. <br>Code updated to use Cypress Secure Sockets and Wi-Fi Connection Manager libraries. |
| 1.2.0   | Makefile updated to sync with BSP changes. <br>Code updated to use RTOS Task Notification. |
| 1.3.0   | Updated to add link-local IPv6 support. |
| 2.0.0   | Major update to support ModusToolbox&trade; software v2.2, added support for new kits.<br />Added soft AP Wi-Fi interface mode<br /> This version is not backward compatible with ModusToolbox&trade; software v2.1.<br /> Updated to support FreeRTOS v10.3.1 |
------

All other trademarks or registered trademarks referenced herein are the property of their respective owners.

![banner](images/ifx-cy-banner.png)

-------------------------------------------------------------------------------

© Cypress Semiconductor Corporation, 2020-2021. This document is the property of Cypress Semiconductor Corporation, an Infineon Technologies company, and its affiliates ("Cypress").  This document, including any software or firmware included or referenced in this document ("Software"), is owned by Cypress under the intellectual property laws and treaties of the United States and other countries worldwide.  Cypress reserves all rights under such laws and treaties and does not, except as specifically stated in this paragraph, grant any license under its patents, copyrights, trademarks, or other intellectual property rights.  If the Software is not accompanied by a license agreement and you do not otherwise have a written agreement with Cypress governing the use of the Software, then Cypress hereby grants you a personal, non-exclusive, nontransferable license (without the right to sublicense) (1) under its copyright rights in the Software (a) for Software provided in source code form, to modify and reproduce the Software solely for use with Cypress hardware products, only internally within your organization, and (b) to distribute the Software in binary code form externally to end users (either directly or indirectly through resellers and distributors), solely for use on Cypress hardware product units, and (2) under those claims of Cypress’s patents that are infringed by the Software (as provided by Cypress, unmodified) to make, use, distribute, and import the Software solely for use with Cypress hardware products.  Any other use, reproduction, modification, translation, or compilation of the Software is prohibited.
<br>
TO THE EXTENT PERMITTED BY APPLICABLE LAW, CYPRESS MAKES NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, WITH REGARD TO THIS DOCUMENT OR ANY SOFTWARE OR ACCOMPANYING HARDWARE, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  No computing device can be absolutely secure.  Therefore, despite security measures implemented in Cypress hardware or software products, Cypress shall have no liability arising out of any security breach, such as unauthorized access to or use of a Cypress product.  CYPRESS DOES NOT REPRESENT, WARRANT, OR GUARANTEE THAT CYPRESS PRODUCTS, OR SYSTEMS CREATED USING CYPRESS PRODUCTS, WILL BE FREE FROM CORRUPTION, ATTACK, VIRUSES, INTERFERENCE, HACKING, DATA LOSS OR THEFT, OR OTHER SECURITY INTRUSION (collectively, "Security Breach").  Cypress disclaims any liability relating to any Security Breach, and you shall and hereby do release Cypress from any claim, damage, or other liability arising from any Security Breach.  In addition, the products described in these materials may contain design defects or errors known as errata which may cause the product to deviate from published specifications.  To the extent permitted by applicable law, Cypress reserves the right to make changes to this document without further notice. Cypress does not assume any liability arising out of the application or use of any product or circuit described in this document.  Any information provided in this document, including any sample design information or programming code, is provided only for reference purposes.  It is the responsibility of the user of this document to properly design, program, and test the functionality and safety of any application made of this information and any resulting product.  "High-Risk Device" means any device or system whose failure could cause personal injury, death, or property damage.  Examples of High-Risk Devices are weapons, nuclear installations, surgical implants, and other medical devices.  "Critical Component" means any component of a High-Risk Device whose failure to perform can be reasonably expected to cause, directly or indirectly, the failure of the High-Risk Device, or to affect its safety or effectiveness.  Cypress is not liable, in whole or in part, and you shall and hereby do release Cypress from any claim, damage, or other liability arising from any use of a Cypress product as a Critical Component in a High-Risk Device.  You shall indemnify and hold Cypress, including its affiliates, and its directors, officers, employees, agents, distributors, and assigns harmless from and against all claims, costs, damages, and expenses, arising out of any claim, including claims for product liability, personal injury or death, or property damage arising from any use of a Cypress product as a Critical Component in a High-Risk Device.  Cypress products are not intended or authorized for use as a Critical Component in any High-Risk Device except to the limited extent that (i) Cypress’s published data sheet for the product explicitly states Cypress has qualified the product for use in a specific High-Risk Device, or (ii) Cypress has given you advance written authorization to use the product as a Critical Component in the specific High-Risk Device and you have signed a separate indemnification agreement.
<br>
Cypress, the Cypress logo, and combinations thereof, WICED, ModusToolBox, PSoC, CapSense, EZ-USB, F-RAM, and Traveo are trademarks or registered trademarks of Cypress or a subsidiary of Cypress in the United States or in other countries.  For a more complete list of Cypress trademarks, visit cypress.com.  Other names and brands may be claimed as property of their respective owners.
