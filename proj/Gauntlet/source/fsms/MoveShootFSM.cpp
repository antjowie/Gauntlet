#include "fsms\MoveShootFSM.h"

MoveShootFSM::MoveShootFSM(Command *move, Command *shoot) :
	m_coMove(move),
	m_coShoot(shoot)
{
}

MoveShootFSM::~MoveShootFSM()
{
	delete m_coMove;
	delete m_coShoot;
}

void MoveShootFSM::update(float elapsedTime)
{
	m_coMove->excecute(m_entity, elapsedTime);
}

void MoveShootFSM::updateAfterMove(float elapsedTime)
{
	m_coShoot->excecute(m_entity, elapsedTime);
}