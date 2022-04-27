#include "GameGraphicModule.h"
#include "GameBlockAllocator.h"

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL2_gfxPrimitives.h>

#include <new>
#include <vector>

class GameTexture::Impl
{
public:
	SDL_Texture* m_pTexture;

public:
	Impl() : m_pTexture(nullptr) {}

	~Impl()
	{
		if (m_pTexture)
		{
			SDL_DestroyTexture(m_pTexture);
		}
	}
};

const GameColor& GameTexture::GetColor()
{
	GameColor color;
	SDL_GetTextureColorMod(m_pImpl->m_pTexture, &color.r, &color.g, &color.b);
	SDL_GetTextureAlphaMod(m_pImpl->m_pTexture, &color.a);
	return color;
}

void GameTexture::SetColor(const GameColor& color)
{
	SDL_SetTextureColorMod(m_pImpl->m_pTexture, color.r, color.g, color.b);
	SDL_SetTextureAlphaMod(m_pImpl->m_pTexture, color.a);
}

GameVec2 GameTexture::GetSize()
{
	GameVec2 vec2Size;
	SDL_QueryTexture(m_pImpl->m_pTexture, nullptr, nullptr, &vec2Size.x, &vec2Size.y);
	return vec2Size;
}

GameTexture::AccessMode GameTexture::GetAccessMode()
{
	int nAccessMode;
	SDL_QueryTexture(m_pImpl->m_pTexture, nullptr, &nAccessMode, nullptr, nullptr);
	return (GameTexture::AccessMode)nAccessMode;
}

GameBlendMode GameTexture::GetBlendMode()
{
	SDL_BlendMode blendMode;
	SDL_GetTextureBlendMode(m_pImpl->m_pTexture, &blendMode);
	return (GameBlendMode)blendMode;
}

void GameTexture::SetBlendMode(GameBlendMode emBlend)
{
	SDL_SetTextureBlendMode(m_pImpl->m_pTexture, (SDL_BlendMode)emBlend);
}

GameTexture::ScaleMode GameTexture::GetScaleMode()
{
	SDL_ScaleMode scaleMode;
	SDL_GetTextureScaleMode(m_pImpl->m_pTexture, &scaleMode);
	return (GameTexture::ScaleMode)scaleMode;
}

void GameTexture::SetScaleMode(GameTexture::ScaleMode emScale)
{
	SDL_SetTextureScaleMode(m_pImpl->m_pTexture, (SDL_ScaleMode)emScale);
}

GameTexture::GameTexture()
{
	void* pMem = GameBlockAllocator::GetInstance().Allocate(sizeof(Impl));
	m_pImpl = new (pMem) Impl();
}

GameTexture::~GameTexture()
{
	m_pImpl->~Impl();
	GameBlockAllocator::GetInstance().Free(m_pImpl, sizeof(Impl));
}

class GameFont::Impl
{
public:
	TTF_Font* m_pFont;

public:
	Impl()
	{
		m_pFont = nullptr;
	}
	~Impl()
	{
		if (m_pFont)
		{
			TTF_CloseFont(m_pFont);
		}
	}
};

GameFont::Style GameFont::GetStyle()
{
	return (GameFont::Style)TTF_GetFontStyle(m_pImpl->m_pFont);
}

void GameFont::SetStyle(GameFont::Style emStyle)
{
	TTF_SetFontStyle(m_pImpl->m_pFont, (int)emStyle);
}

GameFont::Hinting GameFont::GetHinting()
{
	return (GameFont::Hinting)TTF_GetFontHinting(m_pImpl->m_pFont);
}

void GameFont::SetHinting(GameFont::Hinting emHinting)
{
	TTF_SetFontHinting(m_pImpl->m_pFont, (int)emHinting);
}

int GameFont::GetKerning()
{
	return TTF_GetFontKerning(m_pImpl->m_pFont);
}

void GameFont::SetKerning(int nKerning)
{
	TTF_SetFontKerning(m_pImpl->m_pFont, nKerning);
}

int GameFont::GetOutline()
{
	return TTF_GetFontOutline(m_pImpl->m_pFont);
}

void GameFont::SetOutline(int nOutlineWidth)
{
	TTF_SetFontOutline(m_pImpl->m_pFont, nOutlineWidth);
}

GameFont::GameFont()
{
	void* pMem = GameBlockAllocator::GetInstance().Allocate(sizeof(Impl));
	m_pImpl = new (pMem) Impl();
}

