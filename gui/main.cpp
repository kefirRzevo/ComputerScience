#include "include/mygraphlib.hpp"
#include <SFML/Graphics.hpp>

int main()
{


    
    Window myWindow({WINDOW_WIDTH, WINDOW_HEIGHT}, "hi");

    Renderer* rend = Renderer::Get(&myWindow);

    rend->DrawPixel({10, 10});

    Event event{};
    while(rend->OnRender())
    {
        event.Poll();
        rend->Display();
    }


/*
sf::RenderWindow window(sf::VideoMode(300, 400), "lala");
sf::Font font;
font.loadFromFile("arial.ttf");
// Create a text
sf::Text text("hello", font);
text.setCharacterSize(30);
text.setStyle(sf::Text::Bold);
text.setFillColor(sf::Color::Red);
// Draw it
window.draw(text);

    *//*
    // create the window
    sf::RenderWindow window(sf::VideoMode(800, 600), "My window");

    sf::Texture texture;
    // load a 32x32 rectangle that starts at (10, 10)
    if (!texture.loadFromFile("main.cpp", sf::IntRect(50, 50, 200, 200)))
    {
        // error...
    }

    sf::Sprite sprite;
    sprite.setTexture(texture);
    sprite.scale(32/sprite.getLocalBounds().width,
                 32/sprite.getLocalBounds().height);
    sprite.move(sf::Vector2f(50, 100));

    //cout << sprite.getScale().x << " " << sprite.getScale().y << "\n";
    //sprite.setTextureRect(sf::IntRect(50, 50, 320, 100));
    //cout << texture.getSize().x << " " << texture.getSize().y << "\n";

    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // clear the window with black color
        window.clear(sf::Color::Black);
        window.draw(sprite);

        // draw everything here...
        // window.draw(...);

        // end the current frame
        window.display();
    }*/

    return 0;
}