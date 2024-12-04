#include "TelemetryGetter.h"

#define ASKWIFI 0

#include <WiFi.h>

#include <nvs_flash.h>
#include <nvs.h>

// Handle associated to namespace for non volatile storage
nvs_handle_t NVSHandle;

AsyncUDP udp;

TelemetryPackets latestTelemetryPackets = TelemetryPackets();

void setupTelemetry() {
  bool success = setupNVS();
  while (!success) delay(10);

  success = setupUDP();
  while (!success) delay(10);
}

bool setupNVS() {

  Serial.println("Setting up NVS");

  esp_err_t err = nvs_flash_init();
  if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    err = nvs_flash_init();
  }
  ESP_ERROR_CHECK(err);

  err = nvs_open("ESPRacingWheel", NVS_READWRITE, &NVSHandle);
  if (err != ESP_OK) {
    Serial.printf("Could not open NVS handle. (%s)", esp_err_to_name(err));
    return false;
  }

  Serial.println("NVS set up.");

  return true;
}

void gatherWiFiInfo() {
  uint8_t ssidlen = 0;
  char ssid[255]{};

  uint8_t passlen = 0;
  char pass[255]{};

getSSID:
  Serial.println("Enter ssid");

  while (!Serial.available()) delay(10);
  if (Serial.available() > 255) {
    Serial.println("SSID length too long.");
    goto getSSID;
  }
  ssidlen = Serial.available();
  Serial.read(ssid, ssidlen);
  ssid[ssidlen - 1] = '\0';

getPass:
  Serial.println("Enter pass");

  while (!Serial.available()) delay(10);
  if (Serial.available() > 255) {
    Serial.println("Pass length too long.");
    goto getPass;
  }
  passlen = Serial.available();
  Serial.read(pass, passlen);
  pass[passlen - 1] = '\0';

  nvs_set_u8(NVSHandle, "ssidlen", ssidlen);
  nvs_set_u8(NVSHandle, "passlen", passlen);

  nvs_set_str(NVSHandle, "ssid", ssid);
  nvs_set_str(NVSHandle, "pass", pass);

  nvs_commit(NVSHandle);
}

bool setupUDP() {

  uint8_t ssidlen = 0;
  uint8_t passlen = 0;

  nvs_get_u8(NVSHandle, "ssidlen", &ssidlen);
  nvs_get_u8(NVSHandle, "passlen", &passlen);

  if (!ssidlen || ASKWIFI) {
    Serial.println("WiFi info not setup or found");
    Serial.println("Please enter info.");
    gatherWiFiInfo();
  }

  char ssid[255]{};
  char pass[255]{};

  nvs_get_u8(NVSHandle, "ssidlen", &ssidlen);
  nvs_get_u8(NVSHandle, "passlen", &passlen);

  nvs_get_str(NVSHandle, "ssid", ssid, (size_t*)&ssidlen);
  nvs_get_str(NVSHandle, "pass", pass, (size_t*)&passlen);

  WiFi.disconnect(true);
  WiFi.mode(WIFI_STA);

  Serial.print("SSID used: ");
  Serial.println(ssid);

  Serial.print("Pass used: ");
  Serial.println(pass);

  Serial.print("Connecting to WiFi");

  for (int i = 0; i < 3; i++) {
    Serial.print('.');
    WiFi.begin(ssid, pass);
    for (int j = 0; j < 20; j++) {
      if (WiFi.status() == WL_CONNECTED) goto beginUDP;
      delay(50);
    }
    Serial.println(WiFi.status());
  }

  // Failed case

  Serial.println("\nFailed to connect to WiFi");

  return false;

beginUDP:

  Serial.println();

  if (udp.listen(20777)) {
    udp.onPacket(processIncomingUDPPacket);
  }

  Serial.println("UDP setup.");

  return true;
}

