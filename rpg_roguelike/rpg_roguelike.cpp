
#include <iostream>
#include <vector>
#include <string>
#include <conio.h>
#include <ctime>
#include <windows.h>

const int gamezone_widght = 20;
const int gamezone_hight = 20;

class Map {
private:
	std::vector<std::string> gamezone;

public:

	Map() {
		gamezone.resize(gamezone_hight);
		for (int j = 0; j < gamezone_hight; j++) {
			gamezone[j].resize(gamezone_widght, ' ');
		}
	}

	void Generate() {
		for (int i = 0; i < gamezone_widght; i++) {
			for (int j = 0; j < gamezone_hight; j++) {
				if (i == 0 || i == gamezone_widght - 1 || j == 0 || j == gamezone_hight - 1) {
					gamezone[i][j] = '#';
				}
				else {
					int gold_random_value = rand() % 20;
					int random_value = rand() % 10;
					if (random_value == 0) {
						gamezone[i][j] = 'S';
					}
					else if (gold_random_value == 2) {
						gamezone[i][j] = 'G';
					}
					else {
						gamezone[i][j] = ' ';
					}
				}
			}
		}
		gamezone[1][1] = ' ';
	}

	char Getchar(int x, int y) {
		return gamezone[y][x];
	}

	void SetChar(int x, int y, char c) {
		gamezone[y][x] = c;
	}

	void Draw() {

	}

};

class Hero;

class Enemy {
private:
	int x;
	int y;
	int healthpoint = 20;

public:
	Enemy(int enemy_x, int enemy_y) {
		x = enemy_x;
		y = enemy_y;
	}

	int getenemy_X() {
		return x;
	}

	int getenemy_Y() {
		return y;
	}

	int getenemy_HP() {
		return healthpoint;
	}


	void Move(Map& map, Hero& hero);

	void EnemyTakeDamage() {
		healthpoint -= 10;
		if (healthpoint < 0) {
			healthpoint = 0;
		}
	}

	void EnemyStatus() {
		std::cout << "\nEnemy Healthpoint: " << healthpoint << std::endl;
	}
};

class Hero {
private:
	int x = 1;
	int y = 1;
	int healtpoint = 100;
	int score = 0;
	std::string name;

public:

	int getX() {
		return x;
	}

	int getY() {
		return y;
	}

	void Input(char key, Map& map, std::vector<Enemy>& enemies) {

		int Next_x = x;
		int Next_y = y;

		if (key == 'w') {
			Next_y--;
		}
		else if (key == 's') {
			Next_y++;
		}
		else if (key == 'a') {
			Next_x--;
		}
		else if (key == 'd') {
			Next_x++;
		}
		if (map.Getchar(Next_x, Next_y) == '#' || map.Getchar(Next_x, Next_y) == 'S') {
			return;
		}
			for (int k = 0; k < enemies.size(); k++) {
				if (Next_x == enemies[k].getenemy_X() && Next_y == enemies[k].getenemy_Y()) {
					enemies[k].EnemyTakeDamage();
					return;
				}
			}

			if (map.Getchar(Next_x, Next_y) == 'G') {
				score += 10;
				map.SetChar(Next_x, Next_y, ' ');
			}

			x = Next_x;
			y = Next_y;
		}

	int getHP() {

		return healtpoint;
	}


	void TakeDamage() {
		healtpoint -= 10;
		if (healtpoint < 0) {
			healtpoint = 0;
		}
	}

	void Status() {
		std::cout << "Healthpoint: " << healtpoint << std::endl;
		std::cout << "Gold: " << score << std::endl;


	}

};

void Enemy::Move(Map& map, Hero& hero) {

	int Next_X = x;
	int Next_Y = y;

	int dir = rand() % 4;

	if (dir == 0) {
		Next_Y--;
	}
	else if (dir == 1) {
		Next_Y++;
	}
	else if (dir == 2) {
		Next_X--;
	}
	else if (dir == 3) {
		Next_X++;
	}

	if (map.Getchar(Next_X, Next_Y) != '#' && map.Getchar(Next_X, Next_Y) != 'S') {

		if (Next_X == hero.getX() && Next_Y == hero.getY()) {
			hero.TakeDamage();
		}

		x = Next_X;
		y = Next_Y;
	}
}



int main()
{
	srand(time(0));

	Map dungeon;
	dungeon.Generate();

	Hero player;

	std::vector<Enemy> enemies;
	

	for (int i = 0; i < 1; i++) {
		int enemy_x = rand() % gamezone_widght - 2;
		int enemy_y = rand() % gamezone_hight - 2;

		enemies.push_back(Enemy(enemy_x, enemy_y));
	}

	int Timer = 0;

	while (true) {
		system("cls");

		for (int j = 0; j < gamezone_hight; j++) {
			for (int i = 0; i < gamezone_widght; i++) {
				if (i == player.getX() && j == player.getY()) {
					std::cout << "@";
				}
				else {
					bool isEnemyHere = false;

					for (int k = 0; k < enemies.size(); k++) {
						if (enemies[k].getenemy_X() == i && enemies[k].getenemy_Y() == j) {
							std::cout << "E";
							isEnemyHere = true;
							break;
						}
					}
					if (!isEnemyHere) {
						std::cout << dungeon.Getchar(i, j);
					}
				}
			}
			std::cout << std::endl;
		}

		if (_kbhit()) {
			char key = _getch();
			player.Input(key, dungeon, enemies);
		}
		Timer++;
		if (Timer > 2) {
			for (int k = 0; k < enemies.size(); k++) {
				enemies[k].Move(dungeon, player);
			}

			Timer = 0;
		}

		for (int k = 0; k < enemies.size(); k++) {
			if (enemies[k].getenemy_HP() <= 0) {
				enemies.erase(enemies.begin() +k);
				break;
			}
		}

		player.Status();
		
		if (player.getHP() <= 0) {
			std::cout << "\nYou die..." << std::endl;
			_getch();
			break;
		}

		if (enemies.empty()) {
			std::cout << "Enemy die... You win!" << std::endl;
			_getch();
			break;
		}

		Sleep(50);
	}

	return 0;
}