#include "TelemetryGetter.h"

void setup() {
  delay(500);

  Serial.begin(115200);

  setupTelemetry();
}

void loop() {
  PacketCarTelemetryData* carTelemetryPacket = latestTelemetryPackets.carTelemetry;
  if(!carTelemetryPacket) return;
  Serial.println(carTelemetryPacket->m_carTelemetryData[carTelemetryPacket->m_header.m_playerCarIndex].m_speed);
}