GameFont::~GameFont()
{
	m_pImpl->~Impl();
	GameBlockAllocator::GetInstance().Free(m_pImpl, sizeof(Impl));
}

class GameGraphicModule::Impl
{
public:
	// SDL 窗口及渲染器
	SDL_Window* m_pWindow;
	SDL_Renderer* m_pRenderer;

	// Font 创建辅助变量
	SDL_Color				m_colorForeground;
	SDL_Color				m_colorBackground;

	// RenderCopy 辅助变量
	SDL_Rect				m_rectSrc;
	SDL_FRect				m_rectDst;
	SDL_FPoint				m_pointAnchor;

	// RenderDraw 辅助变量
	GameColor				m_renderDrawColor;
	GameBlendMode			m_renderDrawBlendMode;
	GameTexture* m_renderDrawTarget;
	std::vector<int16_t>	m_vecPointX;
	std::vector<int16_t>	m_vecPointY;

public:
	Impl()
	{
		SDL_InitSubSystem(SDL_INIT_VIDEO);
		IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP);
		if (!TTF_WasInit()) TTF_Init();

		m_pWindow = SDL_CreateWindow("Aden", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
		m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, SDL_RENDERER_ACCELERATED);

		SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "best");

		m_colorForeground = { 0, 0, 0, 0 };
		m_colorBackground = { 0, 0, 0, 0 };

		m_rectSrc = { 0, 0, 0, 0 };
		m_rectDst = { 0.0f, 0.0f, 0.0f, 0.0f };
		m_pointAnchor = { 0.0f, 0.0f };

		SDL_GetRenderDrawColor(m_pRenderer, &m_renderDrawColor.r, &m_renderDrawColor.g, &m_renderDrawColor.b, &m_renderDrawColor.a);
		SDL_GetRenderDrawBlendMode(m_pRenderer, (SDL_BlendMode*)(&m_renderDrawBlendMode));
		m_renderDrawTarget = nullptr;
	}
	~Impl()
	{
		SDL_DestroyWindow(m_pWindow);
		SDL_DestroyRenderer(m_pRenderer);

		TTF_Quit();
		IMG_Quit();
		SDL_QuitSubSystem(SDL_INIT_VIDEO);
	}
};

GameTexture* GameGraphicModule::LoadTexture(const GameData& dataTexture)
{
	SDL_Surface* pSurface = nullptr;

	if (pSurface = IMG_Load_RW(SDL_RWFromMem(dataTexture.pData, dataTexture.nLength), 1))
	{
		void* pMem = GameBlockAllocator::GetInstance().Allocate(sizeof(GameTexture));
		GameTexture* pGameTexture = new (pMem) GameTexture();
		pGameTexture->m_pImpl->m_pTexture = SDL_CreateTextureFromSurface(m_pImpl->m_pRenderer, pSurface);

		SDL_FreeSurface(pSurface);
		return pGameTexture;
	}
	return nullptr;
}

GameFont* GameGraphicModule::LoadFont(const GameData& dataFont, const int& nFontSize)
{
	TTF_Font* pFont = nullptr;

	if (pFont = TTF_OpenFontRW(SDL_RWFromMem(dataFont.pData, dataFont.nLength), 1, nFontSize))
	{
		void* pMem = GameBlockAllocator::GetInstance().Allocate(sizeof(GameFont));
		GameFont* pGameFont = new (pMem) GameFont();
		pGameFont->m_pImpl->m_pFont = pFont;

		return pGameFont;
	}
	return nullptr;
}

GameTexture* GameGraphicModule::CreateTextSolid(GameFont* pFont, const std::string& strText, const GameColor& colorFg)
{
	m_pImpl->m_colorForeground.r = colorFg.r;
	m_pImpl->m_colorForeground.g = colorFg.g;
	m_pImpl->m_colorForeground.b = colorFg.b;
	m_pImpl->m_colorForeground.a = colorFg.a;

	SDL_Surface* pSurface = nullptr;
	if (pSurface = TTF_RenderUTF8_Solid(pFont->m_pImpl->m_pFont, strText.c_str(), m_pImpl->m_colorForeground))
	{
		void* pMem = GameBlockAllocator::GetInstance().Allocate(sizeof(GameTexture));
		GameTexture* pGameTexture = new (pMem) GameTexture();
		pGameTexture->m_pImpl->m_pTexture = SDL_CreateTextureFromSurface(m_pImpl->m_pRenderer, pSurface);
		SDL_FreeSurface(pSurface);
		return pGameTexture;
	}
	return nullptr;
}

