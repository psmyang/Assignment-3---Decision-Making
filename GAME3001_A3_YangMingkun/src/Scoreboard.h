#pragma once
#ifndef __SCOREBOARD__
#define __SCOREBOARD__

#include <string>

#include "UIControl.h"
#include "FontManager.h"

class ScoreBoard final : public UIControl
{
public:
	explicit ScoreBoard(const std::string& text = "Default ScoreBoard Text", const std::string& font_name = "lazy", int size = 20, SDL_Color colour = { 0, 0, 0, 255 }, glm::vec2 position = glm::vec2(), int font_style = TTF_STYLE_NORMAL, bool is_centered = true);
	~ScoreBoard();

	// Inherited via GameObject
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;

	void setText(const std::string& new_text);
	void setColour(SDL_Color new_colour) const;
	void setSize(int new_size);
	int PLayerScore = 0;
private:


	std::string m_fontPath;
	std::string m_fontID;
	SDL_Color m_fontColour;
	std::string m_fontName;
	std::string m_text;
	bool m_isCentered = true;
	int m_fontSize;
	int m_fontStyle;

	void m_buildFontID();
};

#endif /* defined (__SCOREBOARD__) */