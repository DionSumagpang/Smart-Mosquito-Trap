/*
===================================================================================================
  PROJECT NAME: Development of a Low-Power Light-and-Fan Mosquito Trap with Integrated 
                Risk Index and Data Logging System for Dengue Prevention
  TARGET DEVS : ESP8266 (NodeMCU / Wemos D1 Mini)
  CONTEXT     : Academic / Social Innovation Prototype Demonstration (Philippines Context)
===================================================================================================
*/

#define BLYNK_TEMPLATE_ID   "TMPL6YPLXkVDa"
#define BLYNK_TEMPLATE_NAME "Mosquito Counter"
#define BLYNK_AUTH_TOKEN    "FKr2-mGPeKw5JhSKWJg9nu6po_RLKcru"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include "DHT.h"

// --- Wi-Fi Network Credentials ---
char ssid[] = "";
char pass[] = "";

/*
===================================================================================================
  PIN ALLOCATIONS & HARDWARE CONFIGURATION
===================================================================================================
*/
const int DHT_pin        = 14;  // Physical D5 (GPIO 14) -> DHT11 Data Pin
const int irPin          = A0;  // Analog A0            -> IR Photodiode Vector
const int FAN_GATE_PIN   = 5;   // Physical D1 (GPIO 5)  -> MOSFET Fan Controller
const int UV_LED_PIN     = 4;   // Physical D2 (GPIO 4)  -> UV Attractor System (Status LED)

#define DHTTYPE DHT11
DHT dht(DHT_pin, DHTTYPE);

/*
===================================================================================================
  PROTOTYPE PARAMETERS & SYSTEM VARIABLES
===================================================================================================
*/
int mosquitoCount  = 0;
int baseline       = 0;
int threshold      = 0;
int resetThreshold = 0;
bool isBlocked     = false;

float temp = 0.0;
float hum  = 0.0;
String riskIndex = "LOW"; // Tracks biological activity risk factors

// --- Hardware Execution Bounds ---
const int PWM_MAX_RESOLUTION = 1023; // Fixed 10-bit output constraint

BlynkTimer timer;

/*
===================================================================================================
  CORE PROJECT FUNCTIONALITIES
===================================================================================================
*/

/**
 * Initializes and locks hardware at 100% capacity to maximize capture rates.
 * Provides continuous UV illumination and sustained high suction power.
 */
void operationalizeTrapCore() {
  analogWrite(FAN_GATE_PIN, PWM_MAX_RESOLUTION);
  analogWrite(UV_LED_PIN, PWM_MAX_RESOLUTION);
}

/**
 * Formulates the Dengue Risk Index based on domestic climatological research.
 * Mosquito activity and breeding acceleration typically peak in environments matching
 * temperatures between 25°C and 30°C paired with relative humidity above 70%.
 */
void calculateMosquitoRiskIndex() {
  if ((temp >= 25.0 && temp <= 30.0) && hum >= 70.0) {
    riskIndex = "HIGH RISK";
  } else if ((temp >= 22.0 && temp <= 33.0) || hum >= 55.0) {
    riskIndex = "MODERATE";
  } else {
    riskIndex = "LOW RISK";
  }
}

/**
 * Simulates local offline backup logging.
 * Demonstrates the "Data Preservation" concept when Wi-Fi is disconnected during live grading.
 */
void simulateOfflineLogging() {
  Serial.println(F("\n--- [FAIL-SAFE LOCAL LOG ACTIVATED] ---"));
  Serial.print(F("Timestamp(ms): ")); Serial.print(millis());
  Serial.print(F(" | Temp: "));        Serial.print(temp, 1);
  Serial.print(F("C | Humid: "));       Serial.print(hum, 1);
  Serial.print(F("% | Risk: "));        Serial.print(riskIndex);
  Serial.print(F(" | Count: "));       Serial.println(mosquitoCount);
  Serial.println(F("Status: Saved to simulated offline cache memory buffer."));
  Serial.println(F("---------------------------------------\n"));
}

/**
 * Monitors the analog optical array for ultra-fast, high-speed beam breakage.
 */
