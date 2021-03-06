#include <stdexcept>
#include <memory>
#include <math.h>

#include "sine_scroller.h"
#include "utility.h"

#include "resources.h"

using namespace utility;
using namespace resources;

namespace effects
{
    SineScroller::SineScroller(sf::RenderWindow &window, const std::string &msg)
        : Effect(window), delta_angle_(0.0f)
    {
        if (!font_.loadFromMemory(&font[0], font.size()))
        {
            throw std::runtime_error("Failed to load font"); 
        }

        auto window_size = window.getSize();
        auto char_spacing = window_size.x / 30.0f;
        auto xpos = window_size.x;
        auto ypos = window_size.y * 0.75f;

        for(const auto &letter : msg)
        {
            xpos+=char_spacing;
            std::shared_ptr<sf::Text> character(new sf::Text(letter, font_));
            character->setCharacterSize(char_spacing * 1.2f);
            character->setPosition(xpos, ypos); 
            characters_.push_back(std::move(character));
        }

        restart_point_ = xpos - window_size.x + (char_spacing * 5);

        // Configure the sin function
        arg1_ = 5;  // hmm k 
        arg2_ = 10; // height? 
        arg3_ = 25; // x axis scroll speed 
    }

    SineScroller::~SineScroller()
    {}

    void SineScroller::update(sf::Time elapsed)
    {
        for (unsigned int i = 0; i < characters_.size(); i++)
        {
            auto character = characters_[i];
            auto existing_x = character->getPosition().x;
            float x;

            if (existing_x < 0)
            {
                x = -existing_x + restart_point_; 
            }
            else
            {
                x = character->getPosition().x - elapsed.asMilliseconds() / 5.0f;
            }
            float y = character->getPosition().y + (arg1_ * sin((delta_angle_- (i*arg2_))/(arg3_)));
            character->setColor(sf::Color(y * 0.9, -0.6 * y, 0.2 * y, 255));
            character->setPosition(x, y);
        } 

        delta_angle_+=(elapsed.asMilliseconds() / 4.0f);
    }

    void SineScroller::draw()
    {
        window_.pushGLStates();
        for (const auto &character : characters_)
        {
            window_.draw(*character);
        }
        window_.popGLStates();
    }
}
