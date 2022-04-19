#ifndef _GAME_GRAPHIC_MODULE_H_
#define _GAME_GRAPHIC_MODULE_H_

#include "GameData.h"
#include "GameMath.h"

#include <string>
#include <initializer_list>

// RGBA 模型颜色
struct GameColor
{
	uint8_t r, g, b, a;

	GameColor() :
		r(0), g(0), b(0), a(0) {}

	GameColor(uint8_t rIn, uint8_t gIn, uint8_t bIn, uint8_t aIn) :
		r(rIn), g(gIn), b(bIn), a(aIn) {}
};

// RGBA 模型颜色混合模式
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

	INVALID = 0x7FFFFFFF	// 无效枚举值
};

// 图片翻转模式
enum class GameFlipMode
{
	NONE = 0,				// 无翻转
	HORIZONTAL,				// 水平方向翻转
	VERTICAL				// 竖直方向翻转
};

// 游戏纹理
class GameTexture
{
public:
	// 纹理采样模式
	enum class ScaleMode
	{
		NEAREST = 0,		// 按最近的颜色值采样
		LINEAR,				// 线性计算颜色值采样
		BEST				// 各向异性滤波采样
	};

	// 纹理访问模式
	enum class AccessMode
	{
		STATIC = 0,			// 静态纹理，不可修改
		STREAMING,			// 流纹理，可以通过锁定获取像素来修改
		TARGET				// 可作为渲染器目标的纹理
	};

	// 获取纹理的混合模式
	GameBlendMode GetBlendMode();
	// 设置纹理的混合模式
	void SetBlendMode(GameBlendMode emBlend);

	// 获取纹理的采样模式
	ScaleMode GetScaleMode();
	// 设置纹理的采样模式
	void SetScaleMode(ScaleMode emScale);

	// 获取纹理的 RGBA 颜色
	const GameColor& GetColor();
	// 设置纹理的 RGBA 颜色
	void SetColor(const GameColor& color);

	// 获取纹理的矩形大小
	GameVec2 GetRect();

	// 获取纹理的访问模式
	AccessMode GetAccessMode();

private:
	class Impl;
	Impl* m_pImpl;

public:
	GameTexture();
	~GameTexture();

	friend class GameGraphicModule;
};

