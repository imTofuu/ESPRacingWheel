#include "Screen.h"
#include "TelemetryGetter.h"

// Screen connections are using IOMux SPI pins

void setup() {
  Serial.begin(115200);

  setupTelemetry();

  addPenaltyIssuedCallback(penaltyIssued);

  bool success = initScreen();
  while(!success) delay(10);

  setCurrentPage(&Pages::testPage);
}

void loop() {
  updateScreen();

  PacketCarTelemetryData* carTelemetryPacket = latestTelemetryPackets.carTelemetry;
  if(!carTelemetryPacket) return;
  speed = carTelemetryPacket->m_carTelemetryData[carTelemetryPacket->m_header.m_playerCarIndex].m_speed;
}

void penaltyIssued(const PacketEventData* eventPacket) {
  if(eventPacket->m_eventDetails.Penalty.vehicleIdx != latestTelemetryPackets.motion->m_header.m_playerCarIndex) return;
    Serial.println("Player has been issued penalty");
}