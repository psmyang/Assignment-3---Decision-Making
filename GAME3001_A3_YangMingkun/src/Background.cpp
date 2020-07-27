#include "Background.h"
#include "TextureManager.h"


#define SCROLLSPD 2

Background::Background(const std::string& image_path, std::string background_name, GameObjectType type, glm::vec2 position, bool is_cenetered) :
	m_alpha(255), m_name(std::move(background_name)), m_isCentered(is_cenetered), m_active(true)
{
	TextureManager::Instance()->load(image_path, m_name);

	const auto size = TextureManager::Instance()->getTextureSize(m_name);
	setWidth(size.x);
	setHeight(size.y);
	getTransform()->position = position;
	setType(type);
}

Background::~Background() = default;


void Background::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the button
	TextureManager::Instance()->draw(m_name, x, y, 0, m_alpha, m_isCentered);
}

void Background::update()
{
	SCROLLSPD;



}

void Background::clean()
{
}
