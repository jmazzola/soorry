#pragma once
class StatTracker
{
public:

	static StatTracker* GetInstance( void );

	void Load(const char* path);
	void Save(const char* path);
	void Render(float y);

	void IncreaseTime(float time, bool survival);
	void Walk(float distance);
	void SpillBlood(float liters);
	void SurvivedRound(int wave);
	void GrenadeThrown(void);
	void ShotsFired(int type);
	void WallExchange(bool placed);
	void WindowExchange(bool placed);
	void TowerExchange(bool bought);
	void TrapExchange(bool bought);
	void SpendItUp(unsigned int money);


private:


	StatTracker () = default;
	~StatTracker () = default;

	StatTracker( const StatTracker&) = delete;
	StatTracker& operator= (const StatTracker& ) = delete;


	float			m_fTimePlayed;
	float			m_fBuildPhaseTime;
	float			m_fSurvivalTime;
	float			m_fDistanceWalked;
	float			m_fBloodSpilled;
	unsigned int	m_uGrenadesThrown;
	unsigned int	m_uTotalShotsFired;
	unsigned int	m_uMachineGunBullets;
	unsigned int	m_uShotgunShells;
	unsigned int	m_uRPGRounds;
	unsigned int	m_uHatTrickShots;
	unsigned int	m_uWallsPlaced;
	unsigned int	m_uWallsPickedUp;
	unsigned int	m_uWindowsPlaced;
	unsigned int	m_uWindowsPickedUp;
	unsigned int	m_uTowersBought;
	unsigned int	m_uTowersSold;
	unsigned int	m_uTrapsBought;
	unsigned int	m_uTrapsSold;
	unsigned int	m_uMoneySpent;
	unsigned int	m_uTotalKills;
	unsigned int	m_uRoundsSurvived;
	unsigned int	m_uConsecutiveRoundsSurvived;
	
};

