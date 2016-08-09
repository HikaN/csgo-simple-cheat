#pragma once

extern ProcMem csgo;
extern DWORD dwClient;
extern DWORD dwEngine;

// Structure Vector3
struct Vector3
{
	float x, y, z;
	const Vector3 operator-(const Vector3& next)
	{
		x -= next.x;
		y -= next.y; 
		z -= next.z;
		Clamp();
	}
	const Vector3 operator+(const Vector3& next)
	{
		x += next.x;
		y += next.y;
		z += next.z;
		Clamp();
	}
	void Clamp()
	{
		while (y > 180) y -= 360;
		while (y < -180) y += 360;
		while (x > 89) y -= 178;
		while (x < -89) y += 178;
	}
};

// Game status
enum gamestate
{
	GAMESTATE_UNKNOWN = 0,
	GAME_NOT_LAUNCHED,
	GAME_LAUNCHED,
	GAME_IN_PROGRESS
};
gamestate gGameState = GAME_NOT_LAUNCHED; // State game (is not running, running, in game)

// The entire list is available for the activation of the functional
enum active_enum
{
	ESP_TYPE_2,			// Health ESP
	AE_TOTAL_ITEMS		// Used for counting the number of elements below, nothing to add
};
bool bActive[AE_TOTAL_ITEMS]; // Active functional (см. active_enum)

// Values m_lifeState
enum LIFESTATE
{
	LIFE_ALIVE = 0, // alive
	LIFE_DYING, // dead. 
	LIFE_DEAD, // dead. 
	LIFE_RESPAWNABLE,
	LIFE_DISCARDBODY
};

// structure local player
struct myPlayer_t {
	DWORD pLocal; int health; int team;
	int dormant; int armor;
	int CrossHairID; Vector3 pos;
	bool isAlive; int flags;
	Vector3 punch; int ShotsFired;
	void ReadInfo() {
		pLocal = csgo.Read<DWORD>(dwClient + m_dwLocalPlayer);
		health = csgo.Read<int>(pLocal + m_iHealth);
		team = csgo.Read<int>(pLocal + m_iTeamNum);
		armor = csgo.Read<int>(pLocal + m_ArmorValue);
		CrossHairID = csgo.Read<int>(pLocal + m_iCrossHairID);
		dormant = csgo.Read<int>(pLocal + m_bDormant);
		isAlive = csgo.Read<int>(pLocal + m_lifeState) == LIFE_ALIVE;
		flags = csgo.Read<int>(pLocal + m_fFlags);
		pos = csgo.Read<Vector3>(pLocal + m_vecOrigin);
		punch = csgo.Read<Vector3>(pLocal + m_vecPunch);
		ShotsFired = csgo.Read<int>(pLocal + m_iShotsFired);
	}
} myPlayer;

// structure other players
struct otherPlayer_t {
	DWORD CEntityList; int health;
	int team; int dormant;
	int armor; Vector3 pos;
	bool isAlive;
	void ReadInfo(int _pInfo) {
		CEntityList = csgo.Read<DWORD>(dwClient + m_dwEntityList + (_pInfo * 0x10));
		health = csgo.Read<int>(CEntityList + m_iHealth);
		team = csgo.Read<int>(CEntityList + m_iTeamNum);
		armor = csgo.Read<int>(CEntityList + m_ArmorValue);
		dormant = csgo.Read<int>(CEntityList + m_bDormant);
		isAlive = csgo.Read<int>(CEntityList + m_lifeState) == LIFE_ALIVE;
		pos = csgo.Read<Vector3>(CEntityList + m_vecOrigin);
	}
} otherPlayer[32];

struct Player_t {
	DWORD CEntityList; int health;
	int team; int dormant;
	int armor; Vector3 pos;
	bool isAlive; int id;
	void ReadInfo() {
		CEntityList = csgo.Read<DWORD>(dwClient + m_dwEntityList + (id * 0x10));
		health = csgo.Read<int>(CEntityList + m_iHealth);
		team = csgo.Read<int>(CEntityList + m_iTeamNum);
		armor = csgo.Read<int>(CEntityList + m_ArmorValue);
		dormant = csgo.Read<int>(CEntityList + m_bDormant);
		isAlive = csgo.Read<int>(CEntityList + m_lifeState) == LIFE_ALIVE;
		pos = csgo.Read<Vector3>(CEntityList + m_vecOrigin);
	}
};

// Structure Glow
struct glow_t
{
	DWORD dwBase;
	float r;
	float g;
	float b;
	float a;
	unsigned char unk1[16];
	bool m_bRenderWhenOccluded;
	bool m_bRenderWhenUnoccluded;
	bool m_bFullBloom;
	unsigned char unk2[14];
};

// Get player CEntityList'
otherPlayer_t* getEntityByCEntityList(DWORD CEntityList) {
	for (int i = 0; i < 64; i++) {
		otherPlayer[i].ReadInfo(i);
		if (CEntityList == otherPlayer[i].CEntityList)
			return &otherPlayer[i];
	}
	return nullptr;
}

//  function glow player
void glow_player(DWORD mObj, float r = 0, float g = 0, float b = 0, float a = 1.0f, bool m_bRenderWhenOccluded = true, bool m_bRenderWhenUnoccluded = false)
{
	csgo.Write<float>(mObj + 0x4, r);
	csgo.Write<float>(mObj + 0x8, g);
	csgo.Write<float>(mObj + 0xC, b);
	csgo.Write<float>(mObj + 0x10, a);
	csgo.Write<BOOL>(mObj + 0x24, m_bRenderWhenOccluded);
	csgo.Write<BOOL>(mObj + 0x25, m_bRenderWhenUnoccluded);
}

// Write the vector angle
void SetViewAngles(Vector3& vecViewAngles)
{
	vecViewAngles.Clamp();
	DWORD dwClientState = csgo.Read<int>(dwEngine + m_dwClientState);
	csgo.Write<Vector3>(dwClientState + m_dwViewAngles, vecViewAngles);
}

// Read vector angle
Vector3 GetViewAngles()
{
	DWORD dwClientState = csgo.Read<int>(dwEngine + m_dwClientState);
	return csgo.Read<Vector3>(dwClientState + m_dwViewAngles);
}

// Game status (player)
enum SIGNONSTATE
{
	SIGNONSTATE_NONE = 0, // in menu
	SIGNONSTATE_CHALLENGE,
	SIGNONSTATE_CONNECTED,
	SIGNONSTATE_NEW,
	SIGNONSTATE_PRESPAWN,
	SIGNONSTATE_SPAWN,
	SIGNONSTATE_FULL, // in game
	SIGNONSTATE_CHANGELEVEL
};

// get state of the game (player)
int getSignOnState()
{
	DWORD dwClientState = csgo.Read<int>(dwEngine + m_dwClientState);
	return csgo.Read<int>(dwClientState + m_dwInGame);
}

// Bones
enum Bones : int
{
	Head = 6,
	Neck = 5,
	Spine1 = 0,
	Spine2 = 1,
	Spine3 = 2,
	Spine4 = 3,
	Spine5 = 4,
	HandLeft = 21,
	ElbowLeft = 9,
	ShoulderLeft = 8,
	ShoulderRight = 36,
	ElbowRight = 37,
	HandRight = 15,
	FootLeft = 65,
	KneeLeft = 64,
	HipLeft = 63,
	HipRight = 69,
	KneeRight = 70,
	FootRight = 71
};
