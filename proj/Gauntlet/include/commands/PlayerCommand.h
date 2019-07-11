#pragma once
#include "Command.h"
#include "objects\Projectile.h"

class PlayerKeyboardMove : public Command
{
public:

	virtual void excecute(Entity *entity, const float elapsedTime) override final;
};

class PlayerKeyboardShoot : public Command
{
public: 
	PlayerKeyboardShoot(ProjectileLauncher * launcher);
	virtual ~PlayerKeyboardShoot();

	virtual void excecute(Entity *entity, const float elapsedTime) override final;

private:

	ProjectileLauncher *m_launcher;
};