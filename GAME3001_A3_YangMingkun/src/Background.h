#pragma once
#ifndef __BACKGROUND__
#define __BACKGROUND__

#include "UIControl.h"

class Background : public UIControl
{
public:
	explicit Background(const std::string& image_path, std::string background_name, GameObjectType type, glm::vec2 position, bool is_cenetered);
	~Background();

	// Life Cycle Events
	void draw() override;
	void update() override;
	void clean() override;
private:

	Uint8 m_alpha;
	std::string m_name;
	bool m_isCentered;
	bool m_active;
};

#endif
