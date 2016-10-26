#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include <string>
#include <iomanip>
#include <cctype>
#include <iostream>

class pSocket{
public:
	pSocket();
	void run();
	void connect();
	void initValues();
	void userSetting();
	void aquireInfo();
	void initializeBoard();
	void game();
	void drawboard();
	void movement();
	void tutorial();
private:
	sf::Event Event;
	sf::TcpSocket socket;
	sf::TcpListener listener;
	sf::IpAddress ip;
	sf::Packet packet;
	std::string ipAddress;
	sf::RenderWindow initwindow;
	sf::RenderWindow window;
	sf::Font font;
	sf::Text initText;
	float initwindowx;
	float initwindowy;
	float windowx;
	float windowy;
	bool ready;
	sf::Mouse mouse;
	sf::Vector2i mousePos;
	sf::RectangleShape optionRect;
	char connectionType;
	unsigned short port;
	sf::Time timeout;
	std::string text;
	int stackamount;
	int counter;
	int gridsize;
	int **grid;
	sf::Vector2i gridposition;
	float rectsize;
	sf::RectangleShape gridRect;
	sf::Vector2i playerPos;
	sf::Vector2i prevPosition;
	sf::Vector2i enemyPos;
};