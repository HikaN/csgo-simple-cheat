#pragma once

ProcMem csgo;
DWORD dwClient;
DWORD dwEngine;

//application class

class netrogaj
{

public:

	// update csgo.exe, client.dll and engine.dll
	bool updateProcess() {
		if (!csgo.Process("csgo.exe") || !csgo.Module("client.dll", dwClient) || !csgo.Module("engine.dll", dwEngine))
			return false;
		return true;
	}

	// update process and heck status player
	bool checkGameStarted()
	{
		//Process check
		if (!updateProcess()) {
			gGameState = GAME_NOT_LAUNCHED;
			//Sleep(200);
			return false;
		}

		//Check in the game whether the player
		if (getSignOnState() < SIGNONSTATE_FULL)
		{
			gGameState = GAME_LAUNCHED;
			//Sleep(100);
			return false;
		}
		else
		{
			gGameState = GAME_IN_PROGRESS;
		}

		return true;
	}
};

netrogaj base;