// 游戏字体
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

	// 获取字体的风格
	Style	GetStyle();
	// 设置字体的风格
	void	SetStyle(Style emStyle);

	// 获取字体的微调
	Hinting	GetHinting();
	// 设置字体的微调
	void	SetHinting(Hinting emHinting);

	// 获取字体的字距
	int		GetKerning();
	// 设置字体的字距
	void	SetKerning(int nKerning);

	// 获取字体的轮框线宽度
	int		GetOutline();
	// 设置字体的轮框线宽度
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
	// 使用数据对象构造纹理对象
	GameTexture* LoadTexture(const GameData& dataTexture);

	// 使用数据对象构造字体对象
	GameFont* LoadFont(const GameData& strFilePath, const int& nFontSize);

	// 使用字体创建指定颜色的 Solid 文本纹理
	GameTexture* CreateTextSolid(GameFont* pFont, const std::string& strText, const GameColor& colorFg);

	// 使用字体创建指定颜色的 Blended 文本纹理
	GameTexture* CreateTextBlended(GameFont* pFont, const std::string& strText, const GameColor& colorFg);

	// 使用字体创建指定颜色的 Shaded文本纹理
	GameTexture* CreateTextShaded(GameFont* pFont, const std::string& strText, const GameColor& colorFg, const GameColor& colorBg);

	// 销毁一个纹理对象
	void DestroyTexture(GameTexture* pGameTexture);

	// 销毁一个字体对象
	void DestroyFont(GameFont* pGameFont);

	// 获取渲染器绘制的视口
	GameRect RenderGetViewport();

	// 设置渲染器绘制的视口
	void RenderSetViewport(const GameRect& rectViewport);

	// 获取渲染器绘制的 RGBA 颜色
	const GameColor& RenderGetDrawColor();

	// 设置渲染器绘制的 RGBA 颜色
	void RenderSetDrawColor(const GameColor& color);

	// 获取渲染器绘制的混合模式
	GameBlendMode RenderGetDrawBlendMode();

	// 设置渲染器绘制的混合模式
	void RenderSetDrawBlendMode(GameBlendMode blend);

	// 开始创建新纹理，纹理成为渲染器的新目标
	// 此后的所有渲染器操作都会作用在新纹理上
	void BeginCreatingTexture(const GameVec2& vec2Size);

	// 结束创建新纹理，并返回创建的纹理
	// 此时渲染器的目标重新回到屏幕上
	GameTexture* EndCreatingTexture();

	// 开始修改纹理，纹理成为渲染器的新目标
	// 此后的所有渲染器操作都会作用在该纹理上
	// 注意，只可修改 BeginCreatingTexture 创建的纹理
	void BeginEditingTexture(GameTexture* pTexture);

	// 结束修改纹理，此时渲染器目标重新回到屏幕上
	// 注意，只可修改 BeginCreatingTexture 创建的纹理
	void EndEditingTexture();

	// 裁取纹理指定矩形区域内的像素，
	// 绘制到渲染器目标的指定矩形区域上，
	// 并按照旋转规则进行旋转
	// @param pGameTexture: 游戏纹理类(静态/动态)
	// @param rectSrc: 裁剪矩形
	// @param rectDst: 绘制矩形
	// @param fRadian: 旋转弧度
	// @param vec2Anchor: 锚点,默认为中心点(0.5f, 0.5f)
	// @param emFlip: 翻转类型
	void RenderCopy(GameTexture* pGameTexture,
		const GameRect& rectSrc, const GameFRect& rectDst,
		float fRadian, const GameFVec2& vec2Anchor, GameFlipMode emFlip
	);

	// 清空当前渲染器目标画面内容
	void RenderClear();

	// 刷新当前渲染器目标画面内容
	void RenderPresent();

	// 在渲染器目标对象中绘制一个点
	void RenderDrawPoint(const GameVec2& vec2Point);

	// 在渲染器目标对象中绘制一条线
	void RenderDrawLine(const GameVec2& vec2Point1, const GameVec2& vec2Point2, uint8_t nWidth = 1);

	// 在渲染器目标对象中绘制一个直角矩形
	void RenderDrawRectangle(const GameRect& rect, bool isFilled = true);

	// 在渲染器目标对象中绘制一个圆角矩形
	void RenderDrawRoundRectangle(const GameRect& rect, int nRadius, bool isFilled = true);

	// 在渲染器目标对象中绘制一个圆
	void RenderDrawCircle(const GameVec2& vec2Point, int nRadius, bool isFilled = true);

	// 在渲染器目标对象中绘制一个椭圆
	void RenderDrawEllipse(const GameVec2& vec2Point, int nRadiusX, int nRadiusY, bool isFilled = true);

	// 在渲染器目标对象中绘制一个扇形
	void RenderDrawPie(const GameVec2& vec2Point, int nRadius, int nAngleStart, int nAngleEnd, bool isFilled = true);

	// 在渲染器目标对象中绘制一个三角形
	void RenderDrawTriangle(const GameVec2& vec2Point1, const GameVec2& vec2Point2, const GameVec2& vec2Point3, bool isFilled = true);

	// 在渲染器目标对象中绘制一个多边形
	void RenderDrawPolygon(const std::initializer_list<GameVec2>& ilPoints, bool isFilled = true);

	// 在渲染器目标对象中绘制一个贝塞尔曲线
	void RenderDrawBezier(const std::initializer_list<GameVec2>& ilPoints, int nInterpolationCount);

	// 获取窗口的尺寸
	GameVec2 GetWindowSize();

	// 设置窗口的尺寸
	void SetWindowSize(const GameVec2& vec2Size);

	// 获取窗口的最大尺寸
	GameVec2 GetWindowMaxSize();

	// 设置窗口的最大尺寸
	void SetWindowMaxSize(const GameVec2& vec2Size);

	// 获取窗口的最小尺寸
	GameVec2 GetWindowMinSize();

	// 设置窗口的最小尺寸
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