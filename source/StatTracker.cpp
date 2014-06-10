#include "StatTracker.h"
#include "BitmapFont.h"
#include "Game.h"
#include <sstream>
#include "../TinyXML/tinyxml.h"

// Helper Function because lazy
void Increment(SGD::Point& pos, std::stringstream& display, int multi = 1)
{
		pos.y += 35.0f * multi;
		display.str(std::string());
}

StatTracker* StatTracker::GetInstance ( void )
{
	static StatTracker m_pStatTracker;
	return &m_pStatTracker;
}

void StatTracker::Reset( void )
{
	m_fTimePlayed						= 0;
	m_fBuildPhaseTime					= 0;
	m_fSurvivalTime						= 0;
	m_fDistanceWalked					= 0;
	m_fBloodSpilled						= 0;
	m_uGrenadesThrown					= 0;
	m_uTotalShotsFired					= 0;
	m_uMachineGunBullets				= 0;
	m_uShotgunShells					= 0;
	m_uRPGRounds						= 0;
	m_uHatTrickShots					= 0;
	m_uWallsPlaced						= 0;
	m_uWallsPickedUp					= 0;
	m_uWindowsPlaced					= 0;
	m_uWindowsPickedUp					= 0;
	m_uTowersBought						= 0;
	m_uTowersSold						= 0;
	m_uTrapsBought						= 0;
	m_uTrapsSold						= 0;
	m_uMoneySpent						= 0;
	m_uTotalKills						= 0;
	m_uRoundsSurvived					= 0;
	m_uConsecutiveRoundsSurvived		= 0;
	Save("resource/data/stats.xml");
}

