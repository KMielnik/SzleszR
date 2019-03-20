#pragma once

class IEntity
{
public:
	virtual ~IEntity() = default;
	virtual void Draw() = 0;
};
