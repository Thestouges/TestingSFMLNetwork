#include "pSocket.h"

pSocket::pSocket(){
	run();
}

void pSocket::run(){
	initValues();
	connect();

	if(ready){
		window.create(sf::VideoMode(windowx,windowy), "Testing Network");
		while(window.isOpen()){
			while(window.pollEvent(Event)){
				if(Event.type == sf::Event::Closed)
					window.close();
			}
			window.clear(sf::Color::Black);
			game();
			window.display();
		}
	}
}

void pSocket::initValues(){
	windowx = 800;
	windowy = 600;
	initwindowx = windowx/3;
	initwindowy = windowy/3;
	ready = false;
	timeout = sf::seconds(60);
	stackamount = 20;
	counter = 0;
	rectsize = 4;

	if (!font.loadFromFile("times.ttf")){
		// error...
	}

	initText.setFont(font);

	initializeBoard();
}

void pSocket::connect(){
	initwindow.create(sf::VideoMode(initwindowx, initwindowy), "Testing Network");
	while(initwindow.isOpen() && !ready){
		while(initwindow.pollEvent(Event)){
			if(Event.type == sf::Event::Closed)
				initwindow.close();
		}
		initwindow.clear(sf::Color::Black);
		userSetting();
		initwindow.display();
	}

	if(ready){
		initwindow.close();
		aquireInfo();
	}
}

void pSocket::initializeBoard(){
	gridsize = 100;
	grid = new int*[gridsize];
	for(int i = 0; i < gridsize; i++){
		grid[i] = new int[gridsize];
	}

	for(int i = 0; i < gridsize; i++)
		for(int j = 0; j < gridsize; j++)
			grid[i][j] = 0;

	grid[0][0] = 1;
	grid[gridsize-1][gridsize-1] = 2;
}

void pSocket::userSetting(){
	optionRect.setSize(sf::Vector2f(initwindowx/2, initwindowy/2));
	optionRect.setPosition(0,0);
	optionRect.setFillColor(sf::Color::Red);
	initwindow.draw(optionRect);

	optionRect.setPosition(0,initwindowy/2);
	optionRect.setFillColor(sf::Color::Blue);
	initwindow.draw(optionRect);

	initText.setString("Server");
	initText.setCharacterSize(16);
	initText.setFillColor(sf::Color::Green);
	initText.setPosition(initwindowx/6, initwindowy/6);
	initwindow.draw(initText);

	initText.setString("Client");
	initText.setPosition(initwindowx/6, 4*initwindowy/6);
	initwindow.draw(initText);

	mousePos = mouse.getPosition(initwindow);
	initText.setString(std::to_string(mousePos.x)+" "+std::to_string(mousePos.y));
	initText.setPosition(0,0);
	initwindow.draw(initText);

	initText.setString(std::to_string(initwindowx)+" "+std::to_string(initwindowy));
	initText.setPosition(initwindowx/2,0);
	initwindow.draw(initText);

	if(mouse.isButtonPressed(sf::Mouse::Left)){
		if((mousePos.x >= 0 && mousePos.x <= initwindowx/2) && (mousePos.y >= 0 && mousePos.y <= initwindowx/2)){
			connectionType = 's';
			ready = true;
		}
		else if((mousePos.x >= 0 && mousePos.x <= initwindowx/2) && (mousePos.y >= initwindowx/2 && mousePos.y <= initwindowx)){
			connectionType = 'c';
			ready = true;
		}
	}
}

void pSocket::aquireInfo(){
	std::cout<<"Enter port: ";
	std::cin>>port;

	text = "Connect to: ";

	if(connectionType == 'c'){
		playerPos = sf::Vector2i(gridsize-1,gridsize-1);
		enemyPos = sf::Vector2i(0,0);
		std::cout<<"Enter IP: ";
		std::cin>>ipAddress;
		ip = sf::IpAddress::IpAddress(ipAddress);
		socket.connect(ip,port,timeout);
		text += "Server";
	}
	else if(connectionType == 's'){
		playerPos = sf::Vector2i(0,0);
		enemyPos = sf::Vector2i(gridsize-1,gridsize-1);
		listener.listen(port);
		listener.accept(socket);
		text += "Client";
	}

	socket.setBlocking(false);
}

