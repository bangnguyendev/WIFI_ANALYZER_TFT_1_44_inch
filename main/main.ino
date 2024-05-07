/* LIBRARY ARDUINO DEFINE  */
#include <ESP8266WiFi.h>
#include <Adafruit_GFX.h>	 // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <SPI.h>

/*   TFT 1.44 inch   */
//GPIO Wemos D1 mini
#define TFT_DC D0 // PIN RS OR DC
#define TFT_RST -1

//SPI interface - none using MISO
#define TFT_CS D8 // CS
#define TFT_MOSI D7 // Data out
#define TFT_SCLK D5 // Clock out

// For ST7735-based displays, we will use this call
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);
// yield(); // https://forum.arduino.cc/t/soft-wdt-reset-nodemcu/425567/2
char string_tft[50] = "";
/*  End TFT 1.44 inch   */

#define WIDTH 128
#define HEIGHT 128
#define BANNER_HEIGHT 8

#define GRAPH_BASELINE (HEIGHT - 18)
#define GRAPH_HEIGHT (HEIGHT - 52)
#define CHANNEL_WIDTH (WIDTH / 11)

// RSSI RANGE
#define RSSI_CEILING -40
#define RSSI_FLOOR -100
#define NEAR_CHANNEL_RSSI_ALLOW -70

// define color
#define TFT_WHITE   ST7735_WHITE    /* 255, 255, 255 */
#define TFT_BLACK   ST7735_BLACK    /*   0,   0,   0 */
#define TFT_RED     ST7735_RED      /* 255,   0,   0 */
#define TFT_ORANGE  0xFD20          /* 255, 165,   0 */
#define TFT_YELLOW  ST7735_YELLOW   /* 255, 255,   0 */
#define TFT_GREEN   ST7735_GREEN    /*   0, 255,   0 */
#define TFT_CYAN    ST7735_CYAN     /*   0, 255, 255 */
#define TFT_BLUE    ST7735_BLUE     /*   0,   0, 255 */
#define TFT_MAGENTA ST7735_MAGENTA  /* 255,   0, 255 */


// Channel color mapping from channel 1 to 14
uint16_t channel_color[] = {
  TFT_RED, TFT_YELLOW, TFT_GREEN, TFT_CYAN, TFT_MAGENTA,
  TFT_RED, TFT_YELLOW, TFT_GREEN, TFT_CYAN, TFT_MAGENTA,
  TFT_RED, TFT_YELLOW, TFT_GREEN,
};

uint8_t scan_count = 0;


void setup() {
  Serial.begin(115200);
  // Initialize LED_BUILTIN
  pinMode(LED_BUILTIN, OUTPUT);
  // Initialize TFT 1.44 inch
	tft.initR(INITR_144GREENTAB); // Init ST7735R chip, green tab
  tft.setRotation(3);
  tft.fillScreen(ST7735_BLUE);
  yield();
  // put your setup code here, to run once:
  tft.setTextWrap(false);
  tft.setTextColor(ST77XX_WHITE, TFT_RED);
  tft.print(" ESP");
  tft.setTextColor(ST77XX_WHITE, TFT_ORANGE);
  tft.print("8266");
  tft.setTextColor(ST77XX_BLACK, TFT_GREEN);
  tft.print(" WiFi");
  tft.setTextColor(ST77XX_WHITE, TFT_BLUE);
  tft.print("Analyzer");

  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  tft.setCursor(0, BANNER_HEIGHT + 20);
  tft.setTextSize(1);
  tft.print(" Wi-Fi scanning . . .");
  tft.setTextSize(2);
  digitalWrite(LED_BUILTIN, LOW); // On led
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  // rest for WiFi routine?
  delay(100);
  digitalWrite(LED_BUILTIN, HIGH); // Off led
}

