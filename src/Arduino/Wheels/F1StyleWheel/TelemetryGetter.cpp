#include "TelemetryGetter.h"

#define ASKWIFI 1

#include <WiFi.h>

#include <nvs_flash.h>
#include <nvs.h>

// Handle associated to namespace for non volatile storage
nvs_handle_t NVSHandle;

AsyncUDP udp;

TelemetryPackets latestTelemetryPackets = TelemetryPackets();

void addSessionStartedCallback(TelemetryEventCallback callback) {
  sessionStartedCallbacks.addCallback(callback);
}
void addSessionEndedCallback(TelemetryEventCallback callback) {
  sessionEndedCallbacks.addCallback(callback);
}
void addFastestLapCallback(TelemetryEventCallback callback) {
  fastestLapCallbacks.addCallback(callback);
}
void addRetirementCallback(TelemetryEventCallback callback) {
  retirementCallbacks.addCallback(callback);
}
void addDRSEnabledCallback(TelemetryEventCallback callback) {
  drsEnabledCallbacks.addCallback(callback);
}
void addDRSDisabledCallback(TelemetryEventCallback callback) {
  drsDisabledCallbacks.addCallback(callback);
}
void addTeamMateInPitsCallback(TelemetryEventCallback callback) {
  teamMateInPitsCallbacks.addCallback(callback);
}
void addChequeredFlagCallback(TelemetryEventCallback callback) {
  chequeredFlagCallbacks.addCallback(callback);
}
void addRaceWinnerCallback(TelemetryEventCallback callback) {
  raceWinnerCallbacks.addCallback(callback);
}
void addPenaltyIssuedCallback(TelemetryEventCallback callback) {
  penaltyIssuedCallbacks.addCallback(callback);
}
void addSpeedTrapTriggeredCallback(TelemetryEventCallback callback) {
  speedTrapTriggeredCallbacks.addCallback(callback);
}
void addStartLightsCallback(TelemetryEventCallback callback) {
  startLightsCallbacks.addCallback(callback);
}
void addLightsOutCallback(TelemetryEventCallback callback) {
  lightsOutCallbacks.addCallback(callback);
}
void addDriveThroughServedCallback(TelemetryEventCallback callback) {
  driveThroughServedCallbacks.addCallback(callback);
}
void addStopGoServedCallback(TelemetryEventCallback callback) {
  stopGoServedCallbacks.addCallback(callback);
}
void addFlashbackCallback(TelemetryEventCallback callback) {
  flashbackCallbacks.addCallback(callback);
}
void addButtonStatusChangedCallback(TelemetryEventCallback callback) {
  buttonStatusChangedCallbacks.addCallback(callback);
}
void addRedFlagCallback(TelemetryEventCallback callback) {
  redFlagCallbacks.addCallback(callback);
}
void addOvertakeCallback(TelemetryEventCallback callback) {
  overtakeCallbacks.addCallback(callback);
}
void addSafetyCarCallback(TelemetryEventCallback callback) {
  safetyCarCallbacks.addCallback(callback);
}
void addCollisionCallback(TelemetryEventCallback callback) {
  collisionCallbacks.addCallback(callback);
}