void StatTracker::Render(float y)
{
	BitmapFont* pFont = Game::GetInstance()->GetFont();

	SGD::Point pos;
	pos.x = 200;
	pos.y = y;

	

	std::stringstream display;

	// Total Time Played
	int timePlayed = (int)m_fTimePlayed;
	display << "Total Time Played:";
	pFont->Draw ( display.str ().c_str () , (int)pos.x , (int)pos.y , 0.5f , { 0 , 0 , 0 } );

	Increment(pos, display);
	int days = timePlayed / 86400;
	timePlayed -= days * 86400;
	display << "\t\t";
	display <<  days;
	display << " Days";
	pFont->Draw ( display.str ().c_str () , (int)pos.x , (int)pos.y , 0.5f , { 0 , 0 , 0 } );

	Increment(pos, display);
	int hours = timePlayed / 3600;
	timePlayed -= hours * 3600;
	display << "\t\t";
	display << hours;
	display << " Hours";
	pFont->Draw ( display.str ().c_str () , (int)pos.x , (int)pos.y , 0.5f , { 0 , 0 , 0 } );
	
	Increment(pos, display);
	int minutes = timePlayed / 60;
	timePlayed -= minutes * 60;
	display << "\t\t";
	display << minutes;
	display << " Minutes";
	pFont->Draw ( display.str ().c_str () , (int)pos.x , (int)pos.y , 0.5f , { 0 , 0 , 0 } );
	
	Increment(pos, display);
	display << "\t\t";
	display << timePlayed;
	display << " Seconds";
	pFont->Draw ( display.str ().c_str () , (int)pos.x , (int)pos.y , 0.5f , { 0 , 0 , 0 } );
	
	Increment(pos, display, 3);

	// Build Phase Time
	Increment(pos, display);
	display << "Build Phase Time:";
	pFont->Draw ( display.str ().c_str () , (int)pos.x , (int)pos.y , 0.5f , { 0 , 0 , 0 } );
	
	timePlayed = (int)m_fBuildPhaseTime;

	Increment(pos, display);
	days = timePlayed / 86400;
	timePlayed -= days * 86400;
	display << "\t\t";
	display <<  days;
	display << " Days";
	pFont->Draw ( display.str ().c_str () , (int)pos.x , (int)pos.y , 0.5f , { 0 , 0 , 0 } );

	Increment(pos, display);
	hours = timePlayed / 3600;
	timePlayed -= hours * 3600;
	display << "\t\t";
	display << hours;
	display << " Hours";
	pFont->Draw ( display.str ().c_str () , (int)pos.x , (int)pos.y , 0.5f , { 0 , 0 , 0 } );
	
	Increment(pos, display);
	minutes = timePlayed / 60;
	timePlayed -= minutes * 60;
	display << "\t\t";
	display << minutes;
	display << " Minutes";
	pFont->Draw ( display.str ().c_str () , (int)pos.x , (int)pos.y , 0.5f , { 0 , 0 , 0 } );
	
	Increment(pos, display);
	display << "\t\t";
	display << timePlayed;
	display << " Seconds";
	pFont->Draw ( display.str ().c_str () , (int)pos.x , (int)pos.y , 0.5f , { 0 , 0 , 0 } );
	
	Increment(pos, display, 3);

	// Survival Phase Time
	Increment(pos, display);
	display << "Survival Phase Time:";
	pFont->Draw ( display.str ().c_str () , (int)pos.x , (int)pos.y , 0.5f , { 0 , 0 , 0 } );
	
	timePlayed = (int)m_fSurvivalTime;

	Increment(pos, display);
	days = timePlayed / 86400;
	timePlayed -= days * 86400;
	display << "\t\t";
	display <<  days;
	display << " Days";
	pFont->Draw ( display.str ().c_str () , (int)pos.x , (int)pos.y , 0.5f , { 0 , 0 , 0 } );

	Increment(pos, display);
	hours = timePlayed / 3600;
	timePlayed -= hours * 3600;
	display << "\t\t";
	display << hours;
	display << " Hours";
	pFont->Draw ( display.str ().c_str () , (int)pos.x , (int)pos.y , 0.5f , { 0 , 0 , 0 } );
	
	Increment(pos, display);
	minutes = timePlayed / 60;
	timePlayed -= minutes * 60;
	display << "\t\t";
	display << minutes;
	display << " Minutes";
	pFont->Draw ( display.str ().c_str () , (int)pos.x , (int)pos.y , 0.5f , { 0 , 0 , 0 } );
	
	Increment(pos, display);
	display << "\t\t";
	display << timePlayed;
	display << " Seconds";
	pFont->Draw ( display.str ().c_str () , (int)pos.x , (int)pos.y , 0.5f , { 0 , 0 , 0 } );
	
	Increment(pos, display, 3);

	// Distance Walked
	Increment(pos, display);
	display << "Distance Walked:";
	pFont->Draw ( display.str ().c_str () , (int)pos.x , (int)pos.y , 0.5f , { 0 , 0 , 0 } );
	
	int distance = (int)m_fDistanceWalked;

	Increment(pos, display);
	int kilo = distance/1000;
	distance -= kilo * 1000;
	display << "\t\t";
	display << kilo;
	display << ".";
	display << distance;
	display << "Kilometers";
	pFont->Draw ( display.str ().c_str () , (int)pos.x , (int)pos.y , 0.5f , { 0 , 0 , 0 } );

	Increment(pos, display, 6);

	// Total Rounds Survived
	Increment(pos, display);
	display << "Total Rounds Survived: ";
	display << m_uRoundsSurvived;
	pFont->Draw ( display.str ().c_str () , (int)pos.x , (int)pos.y , 0.5f , { 0 , 0 , 0 } );

	Increment(pos, display, 7);

	// Survival Streak
	Increment(pos, display);
	display << "Highest Round Reached: ";
	display << m_uConsecutiveRoundsSurvived;
	pFont->Draw ( display.str ().c_str () , (int)pos.x , (int)pos.y , 0.5f , { 0 , 0 , 0 } );

	Increment(pos, display, 7);

	// Total Kills
	Increment(pos, display);
	display << "Total Kills: ";
	display << m_uTotalKills;
	pFont->Draw ( display.str ().c_str () , (int)pos.x , (int)pos.y , 0.5f , { 0 , 0 , 0 } );

	Increment(pos, display, 7);

	// Blood Spilled
	Increment(pos, display);
	display << "Blood Spilled:";
	pFont->Draw ( display.str ().c_str () , (int)pos.x , (int)pos.y , 0.5f , { 0 , 0 , 0 } );

	Increment(pos, display);
	float blood = m_fBloodSpilled;
	float gallon = blood / 4.54f;
	display << "\t\t";
	display << gallon;
	display << " Imperial Gallons";
	pFont->Draw ( display.str ().c_str () , (int)pos.x , (int)pos.y , 0.5f , { 0 , 0 , 0 } );
	
	Increment(pos, display, 6);

	// Grenades Thrown
	Increment(pos, display);
	display << "Weight of Grenades Thrown:";
	pFont->Draw ( display.str ().c_str () , (int)pos.x , (int)pos.y , 0.5f , { 0 , 0 , 0 } );

	float weight = (float)((m_uGrenadesThrown * 400.0f)/1000.0f);
	
	Increment(pos, display);
	if ( weight > 1.0f )
	{
		display << weight;
		display << " Kilograms";
	}
	else
	{
		display << weight * 1000.0f;
		display << " Grams";
	}
	pFont->Draw ( display.str ().c_str () , (int)pos.x , (int)pos.y , 0.5f , { 0 , 0 , 0 } );

	Increment(pos, display, 6);

	// Shots Fired
	Increment(pos, display);
	display << "Total Shots Fired: ";
	display << m_uTotalShotsFired;
	pFont->Draw ( display.str ().c_str () , (int)pos.x , (int)pos.y , 0.5f , { 0 , 0 , 0 } );

	Increment(pos, display, 7);

	// Machine Gun
	Increment(pos, display);
	display << "Weight in Machine Gun Shells Fired:";
	pFont->Draw ( display.str ().c_str () , (int)pos.x , (int)pos.y , 0.5f , { 0 , 0 , 0 } );

	weight = (float)((m_uMachineGunBullets * 11.8f)/1000.0f);
	
	Increment(pos, display);
	if ( weight > 1.0f )
	{
		display << weight;
		display << " Kilograms";
	}
	else
	{
		display << weight * 1000.0f;
		display << " Grams";
	}
	pFont->Draw ( display.str ().c_str () , (int)pos.x , (int)pos.y , 0.5f , { 0 , 0 , 0 } );

	Increment(pos, display, 6);

	// Shotgun
	Increment(pos, display);
	display << "Weight in Shotgun Shells Fired:";
	pFont->Draw ( display.str ().c_str () , (int)pos.x , (int)pos.y , 0.5f , { 0 , 0 , 0 } );

	weight = (float)((m_uShotgunShells *  7.795f)/1000.0f);
	
	Increment(pos, display);
	if ( weight > 1.0f )
	{
		display << weight;
		display << " Kilograms";
	}
	else
	{
		display << weight * 1000.0f;
		display << " Grams";
	}
	pFont->Draw ( display.str ().c_str () , (int)pos.x , (int)pos.y , 0.5f , { 0 , 0 , 0 } );

	Increment(pos, display, 6);

	// RPG
	Increment(pos, display);
	display << "Weight in RPG's Fired:";
	pFont->Draw ( display.str ().c_str () , (int)pos.x , (int)pos.y , 0.5f , { 0 , 0 , 0 } );

	weight = (float)((m_uRPGRounds *  2200.0f)/1000.0f);
	
	Increment(pos, display);
	if ( weight > 1.0f )
	{
		display << weight;
		display << " Kilograms";
	}
	else
	{
		display << weight * 1000.0f;
		display << " Grams";
	}
	pFont->Draw ( display.str ().c_str () , (int)pos.x , (int)pos.y , 0.5f , { 0 , 0 , 0 } );

	Increment(pos, display, 6);

	// HatTrick
	Increment(pos, display);
	display << "Weight in Hat Trick Pucks Fired:";
	pFont->Draw ( display.str ().c_str () , (int)pos.x , (int)pos.y , 0.5f , { 0 , 0 , 0 } );
	
	weight = (float)((m_uHatTrickShots *  185.0f)/1000.0f);
	
	Increment(pos, display);
	if ( weight > 1.0f )
	{
		display << weight;
		display << " Kilograms";
	}
	else
	{
		display << weight * 1000.0f;
		display << " Grams";
	}
	pFont->Draw ( display.str ().c_str () , (int)pos.x , (int)pos.y , 0.5f , { 0 , 0 , 0 } );

	Increment(pos, display, 6);

	// Walls
	Increment(pos, display);
	display << "Walls:";
	pFont->Draw ( display.str ().c_str () , (int)pos.x , (int)pos.y , 0.5f , { 0 , 0 , 0 } );

	Increment(pos, display);
	display << m_uWallsPlaced;
	display << " Placed";
	pFont->Draw ( display.str ().c_str () , (int)pos.x , (int)pos.y , 0.5f , { 0 , 0 , 0 } );

	Increment(pos, display);
	display << m_uWallsPickedUp;
	display << " Picked Up";
	pFont->Draw ( display.str ().c_str () , (int)pos.x , (int)pos.y , 0.5f , { 0 , 0 , 0 } );

	Increment(pos, display, 5);

	// Windows
	Increment(pos, display);
	display << "Windows:";
	pFont->Draw ( display.str ().c_str () , (int)pos.x , (int)pos.y , 0.5f , { 0 , 0 , 0 } );

	Increment(pos, display);
	display << m_uWindowsPlaced;
	display << " Placed";
	pFont->Draw ( display.str ().c_str () , (int)pos.x , (int)pos.y , 0.5f , { 0 , 0 , 0 } );

	Increment(pos, display);
	display << m_uWindowsPickedUp;
	display << " Picked Up";
	pFont->Draw ( display.str ().c_str () , (int)pos.x , (int)pos.y , 0.5f , { 0 , 0 , 0 } );

	Increment(pos, display, 5);

	// Towers
	Increment(pos, display);
	display << "Towers:";
	pFont->Draw ( display.str ().c_str () , (int)pos.x , (int)pos.y , 0.5f , { 0 , 0 , 0 } );

	Increment(pos, display);
	display << m_uTowersBought;
	display << " Bought";
	pFont->Draw ( display.str ().c_str () , (int)pos.x , (int)pos.y , 0.5f , { 0 , 0 , 0 } );

	Increment(pos, display);
	display << m_uTowersSold;
	display << " Sold";
	pFont->Draw ( display.str ().c_str () , (int)pos.x , (int)pos.y , 0.5f , { 0 , 0 , 0 } );

	Increment(pos, display, 5);

	// Traps
	Increment(pos, display);
	display << "Traps:";
	pFont->Draw ( display.str ().c_str () , (int)pos.x , (int)pos.y , 0.5f , { 0 , 0 , 0 } );

	Increment(pos, display);
	display << m_uTrapsBought;
	display << " Bought";
	pFont->Draw ( display.str ().c_str () , (int)pos.x , (int)pos.y , 0.5f , { 0 , 0 , 0 } );

	Increment(pos, display);
	display << m_uTrapsSold;
	display << " Sold";
	pFont->Draw ( display.str ().c_str () , (int)pos.x , (int)pos.y , 0.5f , { 0 , 0 , 0 } );

	Increment(pos, display, 5);

	// Money
	Increment(pos, display);
	display << "Loonies Spent: ";
	display << m_uMoneySpent;
	pFont->Draw ( display.str ().c_str () , (int)pos.x , (int)pos.y , 0.5f , { 0 , 0 , 0 } );


}

