#include <SPI.h>
#include <LoRa.h>
#include <math.h>

// =====================================================
// LORA PINS
// =====================================================

#define LORA_SCK   18
#define LORA_MISO  19
#define LORA_MOSI  23
#define LORA_SS     5
#define LORA_RST    2
#define LORA_DIO0   4

#define LORA_FREQUENCY 433E6
#define LORA_SYNC_WORD 0xAB


// =====================================================
// BOAT OUTPUTS
// =====================================================

#define RED_LED       25
#define YELLOW_LED    26
#define GREEN_LED     27
#define BUZZER        33


// =====================================================
// SOS BUTTON
// =====================================================

#define SOS_BUTTON    32


// =====================================================
// FAKE GPS COORDINATES
// =====================================================

// Boundary/reference point
// These are ONLY test coordinates.

const double BOUNDARY_LAT = 13.050000;
const double BOUNDARY_LON = 80.250000;


// Fake boat position
double boatLat = 13.100000;
double boatLon = 80.250000;


// =====================================================
// SIMULATION SETTINGS
// =====================================================

// Change this to true if you want the boat position
// to automatically move toward the boundary.

bool AUTO_SIMULATION = true;


// =====================================================
// HAVERSINE DISTANCE
// Returns distance in kilometres
// =====================================================

double calculateDistanceKm(
  double lat1,
  double lon1,
  double lat2,
  double lon2
) {

  const double EARTH_RADIUS_KM = 6371.0;

  double dLat = radians(lat2 - lat1);
  double dLon = radians(lon2 - lon1);

  double a =
    sin(dLat / 2) * sin(dLat / 2) +
    cos(radians(lat1)) *
    cos(radians(lat2)) *
    sin(dLon / 2) *
    sin(dLon / 2);

  double c = 2 * atan2(sqrt(a), sqrt(1 - a));

  return EARTH_RADIUS_KM * c;
}


// =====================================================
// SETUP
// =====================================================

void setup() {

  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.println("======================================");
  Serial.println("          BOAT UNIT");
  Serial.println("       KADAL KAAVALAN");
  Serial.println("       FAKE GPS MODE");
  Serial.println("======================================");

  // ---------------------------------------------------
  // LEDs
  // ---------------------------------------------------

  pinMode(RED_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);

  // ---------------------------------------------------
  // Buzzer
  // ---------------------------------------------------

  pinMode(BUZZER, OUTPUT);

  // ---------------------------------------------------
  // SOS button
  // Button = GPIO32 to GND
  // ---------------------------------------------------

  pinMode(SOS_BUTTON, INPUT_PULLUP);


  // ---------------------------------------------------
  // Turn everything OFF
  // ---------------------------------------------------

  digitalWrite(RED_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(BUZZER, LOW);


  // ===================================================
  // LORA
  // ===================================================

  Serial.println("Initializing LoRa...");

  SPI.begin(
    LORA_SCK,
    LORA_MISO,
    LORA_MOSI,
    LORA_SS
  );

  LoRa.setPins(
    LORA_SS,
    LORA_RST,
    LORA_DIO0
  );

  if (!LoRa.begin(LORA_FREQUENCY)) {

    Serial.println("ERROR: LoRa initialization failed!");

    while (true) {

      digitalWrite(RED_LED, HIGH);
      delay(200);

      digitalWrite(RED_LED, LOW);
      delay(200);
    }
  }

  LoRa.setSyncWord(LORA_SYNC_WORD);

  Serial.println("LoRa initialized!");
  Serial.println("Frequency : 433 MHz");
  Serial.println("Sync Word : 0xAB");

  Serial.println();
  Serial.println("FAKE GPS MODE ACTIVE");

  Serial.print("Boundary Latitude  : ");
  Serial.println(BOUNDARY_LAT, 6);

  Serial.print("Boundary Longitude : ");
  Serial.println(BOUNDARY_LON, 6);

  Serial.println();
}


// =====================================================
// MAIN LOOP
// =====================================================

void loop() {

  // ===================================================
  // SOS CHECK
  // ===================================================

  bool sosActive = (digitalRead(SOS_BUTTON) == LOW);

  if (sosActive) {

    setSOSMode();

    String message =
      "SOS=1,STATUS=SOS,LAT=" +
      String(boatLat, 6) +
      ",LON=" +
      String(boatLon, 6);

    sendLoRaMessage(message);

    Serial.println();
    Serial.println("!!!!!!!! SOS ACTIVE !!!!!!!!");

    delay(1000);

    return;
  }


  // ===================================================
  // CALCULATE DISTANCE
  // ===================================================

  double distanceKm = calculateDistanceKm(
    boatLat,
    boatLon,
    BOUNDARY_LAT,
    BOUNDARY_LON
  );


  // ===================================================
  // DETERMINE STATUS
  // ===================================================

  String status;


  if (distanceKm > 5.0) {

    status = "SAFE";

    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(RED_LED, LOW);
    digitalWrite(BUZZER, LOW);
  }


  else if (distanceKm > 2.0) {

    status = "WARNING";

    digitalWrite(GREEN_LED, LOW);
    digitalWrite(YELLOW_LED, HIGH);
    digitalWrite(RED_LED, LOW);
    digitalWrite(BUZZER, LOW);
  }


  else {

    status = "ALERT";

    digitalWrite(GREEN_LED, LOW);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(RED_LED, HIGH);
    digitalWrite(BUZZER, HIGH);
  }


  // ===================================================
  // SERIAL MONITOR
  // ===================================================

  Serial.println();
  Serial.println("--------------------------------------");

  Serial.print("Boat Latitude  : ");
  Serial.println(boatLat, 6);

  Serial.print("Boat Longitude : ");
  Serial.println(boatLon, 6);

  Serial.print("Boundary Lat   : ");
  Serial.println(BOUNDARY_LAT, 6);

  Serial.print("Boundary Lon   : ");
  Serial.println(BOUNDARY_LON, 6);

  Serial.print("Distance       : ");
  Serial.print(distanceKm, 3);
  Serial.println(" km");

  Serial.print("STATUS         : ");
  Serial.println(status);


  // ===================================================
  // SEND DATA TO SHORE
  // ===================================================

  String message =
    "SOS=0,STATUS=" +
    status +
    ",LAT=" +
    String(boatLat, 6) +
    ",LON=" +
    String(boatLon, 6) +
    ",DIST=" +
    String(distanceKm, 3);

  sendLoRaMessage(message);


  // ===================================================
  // AUTOMATIC FAKE GPS MOVEMENT
  // ===================================================

  if (AUTO_SIMULATION) {

    /*
       Move boat toward the boundary.

       0.005 degrees latitude is approximately
       0.55 km.

       This lets us simulate the boat approaching
       the boundary.
    */

    boatLat -= 0.005;

    // Once we cross the boundary,
    // restart the simulation.

    if (boatLat < 13.000000) {

      boatLat = 13.100000;

      Serial.println();
      Serial.println("=== SIMULATION RESET ===");
    }
  }


  delay(2000);
}


// =====================================================
// SOS MODE
// =====================================================

void setSOSMode() {

  digitalWrite(GREEN_LED, LOW);

  digitalWrite(RED_LED, HIGH);
  digitalWrite(YELLOW_LED, HIGH);

  digitalWrite(BUZZER, HIGH);
}


// =====================================================
// SEND LORA PACKET
// =====================================================

void sendLoRaMessage(String message) {

  Serial.print("LoRa TX: ");
  Serial.println(message);

  LoRa.beginPacket();

  LoRa.print(message);

  LoRa.endPacket();
}