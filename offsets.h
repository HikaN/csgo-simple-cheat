#pragma once

// updated 09.08.2016

// often update
const DWORD m_dwEntityList = 0x4A56D04;	// EntityList -> m_dwEntityList
const DWORD m_dwGlowObject = 0x4F6C8E4;	// Extra -> m_dwGlowObject
const DWORD m_dwLocalPlayer = 0xA3943C;	// LocalPlayer -> m_dwLocalPlayer
const DWORD m_dwClientState = 0x5BB2C4;	// ClientState -> m_dwClientState

//const DWORD m_dwForceJump = 0x4EEC178;		// Extra -> m_dwForceJump
//const DWORD m_dwInput = 0x04E9B7F0;			// Extra -> m_dwInput
//const DWORD m_dwRadarBase = 0x04E85A7C;		// RadarBase -> m_dwRadarBase
//const DWORD m_dwViewMatrix = 0x04A48CC4;	// EngineRender -> m_dwViewMatrix

// rarely update
const DWORD m_bDormant = 0xE9;				// BaseEntity -> m_bDormant
const DWORD m_iTeamNum = 0xF0;				// DT_BaseEntity -> m_iTeamNum
const DWORD m_vecOrigin = 0x134;			// DT_BaseEntity -> m_vecOrigin
const DWORD m_dwViewAngles = 0x4D0C;		// ClientState -> m_dwViewAngles
const DWORD m_vecPunch = 0x3018;			// DT_Local -> m_vecPunch
const DWORD m_dwBoneMatrix = 0x2698;		// BaseEntity -> m_dwBoneMatrix
const DWORD m_iHealth = 0xFC;				// DT_BasePlayer -> m_iHealth
const DWORD m_fFlags = 0x100;				// DT_BasePlayer -> m_fFlags
const DWORD m_lifeState = 0x25B;			// DT_BasePlayer -> m_lifeState
const DWORD m_iCrossHairID = 0xAA44;		// DT_Local -> m_iCrossHairID
const DWORD m_ArmorValue = 0xA9E8;			// DT_CSPlayer -> m_ArmorValue
const DWORD m_flFlashMaxAlpha = 0xA2F4;		// DT_CSPlayer -> m_flFlashMaxAlpha
const DWORD m_iShotsFired = 0xA2B0;			// DT_CSPlayer -> m_iShotsFired
const DWORD m_flNextPrimaryAttack = 0x31C8;	// DT_BaseCombatWeapon -> m_flNextPrimaryAttack
const DWORD m_nTickBase = 0x3414;			// DT_BasePlayer -> m_nTickBase
const DWORD m_dwInGame = 0x100;				// ClientState -> m_dwInGame
