#include <Windows.h>

#include "Offsets.h"
#include "Vector3.h"
#include "loop.h"

#include <math.h>
#include <corecrt_math_defines.h>

class Engine {
public:
    int GetMaxPlayers() {
        //uintptr_t a = GetClientState() + Offset::dwClientState_MaxPlayer;
        //return *(int*)a;
        return *(int*)(GetClientState() + Offset::dwClientState_MaxPlayer);
    }

    Vector3* GetViewAnglesAddress() {
        return (Vector3*)(GetClientState() + Offset::dwClientState_ViewAngles);
    }

    uintptr_t GetClientState() {
        return *(uintptr_t*)(this + Offset::dwClientState);
    }
};

class Ent {
public:
    Vector3 GetPosition() {
        return *(Vector3*)(this + Offset::m_vecOrigin);
    }

    Vector3 GetHeadPosition() {
        return { *(float*)(*(uintptr_t*)(this + Offset::m_dwBoneMatrix) + 0x30 * 8 + 0x0C),
            *(float*)(*(uintptr_t*)(this + Offset::m_dwBoneMatrix) + 0x30 * 8 + 0x1C),
            *(float*)(*(uintptr_t*)(this + Offset::m_dwBoneMatrix) + 0x30 * 8 + 0x2C) };
    }

    int GetTeam() {
        return *(int*)(this + Offset::m_iTeamNum);
    }

    Vector3 GetViewOffset() {
        return *(Vector3*)(this + Offset::m_vecViewOffset);
    }

    int GetHealth() {
        return *(int*)(this + Offset::m_iHealth);
    }

    int GetCrosshairID() {
        return *(int*)(this + Offset::m_iCrosshairId);
    }

    int GetShotsFired() {
        return *(int*)(this + Offset::m_iShotsFired);
    }
};

class Client {
public:
    Ent* GetPlayerAddress() {
        return (Ent*)*(uintptr_t*)(this + Offset::dwLocalPlayer);
    }

    Ent* GetEntityAddress(int i) {
        return (Ent*)*(uintptr_t*)(GetEntityList() + i * 0x10);
    }

    uintptr_t GetEntityList() {
        return (uintptr_t)(this + Offset::dwEntityList);
    }

    int* GetForceAttackAddress() {
        return (int*)(this + Offset::dwForceAttack);
    }
};

void Run() {

    Client* client = (Client*)GetModuleHandle(L"client.dll");
    Engine* engine = (Engine*)GetModuleHandle(L"engine.dll");
    Ent* player;

    bool bAimbot = false, bTriggerbot = false;

    if (client && engine) {
        while (!GetAsyncKeyState(VK_END)) {
            
            if (GetAsyncKeyState(VK_NUMPAD1) & 1) {
                bAimbot = !bAimbot;
                bTriggerbot = !bTriggerbot;
                std::cout << "aimbot: " << bAimbot << std::endl;b
            }

            /*if (GetAsyncKeyState(VK_NUMPAD2) & 1) {
                bTriggerbot = !bTriggerbot;
                std::cout << "triggerbot: " << bTriggerbot << std::endl;
            }*/

            player = client->GetPlayerAddress();

            if (bAimbot && player) {
                float maxRange = 1000000;
                float closestDistance = maxRange;
                int indexOfEnt = 0;
                for (int i = 1; i < engine->GetMaxPlayers(); i++) { // loop through each entity except for player
                    Ent* entity = client->GetEntityAddress(i);

                    if (entity && entity->GetTeam() != player->GetTeam() && entity->GetTeam() && !(entity->GetHealth() < 1)) {
                        Vector3 deltaVector = player->GetPosition() - entity->GetPosition(); // check the distance between player and entity
                        float distance = sqrt(deltaVector.x * deltaVector.x + deltaVector.y * deltaVector.y + deltaVector.z * deltaVector.z);

                        if (distance < closestDistance) {
                            closestDistance = distance;
                            indexOfEnt = i;
                        }
                    }
                }

                if (indexOfEnt) { // if an enemy was found within maxRange
                    Ent* entity = client->GetEntityAddress(indexOfEnt);

                    Vector3 deltaVector = (entity->GetHeadPosition()) - (player->GetPosition() + player->GetViewOffset()); // get the distance vector between heads

                    float pitch = -atan2(deltaVector.z, sqrt(deltaVector.x * deltaVector.x + deltaVector.y * deltaVector.y)) * 180 / M_PI;
                    float yaw = atan2(deltaVector.y, deltaVector.x) * 180 / M_PI;

                    while (yaw < -180) yaw += 360;
                    while (yaw > 180) yaw -= 360;
                    if (pitch > 89) pitch = 89;
                    if (pitch < -89) pitch = -89;

                    (*(engine->GetViewAnglesAddress())).x = pitch;
                    (*(engine->GetViewAnglesAddress())).y = yaw;
                }
            }

            if (bTriggerbot && player && player->GetCrosshairID() && !player->GetShotsFired()) {
                *(client->GetForceAttackAddress()) = 5;
                Sleep(1);
                *(client->GetForceAttackAddress()) = 4;
            }
            Sleep(1);
        }
    }
}