GameTexture* GameGraphicModule::CreateTextBlended(GameFont* pFont, const std::string& strText, const GameColor& colorFg)
{
	m_pImpl->m_colorForeground.r = colorFg.r;
	m_pImpl->m_colorForeground.g = colorFg.g;
	m_pImpl->m_colorForeground.b = colorFg.b;
	m_pImpl->m_colorForeground.a = colorFg.a;

	SDL_Surface* pSurface = nullptr;
	if (pSurface = TTF_RenderUTF8_Blended(pFont->m_pImpl->m_pFont, strText.c_str(), m_pImpl->m_colorForeground))
	{
		void* pMem = GameBlockAllocator::GetInstance().Allocate(sizeof(GameTexture));
		GameTexture* pGameTexture = new (pMem) GameTexture();
		pGameTexture->m_pImpl->m_pTexture = SDL_CreateTextureFromSurface(m_pImpl->m_pRenderer, pSurface);

		SDL_FreeSurface(pSurface);
		return pGameTexture;
	}
	return nullptr;
}

GameTexture* GameGraphicModule::CreateTextShaded(GameFont* pFont, const std::string& strText, const GameColor& colorFg, const GameColor& colorBg)
{
	m_pImpl->m_colorForeground.r = colorFg.r;
	m_pImpl->m_colorForeground.g = colorFg.g;
	m_pImpl->m_colorForeground.b = colorFg.b;
	m_pImpl->m_colorForeground.a = colorFg.a;

	m_pImpl->m_colorBackground.r = colorBg.r;
	m_pImpl->m_colorBackground.g = colorBg.g;
	m_pImpl->m_colorBackground.b = colorBg.b;
	m_pImpl->m_colorBackground.a = colorBg.a;

	SDL_Surface* pSurface = nullptr;

	if (pSurface = TTF_RenderUTF8_Shaded(pFont->m_pImpl->m_pFont, strText.c_str(), m_pImpl->m_colorForeground, m_pImpl->m_colorBackground))
	{
		void* pMem = GameBlockAllocator::GetInstance().Allocate(sizeof(GameTexture));
		GameTexture* pGameTexture = new (pMem) GameTexture();
		pGameTexture->m_pImpl->m_pTexture = SDL_CreateTextureFromSurface(m_pImpl->m_pRenderer, pSurface);

		SDL_FreeSurface(pSurface);
		return pGameTexture;
	}
	return nullptr;
}

void GameGraphicModule::DestroyTexture(GameTexture* pGameTexture)
{
	if (!pGameTexture) return;

	pGameTexture->~GameTexture();
	GameBlockAllocator::GetInstance().Free(pGameTexture, sizeof(GameTexture));
}

void GameGraphicModule::DestroyFont(GameFont* pGameFont)
{
	if (!pGameFont) return;

	pGameFont->~GameFont();
	GameBlockAllocator::GetInstance().Free(pGameFont, sizeof(GameFont));
}

void GameGraphicModule::BeginCreatingTexture(const GameVec2& vec2Size)
{
	SDL_Texture* pTexture = SDL_CreateTexture(m_pImpl->m_pRenderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_TARGET, vec2Size.x, vec2Size.y);
	SDL_SetRenderTarget(m_pImpl->m_pRenderer, pTexture);
}

GameTexture* GameGraphicModule::EndCreatingTexture()
{
	SDL_Texture* pTexture = SDL_GetRenderTarget(m_pImpl->m_pRenderer);

	void* pMem = GameBlockAllocator::GetInstance().Allocate(sizeof(GameTexture));
	GameTexture* pGameTexture = new (pMem) GameTexture();
	pGameTexture->m_pImpl->m_pTexture = pTexture;

	SDL_SetRenderTarget(m_pImpl->m_pRenderer, nullptr);

	return pGameTexture;
}

void GameGraphicModule::BeginEditingTexture(GameTexture* pTexture)
{
	SDL_SetRenderTarget(m_pImpl->m_pRenderer, pTexture->m_pImpl->m_pTexture);
}

void GameGraphicModule::EndEditingTexture()
{
	SDL_SetRenderTarget(m_pImpl->m_pRenderer, nullptr);
}

