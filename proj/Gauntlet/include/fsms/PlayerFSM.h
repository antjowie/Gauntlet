#pragma once
#include "FSM.h"

class PlayerFSM : public FSM
{
public:

	PlayerFSM(Command *move, Command *shoot);
	virtual ~PlayerFSM() override;

	virtual void update(float elapsedTime) override final;
	virtual void updateAfterMove(float elapsedTime) override final;
	
private:

	Command *m_coMove;
	Command *m_coShoot;
};