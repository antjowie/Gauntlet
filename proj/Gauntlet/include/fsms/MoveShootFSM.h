#pragma once
#include "FSM.h"

class MoveShootFSM: public FSM
{
public:

	MoveShootFSM(Command *move, Command *shoot);
	virtual ~MoveShootFSM() override;

	virtual void update(float elapsedTime) override final;
	virtual void updateAfterMove(float elapsedTime) override final;
	//virtual void onCollision(Object *obj);

private:

	Command *m_coMove;
	Command *m_coShoot;
};