#ifndef GameObject_h__
#define GameObject_h__
#pragma once

#include "Texture.h"
#include "Vec2D.h"

class Renderer;

class GameObject
{
public:
	GameObject();
	virtual ~GameObject();

	bool CreateTexture(Renderer& renderer, std::string filename);
	void Render(Renderer& renderer) const;

	const Vec2D& GetVel() const { return m_Velocity; }
	const Vec2D& GetPos() const { return m_Position; }

	int GetWidth() const { return m_Texture.GetWidth(); }
	int GetHeight() const { return m_Texture.GetHeight(); }

	void SetVelocity(const Vec2D& velocity) { m_Velocity = velocity; }
	void SetPosition(const Vec2D& position) { m_Position = position; }

	void Update(double dt);

protected:
	Vec2D m_Position;
	Vec2D m_Velocity;

	Texture m_Texture;
};

#endif GameObject_h__