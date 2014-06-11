/***************************************************************
|	File:		RZBN.h
|	Author:		Justin Mazzola
|	Course:		SGP
|	Purpose:	This class will handle saving and loading our
|				own custom binary file format : .rzbn
***************************************************************/
#pragma once

#include <string>
using namespace std;

class Player;

class RZBN
{

public:
	// Ctor and dtor
	RZBN() = default;
	virtual ~RZBN() = default;

	// Accessors
	Player* GetPlayer() const { return m_pPlayer; }

	// Mutators
	void SetPlayer(Player* player) { m_pPlayer = player; }

	// Load Savefile
	bool LoadRZBNFile(string rzbnFilePath);

	// Save savefile
	void SaveRZBNFile(string rzbnFilePath);

private:

	// Player object
	Player* m_pPlayer;
};