GameRect GameGraphicModule::RenderGetViewport()
{
	SDL_Rect _rectViewport;
	SDL_RenderGetViewport(m_pImpl->m_pRenderer, &_rectViewport);

	GameRect rectViewport{
		_rectViewport.x,
		_rectViewport.y,
		_rectViewport.w,
		_rectViewport.h
	};
	return rectViewport;
}

void GameGraphicModule::RenderSetViewport(const GameRect& rectViewport)
{
	SDL_Rect _rectViewport{
		rectViewport.x,
		rectViewport.y,
		rectViewport.w,
		rectViewport.h
	};

	SDL_RenderSetViewport(m_pImpl->m_pRenderer, &_rectViewport);
}

const GameColor& GameGraphicModule::RenderGetDrawColor()
{
	return m_pImpl->m_renderDrawColor;
}

void GameGraphicModule::RenderSetDrawColor(const GameColor& color)
{
	SDL_SetRenderDrawColor(m_pImpl->m_pRenderer,
		color.r, color.g, color.b, color.a);

	m_pImpl->m_renderDrawColor.r = color.r;
	m_pImpl->m_renderDrawColor.g = color.g;
	m_pImpl->m_renderDrawColor.b = color.b;
	m_pImpl->m_renderDrawColor.a = color.a;
}

GameBlendMode GameGraphicModule::RenderGetDrawBlendMode()
{
	return (GameBlendMode)m_pImpl->m_renderDrawBlendMode;
}

void GameGraphicModule::RenderSetDrawBlendMode(GameBlendMode blend)
{
	SDL_SetRenderDrawBlendMode(m_pImpl->m_pRenderer, (SDL_BlendMode)blend);

	m_pImpl->m_renderDrawBlendMode = blend;
}

void GameGraphicModule::RenderCopy(GameTexture* pGameTexture,
	const GameRect& rectSrc, const GameFRect& rectDst,
	float fAngle, const GameFVec2& vec2Anchor, GameFlipMode emFlip)
{
	if (!pGameTexture) return;

	m_pImpl->m_rectSrc.x = rectSrc.x;
	m_pImpl->m_rectSrc.y = rectSrc.y;
	m_pImpl->m_rectSrc.w = rectSrc.w;
	m_pImpl->m_rectSrc.h = rectSrc.h;

	m_pImpl->m_rectDst.x = rectDst.x;
	m_pImpl->m_rectDst.y = rectDst.y;
	m_pImpl->m_rectDst.w = rectDst.w;
	m_pImpl->m_rectDst.h = rectDst.h;

	m_pImpl->m_pointAnchor.x = vec2Anchor.x * rectDst.w;
	m_pImpl->m_pointAnchor.y = vec2Anchor.y * rectDst.h;

	SDL_RenderCopyExF(m_pImpl->m_pRenderer, pGameTexture->m_pImpl->m_pTexture,
		&m_pImpl->m_rectSrc, &m_pImpl->m_rectDst,
		fAngle, &m_pImpl->m_pointAnchor, (SDL_RendererFlip)emFlip);
}

void GameGraphicModule::RenderClear()
{
	SDL_RenderClear(m_pImpl->m_pRenderer);
}

void GameGraphicModule::RenderPresent()
{
	SDL_RenderPresent(m_pImpl->m_pRenderer);
}

void GameGraphicModule::RenderDrawPoint(const GameVec2& vec2Point)
{
	SDL_RenderDrawPoint(m_pImpl->m_pRenderer, vec2Point.x, vec2Point.y);
}

void GameGraphicModule::RenderDrawLine(const GameVec2& vec2Point1, const GameVec2& vec2Point2, uint8_t nWidth)
{
	if (nWidth == 1)
	{
		aalineRGBA(m_pImpl->m_pRenderer, vec2Point1.x, vec2Point1.x, vec2Point2.x, vec2Point2.y,
			m_pImpl->m_renderDrawColor.r, m_pImpl->m_renderDrawColor.g, m_pImpl->m_renderDrawColor.b, m_pImpl->m_renderDrawColor.a);
		return;
	}

	thickLineRGBA(m_pImpl->m_pRenderer, vec2Point1.x, vec2Point1.x, vec2Point2.x, vec2Point2.y, nWidth,
		m_pImpl->m_renderDrawColor.r, m_pImpl->m_renderDrawColor.g, m_pImpl->m_renderDrawColor.b, m_pImpl->m_renderDrawColor.a);
}