void processIncomingUDPPacket(AsyncUDPPacket packet) {
  PacketHeader* gamePacket = (PacketHeader*)packet.data();
  switch (gamePacket->m_packetId) {
    case 0:
      {
        PacketMotionData*& packetSlot = latestTelemetryPackets.motion;
        if (!packetSlot) { packetSlot = new PacketMotionData(); }
        *packetSlot = *(PacketMotionData*)gamePacket;
        return;
      }
    case 1:
      {
        PacketSessionData*& packetSlot = latestTelemetryPackets.session;
        if (!packetSlot) { packetSlot = new PacketSessionData(); }
        *packetSlot = *(PacketSessionData*)gamePacket;
        return;
      }
    case 2:
      {
        PacketLapData*& packetSlot = latestTelemetryPackets.lapData;
        if (!packetSlot) { packetSlot = new PacketLapData(); }
        *packetSlot = *(PacketLapData*)gamePacket;
        return;
      }
    case 3:
      {
        PacketEventData* eventPacket = (PacketEventData*)gamePacket;
        switch(*(uint32_t*)eventPacket->m_eventStringCode) {
          case 1096045395: // SSTA
            sessionStartedCallbacks.callAll(eventPacket);
            return;
          case 1145980243: // SEND
            sessionEndedCallbacks.callAll(eventPacket);
            return;
          case 1347179590: // FTLP
            fastestLapCallbacks.callAll(eventPacket);
            return;
          case 1414354002: // RTMT
            retirementCallbacks.callAll(eventPacket);
            return;
          case 1163088452: // DRSE
            drsEnabledCallbacks.callAll(eventPacket);
            return;
          case 1146311236: // DRSD
            drsDisabledCallbacks.callAll(eventPacket);
            return;
          case 1414548820: // TMPT
            teamMateInPitsCallbacks.callAll(eventPacket);
            return;
          case 1179732035: // CHQF
            chequeredFlagCallbacks.callAll(eventPacket);
            return;
          case 1314341714: // RCWN
            raceWinnerCallbacks.callAll(eventPacket);
            return;
          case 1095648592: // PENA
            penaltyIssuedCallbacks.callAll(eventPacket);
            return;
          case 1347702867: // SPTP
            speedTrapTriggeredCallbacks.callAll(eventPacket);
            return;
          case 1196184659: // STLG
            startLightsCallbacks.callAll(eventPacket);
            return;
          case 1414481740: // LGOT
            lightsOutCallbacks.callAll(eventPacket);
            return;
          case 1448301636: // DTSV
            driveThroughServedCallbacks.callAll(eventPacket);
            return;
          case 1448298323: // SGSV
            stopGoServedCallbacks.callAll(eventPacket);
            return;
          case 1262636102: // FLBK
            flashbackCallbacks.callAll(eventPacket);
            return;
          case 1314149698: // BUTN
            buttonStatusChangedCallbacks.callAll(eventPacket);
            return;
          case 1279673426: // RDFL
            redFlagCallbacks.callAll(eventPacket);
            return;
          case 1263818319: // OVTK
            overtakeCallbacks.callAll(eventPacket);
            return;
          case 1380008787: // SCAR
            safetyCarCallbacks.callAll(eventPacket);
            return;
          case 1280069443: // COLL
            collisionCallbacks.callAll(eventPacket);
            return;
        }
        Serial.println("Unknown event code");
        return;
      }
    case 4:
      {
        PacketParticipantsData*& packetSlot = latestTelemetryPackets.participants;
        if (!packetSlot) { packetSlot = new PacketParticipantsData(); }
        *packetSlot = *(PacketParticipantsData*)gamePacket;
        return;
      }
    case 5:
      {
        PacketCarSetupData*& packetSlot = latestTelemetryPackets.carSetups;
        if (!packetSlot) { packetSlot = new PacketCarSetupData(); }
        *packetSlot = *(PacketCarSetupData*)gamePacket;
        return;
      }
    case 6:
      {
        PacketCarTelemetryData*& packetSlot = latestTelemetryPackets.carTelemetry;
        if (!packetSlot) { packetSlot = new PacketCarTelemetryData(); }
        *packetSlot = *(PacketCarTelemetryData*)gamePacket;
        return;
      }
    case 7:
      {
        PacketCarStatusData*& packetSlot = latestTelemetryPackets.carStatus;
        if (!packetSlot) { packetSlot = new PacketCarStatusData(); }
        *packetSlot = *(PacketCarStatusData*)gamePacket;
        return;
      }
    case 8:
      {
        PacketFinalClassificationData*& packetSlot = latestTelemetryPackets.finalClassification;
        if (!packetSlot) { packetSlot = new PacketFinalClassificationData(); }
        *packetSlot = *(PacketFinalClassificationData*)gamePacket;
        return;
      }
    case 9:
      {
        PacketLobbyInfoData*& packetSlot = latestTelemetryPackets.lobbyInfo;
        if (!packetSlot) { packetSlot = new PacketLobbyInfoData(); }
        *packetSlot = *(PacketLobbyInfoData*)gamePacket;
        return;
      }
    case 10:
      {
        PacketCarDamageData*& packetSlot = latestTelemetryPackets.carDamage;
        if (!packetSlot) { packetSlot = new PacketCarDamageData(); }
        *packetSlot = *(PacketCarDamageData*)gamePacket;
        return;
      }
    case 11:
      {
        PacketSessionHistoryData* sessionHistoryPacket = (PacketSessionHistoryData*)gamePacket;
        PacketSessionHistoryData*& packetSlot = latestTelemetryPackets.sessionHistory[sessionHistoryPacket->m_carIdx];
        if (!packetSlot) { packetSlot = new PacketSessionHistoryData(); }
        *packetSlot = *sessionHistoryPacket;
        return;
      }
    case 12:
      {
        PacketTyreSetsData*& packetSlot = latestTelemetryPackets.tyreSets;
        if (!packetSlot) { packetSlot = new PacketTyreSetsData(); }
        *packetSlot = *(PacketTyreSetsData*)gamePacket;
        return;
      }
    case 13:
      {
        PacketMotionExData*& packetSlot = latestTelemetryPackets.motionEx;
        if (!packetSlot) { packetSlot = new PacketMotionExData(); }
        *packetSlot = *(PacketMotionExData*)gamePacket;
        return;
      }
    case 14:
      {
        PacketTimeTrialData*& packetSlot = latestTelemetryPackets.timeTrial;
        if (!packetSlot) { packetSlot = new PacketTimeTrialData(); }
        *packetSlot = *(PacketTimeTrialData*)gamePacket;
        return;
      }
  }
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

void setupTelemetry() {
  bool success = setupNVS();
  while (!success) delay(10);

  success = setupUDP();
  while (!success) delay(10);

  Serial.print("Local ip: ");
  Serial.println(WiFi.localIP());
}