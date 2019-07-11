#include "FSM.h"

class MoveFSM : public FSM
{
public:

	MoveFSM(Command *move);
	virtual ~MoveFSM() override;

	virtual void update(const float elapsedTime) override;

private:

	Command *m_coMove;
};