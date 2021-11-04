# M17 Analog Hotspot Gateway Project
How to Make digital voice radio (M17 Digital Voice) analog gateway yourself over the internet width ESP32 NodeMCU/DOIT/DevKIT Module.
The project made M17AnalogGateway to convert digital voice with M17 mode to Analog Radio.

## Simple Circuit
![image](https://github.com/nakhonthai/M17AnalogGateway/blob/master/schematic/M17Hotspot.png)

To connect, connect the MIC,PTT,SPEKER to Analog Radio.

## Howto Devellop
-Pull and Compile by PlatformIO on the Visual Studio Code.

## M17Hotspot firmware installation (do it first, next, update via web)
- 1.Connect the USB cable to the ESP32 Module.
- 2.Download firmware and open the program ESP32 DOWNLOAD TOOL, set it in the firmware upload program, set the firmware to M17Hotspot_Vxx.bin, location 0x10000 and partitions.bin at 0x8000 and bootloader.bin at 0x1000 and boot.bin at 0xe000, if not loaded, connect GPIO0 cable to GND, press START button finished, press power button or reset (red) again.
- 3.Then go to WiFi AP SSID: M17Hotspot and open a browser to the website. http://192.168.4.1 Can be fixed Or turn on your Wi-Fi router.

## ESP32 Flash Download Tools
https://www.espressif.com/en/support/download/other-tools

## Setting up using a web browser
- 1.Set up Wi-Fi in the Settings tab. Connect to a router or device that shares the Internet. According to the router name (SSID) and password
- 2.Set up digital voices in the IoT tab in the server side. Refactor service also does not need to be modified (except for testing elsewhere), the test room in the M17 Module compartment has 26 rooms, AZ insert one. letter Users put their own name in the myCallSign field and the device sequence (some people use multiple machines) can be assigned 26 characters, one letter A-Z as well.

## M17-THA Reflector
	Name: M17-THA
	IP: 203.150.19.24
	PORT: 17000
	Dashboard: https://m17.dprns.com