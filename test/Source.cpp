#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;
using namespace sf;
const int drawmaze[20][20] = {
{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,1,1,1,1,1,1,1,1,0,1,0,1,1,1,1,1,1,1},
{1,0,0,2,0,0,0,0,0,1,0,1,0,1,0,0,0,0,0,1},
{1,0,1,1,1,1,1,1,0,1,0,1,0,1,0,1,1,1,1,1},
{1,0,0,0,0,0,0,1,0,1,0,1,0,1,0,0,0,0,0,1},
{1,0,0,0,0,0,0,1,0,1,0,1,0,1,0,1,0,0,0,1},
{1,0,1,1,1,1,1,1,0,1,0,1,0,1,0,1,1,1,1,1},
{1,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,1},
{1,0,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,1,0,1,0,1,1,1,1,1,1,1,1,0,1,0,1,0,1},
{1,0,1,0,1,0,1,2,2,2,2,2,2,1,0,1,0,1,0,1},
{1,0,1,0,1,0,1,2,2,2,2,2,2,1,0,1,0,1,0,1},
{1,0,1,0,1,0,1,2,2,2,2,2,2,1,0,1,0,1,0,1},
{1,0,1,0,1,0,1,1,1,1,1,1,1,1,0,1,0,1,0,1},
{1,0,1,0,1,0,0,3,0,0,0,0,0,0,0,1,0,0,0,1},
{1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
};

bool bigeaten = false;
fstream myFile;

void incscore();
int declives();
void collision(bool, Vector2f);

struct splayer
{
	Sprite player;
	int score = 0;
	int lives = 3;
	int maxscore = 0;
}s;

struct ghoosts
{
	Sprite pinky, inky, blinky, clyde;
}G;

int main()
{
	RenderWindow window(VideoMode(1050, 700), "Pacman");
	RectangleShape map[20][20];
	CircleShape dot[20][20], bigdot;
	int x = 0;


	//TEXT//
	Font font;
	font.loadFromFile("./Organic Relief.ttf");
	Text text1;
	text1.setFont(font);
	text1.setString("SCORE: " + to_string(s.score));
	text1.setFillColor(Color::Yellow);
	text1.setPosition(750, 20);
	text1.setCharacterSize(24);
	Text text2;
	text2.setFont(font);
	text2.setString("LIVES: " + to_string(s.lives));
	text2.setFillColor(Color::Red);
	text2.setPosition(750, 100);
	text2.setCharacterSize(24);


	Texture Pacman, ghost1, ghost2, ghost3, ghost4, lose, levelup;


	//GHOSTS//
	ghost1.loadFromFile("pinky.png");
	G.pinky.setTexture(ghost1);
	G.pinky.setTextureRect(IntRect(0, 0, 19.75, 15.75));
	G.pinky.setPosition(315, 345);
	G.pinky.setScale(2, 2);
	ghost2.loadFromFile("inky.jpg");
	G.inky.setTexture(ghost2);
	G.inky.setTextureRect(IntRect(0, 0, 19.75, 15.75));
	G.inky.setPosition(347, 60);
	G.inky.setScale(2, 2);
	ghost3.loadFromFile("blinky.png");
	G.blinky.setTexture(ghost3);
	G.blinky.setTextureRect(IntRect(0, 0, 19.75, 15.75));
	G.blinky.setPosition(30, 40);
	G.blinky.setScale(2, 2);
	ghost4.loadFromFile("clyde.png");
	G.clyde.setTexture(ghost4);
	G.clyde.setTextureRect(IntRect(0, 0, 19.75, 15.75));
	G.clyde.setPosition(100, 627);
	G.clyde.setScale(2, 2);

	lose.loadFromFile("Game Over.jpg");
	Sprite loss(lose);
	loss.setScale(1.5, 1.5);
	levelup.loadFromFile("level up.png");
	Sprite complete(levelup);
	complete.setScale(0.95, 0.95);


	//SOUNDS//
	SoundBuffer buffer1, buffer2, buffer3, buffer4;
	buffer1.loadFromFile("dot.WAV");
	Sound dotsound;
	dotsound.setBuffer(buffer1);
	buffer2.loadFromFile("death.WAV");
	Sound deathsound;
	deathsound.setBuffer(buffer2);
	buffer3.loadFromFile("eatfruit.WAV");
	Sound reddot;
	reddot.setBuffer(buffer3);
	buffer4.loadFromFile("eatghost.WAV");
	Sound eatghost;
	eatghost.setBuffer(buffer4);
	Music music;
	music.openFromFile("motion.WAV");
	music.play();
	music.setLoop(true);
	music.setVolume(40);


	//PACMAN//
	Pacman.loadFromFile("dhsk.png");
	s.player.setPosition(100, 110);
	s.player.setScale(0.1, 0.1);
	s.player.setTexture(Pacman);
	s.player.setTextureRect(IntRect(x * 217, 0, 217, 215));


	bool canMove = true;
	Vector2f lastMove;
	Vector2f ghosti(0.1, 0);
	Vector2f ghostp(0, 0.1);
	Vector2f ghostb(0, 0.1);
	Vector2f ghostc(0.1, 0);


	//DRAWING MAZE//
	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			if (drawmaze[i][j] == 1)
			{
				map[i][j].setSize(Vector2f(30, 30));
				map[i][j].setFillColor(Color::Blue);
				map[i][j].setPosition(j * 35, i * 35);
			}
			else if (drawmaze[i][j] == 0)
			{
				dot[i][j].setRadius(5);
				dot[i][j].setPosition(j * 35 + 8, i * 35 + 8);
			}
			else if (drawmaze[i][j] == 3) 
			{
				bigdot.setRadius(7);
				bigdot.setFillColor(Color::Red);
				bigdot.setPosition(j * 35 + 8, i * 35 + 8);
			}
		}
	}


	//FILE//
	myFile.open("pac.txt");
	string maxTemp;
	getline(myFile, maxTemp);
	int highestscore = stoi(maxTemp);
	Text text3;
	text3.setFont(font);
	text3.setFillColor(Color::Blue);
	text3.setPosition(700, 180);
	text3.setCharacterSize(18);
	text3.setString("HIGHEST SCORE: " + to_string(highestscore));


	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();

			//PACMAN MOVEMENT//
			if (Keyboard::isKeyPressed(Keyboard::Right))
			{
				s.player.setRotation(0);
				s.player.move(5, 0);
				lastMove = Vector2f(5, 0);
				x++;
				s.player.setOrigin(0, 0);
			}
			if (Keyboard::isKeyPressed(Keyboard::Left))
			{
				s.player.setRotation(180);
				s.player.move(-5, 0);
				lastMove = Vector2f(-5, 0);
				x++;
				s.player.setOrigin(s.player.getLocalBounds().width, s.player.getLocalBounds().height);
			}
			if (Keyboard::isKeyPressed(Keyboard::Up))
			{
				s.player.setRotation(270);
				s.player.move(0, -5);
				lastMove = Vector2f(0, -5);
				x++;
				s.player.setOrigin(s.player.getLocalBounds().width, 0);
			}

			if (Keyboard::isKeyPressed(Keyboard::Down))
			{
				s.player.setRotation(90);
				s.player.move(0, 5);
				lastMove = Vector2f(0, 5);
				x++;
				s.player.setOrigin(0, s.player.getLocalBounds().height);
			}


			//PACMAN WITH MAZE//
			canMove = true;
			for (int i = 0; i < 20; i++)
			{
				for (int j = 0; j < 20; j++)
				{
					if (drawmaze[i][j] == 1)
					{
						if (s.player.getGlobalBounds().intersects(map[i][j].getGlobalBounds()))
							canMove = false;
					}
				}
			}

			collision(canMove, lastMove);       //CALLING COLLISION//


			//LOSS//
			if (s.lives == 0)
			{
				if (s.score > highestscore) 
				{
					highestscore = s.score;
				}

				myFile.open("pac.txt");
				myFile << highestscore << "\t";
				text3.setString("HIGHEST SCORE: " + to_string(highestscore));
				myFile.close();

				if (Mouse::isButtonPressed(Mouse::Left))
				{
					s.lives = 3;
					s.score = 0;

					for (int j = 0; j < 20; j++)
					{
						for (int i = 0; i < 20; i++)
						{
							if (drawmaze[i][j] == 0)
							{
								dot[i][j].setScale(1, 1);
							}
						}
					}
				}
			}


			//PACMAN WITH DOTS//
			for (int j = 0; j < 20; j++)
			{
				for (int i = 0; i < 20; i++)
				{
					if (s.player.getGlobalBounds().intersects(dot[i][j].getGlobalBounds()))
					{
						dot[i][j].setScale(0, 0);
						incscore();                    //CALLING SCORE//
						dotsound.play();
					}
					else if (s.player.getGlobalBounds().intersects(bigdot.getGlobalBounds())) 
					{
						bigdot.setScale(0, 0);
						s.score += 50;
						reddot.play();
						bigeaten = true;
						s.player.setScale(0.12, 0.12);
					}
				}
			}
		}


		//PACMAN WITH GHOSTS & POWER UP//
		if ((s.player.getGlobalBounds().intersects(G.inky.getGlobalBounds()))) 
		{
			if (!bigeaten) 
			{
				declives();                          //CALLING LIVES//
				deathsound.play();
			}
			else 
			{
				G.inky.setScale(0, 0);
				eatghost.play();
				s.score += 20;
				bigeaten = false;
			}
		}
		else if ((s.player.getGlobalBounds().intersects(G.blinky.getGlobalBounds())))
		{
			if (!bigeaten) 
			{
				declives();                             //CALLING LIVES//
				deathsound.play();
			}
			else 
			{
				G.blinky.setScale(0, 0);
				eatghost.play();
				s.score += 20;
				bigeaten = false;
			}
		}
		else if ((s.player.getGlobalBounds().intersects(G.clyde.getGlobalBounds()))) 
		{
			if (!bigeaten) 
			{
				declives();                             //CALLING LIVES//
				deathsound.play();
			}
			else 
			{
				G.clyde.setScale(0, 0);
				eatghost.play();
				s.score += 20;
				bigeaten = false;
			}
		}
		else if ((s.player.getGlobalBounds().intersects(G.pinky.getGlobalBounds())))
		{
			if (!bigeaten)
			{
				declives();                            //CALLING LIVES//
				deathsound.play();
			}
			else 
			{
				G.pinky.setScale(0, 0);
				eatghost.play();
				s.score += 20;
				bigeaten = false;
			}
		}

		text1.setString("SCORE: " + to_string(s.score));
		text2.setString("LIVES: " + to_string(s.lives));

		s.player.setTextureRect(IntRect(x * 217, 0, 217, 215));
		x %= 2;
		

		//GOHSTS MOVEMENT//
		G.pinky.move(ghosti.x, ghosti.y);
		G.inky.move(ghostb.x, ghostb.y);
		G.blinky.move(ghostp.x, ghostp.y);
		G.clyde.move(ghostc.x, ghostc.y);


		//GOHSTS WITH MAP//
			for (int j = 0; j < 20; j++)
			{
				for (int i = 0; i < 20; i++)
				{
					if (G.pinky.getGlobalBounds().intersects(map[i][j].getGlobalBounds()))
						ghosti = Vector2f(-ghosti.x, 0);
					if (G.inky.getGlobalBounds().intersects(map[i][j].getGlobalBounds()))
						ghostb = Vector2f(0, -ghostb.y);
					if (G.blinky.getGlobalBounds().intersects(map[i][j].getGlobalBounds()))
						ghostp = Vector2f(0, -ghostp.y);
					if (G.clyde.getGlobalBounds().intersects(map[i][j].getGlobalBounds()))
						ghostc = Vector2f(-ghostc.x, 0);
				}
			}

		window.clear();

		if (s.lives == 0)
		{
			window.draw(loss);
		}

		//LEVEL UP//
		else if (s.score == 1810)
		{
			highestscore = 1810;
			myFile.open("pac.txt");
			myFile << highestscore << "\t";
			text3.setString("HIGHEST SCORE: " + to_string(highestscore));
			myFile.close();
			window.draw(complete);
			s.player.setPosition(100, 100);

			if (Mouse::isButtonPressed(Mouse::Left)) 
			{
				s.score = 0;
				for (int i = 0; i < 20; i++)
				{
					for (int j = 0; j < 20; j++)
					{
						dot[i][j].setScale(1, 1);
						window.draw(dot[i][j]);
						window.draw(map[i][j]);
						window.draw(bigdot);
					}
				}
			}
		}
		else if (s.score < 1810)
		{
			for (int i = 0; i < 20; i++)
			{
				for (int j = 0; j < 20; j++)
				{
					window.draw(dot[i][j]);
					window.draw(map[i][j]);
					window.draw(bigdot);

				}
			}
			window.draw(s.player);
			window.draw(G.pinky);
			window.draw(G.inky);
			window.draw(G.blinky);
			window.draw(G.clyde);
			window.draw(bigdot);
			window.draw(text1);
			window.draw(text2);
			window.draw(text3);
		}
		window.display();
	}
	system("pause");
	return 0;
}
void incscore()
{
	s.score += 10;
}
int declives()
{
	s.player.setPosition(100, 110);
	s.lives--;
	return s.lives;
}
void collision(bool canMove, Vector2f lastMove) 
{
	if (!canMove)
		s.player.move(-lastMove.x, -lastMove.y);
}