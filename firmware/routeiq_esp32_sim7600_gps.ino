/*
 * ROUTEIQ ESP32 + SIM7600 + GPS reference firmware
 *
 * Hardware assumptions:
 * - ESP32
 * - SIM7600 LTE/GSM modem on UART2 (RX=16, TX=17)
 * - GPS module on UART1 (RX=4, TX=5)
 * - SIM card with data enabled and correct APN
 *
 * Arduino IDE libraries:
 *   TinyGSM, TinyGPSPlus
 *
 * IMPORTANT:
 * - Replace APN for your SIM/network.
 * - Set DEVICE_TOKEN to the same random secret as Supabase Edge Function secret BUS_INGEST_SECRET.
 * - Do not commit your real device token to GitHub. Prefer provisioning it locally.
 * - Confirm modem UART voltage/pinout and power requirements for your exact board.
 */

#define TINY_GSM_MODEM_SIM7600
#define SerialMon Serial
#define SerialAT Serial2

#include <TinyGsmClient.h>
#include <TinyGPSPlus.h>

static const char APN[] = "YOUR_SIM_APN";
static const char GPRS_USER[] = "";
static const char GPRS_PASS[] = "";
static const char DEVICE_TOKEN[] = "REPLACE_WITH_DEVICE_INGEST_SECRET";
static const char BUS_ID[] = "BUS-01";

static const char SUPABASE_HOST[] = "gdlvhthtbcepzvblihtg.supabase.co";
static const int SUPABASE_PORT = 443;
static const char FUNCTION_PATH[] = "/functions/v1/bus-location-ingest";

static const int MODEM_RX_PIN = 16; // ESP32 RX <- modem TX
static const int MODEM_TX_PIN = 17; // ESP32 TX -> modem RX
static const int GPS_RX_PIN = 4;    // ESP32 RX <- GPS TX
static const int GPS_TX_PIN = 5;    // ESP32 TX -> GPS RX

static const unsigned long SEND_INTERVAL_MS = 10000;
static const unsigned long GPS_FIX_MAX_AGE_MS = 5000;

TinyGsm modem(SerialAT);
TinyGsmClientSecure tlsClient(modem);
TinyGPSPlus gps;
HardwareSerial GPSSerial(1);
unsigned long lastSend = 0;

bool connectCellular() {
  SerialMon.println("Checking modem...");
  if (!modem.testAT()) {
    SerialMon.println("Modem not responding. Check power, UART pins and baud rate.");
    return false;
  }
  if (!modem.isNetworkConnected()) {
    SerialMon.println("Waiting for cellular network...");
    if (!modem.waitForNetwork(60000L)) {
      SerialMon.println("Cellular network not available.");
      return false;
    }
  }
  if (!modem.isGprsConnected()) {
    SerialMon.println("Connecting mobile data...");
    if (!modem.gprsConnect(APN, GPRS_USER, GPRS_PASS)) {
      SerialMon.println("Mobile data connection failed. Check SIM/APN/data plan.");
      return false;
    }
  }
  return true;
}

bool postLocation(double lat, double lng, double speedKmh, double heading) {
  if (!connectCellular()) return false;

  String body = "{";
  body += "\"bus_id\":\"" + String(BUS_ID) + "\",";
  body += "\"latitude\":" + String(lat, 7) + ",";
  body += "\"longitude\":" + String(lng, 7) + ",";
  body += "\"speed_kmh\":" + String(speedKmh, 1) + ",";
  body += "\"heading_degrees\":" + String(heading, 1);
  body += "}";

  SerialMon.println("Connecting to Supabase function...");
  if (!tlsClient.connect(SUPABASE_HOST, SUPABASE_PORT)) {
    SerialMon.println("TLS connection failed.");
    return false;
  }

  tlsClient.print(String("POST ") + FUNCTION_PATH + " HTTP/1.1\r\n");
  tlsClient.print(String("Host: ") + SUPABASE_HOST + "\r\n");
  tlsClient.print("Content-Type: application/json\r\n");
  tlsClient.print(String("x-device-token: ") + DEVICE_TOKEN + "\r\n");
  tlsClient.print("Connection: close\r\n");
  tlsClient.print(String("Content-Length: ") + body.length() + "\r\n\r\n");
  tlsClient.print(body);

  unsigned long start = millis();
  while (tlsClient.connected() && !tlsClient.available() && millis() - start < 15000) {
    delay(50);
  }
  String statusLine = tlsClient.readStringUntil('\n');
  statusLine.trim();
  SerialMon.print("Server response: ");
  SerialMon.println(statusLine);
  while (tlsClient.connected() || tlsClient.available()) {
    while (tlsClient.available()) tlsClient.read();
    delay(10);
  }
  tlsClient.stop();
  return statusLine.indexOf("200") >= 0;
}

void setup() {
  SerialMon.begin(115200);
  delay(1000);
  SerialMon.println("\nROUTEIQ bus GPS/GSM tracker starting...");
  SerialAT.begin(115200, SERIAL_8N1, MODEM_RX_PIN, MODEM_TX_PIN);
  GPSSerial.begin(9600, SERIAL_8N1, GPS_RX_PIN, GPS_TX_PIN);
  delay(3000);
  modem.restart();
  SerialMon.println("Modem: " + modem.getModemInfo());
  connectCellular();
}

void loop() {
  while (GPSSerial.available()) gps.encode(GPSSerial.read());

  if (millis() - lastSend >= SEND_INTERVAL_MS) {
    lastSend = millis();
    if (!gps.location.isValid() || gps.location.age() > GPS_FIX_MAX_AGE_MS) {
      SerialMon.println("Waiting for a fresh GPS fix. Check antenna and open sky.");
      return;
    }

    const double lat = gps.location.lat();
    const double lng = gps.location.lng();
    const double speedKmh = gps.speed.isValid() ? gps.speed.kmph() : 0.0;
    const double heading = gps.course.isValid() ? gps.course.deg() : 0.0;

    SerialMon.printf("Bus %s: %.6f, %.6f speed %.1f km/h\n", BUS_ID, lat, lng, speedKmh);
    if (postLocation(lat, lng, speedKmh, heading)) {
      SerialMon.println("GPS update accepted.");
    } else {
      SerialMon.println("GPS update failed; will retry next interval.");
    }
  }
}
