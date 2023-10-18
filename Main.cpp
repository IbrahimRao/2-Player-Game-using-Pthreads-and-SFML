#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <pthread.h>

using namespace std;
void *Thread_pacman_movement(void *args1);
void *Thread_paceater_movement(void *args1);

//Pacman
const float pacman_speed = 2000.f;

int pacman_score = 0;
bool leftArrow = 0;
bool rightArrow = 0;
bool upArrow = 0;
bool downArrow = 0;

//Array for cherries on grid
sf::Sprite cherries[10];

//Pac-Eater
const float paceater_speed = 3000.f;

int paceater_score = 0;
bool pressA = 0;
bool pressD = 0;
bool pressW = 0;
bool pressS = 0;

const int winbreadth = 950;
const int winlength = 950;

struct Pacman
{
	sf::Sprite* pac;
	sf::Time* time_pac;
};

struct Paceater
{
	sf::Sprite* pacE;
	sf::Time* time_pacE;
};

int main()
{
	sf::RenderWindow window(sf::VideoMode(950, 950), "Multiplayer Game");
	int Board = 10;	int Size_Cell = window.getSize().x / Board;

	//pac 1
	sf::Texture PlayerLook; PlayerLook.loadFromFile("pacman.png");
	PlayerLook.setSmooth(true); sf::Sprite pac(PlayerLook);
	pac.setOrigin(PlayerLook.getSize().x / 1.9f, PlayerLook.getSize().y / 1.9f); pac.setScale(0.05f, 0.05f);
	pac.setPosition(Size_Cell * 5 + Size_Cell / 1.9f, Size_Cell + Size_Cell / 1.9f);
	sf::Clock clock; sf::Time time_pac = sf::Time::Zero;
	
	//pac 2
	sf::Texture PlayerLook2; PlayerLook2.loadFromFile("paceater.png");
	PlayerLook2.setSmooth(true); sf::Sprite pacE(PlayerLook2);
	pacE.setOrigin(PlayerLook2.getSize().x / 1.9f, PlayerLook2.getSize().y / 1.9f); pacE.setScale(0.06f, 0.06f);
	pacE.setPosition(Size_Cell * 5 + Size_Cell / 1.9f, Size_Cell *(Board - 1) + Size_Cell / 1.9f);
	sf::Clock clock1; sf::Time time_pacE = sf::Time::Zero;
	
	Pacman Tdata = {&pac, &time_pac};
	pthread_t id;
	pthread_create(&id, NULL, Thread_pacman_movement, &Tdata);
	
	Paceater Tdata1 = {&pacE, &time_pacE};
	pthread_t id2;
	pthread_create(&id2, NULL, Thread_paceater_movement, &Tdata1);
	
	sf::RectangleShape cell(sf::Vector2f(Size_Cell, Size_Cell));
	cell.setFillColor(sf::Color::Black);
	cell.setOutlineThickness(1.5);
	cell.setOutlineColor(sf::Color::Blue);

	sf::Texture spriteTexture;
	spriteTexture.loadFromFile("pacman-food.png");
	spriteTexture.setSmooth(true);

	srand(time(NULL));
	for (int j=0;j<10;j++)
	{
		int y;
		int x;
		bool takenCell;
		do { 
			x = rand() % Board; y = rand() % Board;
			takenCell = false;
			for (int k=0;k<j;k++)
			{
				if (cherries[k].getPosition() == sf::Vector2f(x * Size_Cell + Size_Cell / 2.f, y * Size_Cell + Size_Cell / 2.f))
				{takenCell = true;
				break;}
			}
		} while (takenCell);
		
		cherries[j].setTexture(spriteTexture);
		cherries[j].setScale(0.06f, 0.06f);
		cherries[j].setOrigin(spriteTexture.getSize().x / 2.f, spriteTexture.getSize().y / 2.f);
		cherries[j].setPosition(x * Size_Cell + Size_Cell / 2.f, y * Size_Cell + Size_Cell / 2.f);
	}
	sf::Event event;
	for (;window.isOpen() != false;)
	{
		for (;window.pollEvent(event);)
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Up)
				{
					upArrow = true;
				}
					
				else if (event.key.code == sf::Keyboard::Down)
				{
					downArrow = true;
				}
					
				else if (event.key.code == sf::Keyboard::Left)
				{
					leftArrow = true;
				}
					
				else if (event.key.code == sf::Keyboard::Right)
				{
					rightArrow = true;
				}
					
			}
			
			else if (event.type == sf::Event::KeyReleased)
			{
				if (event.key.code == sf::Keyboard::Up)
				{
					upArrow = false;
				}
					
				else if (event.key.code == sf::Keyboard::Down)
				{
					downArrow = false;
				}
					
				else if (event.key.code == sf::Keyboard::Left)
				{
					leftArrow = false;
				}
					
				else if (event.key.code == sf::Keyboard::Right)
				{
					rightArrow = false;
				}
					
			}
			
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::W)
				{
					pressW = true;
				}
					
				else if (event.key.code == sf::Keyboard::S)
				{
					pressS = true;
				}
					
				else if (event.key.code == sf::Keyboard::A)
				{
					pressA = true;
				}
					
				else if (event.key.code == sf::Keyboard::D)
				{
					pressD = true;
				}
					
			}
			else if (event.type == sf::Event::KeyReleased)
			{
				if (event.key.code == sf::Keyboard::W)
				{
					pressW = false;
				}
					
				else if (event.key.code == sf::Keyboard::S)
				{
					pressS = false;
				}
					
				else if (event.key.code == sf::Keyboard::A)
				{
					pressA = false;
				}
					
				else if (event.key.code == sf::Keyboard::D)
				{
					pressD = false;
				}
					
			}
		}
		time_pac = clock.restart();
		time_pacE = clock1.restart();

		window.clear();

		for (int x=0;x<Board;x++){
			for (int y=0;y<Board;y++){
				cell.setPosition(x * Size_Cell, y * Size_Cell);
				window.draw(cell);
			}
		}
		
		int z=0;
		while (z<10)
		{
			window.draw(cherries[z]);
			z++;
		}

		window.draw(pac);
		window.draw(pacE);

		if (pacman_score>60 || paceater_score>60)
		{
			if (pacman_score>paceater_score)
			{
				std::cout << "Pac-Man Wins :)" << endl;
				exit(0);
			}
			else
			{
				std::cout << "Pac-Eater Wins :)" << endl;
				exit(0);
			}
		}

		window.display();
	}

	pthread_join(id, NULL);
	pthread_join(id2, NULL);

	return 0;

}

