#pragma once

#include "Constants.h"
#include "EntityStats.h"
#include "iPlayer.h"
#include <vector>

const int OPP_KING_WEAK = 4008 / 5;
const int GIANT_NEARBY = (int)NorthPrincessY + 3;
const int OPP_GIANT_POS_X = (int)(KingX + PrincessLeftX) / 2;
const int OPP_GIANT_POS_Y = (int)NorthPrincessY;

enum class Status {
	STOPPED = 0,
	RUNNING,
	SUCCESS,
	FAILURE
};

class Behaviour {
public:
	Behaviour() : m_Status(Status::STOPPED) { }
	virtual ~Behaviour() { }
	virtual void onInitialize() {}
	virtual void onTerminate(Status) {}
	virtual Status update() = 0;
	virtual Status tick();
	Status getStatus() const { return m_Status; }
	void resetStatus() { m_Status = Status::STOPPED; }

private:
	Status m_Status;
};

class Decorator : public Behaviour {
protected:
	Behaviour* m_Child;
public:
	Decorator(Behaviour* child) : m_Child(child) { }
};

class Repeat : public Decorator {
public:
	Repeat(Behaviour* child) : Decorator(child) {}
	void setCount(int count) { m_Limit = count; }
	void onInitialize() { m_Counter = 0; }
	Status update() override;
protected:
	int m_Limit = 0;
	int m_Counter = 0;
};

class Composite : public Behaviour {
public:
	void addChild(Behaviour* child) { m_Children.push_back(child); }
protected:
	typedef std::vector<Behaviour*> Behaviours;
	Behaviours m_Children;
};

class Sequence : public Composite {
protected:
	virtual ~Sequence() { }
	virtual void onInitialize() { m_CurrentChildren = m_Children.begin(); }
	virtual Status update() override;
	Behaviours::iterator m_CurrentChildren;
};

class Selector : public Composite {
protected:
	virtual ~Selector() {}
	virtual void onInitialize() { m_Current = m_Children.begin(); }
	virtual Status update() override;
	Behaviours::iterator m_Current;
};

class WorkBehaviour : public Behaviour {
public:
	WorkBehaviour(iPlayer* player) : m_pPlayer(player) {}
protected:
	iPlayer* m_pPlayer;
};

class checkElixir : public WorkBehaviour {
public:
	checkElixir(iPlayer* player) : WorkBehaviour(player) { };
	virtual ~checkElixir() { }
	virtual Status update() override;
};

class checkArcher : public WorkBehaviour {
public:
	checkArcher(iPlayer* player) : WorkBehaviour(player) { };
	virtual ~checkArcher() { }
	virtual Status update() override;
};

class killArcher : public WorkBehaviour {
public:
	killArcher(iPlayer* player) : WorkBehaviour(player) { };
	virtual ~killArcher() { }
	virtual Status update() override;
};

class checkGiant : public WorkBehaviour {
public:
	checkGiant(iPlayer* player) : WorkBehaviour(player) { };
	virtual ~checkGiant() { }
	virtual Status update() override;
};

class killGiant : public WorkBehaviour {
public:
	killGiant(iPlayer* player) : WorkBehaviour(player) { };
	virtual ~killGiant() { }
	virtual Status update() override;
};

class checkIfEnemyWeak : public WorkBehaviour {
public:
	checkIfEnemyWeak(iPlayer* player) : WorkBehaviour(player) { };
	virtual ~checkIfEnemyWeak() { }
	virtual Status update() override;
};

class normalAttack : public WorkBehaviour {
public:
	normalAttack(iPlayer* player) : WorkBehaviour(player) { };
	virtual ~normalAttack() { }
	virtual Status update() override;
};

class fullAttack : public WorkBehaviour {
public:
	fullAttack(iPlayer* player) : WorkBehaviour(player) { };
	virtual ~fullAttack() { }
	virtual Status update() override;
};

class BehaviourTree {
protected:
	Behaviour* m_Root;
public:
	BehaviourTree(Behaviour* root) : m_Root(root) {}
	void tick() {
		if (m_Root) m_Root->tick();
	}
};