void GameGraphicModule::RenderDrawRectangle(const GameRect& rect, bool isFilled)
{
	SDL_Rect rectDraw = { rect.x, rect.y, rect.w, rect.h };

	if (isFilled)
	{
		SDL_RenderFillRect(m_pImpl->m_pRenderer, &rectDraw);
		return;
	}
	SDL_RenderDrawRect(m_pImpl->m_pRenderer, &rectDraw);
}

void GameGraphicModule::RenderDrawRoundRectangle(const GameRect& rect, int nRadius, bool isFilled)
{
	if (isFilled)
	{
		roundedBoxRGBA(m_pImpl->m_pRenderer, rect.x, rect.y, rect.x + rect.w, rect.y + rect.h, nRadius,
			m_pImpl->m_renderDrawColor.r, m_pImpl->m_renderDrawColor.g, m_pImpl->m_renderDrawColor.b, m_pImpl->m_renderDrawColor.a);
		return;
	}

	roundedRectangleRGBA(m_pImpl->m_pRenderer, rect.x, rect.y, rect.x + rect.w, rect.y + rect.h, nRadius,
		m_pImpl->m_renderDrawColor.r, m_pImpl->m_renderDrawColor.g, m_pImpl->m_renderDrawColor.b, m_pImpl->m_renderDrawColor.a);
}

void GameGraphicModule::RenderDrawCircle(const GameVec2& vec2Point, int nRadius, bool isFilled)
{
	if (isFilled)
	{
		filledCircleRGBA(m_pImpl->m_pRenderer, vec2Point.x, vec2Point.y, nRadius,
			m_pImpl->m_renderDrawColor.r, m_pImpl->m_renderDrawColor.g, m_pImpl->m_renderDrawColor.b, m_pImpl->m_renderDrawColor.a);
		return;
	}

	aacircleRGBA(m_pImpl->m_pRenderer, vec2Point.x, vec2Point.y, nRadius,
		m_pImpl->m_renderDrawColor.r, m_pImpl->m_renderDrawColor.g, m_pImpl->m_renderDrawColor.b, m_pImpl->m_renderDrawColor.a);
}

void GameGraphicModule::RenderDrawEllipse(const GameVec2& vec2Point, int nRadiusX, int nRadiusY, bool isFilled)
{
	if (isFilled)
	{
		filledEllipseRGBA(m_pImpl->m_pRenderer, vec2Point.x, vec2Point.y, nRadiusX, nRadiusY,
			m_pImpl->m_renderDrawColor.r, m_pImpl->m_renderDrawColor.g, m_pImpl->m_renderDrawColor.b, m_pImpl->m_renderDrawColor.a);
		return;
	}

	aaellipseRGBA(m_pImpl->m_pRenderer, vec2Point.x, vec2Point.y, nRadiusX, nRadiusY,
		m_pImpl->m_renderDrawColor.r, m_pImpl->m_renderDrawColor.g, m_pImpl->m_renderDrawColor.b, m_pImpl->m_renderDrawColor.a);
}

void GameGraphicModule::RenderDrawPie(const GameVec2& vec2Point, int nRadius, int nAngleStart, int nAngleEnd, bool isFilled)
{
	if (isFilled)
	{
		filledPieRGBA(m_pImpl->m_pRenderer, vec2Point.x, vec2Point.y, nRadius, nAngleStart, nAngleEnd,
			m_pImpl->m_renderDrawColor.r, m_pImpl->m_renderDrawColor.g, m_pImpl->m_renderDrawColor.b, m_pImpl->m_renderDrawColor.a);
		return;
	}

	pieRGBA(m_pImpl->m_pRenderer, vec2Point.x, vec2Point.y, nRadius, nAngleStart, nAngleEnd,
		m_pImpl->m_renderDrawColor.r, m_pImpl->m_renderDrawColor.g, m_pImpl->m_renderDrawColor.b, m_pImpl->m_renderDrawColor.a);
}

void GameGraphicModule::RenderDrawTriangle(const GameVec2& vec2Point1, const GameVec2& vec2Point2, const GameVec2& vec2Point3, bool isFilled)
{
	if (isFilled)
	{
		filledTrigonRGBA(m_pImpl->m_pRenderer, vec2Point1.x, vec2Point1.y, vec2Point2.x, vec2Point2.y, vec2Point3.x, vec2Point3.y,
			m_pImpl->m_renderDrawColor.r, m_pImpl->m_renderDrawColor.g, m_pImpl->m_renderDrawColor.b, m_pImpl->m_renderDrawColor.a);
		return;
	}

	aatrigonRGBA(m_pImpl->m_pRenderer, vec2Point1.x, vec2Point1.y, vec2Point2.x, vec2Point2.y, vec2Point3.x, vec2Point3.y,
		m_pImpl->m_renderDrawColor.r, m_pImpl->m_renderDrawColor.g, m_pImpl->m_renderDrawColor.b, m_pImpl->m_renderDrawColor.a);
}

