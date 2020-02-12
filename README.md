# AMDTPS-PortSDK-2.3.2-toArduino-Core-1.0.29-Modified-wth-SDK2.3.2-files
Update Arduino Apollo3 core using AmbiqMicro SDK 2.3.2 files and optionally port amdtps example to that Arduino Core

AmbiqMicro SDK - PORT SDK2.3.2 to Arduino 1.0.29 core (SDK 2.3.2)
- Replaces Arduino core based on SDK 2.2.0 with same core based on SDK 2.3.2 files

Assumes you have working Git Bash:
https://learn.sparkfun.com/tutorials/using-sparkfun-edge-board-with-ambiq-apollo3-sdk?_ga=2.161202683.718301707.1581524039-1386820254.1566730454

- Allows complete Arduino compile of amdtps example (SDK 2.3.2) without modification of SDK files on modified Sparkfun Arduino Core 1.0.29 (SDK 2.3.2) - use this repo's amdtps directory and files to to create Arduino Sketch.

- Download platform.txt from this repo and replace the one in your SparkFun Core 1.0.29 directory.  For example:
c:/Users/YourName/AppData/Local/Arduino15/packages/SparkFun/hardware/apollo3/1.0.29

- Copy AmbiqSuiteSDK 2.3.2 files to the Arduino Core 1.0.29

  - if using default location for SDK of c:/Users/YourName/Documents/AmbiqSuiteSDK and core directory listed above, download makesdk.sh       and place in c:/Users/YourName/bin (create directory if needed).
    - From Git Bash terminal window, change directory to SDK: cd /AmbiqSuiteSdk
    - Type makesdk.sh and watch it go...
    Might have to chmod u+x makesdk.sh first.

  - OR, Download Port SDK2_3_2 To SparkFun Arduino Core1_2_29.doc file from this repo and follow the step-by-step copies.

- Should you want working phone to chip bi-directional BLE communication.
    Download amdtps directory from this repo and place in your Arduino sketch directory. 
    Download app-release.apk and install on your phone (google install .apk on android) or use Google Play Store app = Nrf Connect.           (Hint: send TEXT charachters).

