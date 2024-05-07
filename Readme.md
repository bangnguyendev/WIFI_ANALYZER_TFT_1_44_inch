## Wi-Fi Analyzer for ESP8266

This Arduino sketch provides functionality to analyze Wi-Fi networks using an ESP8266 microcontroller and a 1.44-inch TFT display. It scans for nearby Wi-Fi networks, measures their signal strength (RSSI), and displays the information graphically on the screen.

### Hardware Requirements
- ESP8266 microcontroller
- 1.44-inch TFT display (ST7735-based)
- Required Libraries: ESP8266WiFi, Adafruit_GFX, Adafruit_ST7735, SPI

![Ảnh 1](/image/1.jpg)

![Ảnh 1](/image/1.jpg)

![Ảnh 1](/image/1.jpg)

### Pin Configuration
- **TFT 1.44 inch**:
  - TFT_DC: D0
  - TFT_RST: -1
  - TFT_CS: D8
  - TFT_MOSI: D7
  - TFT_SCLK: D5

### Graphical Display
The sketch utilizes the Adafruit_GFX and Adafruit_ST7735 libraries to display graphical information about nearby Wi-Fi networks. It visualizes Wi-Fi networks on a graph, showing signal strength (RSSI) per channel.

### Color Legend
Channel Color Legend:
Channel 1: TFT_RED
Channel 2: TFT_YELLOW
Channel 3: TFT_GREEN
Channel 4: TFT_CYAN
Channel 5: TFT_MAGENTA
Channel 6: TFT_RED
Channel 7: TFT_YELLOW
Channel 8: TFT_GREEN
Channel 9: TFT_CYAN
Channel 10: TFT_MAGENTA
Channel 11: TFT_RED
Channel 12: TFT_YELLOW
Channel 13: TFT_GREEN
Channel 14: TFT_CYAN

### Functionality
1. **Initialization**: Initializes the TFT display and sets up the Wi-Fi scanning process.
2. **Wi-Fi Scanning**: Scans for nearby Wi-Fi networks and displays their information on the screen.
3. **Graphical Representation**: Displays Wi-Fi networks on a graph, indicating signal strength per channel.
4. **Channel Analysis**: Suggests optimal Wi-Fi channels based on signal strength and network availability.

### Usage
1. Upload the sketch to your ESP8266 microcontroller.
2. Connect the TFT display and ESP8266 according to the pin configuration.
3. Power on the device to start Wi-Fi scanning and analysis.

### Notes
- Adjustments can be made to the code for customizations such as color schemes or display layouts.
- Ensure the proper installation of required libraries before compiling the sketch.

Feel free to modify and enhance the code according to your project requirements. Happy coding!