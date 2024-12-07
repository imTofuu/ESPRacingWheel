#include "TelemetryGetter.h"

#include <Arduino.h>

void setup() {
  delay(500);

  Serial.begin(115200);

  setupTelemetry();

  addPenaltyIssuedCallback(penaltyIssued);
}

void loop() {
}

void penaltyIssued(const PacketEventData* eventPacket) {
  if(eventPacket->m_eventDetails.Penalty.vehicleIdx != latestTelemetryPackets.motion->m_header.m_playerCarIndex) return;
    Serial.println("Player has been issued penalty");
}