void StatTracker::Load ( const char* path )
{
	TiXmlDocument doc;

	if ( doc.LoadFile ( path ) == false )
	{
		Reset();
		return;
	}

	TiXmlElement* pRoot = doc.RootElement ();

	if ( pRoot == nullptr )
		return;

	TiXmlElement* pTime = pRoot->FirstChildElement ( "Time" );

	pTime->QueryFloatAttribute	( "Total" , &m_fTimePlayed);
	pTime->QueryFloatAttribute	 ( "Build" , &m_fBuildPhaseTime);
	pTime->QueryFloatAttribute	( "Survival" , &m_fSurvivalTime);

	TiXmlElement* pDistance = pRoot->FirstChildElement ( "General" );

	pDistance->QueryFloatAttribute	 ( "Walking" , &m_fDistanceWalked);
	pDistance->Attribute ( "Rounds" , (int *)&m_uRoundsSurvived);
	pDistance->Attribute ( "RoundSpree" , (int *)&m_uConsecutiveRoundsSurvived);

	TiXmlElement* pViolence = pRoot->FirstChildElement ( "Violence" );

	pViolence->Attribute ( "Kills" , (int *)&m_uTotalKills);
	pViolence->QueryFloatAttribute	 ( "Blood" , &m_fBloodSpilled);
	pViolence->Attribute ( "Grenades" , (int *)&m_uGrenadesThrown);
	pViolence->Attribute ( "TotalShots" , (int *)&m_uTotalShotsFired);
	pViolence->Attribute ( "MachineGun" , (int *)&m_uMachineGunBullets);
	pViolence->Attribute ( "Shotgun" , (int *)&m_uShotgunShells);
	pViolence->Attribute ( "RPG" , (int *)&m_uRPGRounds);
	pViolence->Attribute ( "HatTrick" , (int *)&m_uHatTrickShots);

	TiXmlElement* pExchange = pRoot->FirstChildElement ( "Exchange" );

	pExchange->Attribute ( "WallsPlaced" , (int *)&m_uWallsPlaced);
	pExchange->Attribute ( "WallsPickedUp" , (int *)&m_uWallsPickedUp);
	pExchange->Attribute ( "WindowsPlaced" , (int *)&m_uWindowsPlaced);
	pExchange->Attribute ( "WindowsPickedUp" , (int *)&m_uWindowsPickedUp);
	pExchange->Attribute ( "TowersBought" , (int *)&m_uTowersBought);
	pExchange->Attribute ( "TowersSold" , (int *)&m_uTowersSold);
	pExchange->Attribute ( "TrapsBought" , (int *)&m_uTrapsBought);
	pExchange->Attribute ( "TrapsSold" , (int *)&m_uTrapsSold);
	pExchange->Attribute ( "Money" , (int *)&m_uMoneySpent);

}