void pSocket::game(){
	movement();
	drawboard();
	sf::sleep(sf::seconds(.1));
}

void pSocket::drawboard(){
	gridposition.x = 0;
	gridposition.y = 0;
	gridRect.setSize(sf::Vector2f::Vector2(rectsize,rectsize));
	for(int i = 0; i < gridsize; i++){
		for(int j = 0; j < gridsize; j++){
			if(playerPos.x == i && playerPos.y == j)
				if(connectionType == 's')
					gridRect.setFillColor(sf::Color::Green);
				else
					gridRect.setFillColor(sf::Color::Magenta);
			else if(grid[i][j] == 0){
				gridRect.setFillColor(sf::Color::White);
			}
			else if(grid[i][j] == 1){
				gridRect.setFillColor(sf::Color::Red);
			}
			else if(grid[i][j] == 2){
				gridRect.setFillColor(sf::Color::Blue);
			}
			gridRect.setPosition(gridposition.x,gridposition.y);
			gridposition.y = gridposition.y + rectsize + 1;
			window.draw(gridRect);
		}
		gridposition.y = 0;
		gridposition.x = gridposition.x + rectsize + 1;
	}
}

void pSocket::movement(){
	prevPosition = playerPos;
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && playerPos.x < gridsize-1){
		playerPos = sf::Vector2i::Vector2(playerPos.x+1,playerPos.y);
		initText.setString("Right");
	}
	else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && playerPos.x > 0){
		playerPos = sf::Vector2i::Vector2(playerPos.x-1,playerPos.y);
		initText.setString("Left");
	}
	else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && playerPos.y > 0){
		playerPos = sf::Vector2i::Vector2(playerPos.x,playerPos.y-1);
		initText.setString("Up");
	}
	else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && playerPos.y < gridsize-1){
		playerPos = sf::Vector2i::Vector2(playerPos.x,playerPos.y+1);
		initText.setString("Down");
	}

	initText.setPosition(5*windowx/6,windowy/4);
	window.draw(initText);

	if(connectionType == 's')
		grid[playerPos.x][playerPos.y] = 1;
	else
		grid[playerPos.x][playerPos.y] = 2;

	if(playerPos != prevPosition){
		packet<<playerPos.x<<playerPos.y;
		socket.send(packet);
	}

	initText.setString(std::to_string(playerPos.x)+" "+std::to_string(playerPos.y));
	initText.setPosition(5*windowx/6,0);
	window.draw(initText);

	socket.receive(packet);
	if(packet>>enemyPos.x>>enemyPos.y)
		if(connectionType == 's')
			grid[enemyPos.x][enemyPos.y] = 2;
		else
			grid[enemyPos.x][enemyPos.y] = 1;
}

/*
void pSocket::tutorial(){
	ip = sf::IpAddress::getLocalAddress();
	rect1.setSize(sf::Vector2f(20,20));
	rect1.setFillColor(sf::Color::Red);
	rect2.setSize(sf::Vector2f(20,20));
	rect2.setFillColor(sf::Color::Blue);
	window.create(sf::VideoMode(800,600,32), "Packets");
	update = false;

	std::cout<<"S for server, C for client: ";
	std::cin>>connectionType;

	if(connectionType == 's'){
		listener.listen(2000);
		listener.accept(socket);
	}
	else 
		socket.connect(ip,2000);

	socket.setBlocking(false);

	while(window.isOpen()){
		while(window.pollEvent(Event)){
			if(Event.type == sf::Event::Closed || Event.key.code == sf::Keyboard::Escape)
				window.close();
			else if(Event.type == sf::Event::GainedFocus)
				update = true;
			else if(Event.type == sf::Event::LostFocus)
				update = false;
		}

		prevPosition = rect1.getPosition();

		if(update){
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			rect1.move(.1, 0);
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			rect1.move(-.1, 0);
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			rect1.move(0, -.1);
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			rect1.move(0, .1);
		}
		if(prevPosition != rect1.getPosition()){
			packet<<rect1.getPosition().x<<rect1.getPosition().y;
			socket.send(packet);

		}

		socket.receive(packet);
		if(packet>>p2Position.x>>p2Position.y)
			rect2.setPosition(p2Position);

		window.draw(rect1);
		window.draw(rect2);

		window.display();
		window.clear();
	}
}
*/