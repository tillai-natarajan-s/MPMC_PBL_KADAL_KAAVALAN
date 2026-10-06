#include <SPI.h>
#include <LoRa.h>

// =====================================================
// KADAL KAAVALAN — SHORE UNIT
// LoRa Receiver
// =====================================================

// ---------------- LoRa Pins ----------------
#define LORA_SCK   18
#define LORA_MISO  19
#define LORA_MOSI  23
#define LORA_SS     5
#define LORA_RST    2
#define LORA_DIO0   4

// ---------------- LoRa Settings ----------------
#define LORA_FREQUENCY 433E6
#define LORA_SYNC_WORD 0xAB


void setup()
{
  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.println("========================================");
  Serial.println("        KADAL KAAVALAN");
  Serial.println("          SHORE UNIT");
  Serial.println("========================================");

  // Initialize SPI
  SPI.begin(
    LORA_SCK,
    LORA_MISO,
    LORA_MOSI,
    LORA_SS
  );

  // Configure LoRa pins
  LoRa.setPins(
    LORA_SS,
    LORA_RST,
    LORA_DIO0
  );

  Serial.println("Initializing SX1278...");

  // Initialize LoRa
  if (!LoRa.begin(LORA_FREQUENCY))
  {
    Serial.println("ERROR: SX1278 initialization FAILED!");

    while (1)
    {
      delay(1000);
    }
  }

  // Same sync word as Boat Unit
  LoRa.setSyncWord(LORA_SYNC_WORD);

  Serial.println("SX1278 detected!");
  Serial.println("----------------------------------------");
  Serial.println("Frequency : 433 MHz");
  Serial.println("Sync Word : 0xAB");
  Serial.println("Mode      : RECEIVER");
  Serial.println("----------------------------------------");
  Serial.println("Waiting for boat packets...");
  Serial.println();
}


void loop()
{
  // Check if a LoRa packet has arrived
  int packetSize = LoRa.parsePacket();

  if (packetSize)
  {
    String receivedMessage = "";

    // Read the packet
    while (LoRa.available())
    {
      receivedMessage += (char)LoRa.read();
    }

    // Get signal information
    int rssi = LoRa.packetRssi();
    float snr = LoRa.packetSnr();

    // =========================================
    // Display received packet
    // =========================================

    Serial.println("========================================");
    Serial.println("         PACKET RECEIVED");
    Serial.println("========================================");

    Serial.print("Message : ");
    Serial.println(receivedMessage);

    Serial.print("Size    : ");
    Serial.print(packetSize);
    Serial.println(" bytes");

    Serial.print("RSSI    : ");
    Serial.print(rssi);
    Serial.println(" dBm");

    Serial.print("SNR     : ");
    Serial.print(snr);
    Serial.println(" dB");

    Serial.println("========================================");
    Serial.println();
  }
}