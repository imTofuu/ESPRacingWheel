#pragma once

#include "F12024TelemetryTypes.h"

#include <AsyncUDP.h>
#include "CallbackList.h"

struct TelemetryPackets {
  PacketMotionData* motion = nullptr;
  PacketSessionData* session = nullptr;
  PacketLapData* lapData = nullptr;
  PacketParticipantsData* participants = nullptr;
  PacketCarSetupData* carSetups = nullptr;
  PacketCarTelemetryData* carTelemetry = nullptr;
  PacketCarStatusData* carStatus = nullptr;
  PacketFinalClassificationData* finalClassification = nullptr;
  PacketLobbyInfoData* lobbyInfo = nullptr;
  PacketCarDamageData* carDamage = nullptr;
  PacketSessionHistoryData** sessionHistory = new PacketSessionHistoryData*[22](); // 22 items because of the way that this
                                                                                   // packet is sent (20 times per second,
                                                                                   // cycling through cars). In same order
                                                                                   // as car indicies.
  PacketTyreSetsData* tyreSets = nullptr;
  PacketMotionExData* motionEx = nullptr;
  PacketTimeTrialData* timeTrial = nullptr;
};

extern TelemetryPackets latestTelemetryPackets;

using TelemetryEventCallback = void(*)(const PacketEventData* eventPacket);

static CallbackList<void, const PacketEventData*> sessionStartedCallbacks,
                                                  sessionEndedCallbacks,
                                                  fastestLapCallbacks,
                                                  retirementCallbacks,
                                                  drsEnabledCallbacks,
                                                  drsDisabledCallbacks,
                                                  teamMateInPitsCallbacks,
                                                  chequeredFlagCallbacks,
                                                  raceWinnerCallbacks,
                                                  penaltyIssuedCallbacks,
                                                  speedTrapTriggeredCallbacks,
                                                  startLightsCallbacks,
                                                  lightsOutCallbacks,
                                                  driveThroughServedCallbacks,
                                                  stopGoServedCallbacks,
                                                  flashbackCallbacks,
                                                  buttonStatusChangedCallbacks,
                                                  redFlagCallbacks,
                                                  overtakeCallbacks,
                                                  safetyCarCallbacks,
                                                  collisionCallbacks;


void addSessionStartedCallback(TelemetryEventCallback callback);
void addSessionEndedCallback(TelemetryEventCallback callback);
void addFastestLapCallback(TelemetryEventCallback callback);
void addRetirementCallback(TelemetryEventCallback callback);
void addDRSEnabledCallback(TelemetryEventCallback callback);
void addDRSDisabledCallback(TelemetryEventCallback callback);
void addTeamMateInPitsCallback(TelemetryEventCallback callback);
void addChequeredFlagCallback(TelemetryEventCallback callback);
void addRaceWinnerCallback(TelemetryEventCallback callback);
void addPenaltyIssuedCallback(TelemetryEventCallback callback);
void addSpeedTrapTriggeredCallback(TelemetryEventCallback callback);
void addStartLightsCallback(TelemetryEventCallback callback);
void addLightsOutCallback(TelemetryEventCallback callback);
void addDriveThroughServedCallback(TelemetryEventCallback callback);
void addStopGoServedCallback(TelemetryEventCallback callback);
void addFlashbackCallback(TelemetryEventCallback callback);
void addButtonStatusChangedCallback(TelemetryEventCallback callback);
void addRedFlagCallback(TelemetryEventCallback callback);
void addOvertakeCallback(TelemetryEventCallback callback);
void addSafetyCarCallback(TelemetryEventCallback callback);
void addCollisionCallback(TelemetryEventCallback callback);

static void processIncomingUDPPacket(AsyncUDPPacket packet);

void setupTelemetry();