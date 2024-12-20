#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <sstream>
#include <iomanip>

class Menu {
public:
    int mainMenu(sf::RenderWindow& window);

    int MaxEasySoldierCustom;
    int MaxMediumSoldierCustom;
    int MaxHardSoldierCustom;
    int TimeBeforeBossCustom;

    float volumeMusic = 5.f;

    bool isMousePressed;

    sf::Sprite background_sprite;
    sf::Text title, subtitle;


    sf::Texture settingsExitButtonTexture;
    sf::Sprite settingsExitButtonSprite;
    sf::RectangleShape settingsMenu;
    sf::Text settingsTextGameMusic;
    sf::Text valueSettingsTextGameMusic;
    sf::Text plus;
    sf::Text minus;

private:
    sf::Texture background_texture;

    std::vector<sf::RectangleShape> buttons;
    std::vector<sf::Text> buttonTexts;
    std::vector<std::string> buttonLabels = { "Jouer", "Custom", "Options", "Quitter" };
    std::vector<bool> buttonFled;

    std::vector<int> optionValues = { 0, 0, 0, 0, 0 };

    sf::RectangleShape leftButton1, leftButton2, leftButton3, leftButton4, leftButton5;
    sf::RectangleShape rightButton1, rightButton2, rightButton3, rightButton4, rightButton5;
    sf::Text optionText1, optionText2, optionText3, optionText4, optionText5;
    sf::Text leftButtonText1, leftButtonText2, leftButtonText3, leftButtonText4, leftButtonText5;
    sf::Text rightButtonText1, rightButtonText2, rightButtonText3, rightButtonText4, rightButtonText5;

    sf::RectangleShape playButton;
    sf::Text playButtonText;

    sf::Vector2f buttonSize = { 400.f, 100.f };
    sf::Vector2f hoverSize = buttonSize * 1.05f;
    float buttonSpacing = 40.f;
    float initialYPos = 250.f;

    int mode = 1;
};