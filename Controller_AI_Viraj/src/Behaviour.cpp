#include "Behaviour.h"

Status Behaviour::tick() {
	if (m_Status != Status::RUNNING) onInitialize();
	m_Status = update();
	if (m_Status != Status::RUNNING) onTerminate(m_Status);
	return m_Status;
}

Status Repeat::update() {
	for (;;) {
		m_Child->tick();
		if (m_Child->getStatus() == Status::RUNNING) break;
		if (m_Child->getStatus() == Status::FAILURE) return Status::FAILURE;
		if (++m_Counter == m_Limit) return Status::SUCCESS;
		m_Child->resetStatus();
	}
	return Status::STOPPED;
}

Status Sequence::update() {
	for (;;) {
		int var = 10;
		Status s = (*m_CurrentChildren)->tick();
		if (s != Status::SUCCESS) return s;
		if (++m_CurrentChildren == m_Children.end()) return Status::SUCCESS;
	}
}

Status Selector::update() {
	for (;;) {
		Status s = (*m_Current)->tick();
		if (s != Status::FAILURE) return s;
		if (++m_Current == m_Children.end()) return Status::FAILURE;
	}
}

static const Vec2 ksGiantPos(LEFT_BRIDGE_CENTER_X, RIVER_TOP_Y - 0.5f);
static const Vec2 ksArcherPos(LEFT_BRIDGE_CENTER_X, 0.f);

Status checkArcher::update()
{
	for (unsigned int i = 0; i < m_pPlayer->getNumOpponentMobs(); i++) {
		iPlayer::EntityData currentMob = m_pPlayer->getOpponentMob(i);
		if (currentMob.m_Position.y < RIVER_TOP_Y &&
			currentMob.m_Stats.getMobType() == iEntityStats::MobType::Archer) return Status::SUCCESS;
	}
	return Status::FAILURE;
}

Status killArcher::update()
{
	// Use Swordsman to counter archers
	for (unsigned int i = 0; i < m_pPlayer->getNumMobs(); i++) {
		if (m_pPlayer->getMob(i).m_Stats.getMobType() == iEntityStats::Swordsman &&
			m_pPlayer->getMob(i).m_Position.y < RIVER_TOP_Y) {
			return Status::SUCCESS;
		}
	}
	for (unsigned int i = 0; i < m_pPlayer->getNumOpponentMobs(); i++) {
		iPlayer::EntityData currentMob = m_pPlayer->getOpponentMob(i);
		if (currentMob.m_Stats.getMobType() == iEntityStats::MobType::Archer) {
			if (m_pPlayer->getElixir() >= 3) {
				m_pPlayer->placeMob(iEntityStats::Swordsman, currentMob.m_Position);
				return Status::SUCCESS;
			}
			else return Status::FAILURE;
		}
	}
	return Status::FAILURE;
}

Status checkGiant::update()
{
	for (unsigned int i = 0; i < m_pPlayer->getNumOpponentMobs(); i++) {
		if (m_pPlayer->getOpponentMob(i).m_Stats.getMobType() == iEntityStats::MobType::Giant) {
			if ((m_pPlayer->isNorth() && m_pPlayer->getOpponentMob(i).m_Position.y < GIANT_NEARBY) ||
				m_pPlayer->getOpponentMob(i).m_Position.y > GAME_GRID_HEIGHT - GIANT_NEARBY) {
				return Status::SUCCESS;
			}
		}
	}
	return Status::FAILURE;
}

Status killGiant::update()
{
	// Use archer to counter giants
	bool isNorth = m_pPlayer->isNorth();
	Vec2 archerPos(OPP_GIANT_POS_X, OPP_GIANT_POS_Y);
	Vec2 archerPos_Game = archerPos.Player2Game(isNorth);
	
	if (m_pPlayer->getElixir() >= 2) m_pPlayer->placeMob(iEntityStats::Archer, archerPos_Game);

	return Status::SUCCESS;
}

Status checkElixir::update() {
	if (m_pPlayer->getElixir() >= 9) return Status::SUCCESS;
	return Status::FAILURE;
}

Status checkIfEnemyWeak::update() {
	
	// If the enemy King tower is weak and our king tower is fine then attack aggresive
	for (unsigned int i = 0; i < m_pPlayer->getNumOpponentBuildings(); i++) {
		iPlayer::EntityData currBuilding = m_pPlayer->getOpponentBuilding(i);
		if (currBuilding.m_Stats.getBuildingType() == iEntityStats::BuildingType::King &&
			currBuilding.m_Health < OPP_KING_WEAK) {
			for (unsigned int j = 0; j < m_pPlayer->getNumBuildings(); j++) {
				if (m_pPlayer->getBuilding(j).m_Stats.getBuildingType() == iEntityStats::BuildingType::King &&
					m_pPlayer->getBuilding(j).m_Health >= OPP_KING_WEAK)
					return Status::SUCCESS;
			}
		}
	}
	return Status::FAILURE;
}

Status normalAttack::update() {
	bool isNorth = m_pPlayer->isNorth();
	Vec2 giantPos_Game = ksGiantPos.Player2Game(isNorth);
	Vec2 archerPos_Game = ksArcherPos.Player2Game(isNorth);

	// Create two archers and a giant
	m_pPlayer->placeMob(iEntityStats::Giant, giantPos_Game);
	m_pPlayer->placeMob(iEntityStats::Archer, archerPos_Game);
	m_pPlayer->placeMob(iEntityStats::Archer, archerPos_Game);

	return Status::SUCCESS;
}

Status fullAttack::update() {
	bool isNorth = m_pPlayer->isNorth();
	Vec2 swordsmanPos_Game = ksGiantPos.Player2Game(isNorth);
	Vec2 archerPos_Game = ksArcherPos.Player2Game(isNorth);

	// Create a swordsman and an archer
	if (m_pPlayer->getElixir() >= 5) {
		m_pPlayer->placeMob(iEntityStats::Swordsman, swordsmanPos_Game);
		while (m_pPlayer->getElixir() >= 2) m_pPlayer->placeMob(iEntityStats::Archer, archerPos_Game);
	}

	return Status::SUCCESS;
}