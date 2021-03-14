#include "levelselectscene.h"
#include "gamestate.h"

#include "sfml-engine/game.h"
#include "sfml-engine/shapenode.h"
#include "sfml-engine/spritenode.h"
#include "sfml-engine/textnode.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>

const std::string kTitleScreenFont = "../assets/fonts/orbitron.ttf";
const std::string kTitleScreenBackground = "../assets/gfx/starfield-01.png";
const std::string kLevelSelectFile = "../assets/json/setup.json";

void LevelSelectScene::onInitializeScene()
{
    m_orbitronFont.loadFromFile(kTitleScreenFont);
    
    std::shared_ptr<gbh::SpriteNode> spriteNode = std::make_shared<gbh::SpriteNode>(kTitleScreenBackground);
    spriteNode->setName("Background");
    spriteNode->setPosition(640, 360);
    addChild(spriteNode);

    std::shared_ptr<gbh::TextNode> textNode = std::make_shared<gbh::TextNode>("Select level", m_orbitronFont, 60);
    textNode->setPosition(650, 100);
    textNode->setName("LevelSelect");
    addChild(textNode);
    
    loadLevels(kLevelSelectFile);
    addButtons();
}

void LevelSelectScene::loadLevels(const std::string& fileName)
{
    //load from files
    std::ifstream file(fileName);
    nlohmann::json jsonFile;
    
    try
    {
        jsonFile = nlohmann::json::parse(file);
    }
    catch(const std::exception& ex)
    {
        std::cout << "Failed to load level from file: " << fileName << ": " << ex.what() << "\n";
        return;
    }
    
    //Initialize content to create dynamic buttons
    nlohmann::json btnContent = jsonFile["levelSelectOptions"];
    
    if (btnContent.is_array())
    {
        for(int i = 0; i < btnContent.size(); ++i)
        {
            std::string title = btnContent[i]["title"].get<std::string>();
            std::string level = btnContent[i]["path"].get<std::string>();
            
            levels.push_back({ title, level });
        }
    }
}

void LevelSelectScene::addButtons()
{
    for(int i = 0; i < levels.size(); ++i)
    {
        auto button = createBtn(sf::Vector2f(400, 60), levels[i].level,  levels[i].fileToLoad);
        button->setPosition(640.0f, 240 + 80*i);
        addChild(button);
    }

    auto returnBtn = createBtn(sf::Vector2f(400, 60), "Return", "Return");
    returnBtn->setPosition(640, 620);
    addChild(returnBtn);
}

std::shared_ptr<gbh::Node> LevelSelectScene::createBtn(const sf::Vector2f &size, const std::string &txt, const std::string &name)
{
    auto btn = std::make_shared<gbh::ShapeNode>(sf::RectangleShape(size));
    btn->setPosition(size * 0.5f);
    btn->getShape()->setOutlineThickness(0.5);
    btn->getShape()->setFillColor(sf::Color(0, 255, 0, 1.0));
    btn->setName(name);
    
    auto label = std::make_shared<gbh::TextNode>(txt, m_orbitronFont, 30);
    label->setPosition(size * 0.5f);
    label->setName(name);
    btn->addChild(label);
    
    return btn;
}

void LevelSelectScene::onMouseEvent(sf::Event& event)
{
    if (event.type == sf::Event::MouseButtonPressed)
    {
        sf::Vector2 mousePosition = { (float)event.mouseButton.x, (float)event.mouseButton.y };
        std::shared_ptr<gbh::Node> node = getNodeAtViewPoint(mousePosition);

        if (node)
        {
            if (node->getName() == "Return")
            {
              gbh::Game::getInstance().changeScene("title");
            }
            // Returns false if at least one character is uppercase i.e. a title.
            if (!std::any_of(node->getName().begin(), node->getName().end(), isupper))
            {
                GameState::getInstance().selectedLevel = node->getName();
                gbh::Game::getInstance().changeScene("maingame");
            }
        }
    }
}
