#ifndef _GAME_MATH_H_
#define _GAME_MATH_H_

#include <math.h>

constexpr float GAMEMATH_PI = 3.14159265359f;
constexpr float GAMEMATH_EXSILON = 1.192092896e-07f;

struct GameVec2
{
	int x, y;

	GameVec2&& operator-()
	{
		return { -x, -y };
	}

	GameVec2&& operator+(const GameVec2& v)
	{
		return { x + v.x, y + v.y };
	}

	GameVec2&& operator-(const GameVec2& v)
	{
		return { x - v.x, y - v.y };
	}

	GameVec2&& operator*(int a)
	{
		return { x * a, y * a };
	}

	GameVec2() : x(0), y(0) {}

	GameVec2(int xIn, int yIn) : x(xIn), y(yIn) {}

	GameVec2(const GameVec2& copy)
	{
		x = copy.x;
		y = copy.y;
	}

	GameVec2(GameVec2&& move) noexcept
	{
		x = move.x;
		y = move.y;
	}
};

struct GameFVec2
{
	float x, y;

	GameFVec2&& operator-()
	{
		return { -x, -y };
	}

	GameFVec2&& operator+(const GameFVec2& v)
	{
		return { x + v.x, y + v.y };
	}

	GameFVec2&& operator-(const GameFVec2& v)
	{
		return { x - v.x, y - v.y };
	}

	GameFVec2&& operator*(float a)
	{
		return { x * a, y * a };
	}

	// 返回长度
	float Length()
	{
		return sqrtf(x * x + y * y);
	}

	// 返回长度平方
	float LengthSquared()
	{
		return x * x + y * y;
	}

	// 返回标准化向量
	GameFVec2&& Normalize()
	{
		float nLength = Length();
		if (nLength < GAMEMATH_EXSILON)
		{
			return { 0.0f, 0.0f };
		}
		return { x / nLength, y / nLength };
	}

	GameFVec2() : x(0.0f), y(0.0f) {}

	GameFVec2(float xIn, float yIn) : x(xIn), y(yIn) {}

	GameFVec2(const GameFVec2& copy)
	{
		x = copy.x;
		y = copy.y;
	}

	GameFVec2(GameFVec2&& move) noexcept
	{
		x = move.x;
		y = move.y;
	}
};

struct GameRect
{
	int x, y, w, h;

	GameRect() :
		x(0), y(0), w(0), h(0) {}

	GameRect(int xIn, int yIn, int wIn, int hIn) :
		x(xIn), y(yIn), w(wIn), h(hIn) {}

	GameRect(const GameRect& copy)
	{
		x = copy.x;
		y = copy.y;
		w = copy.w;
		h = copy.h;
	}

	GameRect(GameRect&& move) noexcept
	{
		x = move.x;
		y = move.y;
		w = move.w;
		h = move.h;
	}
};

struct GameFRect
{
	float x, y, w, h;

	GameFRect() :
		x(0.0f), y(0.0f), w(0.0f), h(0.0f) {}

	GameFRect(float xIn, float yIn, float wIn, float hIn) :
		x(xIn), y(yIn), w(wIn), h(hIn) {}

	GameFRect(const GameFRect& copy)
	{
		x = copy.x;
		y = copy.y;
		w = copy.w;
		h = copy.h;
	}

	GameFRect(GameFRect&& move) noexcept
	{
		x = move.x;
		y = move.y;
		w = move.w;
		h = move.h;
	}
};

#endif // !_GAME_MATH_H_