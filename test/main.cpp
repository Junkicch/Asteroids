#include <SFML/Graphics.hpp>
#include <time.h>
#include <list>
#include <math.h>
#include <cmath>
#include <vector>
#include <iostream>
using namespace sf;

const int W = 1200;
const int H = 800;
float x = 10, y = 10;
float dx = 0, dy = 0, ix = 0, iy = 0, Angle = 0;
bool Trust;
bool lua;

float DEGTORAD = 0.017453f;


class bullet
{
public:
    CircleShape shape;
    Vector2f currVelocity;
    float bmaxSpeed;
    bullet(float radius = 5)
        :currVelocity(0.f,0.f), bmaxSpeed(15.f)
    {
        this -> shape.setRadius(radius);
        this -> shape.setFillColor(Color::White);


    }
};
class asteroid
{
public:
    CircleShape bola;
    Vector2f bolaVelocity;
    float amaxSpeed;
    asteroid(float radius = 40)
        :bolaVelocity(0.f,0.f), amaxSpeed(5.f)
    {
        this -> bola.setRadius(radius);
        this -> bola.setFillColor(Color::White);

    }
};

int main()
{
    srand(time(NULL));

    //cria tela
    RenderWindow app(VideoMode(W, H), "Asteroids!", Style::Default);
    app.setFramerateLimit(60);


    //cria asteroid
    int spawncounter = 10;
    asteroid a1;
    std::vector<asteroid> asteroids;

    //cria Player
    CircleShape player(25.f,3);
    player.setFillColor(Color::Yellow);
    player.setPointCount(3);
    player.setOrigin(25.f, 25.f);

    //class bullet
    bullet b1;
    std::vector<bullet> bullets;

    //cria vetores
    Vector2f playercenter;
    Vector2f MouseDir;
    Vector2f aimDir;
    Vector2f aimDirNorm;


    //game loop
    while (app.isOpen())
    {

        Event event;
        while(app.pollEvent(event))
        {
            if (event.type == Event::Closed)
                app.close();
        }


        //para direção do tiro
        playercenter = Vector2f(player.getPosition());
        MouseDir = Vector2f(Mouse::getPosition(app));
        aimDir = MouseDir - playercenter;
        aimDirNorm = sf::Vector2f(aimDir.x / sqrt(pow(aimDir.x, 2) + pow(aimDir.y, 2)), aimDir.y / sqrt(pow(aimDir.x, 2) + pow(aimDir.y, 2)));

        //para rotação da nave
        float PI = 3.14159265f;
        float deg = atan2(aimDirNorm.y, aimDirNorm.x) * 180 / PI;
        player.setRotation(deg + 90);

        //comandos

        //para avançar
        if (Keyboard::isKeyPressed(Keyboard::W)) Trust = true;
        else Trust=false;


        //quando aperta butão w
        if (Trust)
        {
            dx+= cos(deg*DEGTORAD)*0.2;
            dy+= sin(deg*DEGTORAD)*0.2;
        }
        else
        {
            dx*= 0.99;
            dy*= 0.99;
        }

        int maxSpeed = 15;
        float speed = sqrt(dx*dx+dy*dy);
        if (speed>maxSpeed)
        {
            dx *= maxSpeed/speed;
            dy *= maxSpeed/speed;
        }

        x += dx;
        y += dy;


        player.setPosition(x,y);
        player.setRotation(deg + 90);

        if (x>W) x=0;
        if (x<0) x=W;
        if (y>H) y=0;
        if (y<0) y=H;

        //spawnar asteroids
        if (spawncounter < 10)
            spawncounter++;

        if(spawncounter >= 10 && asteroids.size() < 50)
        {
            a1.bola.setPosition(Vector2f(rand() % app.getSize().x, rand() % app.getSize().y));
            a1.bolaVelocity.y = rand()%4*a1.amaxSpeed;
            a1.bolaVelocity.x = rand()%4*a1.amaxSpeed;
            asteroids.push_back(asteroid(a1));

            spawncounter = 0;
        }
        for (size_t i = 0; i < asteroids.size(); i++)
        {
            asteroids[i].bola.move(asteroids[i].bolaVelocity);

            if (asteroids[i].bola.getPosition().x < 0 || asteroids[i].bola.getPosition().x > app.getSize().x
                    || asteroids[i].bola.getPosition().y < 0 || asteroids[i].bola.getPosition().y > app.getSize().y)
            {
                asteroids.erase(asteroids.begin() + i);
            }
            else if (asteroids[i].bola.getGlobalBounds().intersects(player.getGlobalBounds()))
            {
                //colisão projetil->asteroids

                        asteroids.erase(asteroids.begin() + i);
                        x=600;
                        y=400;
                        dx=0;
                        dy=0;
                        break;


            }

        }

        //atirar projetil
        int shoottime;
        if(shoottime <=5) shoottime++;
        if (Mouse::isButtonPressed(Mouse::Left) && shoottime>=5)
        {
            b1.shape.setPosition(playercenter);
            b1.currVelocity = aimDirNorm * b1.bmaxSpeed;

            shoottime=0;
            bullets.push_back(bullet(b1));

        }
        for (size_t i = 0; i < bullets.size(); i++)
        {
            bullets[i].shape.move(bullets[i].currVelocity);

            //quando o projetil sai de tela
            if (bullets[i].shape.getPosition().x < 0 || bullets[i].shape.getPosition().x > app.getSize().x
                    || bullets[i].shape.getPosition().y < 0 || bullets[i].shape.getPosition().y > app.getSize().y)
            {
                bullets.erase(bullets.begin() + i);
            }
            else
            {
                //colisão projetil->asteroids
                for (size_t k = 0; k < asteroids.size(); k++)
                {
                    if (bullets[i].shape.getGlobalBounds().intersects(asteroids[k].bola.getGlobalBounds()))
                    {
                        bullets.erase(bullets.begin() + i);
                        asteroids.erase(asteroids.begin() + k);
                        break;
                    }
                }
            }
        }

        //////desenha/////
        app.clear();
        app.draw(player);
        for (size_t i = 0; i < asteroids.size(); i++)
        {
            app.draw(asteroids[i].bola);
        }
        for (size_t i = 0; i < bullets.size(); i++)
        {
            app.draw(bullets[i].shape);
        }

        app.display();
    }



    return 0;
}