void processIncomingUDPPacket(AsyncUDPPacket packet) {
  PacketHeader* gamePacket = (PacketHeader*)packet.data();
  switch (gamePacket->m_packetId) {
    case 0:
      {
        PacketMotionData*& packetSlot = latestTelemetryPackets.motion;
        if (!packetSlot) { packetSlot = new PacketMotionData(); }
        *packetSlot = *(PacketMotionData*)gamePacket;
        break;
      }
    case 1:
      {
        PacketSessionData*& packetSlot = latestTelemetryPackets.session;
        if (!packetSlot) { packetSlot = new PacketSessionData(); }
        *packetSlot = *(PacketSessionData*)gamePacket;
        break;
      }
    case 2:
      {
        PacketLapData*& packetSlot = latestTelemetryPackets.lapData;
        if (!packetSlot) { packetSlot = new PacketLapData(); }
        *packetSlot = *(PacketLapData*)gamePacket;
        break;
      }
    case 3:
      {
        PacketEventData*& packetSlot = latestTelemetryPackets.event;
        if (!packetSlot) { packetSlot = new PacketEventData(); }
        *packetSlot = *(PacketEventData*)gamePacket;
        break;
      }
    case 4:
      {
        PacketParticipantsData*& packetSlot = latestTelemetryPackets.participants;
        if (!packetSlot) { packetSlot = new PacketParticipantsData(); }
        *packetSlot = *(PacketParticipantsData*)gamePacket;
        break;
      }
    case 5:
      {
        PacketCarSetupData*& packetSlot = latestTelemetryPackets.carSetups;
        if (!packetSlot) { packetSlot = new PacketCarSetupData(); }
        *packetSlot = *(PacketCarSetupData*)gamePacket;
        break;
      }
    case 6:
      {
        PacketCarTelemetryData*& packetSlot = latestTelemetryPackets.carTelemetry;
        if (!packetSlot) { packetSlot = new PacketCarTelemetryData(); }
        *packetSlot = *(PacketCarTelemetryData*)gamePacket;
        break;
      }
    case 7:
      {
        PacketCarStatusData*& packetSlot = latestTelemetryPackets.carStatus;
        if (!packetSlot) { packetSlot = new PacketCarStatusData(); }
        *packetSlot = *(PacketCarStatusData*)gamePacket;
        break;
      }
    case 8:
      {
        PacketFinalClassificationData*& packetSlot = latestTelemetryPackets.finalClassification;
        if (!packetSlot) { packetSlot = new PacketFinalClassificationData(); }
        *packetSlot = *(PacketFinalClassificationData*)gamePacket;
        break;
      }
    case 9:
      {
        PacketLobbyInfoData*& packetSlot = latestTelemetryPackets.lobbyInfo;
        if (!packetSlot) { packetSlot = new PacketLobbyInfoData(); }
        *packetSlot = *(PacketLobbyInfoData*)gamePacket;
        break;
      }
    case 10:
      {
        PacketCarDamageData*& packetSlot = latestTelemetryPackets.carDamage;
        if (!packetSlot) { packetSlot = new PacketCarDamageData(); }
        *packetSlot = *(PacketCarDamageData*)gamePacket;
        break;
      }
    case 11:
      {
        PacketSessionHistoryData*& packetSlot = latestTelemetryPackets.sessionHistory;
        if (!packetSlot) { packetSlot = new PacketSessionHistoryData(); }
        packetSlot[packetSlot->m_carIdx] = *(PacketSessionHistoryData*)gamePacket;
        break;
      }
    case 12:
      {
        PacketTyreSetsData*& packetSlot = latestTelemetryPackets.tyreSets;
        if (!packetSlot) { packetSlot = new PacketTyreSetsData(); }
        *packetSlot = *(PacketTyreSetsData*)gamePacket;
        break;
      }
    case 13:
      {
        PacketMotionExData*& packetSlot = latestTelemetryPackets.motionEx;
        if (!packetSlot) { packetSlot = new PacketMotionExData(); }
        *packetSlot = *(PacketMotionExData*)gamePacket;
        break;
      }
    case 14:
      {
        PacketTimeTrialData*& packetSlot = latestTelemetryPackets.timeTrial;
        if (!packetSlot) { packetSlot = new PacketTimeTrialData(); }
        *packetSlot = *(PacketTimeTrialData*)gamePacket;
        break;
      }
  }
}