#ifndef _GAME_GRAPHIC_MODULE_H_
#define _GAME_GRAPHIC_MODULE_H_

#include "GameData.h"
#include "GameMath.h"

#include <string>
#include <initializer_list>

// RGBA ģ����ɫ
struct GameColor
{
	uint8_t r, g, b, a;

	GameColor() :
		r(0), g(0), b(0), a(0) {}

	GameColor(uint8_t rIn, uint8_t gIn, uint8_t bIn, uint8_t aIn) :
		r(rIn), g(gIn), b(bIn), a(aIn) {}
};

// RGBA ģ����ɫ���ģʽ
enum class GameBlendMode
{
	NONE	= 0x00000000,	// dstRGBA = srcRGBA

	BLEND	= 0x00000001,	// dstRGB = (srcRGB * srcA) + (dstRGB * (1 - srcA))
							// dstA = srcA + (dstA * (1-srcA))

	ADD		= 0x00000002,	// dstRGB = (srcRGB * srcA) + dstRGB
							// dstA = dstA

	MOD		= 0x00000004,	// dstRGB = srcRGB * dstRGB
							// dstA = dstA

	MUL		= 0x00000008,	// dstRGB = (srcRGB * dstRGB) + (dstRGB * (1-srcA))
							// dstA = (srcA * dstA) + (dstA * (1-srcA))

	INVALID = 0x7FFFFFFF	// ��Чö��ֵ
};

// ͼƬ��תģʽ
enum class GameFlipMode
{
	NONE = 0,				// �޷�ת
	HORIZONTAL,				// ˮƽ����ת
	VERTICAL				// ��ֱ����ת
};

// ��Ϸ����
class GameTexture
{
public:
	// �������ģʽ
	enum class ScaleMode
	{
		NEAREST = 0,		// ���������ɫֵ����
		LINEAR,				// ���Լ�����ɫֵ����
		BEST				// ���������˲�����
	};

	// �������ģʽ
	enum class AccessMode
	{
		STATIC = 0,			// ��̬���������޸�
		STREAMING,			// ����������ͨ��������ȡ�������޸�
		TARGET				// ����Ϊ��Ⱦ��Ŀ�������
	};

	// ��ȡ����Ļ��ģʽ
	GameBlendMode GetBlendMode();
	// ��������Ļ��ģʽ
	void SetBlendMode(GameBlendMode emBlend);

	// ��ȡ����Ĳ���ģʽ
	ScaleMode GetScaleMode();
	// ��������Ĳ���ģʽ
	void SetScaleMode(ScaleMode emScale);

	// ��ȡ����� RGBA ��ɫ
	const GameColor& GetColor();
	// ��������� RGBA ��ɫ
	void SetColor(const GameColor& color);

	// ��ȡ����ľ��δ�С
	GameVec2 GetRect();

	// ��ȡ����ķ���ģʽ
	AccessMode GetAccessMode();

private:
	class Impl;
	Impl* m_pImpl;

public:
	GameTexture();
	~GameTexture();

	friend class GameGraphicModule;
};

// ��Ϸ����
class GameFont
{
public:
	enum class Style
	{
		NORMAL			= 0x00,
		BOLD			= 0x01,
		ITALIC			= 0x02,
		UNDERLINE		= 0x04,
		STRIKETHROUGH	= 0x08
	};

	enum class Hinting
	{
		NORMAL = 0,
		LIGHT,
		MONO,
		NONE
	};

	// ��ȡ����ķ��
	Style	GetStyle();
	// ��������ķ��
	void	SetStyle(Style emStyle);

	// ��ȡ�����΢��
	Hinting	GetHinting();
	// ���������΢��
	void	SetHinting(Hinting emHinting);

	// ��ȡ������־�
	int		GetKerning();
	// ����������־�
	void	SetKerning(int nKerning);

	// ��ȡ������ֿ��߿��
	int		GetOutline();
	// ����������ֿ��߿��
	void	SetOutline(int nOutlineWidth);

private:
	class	Impl;
	Impl*	m_pImpl;

public:
	GameFont();
	~GameFont();

	friend class GameGraphicModule;
};

class GameGraphicModule
{
public:
	// ʹ�����ݶ������������
	GameTexture* LoadTexture(const GameData& dataTexture);

	// ʹ�����ݶ������������
	GameFont* LoadFont(const GameData& strFilePath, const int& nFontSize);

	// ʹ�����崴��ָ����ɫ�� Solid �ı�����
	GameTexture* CreateTextSolid(GameFont* pFont, const std::string& strText, const GameColor& colorFg);

	// ʹ�����崴��ָ����ɫ�� Blended �ı�����
	GameTexture* CreateTextBlended(GameFont* pFont, const std::string& strText, const GameColor& colorFg);

	// ʹ�����崴��ָ����ɫ�� Shaded�ı�����
	GameTexture* CreateTextShaded(GameFont* pFont, const std::string& strText, const GameColor& colorFg, const GameColor& colorBg);

	// ����һ���������
	void DestroyTexture(GameTexture* pGameTexture);

	// ����һ���������
	void DestroyFont(GameFont* pGameFont);

	// ��ȡ��Ⱦ�����Ƶ��ӿ�
	GameRect RenderGetViewport();

