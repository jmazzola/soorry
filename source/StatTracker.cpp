#include "StatTracker.h"
#include "../TinyXML/tinyxml.h"


StatTracker* StatTracker::GetInstance ( void )
{
	static StatTracker m_pStatTracker;
	return &m_pStatTracker;
}

void StatTracker::Load ( const char* path )
{
	TiXmlDocument doc;

	if ( doc.LoadFile ( path ) == false )
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
		return;
	}

	TiXmlElement* pRoot = doc.RootElement ();

	if ( pRoot == nullptr )
		return;

	TiXmlElement* pTime = pRoot->FirstChildElement ( "Time" );

	pTime->Attribute ( "Total" , (int *)&m_fTimePlayed);
	pTime->Attribute ( "Build" , (int *)&m_fBuildPhaseTime);
	pTime->Attribute ( "Survival" , (int *)&m_fSurvivalTime);

	TiXmlElement* pDistance = pRoot->FirstChildElement ( "General" );

	pDistance->Attribute ( "Walking" , (int *)&m_fDistanceWalked);
	pDistance->Attribute ( "Rounds" , (int *)&m_uRoundsSurvived);
	pDistance->Attribute ( "RoundSpree" , (int *)&m_uConsecutiveRoundsSurvived);

	TiXmlElement* pViolence = pRoot->FirstChildElement ( "Violence" );

	pViolence->Attribute ( "Kills" , (int *)&m_uTotalKills);
	pViolence->Attribute ( "Blood" , (int *)&m_fBloodSpilled);
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

