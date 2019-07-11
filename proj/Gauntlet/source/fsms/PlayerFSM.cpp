#include "fsms\PlayerFSM.h"
#include "entities\Player.h"

PlayerFSM::PlayerFSM(Command *move, Command *shoot):
	m_coMove(move),
	m_coShoot(shoot)
{
}

PlayerFSM::~PlayerFSM()
{
	delete m_coMove;
	delete m_coShoot;
}

void PlayerFSM::update(float elapsedTime)
{
	m_coMove->excecute(m_entity,elapsedTime);
}

void PlayerFSM::updateAfterMove(float elapsedTime)
{
	m_coShoot->excecute(m_entity, elapsedTime);
}