//Thread function for pacman movement
void *Thread_pacman_movement(void *args)
{
	Pacman *Tdata = static_cast<Pacman*> (args);
	sf::Sprite *pac = Tdata->pac;
	sf::Time *time_pac = Tdata->time_pac;

	while (1)
	{
		float y_axis = 0.f;
		float x_axis = 0.f;
		
		if (leftArrow) 
		{
			x_axis = x_axis - 1.f;
		} 
		if (rightArrow) 
		{
			x_axis = x_axis + 1.f;
		} 
		if (upArrow)
		{
			y_axis = y_axis - 1.f;
		} 
		if (downArrow) 
		{
			y_axis = y_axis + 1.f;
		} 

		sf::Vector2f changed_pos = pac->getPosition() + sf::Vector2f(x_axis, y_axis) * pacman_speed * time_pac->asSeconds();

		
		if (0 > changed_pos.y)
		{
			changed_pos.y = 0;
		}
		else if (950 - pac->getGlobalBounds().height < changed_pos.y)
		{
			changed_pos.y = 950 - pac->getGlobalBounds().height;
		}
		if (0 > changed_pos.x)
		{
			changed_pos.x = 0;
		}
		else if (950 - pac->getGlobalBounds().width < changed_pos.x)
		{
			changed_pos.x = 950 - pac->getGlobalBounds().width;
		}

		pac->setPosition(changed_pos);
		
		int q = 0;
		while (q < 10)
		{
			sf::Sprite &collectables = cherries[q];
			if (pac->getGlobalBounds().intersects(collectables.getGlobalBounds()))
			{
				cherries[q].setPosition(sf::Vector2f(-1200, -1200));
				pacman_score = pacman_score + 10;
			}
			q++;
		}

		sf::sleep(sf::milliseconds(10));
	}
}

// Thread function for pac movement 2
void *Thread_paceater_movement(void *args1)
{
	Paceater *Tdata1 = static_cast<Paceater*> (args1);
	sf::Sprite *pacE = Tdata1->pacE;
	sf::Time *time_pacE = Tdata1->time_pacE;

	while (1)
	{
		float y_axis2 = 0.f;
		float x_axis2 = 0.f;
		
		if (pressD)
		{
			x_axis2 = x_axis2 + 1.5f;
		}
		if (pressA)
		{
			x_axis2 = x_axis2 - 1.5f;
		}
		if (pressW)
		{
			y_axis2 = y_axis2 - 1.5f;
		}
		if (pressS)
		{
			y_axis2 = y_axis2 + 1.5f;
		}
		sf::Vector2f pos_new_2(x_axis2, y_axis2);
		float displacement = (pos_new_2.x * pos_new_2.x + pos_new_2.y * pos_new_2.y);
		if (0.f < displacement)
		{
			pos_new_2 = pos_new_2 / displacement;
			pos_new_2 = pos_new_2 * (paceater_speed * time_pacE->asSeconds());
			sf::Vector2f changed_pos = pacE->getPosition() + pos_new_2;
			if (0.f > changed_pos.y)
			{
				pos_new_2.y = - pacE->getPosition().y;
			}
			else if (950.f < changed_pos.y)
			{
				pos_new_2.y = 950.f - pacE->getPosition().y;
			}
			if (0.f > changed_pos.x)
			{
				pos_new_2.x = - pacE->getPosition().x;
			}
			else if (950.f < changed_pos.x)
			{
				pos_new_2.x = 950.f - pacE->getPosition().x;
			}
			for(int j=0;j<10;j++)
			{
				sf::FloatRect paceater_boundary = pacE->getGlobalBounds();
				sf::FloatRect cherry_boundaries = cherries[j].getGlobalBounds();
				if (paceater_boundary.intersects(cherry_boundaries))
				{
					cherries[j].setPosition(sf::Vector2f(-1200.f, -1200.f));
					paceater_score = paceater_score + 10;
				}
			}
			pacE->move(pos_new_2);
		}
		sf::sleep(sf::milliseconds(10));
	}
}
