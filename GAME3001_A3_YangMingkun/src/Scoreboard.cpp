#include "Scoreboard.h"
#include "TextureManager.h"


ScoreBoard::ScoreBoard(const std::string& text, const std::string& font_name, const int font_size, const SDL_Color colour, const glm::vec2 position, const int font_style, const bool is_centered) :
	m_fontColour(colour), m_fontName(font_name), m_text(text), m_isCentered(is_centered), m_fontSize(font_size), m_fontStyle(font_style)
{
	m_fontPath = "../Assets/fonts/" + font_name + ".ttf";

	m_buildFontID();

	FontManager::Instance()->load(m_fontPath, m_fontID, font_size, font_style);
	FontManager::Instance()->textToTexture(text, m_fontID, m_fontID, colour);
	const auto size = TextureManager::Instance()->getTextureSize(m_fontID);
	setWidth(size.x);
	setHeight(size.y);
	getTransform()->position = position;
}

ScoreBoard::~ScoreBoard()
= default;

void ScoreBoard::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the label
	TextureManager::Instance()->drawText(m_fontID, x, y, 0, 255, m_isCentered);
}

void ScoreBoard::update()
{



}

void ScoreBoard::clean()
{

}

void ScoreBoard::setText(const std::string & new_text)
{
	std::string Score = std::to_string(PLayerScore);
	Score = "more: " + Score;
	const char* tmpText = Score.c_str();


	m_text = new_text;

	m_buildFontID();

	FontManager::Instance()->load(m_fontPath, m_fontID, m_fontSize, m_fontStyle);
	FontManager::Instance()->textToTexture(new_text, m_fontID, m_fontID, m_fontColour);
	const auto size = TextureManager::Instance()->getTextureSize(m_fontID);
	setWidth(size.x);
	setHeight(size.y);
}

void ScoreBoard::setColour(const SDL_Color new_colour) const
{
	FontManager::Instance()->load(m_fontPath, m_fontID, m_fontSize, m_fontStyle);
	FontManager::Instance()->textToTexture(m_text, m_fontID, m_fontID, new_colour);
}

void ScoreBoard::setSize(const int new_size)
{
	m_fontSize = new_size;

	m_buildFontID();

	FontManager::Instance()->load(m_fontPath, m_fontID, m_fontSize, TTF_STYLE_NORMAL);
	FontManager::Instance()->textToTexture(m_text, m_fontID, m_fontID, m_fontColour);
	const auto size = TextureManager::Instance()->getTextureSize(m_fontID);
	setWidth(size.x);
	setHeight(size.y);
}

void ScoreBoard::m_buildFontID()
{
	m_fontID = m_fontName;
	m_fontID += "-";
	m_fontID += std::to_string(m_fontSize);
	m_fontID += "-";
	m_fontID += m_text;
}

