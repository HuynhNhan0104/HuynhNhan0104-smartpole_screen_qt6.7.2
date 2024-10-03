# Smartpole_screen_qt
# Dependency
[QT>=6.7.2](https://www.qt.io/)  <br>
[Qmqtt module of qt](https://code.qt.io/cgit/qt/qtmqtt.git/) <br>
[streamlink](https://streamlink.github.io/) <br>
[ffmepg](https://www.ffmpeg.org/)  <br>
# Description
Implement air sensor system and screen display of an smartpole device. The air sensor will be read and display on top screen, the remain of screen will play an media.
Air sensor will commonication with controller througt modbus 485 protocol and update these value to screen, this screen also display media which source is control throung mqtt protocol. <br>
# Hardware 
ES-INTEGRATED-ODR-01 <br>
Meteorological shutter is a fixed all-in-one ground automatic observation equipment.
Observation items mainly include wind direction, wind speed, temperature, humidity,
atmospheric pressure, illuminance, carbon dioxide concentration, PM2.5, PM10,
oxygen concentration, ammonia concentration, hydrogen sulfide concentration, noise
and other meteorological elements. <br>
Meteorological shutters can be widely used in urban environment measurement,
agricultural monitoring, industrial governance and other environments in order to
collect more abundant and effective monitoring data. <br>
More details in [here](https://epcb.vn/products/cam-bien-giam-sat-cac-moi-truong-ngoai-troi-sensor-outdoor) <br>
![image](https://github.com/user-attachments/assets/32153c4c-dc30-4ace-9c29-4df74b029a8b)
# Layout of screen on smartpole
![image](https://github.com/user-attachments/assets/2b84c36e-38b1-4702-8c7b-888d63208464)
With the radial bar have 3 state color(green if value in low, yellow if value in medium and red in high).<br>
![image](https://github.com/user-attachments/assets/5e4a0e02-2ea8-419a-a357-c970a65b7657) ![image](https://github.com/user-attachments/assets/915a5bae-42da-4fb7-80f3-c44b3e5ef115) ![image](https://github.com/user-attachments/assets/8583b0a0-0f89-4332-9a86-f72b73b613e6)
# Software Design
## File structure 
```plaintext
repository/
├── serial/
│   ├── ODR_01.h
│   ├── ODR_01.cpp
│   ├── rs485.h
│   ├── rs485.cpp
│   ├── serial_interface.h
│   ├── serial_interface.cpp
│   ├── serial_port.h
│   ├── serial_port.cpp
├── main.cpp
├── Main.qml
├── RadialBarProgress.qml
├── dashboardController.h
├── dashboardController.cpp
├── VideoView.qml
├── videoControl.h
├── videoControl.cpp
└── README.md
```
* serial: low level interface using for reading value of sensor.
* RadialBarProgress.qml (quick makeup language): this file describe UI of the radial bar
* VideoView.qml: describe UI of the media player to display media
* dashboardController.h and dashboardController.cpp: control behavior of the radial bar(reading data from sensor, update to the dashboard, sending data to server throught mqtt protocol)
* videoControl.h and videoControl.cpp: control behavior of the media player(mqtt communitcation to update the source of player)
## MVC(model-view-controller)
* Model: serial directory with contain interface to get data from sensor
* View: *.qml file  which describe ui of each components.
* Controller: *.h and *.cpp which control behavior of each components.
![image](https://github.com/user-attachments/assets/1ebf3705-6c87-40af-b1c0-8fc0a9b6fac0)