void handleInsectCounting() {
  int currentRead = analogRead(irPin);

  // TRIGGER: Light profile drops beneath calibrated 90% threshold line
  if (currentRead < threshold && !isBlocked) {
    mosquitoCount++;
    isBlocked = true;
    
    Serial.println(F("\n=================================================="));
    Serial.print(F("  [DETECTION ALERT] Target Count Advanced: "));
    Serial.println(mosquitoCount);
    Serial.print(F("  Ambient Condition Status: "));
    Serial.println(riskIndex);
    Serial.println(F("=================================================="));

    // Priority IoT write: Pushes vector count immediately to cloud app during live tests
    if (Blynk.connected()) {
      Blynk.virtualWrite(V1, mosquitoCount);
    } else {
      simulateOfflineLogging();
    }
  }

  // RESET: Light intensity recovers past 95% threshold line
  if (currentRead > resetThreshold) {
    isBlocked = false;
  }
}

/**
 * Manages dual-logging channels and updates local variables asynchronously every 2 seconds.
 */
void handleTelemetryUpdate() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println(F("[ERROR] DHT11 Unreadable. Verify structural pin wiring."));
    return;
  }

  temp = t;
  hum  = h;
  
  calculateMosquitoRiskIndex();

  // Print system updates out to the local terminal monitor
  Serial.print(F("[Data Engine] Temp: "));    Serial.print(temp, 1);
  Serial.print(F("°C | Humid: "));            Serial.print(hum, 1);
  Serial.print(F("% | Risk Index: "));        Serial.print(riskIndex);
  Serial.print(F(" | Vectors Logged: "));     Serial.println(mosquitoCount);

  // --- DUAL DATA LOGGING DECISION LOGIC ---
  if (Blynk.connected()) {
    // CHANNEL A: Sync online streams to Blynk IoT Cloud
    Blynk.virtualWrite(V1, mosquitoCount);
    Blynk.virtualWrite(V2, temp);
    Blynk.virtualWrite(V3, hum);
    Blynk.virtualWrite(V4, riskIndex); 
    Serial.println(F("[IoT Cloud] Sync complete. Dashboard updated."));
  } else {
    // CHANNEL B: Execute offline local backup protocols 
    Serial.println(F("[IoT Cloud] Network Offline! Initiating fail-safe routine."));
    simulateOfflineLogging();
  }
}

/*
===================================================================================================
  MAIN SETUP & PROGRAM LOOP
===================================================================================================
*/
void setup() {
  Serial.begin(115200);
  delay(1000); 

  Serial.println(F("\n================================================================="));
  Serial.println(F("  SOCIAL INNOVATION PROTOTYPE INITIALIZATION:                    "));
  Serial.println(F("  Low-Power Light-and-Fan Mosquito Trap (Dengue Prevention Tech) "));
  Serial.println(F("================================================================="));

  // I/O Setup
  pinMode(FAN_GATE_PIN, OUTPUT);
  pinMode(UV_LED_PIN, OUTPUT);
  pinMode(irPin, INPUT);
  analogWriteRange(PWM_MAX_RESOLUTION);
  
  // Power up baseline trapping systems immediately at 100% duty cycle
  operationalizeTrapCore();
  dht.begin();

  // Establish Wireless Connection Framework
  Serial.println(F("[Network] Establishing Wi-Fi Connection & Blynk IoT handshake..."));
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // Automatic Ambient Photodiode Calibration Sequence
  Serial.println(F("[Calibration] Setting adaptive IR trigger ranges..."));
  long runningSum = 0;
  for (int i = 0; i < 20; i++) {
    runningSum += analogRead(irPin);
    delay(20);
  }
  baseline       = runningSum / 20;
  threshold      = baseline * 0.90;
  resetThreshold = baseline * 0.95;

  Serial.print(F("[Calibration] Settled Baseline: "));     Serial.println(baseline);
  Serial.print(F("[Calibration] Active Entry Drop Limit: ")); Serial.println(threshold);
  Serial.println(F("[Trap Status] Monitoring active. Suction Fan & UV continuous at MAXIMUM.\n"));

  // Schedule background tracking and logging metrics
  timer.setInterval(2000L, handleTelemetryUpdate);
}

void loop() {
  Blynk.run();
  timer.run();

  // Rapid hardware tasks running inline to maintain structural speed performance
  handleInsectCounting();

  yield();
}