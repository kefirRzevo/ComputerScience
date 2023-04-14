#include "../include/guiview.hpp"
#include "../include/config.hpp"


static sf::IntRect&&
CutRect(const sf::Texture& texture, int i, int j, int w = 5, int h = 4);

static bool
Y_B_lower(const Coordinate& b, const Coordinate& x);

static sf::Sprite
GetSprite(const Coordinate& point, const sf::Texture& texture, 
                                   const sf::IntRect& textureRect = {});

static bool
IsInPolygon(const Size& polygonSize, const Coordinate& point);

static bool
Y_B_greater(const Coordinate& b, const Coordinate& x);

static bool
X_B_greater(const Coordinate& b, const Coordinate& x);

static bool
Y_B_greater(const Coordinate& b, const Coordinate& x);

static bool
Y_B_lower(const Coordinate& b, const Coordinate& x);

static Coordinate
GetTextureIndx(const Coordinate& a, const Coordinate& b, const Coordinate& c);

//----------------------------------------//

KeyCode
fromGraphView(int graphKey)
{
    if(graphKey == sf::Keyboard::Q)
        return keyQuit;
    else if(graphKey == sf::Keyboard::W)
        return keyW;
    else if(graphKey == sf::Keyboard::A)
        return keyA;
    else if(graphKey == sf::Keyboard::S)
        return keyS;
    else if(graphKey == sf::Keyboard::D)
        return keyD;
    else if(graphKey == sf::Keyboard::Up)
        return keyArrowU;
    else if(graphKey == sf::Keyboard::Left)
        return keyArrowL;
    else if(graphKey == sf::Keyboard::Down)
        return keyArrowD;
    else if(graphKey == sf::Keyboard::Right)
        return keyArrowR;

    return keyUnknown;
}

//----------------------------------------//

static sf::IntRect&&
CutRect(const sf::Texture& texture, int i, int j, int w, int h)
{
    sf::IntRect rect{static_cast<int>(texture.getSize().x) * i/w,
                     static_cast<int>(texture.getSize().y) * j/h,
                     static_cast<int>(texture.getSize().x) * 1/w,
                     static_cast<int>(texture.getSize().y) * 1/h};
    return std::move(rect);
}

//----------------------------------------//

static sf::Sprite
GetSprite(const Coordinate& point, const sf::Texture& texture, 
                                   const sf::IntRect& textureRect)
{
    sf::Sprite sprite(texture);
    if(textureRect.width && textureRect.height)
        sprite.setTextureRect(textureRect);

    sprite.setScale(SQUARE_SIZE / sprite.getLocalBounds().width,
                    SQUARE_SIZE / sprite.getLocalBounds().height);

    sprite.move(SQUARE_SIZE * (point.first + PADDING_LEFT),
                SQUARE_SIZE * (point.second + PADDING_TOP));

    return sprite;
}

//----------------------------------------//

static bool
IsInPolygon(const Size& polygonSize, const Coordinate& point)
{
    if(point.first <= 0 || point.first > polygonSize.first)
        return false;

    if(point.second <= 0 || point.second  > polygonSize.second)
        return false;

    return true;
}

//----------------------------------------//

static bool
X_B_greater(const Coordinate& b, const Coordinate& x)
{
    return b.first > x.first;
}

//----------------------------------------//

static bool
Y_B_greater(const Coordinate& b, const Coordinate& x)
{
    return b.second > x.second;
}

//----------------------------------------//

static bool
X_B_lower(const Coordinate& b, const Coordinate& x)
{
    return b.first < x.first;
}

//----------------------------------------//

static bool
Y_B_lower(const Coordinate& b, const Coordinate& x)
{
    return b.second < x.second;
}

//----------------------------------------//

static Coordinate
GetTextureIndx(const Coordinate& a, const Coordinate& b, const Coordinate& c)
{
    if(a.first == 0 && a.second == 0)
    {
        if(X_B_greater(b, c))
            return {4, 0};
        else if(X_B_lower(b, c))
            return {3, 1};
        else if(Y_B_greater(b, c))
            return {4, 1};
        else if(Y_B_lower(b, c))
            return {3, 0};
    }
    else
    if(c.first == 0 && c.second == 0)
    {
        if(X_B_greater(b, a))
            return {3, 3};
        else if(X_B_lower(b, a))
            return {4, 2};
        else if(Y_B_greater(b, a))
            return {3, 2};
        else if(Y_B_lower(b, a))
            return {4, 3};
    }
    else 
    if(b.first == a.first && b.first == c.first)
        return {2, 1};
    else if(b.second == a.second && b.second == c.second)
        return {1, 0};
    else if((Y_B_greater(b, a) && X_B_greater(b, c)) ||
            (Y_B_greater(b, c) && X_B_greater(b, a)))
        return {2, 2};
    else if((Y_B_lower(b, a) && X_B_lower(b, c)) ||
            (Y_B_lower(b, c) && X_B_lower(b, a)))
        return {0, 0};
    else if((Y_B_greater(b, a) && X_B_lower(b, c)) ||
            (Y_B_greater(b, c) && X_B_lower(b, a)))
        return {0, 1};
    else if((Y_B_lower(b, a) && X_B_greater(b, c)) ||
            (Y_B_lower(b, c) && X_B_greater(b, a)))
        return {2, 0};

    return {0, 0};
}

//----------------------------------------//

