#include "objects\Explosion.h"
#include "SpriteManager.h"

Explosion::Explosion(glm::vec2 pos, const ExplosionType type):
	Object(NHTV::SpriteManager::GetInstance()->AddSprite
	("images/spriteSheet.png",glm::vec2(32.f), glm::vec2(0.5f), glm::vec4(0.f, 0.f, 1.f, 1.f), glm::vec4(1.f)), Object::Flyable, false),
	m_animation(24,type == Small ? 26 : 30, (int)type,(int)type +1,0.1f)
{
	NHTV::SpriteManager::GetInstance()->MoveSprite(m_id, glm::value_ptr(pos));
}

Explosion::~Explosion()
{
}

void Explosion::update(const float elapsedTime)
{

	const unsigned beforeUpdate = m_animation.getCurrentFrameCount();

	//std::printf("%i\tGoes till %i\n", m_animation.getCurrentFrameCount(), m_animation.getTotalFrameCount());

	m_animation.update(elapsedTime);
	// This only doesn't work if frame skips one frame
	if (beforeUpdate == m_animation.getTotalFrameCount() - 1 && m_animation.getCurrentFrameCount() != beforeUpdate)
		setDead();
	else
	{
		float u1, v1, u2, v2;
		m_animation.getCurrentFrame(u1, v1, u2, v2);
		NHTV::SpriteManager::GetInstance()->SetSpriteUVCoordinates(m_id, glm::vec2(u1, v1), glm::vec2(u2, v2));
	}

}
