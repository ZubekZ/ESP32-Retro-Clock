#include <TFT_eSPI.h>
#include <SPI.h>
#include <WiFi.h>
#include <time.h>

// ================================
// NETWORK & TIME CONFIGURATION
// ================================
// SECURITY WARNING: Never commit real passwords to GitHub!
const char* ssid     = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// Timezone (Brazil = UTC -3). 
// 3600 seconds * -3 = -10800
const long  gmtOffset_sec = -10800; 
const int   daylightOffset_sec = 0; 

TFT_eSPI tft = TFT_eSPI();

// ================================
// GLOBAL VARIABLES
// ================================
unsigned long lastModeTime = 0;
const unsigned long modeInterval = 5000; // Switch every 5 seconds
int currentMode = 0;
int previousMode = -1; // To detect mode change
const int totalModes = 2;

// To avoid unnecessary redraws
String lastTimeDisplay = ""; 

// ================================
// WIFI & CLOCK SETUP
// ================================
void setupTime() {
  tft.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    tft.print(".");
    attempts++;
  }

  tft.fillScreen(TFT_BLACK);
  
  if (WiFi.status() == WL_CONNECTED) {
      tft.println("\nWiFi Connected!");
      // Config internet time
      configTime(gmtOffset_sec, daylightOffset_sec, "pool.ntp.org", "time.nist.gov");
  } else {
      tft.println("\nWiFi Failed. Offline Mode.");
  }
}

// Get system time
void updateLocalTime(String &timeStr, String &dateStr) {
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    timeStr = "--:--:--";
    dateStr = "--/--/--";
    return;
  }
  
  char timeBuffer[10];
  strftime(timeBuffer, sizeof(timeBuffer), "%H:%M:%S", &timeinfo);
  timeStr = String(timeBuffer);

  char dateBuffer[12];
  strftime(dateBuffer, sizeof(dateBuffer), "%d/%m/%Y", &timeinfo);
  dateStr = String(dateBuffer);
}

// ================================
// RENDERING FUNCTIONS
// ================================

void drawClock() {
  // If mode just changed, clear screen and draw static elements
  if (currentMode != previousMode) {
    tft.fillScreen(TFT_BLACK);
    // Draw decorative lines
    tft.drawLine(20, tft.height()-30, tft.width()-20, tft.height()-30, TFT_GREEN);
    tft.drawLine(20, tft.height()-28, tft.width()-60, tft.height()-28, TFT_GREEN);
    previousMode = currentMode;
    lastTimeDisplay = ""; // Force text redraw
  }

  String timeStr, dateStr;
  updateLocalTime(timeStr, dateStr);

  // Only redraw if time changed to prevent flicker
  if (timeStr != lastTimeDisplay) {
    tft.setTextColor(TFT_GREEN, TFT_BLACK); // BLACK background erases previous number
    tft.setTextDatum(MC_DATUM);

    // Time
    tft.setFreeFont(&FreeSansBold24pt7b);
    tft.drawString(timeStr, tft.width()/2, tft.height()/2 - 20);

    // Date
    tft.setFreeFont(&FreeSansBold12pt7b);
    tft.drawString(dateStr, tft.width()/2, tft.height()/2 + 25);
    
    lastTimeDisplay = timeStr;
  }
}

void drawFace() {
  // If mode changed, clear everything
  if (currentMode != previousMode) {
    tft.fillScreen(TFT_BLACK);
    
    // Static Text
    tft.setTextColor(TFT_CYAN, TFT_BLACK);
    tft.setTextDatum(MC_DATUM);
    tft.setFreeFont(&FreeSansBold12pt7b);
    tft.drawString("HELLO WORLD", tft.width()/2, tft.height()/2 + 70);
    
    previousMode = currentMode;
  }

  // Blinking Logic (Non-blocking)
  static bool isClosed = false;
  static unsigned long blinkTimer = 0;
  
  // Blink every ~3 seconds, duration 200ms
  if (millis() - blinkTimer > (isClosed ? 200 : 3000)) {
    blinkTimer = millis();
    isClosed = !isClosed;
    
    // Redraw only the eyes area
    int cx = tft.width()/2;
    int cy = tft.height()/2 - 20;
    int offset = 40;

    // Clear eye area
    tft.fillRect(cx - 60, cy - 30, 120, 60, TFT_BLACK);

    if (!isClosed) {
      // Open Eyes
      tft.fillCircle(cx - offset, cy, 15, TFT_WHITE); // Left
      tft.fillCircle(cx + offset, cy, 15, TFT_WHITE); // Right
      
      // Eye sparkle (Kawaii style)
      tft.fillCircle(cx - offset - 4, cy - 4, 4, TFT_BLACK);
      tft.fillCircle(cx + offset - 4, cy - 4, 4, TFT_BLACK);
    } else {
      // Closed Eyes (Line)
      tft.fillRect(cx - offset - 15, cy, 30, 4, TFT_WHITE);
      tft.fillRect(cx + offset - 15, cy, 30, 4, TFT_WHITE);
    }
    
    // Mouth (Redraw to ensure no overlap issues)
    tft.fillCircle(cx, cy + 30, 10, TFT_WHITE); 
    tft.fillCircle(cx, cy + 30, 5, TFT_BLACK);  
  }
}

// ================================
// MAIN LOOP
// ================================
void setup() {
  Serial.begin(115200);
  tft.init();
  tft.setRotation(1); // Landscape
  tft.fillScreen(TFT_BLACK);
  
  setupTime();
}

void loop() {
  // Mode Switching Logic
  if (millis() - lastModeTime > modeInterval) {
    lastModeTime = millis();
    currentMode++;
    if (currentMode >= totalModes) currentMode = 0;
  }

  if (currentMode == 0) {
    drawClock();
  } else {
    drawFace();
  }
}