void StatTracker::Save ( const char* path )
{
	TiXmlDocument doc;

	TiXmlDeclaration* pDecl = new TiXmlDeclaration ( "1.0" , "utf-8" , "" );

	doc.LinkEndChild ( pDecl );

	TiXmlElement* pRoot = new TiXmlElement ( "Stats" );


	TiXmlElement* pTime = new TiXmlElement ( "Time" );

	pTime->SetDoubleAttribute ( "Total" , m_fTimePlayed );
	pTime->SetDoubleAttribute ( "Build" , m_fBuildPhaseTime );
	pTime->SetDoubleAttribute ( "Survival" , m_fSurvivalTime );

	pRoot->LinkEndChild ( pTime );


	TiXmlElement* pDistance = new TiXmlElement ( "General" );

	pDistance->SetDoubleAttribute ( "Walking" , m_fDistanceWalked );
	pDistance->SetAttribute ( "Rounds" , m_uRoundsSurvived );
	pDistance->SetAttribute ( "RoundSpree" , m_uConsecutiveRoundsSurvived );

	pRoot->LinkEndChild ( pDistance );


	TiXmlElement* pKills = new TiXmlElement ( "Violence" );

	pKills->SetAttribute ( "Kills" , m_uTotalKills );
	pKills->SetDoubleAttribute ( "Blood" , m_fBloodSpilled );
	pKills->SetAttribute ( "Grenades" , m_uGrenadesThrown );
	pKills->SetAttribute ( "TotalShots" , m_uTotalShotsFired );
	pKills->SetAttribute ( "MachineGun" , m_uMachineGunBullets );
	pKills->SetAttribute ( "Shotgun" , m_uShotgunShells );
	pKills->SetAttribute ( "RPG" , m_uRPGRounds );
	pKills->SetAttribute ( "HatTrick" , m_uHatTrickShots );

	pRoot->LinkEndChild ( pKills );

	TiXmlElement* pExchange = new TiXmlElement ( "Exchange" );

	pExchange->SetAttribute ( "WallsPlaced" , m_uWallsPlaced );
	pExchange->SetAttribute ( "WallsPickedUp" , m_uWallsPickedUp );
	pExchange->SetAttribute ( "WindowsPlaced" , m_uWindowsPlaced );
	pExchange->SetAttribute ( "WindowsPickedUp" , m_uWindowsPickedUp );
	pExchange->SetAttribute ( "TowersBought" , m_uTowersBought );
	pExchange->SetAttribute ( "TowersSold" , m_uTowersSold );
	pExchange->SetAttribute ( "TrapsBought" , m_uTrapsBought );
	pExchange->SetAttribute ( "TrapsSold" , m_uTrapsSold );
	pExchange->SetAttribute ( "Money" , m_uMoneySpent );

	pRoot->LinkEndChild ( pExchange );

	doc.LinkEndChild ( pRoot );

	doc.SaveFile ( path );

}