void loop() {
  // put your main code here, to run repeatedly:
  uint8_t ap_count[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  int32_t max_rssi[] = {-100, -100, -100, -100, -100, -100, -100, -100, -100, -100, -100, -100, -100, -100};

  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();

  // clear old graph
  tft.fillRect(0, BANNER_HEIGHT, 128, 128, TFT_BLACK);
  tft.setTextSize(1);

  int ledState = LOW;
  if (n == 0) {
    tft.setTextColor(ST77XX_BLACK);
    tft.setCursor(0, BANNER_HEIGHT);
    tft.println("no networks found");
  } else {
    // list wifi
    int templist = 0;

    for (int i = 0; i < n; i++) {
      int32_t rssi = WiFi.RSSI(i);
      ledState = !ledState;
      digitalWrite(LED_BUILTIN, ledState); // Bật hoặc tắt LED
      templist = templist + 10;
      Serial.println(rssi);
      int32_t channel = WiFi.channel(i);
      uint16_t color = channel_color[channel - 1];

      // Print SSID, signal strengh and if not encrypted
      Serial.println(WiFi.SSID(i));
      tft.setTextColor(color);
      tft.setCursor(2, BANNER_HEIGHT + templist);
      tft.print('[');
      tft.print(channel);
      tft.print(']');
      tft.setCursor(27, BANNER_HEIGHT + templist);
      tft.print(WiFi.SSID(i));
      tft.print('(');
      tft.print(rssi);
      tft.print(')');
      if (WiFi.encryptionType(i) == ENC_TYPE_NONE) {
        tft.print('*');
      }
      
      // rest for WiFi routine?
      delay(25);
    }
    delay(1000);  

    // clear old graph
    tft.fillRect(0, BANNER_HEIGHT, 128, 128, TFT_BLACK);
    tft.setTextSize(1);

    // plot found WiFi info
    for (int i = 0; i < n; i++) {
      int32_t rssi = WiFi.RSSI(i);
      ledState = !ledState;
      digitalWrite(LED_BUILTIN, ledState); // Bật hoặc tắt LED
      if (rssi > -82 )
      {
        Serial.println(rssi);
        int32_t channel = WiFi.channel(i);
        uint16_t color = channel_color[channel - 1];
        int height = constrain(map(rssi, RSSI_FLOOR, RSSI_CEILING, 1, GRAPH_HEIGHT), 1, GRAPH_HEIGHT);

        // channel stat
        ap_count[channel - 1]++;
        if (rssi > max_rssi[channel - 1]) {
          max_rssi[channel - 1] = rssi;
        }

        tft.drawLine(channel * CHANNEL_WIDTH, GRAPH_BASELINE - height, (channel - 1) * CHANNEL_WIDTH, GRAPH_BASELINE + 1, color);
        tft.drawLine(channel * CHANNEL_WIDTH, GRAPH_BASELINE - height, (channel + 1) * CHANNEL_WIDTH, GRAPH_BASELINE + 1, color);

        // Print SSID, signal strengh and if not encrypted
        tft.setTextColor(color);
        tft.setCursor((channel - 1) * CHANNEL_WIDTH, GRAPH_BASELINE - 10 - height);
        tft.print(WiFi.SSID(i));
        Serial.println(WiFi.SSID(i));
        tft.setCursor((channel - 1) * CHANNEL_WIDTH, GRAPH_BASELINE - 20 - height);
        tft.print('(');
        tft.print(rssi);
        tft.print(')');
        if (WiFi.encryptionType(i) == ENC_TYPE_NONE) {
          tft.print('*');
        }

        // rest for WiFi routine?
        delay(10);
        }
      }
  }

  // print WiFi stat
  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(0, BANNER_HEIGHT);
  tft.print(" Networks found: ");
  tft.print(n);

  tft.setCursor(0, BANNER_HEIGHT + 10);
  tft.print(" Suggested channels: ");
  tft.setCursor(10, BANNER_HEIGHT + 20);
  bool listed_first_channel = false;

  for (int i = 1; i <= 11; i++) { // channels 12-14 may not available
    if ((i == 1) || (max_rssi[i - 2] < NEAR_CHANNEL_RSSI_ALLOW)) { // check previous channel signal strengh
      if ((i == sizeof(channel_color)) || (max_rssi[i] < NEAR_CHANNEL_RSSI_ALLOW)) { // check next channel signal strengh
        if (ap_count[i - 1] == 0) { // check no AP exists in same channel
          if (!listed_first_channel) {
            listed_first_channel = true;
          } else {
            tft.print(", ");
          }
          tft.print(i);
        }
      }
    }
  }

  // draw graph base axle
  tft.drawFastHLine(0, GRAPH_BASELINE, 128, TFT_WHITE);
  for (int i = 1; i <= 11; i++) {
    tft.setTextColor(channel_color[i - 1]);
    tft.setCursor((i * CHANNEL_WIDTH) - ((i < 10)?3:6), GRAPH_BASELINE + 2);
    tft.print(i);
    if (ap_count[i - 1] > 0) {
      tft.setCursor((i * CHANNEL_WIDTH) - ((ap_count[i - 1] < 10)?9:12), GRAPH_BASELINE + 11);
      tft.print(' ');
      tft.print(ap_count[i - 1]);
      tft.print(' ');
    }
  }

  // Wait a bit before scanning again
  delay(2000);
}