	// ������Ⱦ�����Ƶ��ӿ�
	void RenderSetViewport(const GameRect& rectViewport);

	// ��ȡ��Ⱦ�����Ƶ� RGBA ��ɫ
	const GameColor& RenderGetDrawColor();

	// ������Ⱦ�����Ƶ� RGBA ��ɫ
	void RenderSetDrawColor(const GameColor& color);

	// ��ȡ��Ⱦ�����ƵĻ��ģʽ
	GameBlendMode RenderGetDrawBlendMode();

	// ������Ⱦ�����ƵĻ��ģʽ
	void RenderSetDrawBlendMode(GameBlendMode blend);

	// ��ʼ���������������Ϊ��Ⱦ������Ŀ��
	// �˺��������Ⱦ������������������������
	void BeginCreatingTexture(const GameVec2& vec2Size);

	// �������������������ش���������
	// ��ʱ��Ⱦ����Ŀ�����»ص���Ļ��
	GameTexture* EndCreatingTexture();

	// ��ʼ�޸����������Ϊ��Ⱦ������Ŀ��
	// �˺��������Ⱦ���������������ڸ�������
	// ע�⣬ֻ���޸� BeginCreatingTexture ����������
	void BeginEditingTexture(GameTexture* pTexture);

	// �����޸�������ʱ��Ⱦ��Ŀ�����»ص���Ļ��
	// ע�⣬ֻ���޸� BeginCreatingTexture ����������
	void EndEditingTexture();

	// ��ȡ����ָ�����������ڵ����أ�
	// ���Ƶ���Ⱦ��Ŀ���ָ�����������ϣ�
	// ��������ת���������ת
	// @param pGameTexture: ��Ϸ������(��̬/��̬)
	// @param rectSrc: �ü�����
	// @param rectDst: ���ƾ���
	// @param fRadian: ��ת����
	// @param vec2Anchor: ê��,Ĭ��Ϊ���ĵ�(0.5f, 0.5f)
	// @param emFlip: ��ת����
	void RenderCopy(GameTexture* pGameTexture,
		const GameRect& rectSrc, const GameFRect& rectDst,
		float fRadian, const GameFVec2& vec2Anchor, GameFlipMode emFlip
	);

	// ��յ�ǰ��Ⱦ��Ŀ�껭������
	void RenderClear();

	// ˢ�µ�ǰ��Ⱦ��Ŀ�껭������
	void RenderPresent();

	// ����Ⱦ��Ŀ������л���һ����
	void RenderDrawPoint(const GameVec2& vec2Point);

	// ����Ⱦ��Ŀ������л���һ����
	void RenderDrawLine(const GameVec2& vec2Point1, const GameVec2& vec2Point2, uint8_t nWidth = 1);

	// ����Ⱦ��Ŀ������л���һ��ֱ�Ǿ���
	void RenderDrawRectangle(const GameRect& rect, bool isFilled = true);

	// ����Ⱦ��Ŀ������л���һ��Բ�Ǿ���
	void RenderDrawRoundRectangle(const GameRect& rect, int nRadius, bool isFilled = true);

	// ����Ⱦ��Ŀ������л���һ��Բ
	void RenderDrawCircle(const GameVec2& vec2Point, int nRadius, bool isFilled = true);

	// ����Ⱦ��Ŀ������л���һ����Բ
	void RenderDrawEllipse(const GameVec2& vec2Point, int nRadiusX, int nRadiusY, bool isFilled = true);

	// ����Ⱦ��Ŀ������л���һ������
	void RenderDrawPie(const GameVec2& vec2Point, int nRadius, int nAngleStart, int nAngleEnd, bool isFilled = true);

	// ����Ⱦ��Ŀ������л���һ��������
	void RenderDrawTriangle(const GameVec2& vec2Point1, const GameVec2& vec2Point2, const GameVec2& vec2Point3, bool isFilled = true);

	// ����Ⱦ��Ŀ������л���һ�������
	void RenderDrawPolygon(const std::initializer_list<GameVec2>& ilPoints, bool isFilled = true);

	// ����Ⱦ��Ŀ������л���һ������������
	void RenderDrawBezier(const std::initializer_list<GameVec2>& ilPoints, int nInterpolationCount);

	// ��ȡ���ڵĳߴ�
	GameVec2 GetWindowSize();

	// ���ô��ڵĳߴ�
	void SetWindowSize(const GameVec2& vec2Size);

	// ��ȡ���ڵ����ߴ�
	GameVec2 GetWindowMaxSize();

	// ���ô��ڵ����ߴ�
	void SetWindowMaxSize(const GameVec2& vec2Size);

	// ��ȡ���ڵ���С�ߴ�
	GameVec2 GetWindowMinSize();

	// ���ô��ڵ���С�ߴ�
	void SetWindowMinSize(const GameVec2& vec2Size);

private:
	class Impl;
	Impl* m_pImpl;

public:
	~GameGraphicModule();
	GameGraphicModule(const GameGraphicModule&) = delete;
	GameGraphicModule& operator=(const GameGraphicModule&) = delete;
	static GameGraphicModule& GetInstance()
	{
		static GameGraphicModule instance;
		return instance;
	}
private:
	GameGraphicModule();
};

#endif // !_GAME_GRAPHIC_MODULE_H_