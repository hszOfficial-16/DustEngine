#ifndef _GAME_SPRITE_COMPONENT_H_
#define _GAME_SPRITE_COMPONENT_H_

#include "GameComponent.h"
#include "GameGraphicModule.h"

class GameSpriteComponent : public GameComponent
{
public:
	struct Def
	{
		GameTexture*	pTexture;
		GameRect		rectClip;
		GameFVec2		vec2Anchor;
		GameFlipMode	emFlip;

		Def(GameTexture* pTextureIn = nullptr, GameFVec2 vec2AnchorIn = { 0.5f, 0.5f },
			GameFlipMode emFlipIn = GameFlipMode::NONE) :
			pTexture(pTextureIn), vec2Anchor(vec2AnchorIn), emFlip(emFlipIn)
		{
			if (pTextureIn)
			{
				rectClip = { 0, 0, pTextureIn->GetSize().x, pTextureIn->GetSize().y };
			}
		}

		Def(GameTexture* pTextureIn, GameRect rectClipIn,
			GameFVec2 vec2AnchorIn = { 0.5f, 0.5f }, GameFlipMode emFlipIn = GameFlipMode::NONE) :
			pTexture(pTextureIn), rectClip(rectClipIn), vec2Anchor(vec2AnchorIn), emFlip(emFlipIn) {}
	};

	GameTexture* GetTexture() { return m_pTexture; }
	void SetTexture(GameTexture* pTexture) { m_pTexture = pTexture; }

	const GameRect& GetClipRect() { return m_rectClip; }
	void SetClipRect(const GameRect& rectClip) { m_rectClip = rectClip; }

	const GameFVec2& GetAnchor() { return m_vec2Anchor; }
	void SetAnchor(const GameFVec2& vec2Anchor) { m_vec2Anchor = vec2Anchor; }

	GameFlipMode GetFlipMode() { return m_emFlip; }
	void SetFlipMode(GameFlipMode emFlip) { m_emFlip = emFlip; }

private:
	GameTexture*	m_pTexture;
	GameRect		m_rectClip;
	GameFVec2		m_vec2Anchor;
	GameFlipMode	m_emFlip;

public:
	GameSpriteComponent(const Def& defSprite) :
		GameComponent("Sprite", sizeof(GameSpriteComponent))
	{
		m_pTexture = defSprite.pTexture;
		m_rectClip = defSprite.rectClip;
		m_vec2Anchor = defSprite.vec2Anchor;
		m_emFlip = defSprite.emFlip;
	}
};

#endif // !_GAME_SPRITE_COMPONENT_H_