void StatTracker::IncreaseTime ( float time , bool survival )
{
	m_fTimePlayed += time;
	if ( survival == true )
		m_fSurvivalTime += time;
	else
		m_fBuildPhaseTime += time;
}

void StatTracker::Walk ( float distance )
{
	m_fDistanceWalked += distance;
}

void StatTracker::SpillBlood ( float liters )
{
	m_fBloodSpilled += liters;
	m_uTotalKills++;
}

void StatTracker::SurvivedRound ( int wave )
{
	m_uRoundsSurvived++;
	if ( wave > (int)m_uConsecutiveRoundsSurvived )
		m_uConsecutiveRoundsSurvived = wave;
}

void StatTracker::GrenadeThrown ( void )
{
	m_uGrenadesThrown++;
}

void StatTracker::ShotsFired ( int type )
{
	switch ( type )
	{
	case 0:
		m_uMachineGunBullets++;
		break;
	case 1:
		m_uShotgunShells++;
		break;
	case 2:
		m_uRPGRounds++;
		break;
	case 3:
		m_uHatTrickShots++;
		break;
	}

	m_uTotalShotsFired++;
}

void StatTracker::WallExchange ( bool placed )
{
	if ( placed == true )
		m_uWallsPlaced++;
	else
		m_uWallsPickedUp++;
}

void StatTracker::WindowExchange ( bool placed )
{
	if ( placed == true )
		m_uWindowsPlaced++;
	else
		m_uWindowsPickedUp++;
}

void StatTracker::TowerExchange ( bool bought )
{
	if ( bought == true )
		m_uTowersBought++;
	else
		m_uTowersSold++;
}

void StatTracker::TrapExchange ( bool bought )
{
	if ( bought == true )
		m_uTrapsBought++;
	else
		m_uTrapsSold++;
}

void StatTracker::SpendItUp(unsigned int money)
{
	m_uMoneySpent += money;
}

