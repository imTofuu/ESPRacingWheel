#pragma once

#include "F12024TelemetryTypes.h"

#include <AsyncUDP.h>

struct TelemetryPackets {
  PacketMotionData* motion = nullptr;
  PacketSessionData* session = nullptr;
  PacketLapData* lapData = nullptr;
  PacketEventData* event = nullptr;
  PacketParticipantsData* participants = nullptr;
  PacketCarSetupData* carSetups = nullptr;
  PacketCarTelemetryData* carTelemetry = nullptr;
  PacketCarStatusData* carStatus = nullptr;
  PacketFinalClassificationData* finalClassification = nullptr;
  PacketLobbyInfoData* lobbyInfo = nullptr;
  PacketCarDamageData* carDamage = nullptr;
  PacketSessionHistoryData* sessionHistory = new PacketSessionHistoryData[22]();
  PacketTyreSetsData* tyreSets = nullptr;
  PacketMotionExData* motionEx = nullptr;
  PacketTimeTrialData* timeTrial = nullptr;
};

extern TelemetryPackets latestTelemetryPackets;

void processIncomingUDPPacket(AsyncUDPPacket packet);

void setupTelemetry();

static bool setupNVS();
static bool setupUDP();