GuiView::GuiView()
{
    finished = false;
    
    int width  = (POLYGON_WIDTH + PADDING_LEFT + PADDING_RIGHT + 2) * SQUARE_SIZE;
    int height = (POLYGON_HEIGHT + PADDING_TOP + PADDING_BOTTOM + 2) * SQUARE_SIZE;

    sfWindow.create(sf::VideoMode(static_cast<unsigned int>(width), 
                                  static_cast<unsigned int>(height)), "Snake Game");
    sfFont.loadFromFile(FONT_PATH);
    sfSnakeTexture.loadFromFile(TEXTURE_PATH);
    //sf::IntRect rabTextureRect = CutRect(sfSnakeTexture, 0, 3);
    //sfRabbitTexture.loadFromFile(TEXTURE_PATH, rabTextureRect);
    sfRabbitTexture.loadFromFile(RABTEXTURE_PATH);
}

//----------------------------------------//

GuiView::~GuiView()
{
    sfWindow.close();
}

//----------------------------------------//

void
GuiView::DrawRabbits()
{
    for(const auto& rabbit: model->rabbits)
    {
        sf::Sprite rabSprite(GetSprite(rabbit->GetCoordinate(),
                                         sfRabbitTexture));
        sfWindow.draw(rabSprite);
    }
}

//----------------------------------------//

void
GuiView::DrawSnakes()
{
    for(const auto& snake: model->snakes)
    {
        if(!snake->IsInGame())
            continue;

        const Coordinates& coords = snake->GetCoordinates();
        sf::Color fillColor = GetStyleColor(snake->GetStyleType());

        for(auto it = coords.begin(); it != coords.end(); it++)
        {
            if(!IsInPolygon(model->GetPolygonSize(), *it))
                continue;

            Coordinate a{}, b{*it}, c{};

            if(*it != coords.front())
                a = *std::prev(it);
            
            if(*it != coords.back())
                c = *std::next(it);

            Coordinate textureIndx = GetTextureIndx(a, b, c);
            sf::IntRect sectRect = CutRect(sfSnakeTexture, textureIndx.first,
                                                           textureIndx.second);
            sf::Sprite sprite(GetSprite(*it, sfSnakeTexture, sectRect));
            sprite.setColor(fillColor);
            sfWindow.draw(sprite);
        }
    }
}

//----------------------------------------//

void
GuiView::RunLoop()
{
    sf::Event sfEvent;
    sf::Clock sfClock;
    sf::Time start;
    sf::Time end;

    start = sfClock.getElapsedTime();
    while(sfWindow.isOpen())
    {
        if(sfWindow.pollEvent(sfEvent))
        {
            if(sfEvent.type == sf::Event::Closed)
                break;
            else if(sfEvent.type == sf::Event::KeyPressed)
                PollOnKey(fromGraphView(sfEvent.key.code));
        }

        end = sfClock.getElapsedTime();
        PollOnTimer((end-start).asMicroseconds());
        start = sfClock.getElapsedTime();

        sfWindow.clear();
        DrawFrame();
        DrawResults();
        DrawRabbits();
        DrawSnakes();
        sfWindow.display();

        if(finished)
            break;
    }
}

//----------------------------------------//

void
GuiView::DrawFrame()
{
    float unit = static_cast<float>(SQUARE_SIZE);
    float width = static_cast<float>(model->GetPolygonSize().first);
    float height = static_cast<float>(model->GetPolygonSize().second);
    sf::Color fillColor = sf::Color::Blue;
    fillColor.a = 128;

    sf::RectangleShape column{{unit, (height + 2) * unit}};
    column.setFillColor(fillColor);
    column.move(unit * PADDING_LEFT, unit * PADDING_TOP);
    sfWindow.draw(column);
    column.move(unit * (width + 1), 0);
    sfWindow.draw(column);

    sf::RectangleShape row{{width * unit, unit}};
    row.setFillColor(fillColor);
    row.move(unit * (PADDING_LEFT + 1), unit * PADDING_TOP);
    sfWindow.draw(row);
    row.move(0, unit * (height + 1));
    sfWindow.draw(row);
}

//----------------------------------------//

void
GuiView::DrawResults()
{
    float sumIndent = static_cast<float>(PADDING_LEFT * SQUARE_SIZE);
    for(const auto& snake: GetModel()->snakes)
    {
        std::string playerResult;
        playerResult += "Player";
        playerResult += std::to_string(GetModel()->GetSnakeIndex(snake));
        playerResult += " ";
        playerResult += std::to_string(snake->GetScore());
        if(!snake->IsAlive())
            playerResult += "(Died)";
        playerResult += ";";
        
        sf::Text result(playerResult, sfFont);
        result.move(sumIndent, static_cast<float>((PADDING_TOP - 1) * SQUARE_SIZE) / 2);
        result.setFillColor(GetStyleColor(snake->GetStyleType()));
        sfWindow.draw(result);
        sumIndent += result.getLocalBounds().width;
        sumIndent += static_cast<float>(SQUARE_SIZE);
    }
}

//----------------------------------------//

sf::Color
GuiView::GetStyleColor(int snakeStyle) const
{
    switch (snakeStyle)
    {
        case 0:
            return sf::Color::Red;

        case 1:
            return sf::Color::Green;

        case 2:
            return sf::Color::Blue;

        case 3:
            return sf::Color::Yellow;

        case 4:
            return sf::Color::Magenta;

        case 5:
            return sf::Color::Cyan;
        
        default:
            return sf::Color::White;
    }

    return sf::Color::White;
}

//----------------------------------------//
