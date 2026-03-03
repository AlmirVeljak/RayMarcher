#include <SFML/Graphics.hpp>
#include <cmath>

int main() {
    const int mapW = 10;
    const int mapH = 10;
    int mapData[mapH][mapW] = {
        {1,1,1,1,1,1,1,1,1,1},
        {1,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,1,0,0,0,0,1},
        {1,1,1,1,1,0,0,0,0,1},
        {1,0,0,0,1,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,1},
        {1,0,1,1,1,1,1,1,0,1},
        {1,0,0,0,1,0,0,0,0,1},
        {1,0,0,0,1,0,0,0,0,1},
        {1,1,1,1,1,1,1,1,1,1}
    };

    float px = 3.0f;
    float py = 3.0f;
    float angle = 0.0f;
    const float FOV = 3.14159f / 3.0f;

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    sf::RenderWindow window(sf::VideoMode(800,600), "Test", sf::Style::Default, settings);
    window.setFramerateLimit(60);
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            angle -= 0.05f;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            angle += 0.05f;
        }

        float speed = 0.05f;
        float new_px = px;
        float new_py = py;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            new_px += cos(angle) * speed;
            new_py += sin(angle) * speed;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            new_px -= cos(angle) * speed;
            new_py -= sin(angle) * speed;
        }

        if (mapData[(int)new_py][(int)new_px] == 0) {
            px = new_px;
            py = new_py;
        }

        window.clear(sf::Color::Black);

        for (int i = 0; i < 800; i++)
        {
            float rayAngle = (angle - FOV/2.0f) + (i / 800.0f) * FOV;
            float dist = 0.0f;
            bool hit = false;

            float eyeX = cos(rayAngle);
            float eyeY = sin(rayAngle);

            while (!hit && dist < 20) {
                dist += 0.01f;
                int testX = (int)(px + eyeX * dist);
                int testY = (int)(py + eyeY * dist);

                if (mapData[testY][testX] == 1) {
                    hit = true;
                }
            }

            float correctedDist = dist * cos(rayAngle - angle);
            if (correctedDist < 0.01f) {
                correctedDist = 0.01f;
            }

            float brightness = 1.0f / (1.0f + correctedDist * correctedDist * 0.1f);
            brightness = std::min(1.0f, std::max(0.0f,brightness));

            sf::Color wallColor(255*brightness,40*brightness,40*brightness);

            float wallHeight = 600 / (dist + 0.1f);
            float wallCeiling = 300 - wallHeight / 2;
            float wallFloor = 300 + wallHeight / 2;

            if (wallCeiling < 0) {
                wallCeiling = 0;
            }
            if (wallFloor > 600) {
                wallFloor = 600;
            }

            sf::RectangleShape ceiling(sf::Vector2f(1, wallCeiling));
            ceiling.setPosition(i,0);         
            ceiling.setFillColor(sf::Color::Blue);
            window.draw(ceiling);

            sf::RectangleShape line(sf::Vector2f(1, wallHeight));
            line.setPosition(i,300-wallHeight/2);
            line.setFillColor(wallColor);
            window.draw(line);

            sf::RectangleShape floor(sf::Vector2f(1,600 - wallFloor));
            floor.setPosition(i,wallFloor);
            floor.setFillColor(sf::Color::Green);
            window.draw(floor);

        }
        window.display();
    }
    return 0;
}