void GameGraphicModule::RenderDrawPolygon(const std::initializer_list<GameVec2>& ilPoints, bool isFilled)
{
	for (size_t i = 0; i < ilPoints.size(); i++)
	{
		GameVec2 vec2Point = *(ilPoints.begin() + i);
		m_pImpl->m_vecPointX.push_back(vec2Point.x);
		m_pImpl->m_vecPointY.push_back(vec2Point.y);
	}

	if (isFilled)
	{
		filledPolygonRGBA(m_pImpl->m_pRenderer, &m_pImpl->m_vecPointX[0], &m_pImpl->m_vecPointY[0], ilPoints.size(),
			m_pImpl->m_renderDrawColor.r, m_pImpl->m_renderDrawColor.g, m_pImpl->m_renderDrawColor.b, m_pImpl->m_renderDrawColor.a);
		return;
	}
	aapolygonRGBA(m_pImpl->m_pRenderer, &m_pImpl->m_vecPointX[0], &m_pImpl->m_vecPointY[0], ilPoints.size(),
		m_pImpl->m_renderDrawColor.r, m_pImpl->m_renderDrawColor.g, m_pImpl->m_renderDrawColor.b, m_pImpl->m_renderDrawColor.a);

	m_pImpl->m_vecPointX.clear(); m_pImpl->m_vecPointY.clear();
}

void GameGraphicModule::RenderDrawBezier(const std::initializer_list<GameVec2>& ilPoints, int nInterpolationCount)
{
	for (size_t i = 0; i < ilPoints.size(); i++)
	{
		GameVec2 vec2Point = *(ilPoints.begin() + i);
		m_pImpl->m_vecPointX.push_back(vec2Point.x);
		m_pImpl->m_vecPointY.push_back(vec2Point.y);
	}
	bezierRGBA(m_pImpl->m_pRenderer, &m_pImpl->m_vecPointX[0], &m_pImpl->m_vecPointY[0], ilPoints.size(), nInterpolationCount,
		m_pImpl->m_renderDrawColor.r, m_pImpl->m_renderDrawColor.g, m_pImpl->m_renderDrawColor.b, m_pImpl->m_renderDrawColor.a);

	m_pImpl->m_vecPointX.clear(); m_pImpl->m_vecPointY.clear();
}

GameVec2 GameGraphicModule::GetWindowSize()
{
	GameVec2 vec2Size;
	SDL_GetWindowSize(m_pImpl->m_pWindow, &vec2Size.x, &vec2Size.y);
	return vec2Size;
}

void GameGraphicModule::SetWindowSize(const GameVec2& vec2Size)
{
	SDL_SetWindowSize(m_pImpl->m_pWindow, vec2Size.x, vec2Size.y);
}

GameVec2 GameGraphicModule::GetWindowMaxSize()
{
	GameVec2 vec2Size;
	SDL_GetWindowMaximumSize(m_pImpl->m_pWindow, &vec2Size.x, &vec2Size.y);
	return vec2Size;
}

void GameGraphicModule::SetWindowMaxSize(const GameVec2& vec2Size)
{
	SDL_SetWindowMaximumSize(m_pImpl->m_pWindow, vec2Size.x, vec2Size.y);
}

GameVec2 GameGraphicModule::GetWindowMinSize()
{
	GameVec2 vec2Size;
	SDL_GetWindowMinimumSize(m_pImpl->m_pWindow, &vec2Size.x, &vec2Size.y);
	return vec2Size;
}

void GameGraphicModule::SetWindowMinSize(const GameVec2& vec2Size)
{
	SDL_SetWindowMinimumSize(m_pImpl->m_pWindow, vec2Size.x, vec2Size.y);
}

GameGraphicModule::GameGraphicModule()
{
	void* pMem = GameBlockAllocator::GetInstance().Allocate(sizeof(Impl));
	m_pImpl = new (pMem) Impl();
}

GameGraphicModule::~GameGraphicModule()
{
	m_pImpl->~Impl();
	GameBlockAllocator::GetInstance().Free(m_pImpl, sizeof(Impl));
}