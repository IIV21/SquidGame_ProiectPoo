#include <string>
#include <iostream>
#include <vector>
#include <math.h>
#include <cstdlib>
#include <time.h>
#include <random>
#include <fstream>
#include <stdlib.h>

using namespace std;

int totalMoney = 0;

class User {
protected:
	string firstname, lastname, city;
	int moneyDet, weight, number, moneyEarned = 0;

public:
	virtual void print() = 0;
};

class Player : public User {
public:
	Player(string fn, string ln, string c, int md, int w, int nr);
	Player();
	void print() {
		cout << "TAG:" << number << " " << lastname << " " << firstname << " "
			<< city << " " << moneyDet << "$" << " " << weight << "Kg" << endl;
	};
	int getNumber() {
		return number;
	}
	int getMoneyDet() {
		return moneyDet;
	}
	int getWeight() {
		return weight;
	}
	void addEarnedMoney(int m) {
		moneyEarned = m;
	}
	int getEarnedMoney() {
		return moneyEarned;
	}

};

class Supervizor : public User {
public:
	string shape;
	Supervizor(string fn, string ln, string c, int md, int w, int nr, string sh);
	void print() {
		cout << "TAG:" << number << " " << lastname << " " << firstname << " "
			<< city << " " << moneyDet << "$" << " " << moneyEarned << "$" << " " << weight << "Kg" << "  Shape      "
			<< shape << endl << endl;
	}
	Supervizor();

	int getNumber() {
		return number;
	}
	int getMoneyDet() {
		return moneyDet;
	}
	int getWeight() {
		return weight;
	}
	int getMoneyEarned() {
		return moneyEarned;
	}
	void addEarnedMoney(int m) {
		moneyEarned += m;
	}
	void substractDet(int m) {
		moneyEarned -= m;
	}
};

Supervizor::Supervizor() {}

Supervizor::Supervizor(string fn, string ln, string c, int md, int w, int nr, string sh) {
	this->firstname = fn;
	this->lastname = ln;
	this->city = c;
	this->moneyDet = md;
	this->weight = w;
	this->number = nr;
	this->shape = sh;
}


Player::Player(string fn, string ln, string c, int md, int w, int nr) {
	this->firstname = fn;
	this->lastname = ln;
	this->city = c;
	this->moneyDet = md;
	this->weight = w;
	this->number = nr;
}
Player::Player() {}


//int moneyDetRand() { srand(time(NULL));	int x = rand() % 89999 + 10001;	return x; }
//
//int weightRand() { srand(time(NULL));  int x = rand() % 49 + 51;	return x; }

void buildGroupVector(int g[][12]) {
	int counter = 1;
	for (int i = 0; i < 9; i++)
		g[i][0] = i + 1;    // pe prima coloana vom avea numarul corespunzator supraveghetorilor
	for (int i = 0; i < 9; i++) // pe restul vom avea de la 1 la 99 numarul concurentilor
		for (int j = 1; j < 12; j++)
		{
			g[i][j] = counter;
			counter++;
		}
}

void getUsers(vector<Player>& players, vector<Supervizor>& supervizors) {
	int i = 1;
	ifstream f;
	f.open("players.txt");
	while (players.size() != 99) {//!f.eof()
		string a, b, c;
		f >> a >> b >> c;
		Player p(a, b, c, rand() % 89999 + 10001, rand() % 49 + 51, i);//////////////CREEAREA CONCURENTILOR 1-99
		i++;
		players.push_back(p);
	}
	i = 0;
	while (supervizors.size() != 9) {//!f.eof()
		string a, b, c;
		f >> a >> b >> c;
		i++;

		if (i == 1 || i == 2 || i == 3)
		{
			Supervizor p(a, b, c, rand() % 89999 + 10001, rand() % 49 + 51, i, "circle");
			supervizors.push_back(p);
		}

		else if (i == 4 || i == 5 || i == 6)
		{
			Supervizor p(a, b, c, rand() % 89999 + 10001, rand() % 49 + 51, i, "triangle");
			supervizors.push_back(p);
		}

		else if (i == 7 || i == 8 || i == 9)
		{
			Supervizor p(a, b, c, rand() % 89999 + 10001, rand() % 49 + 51, i, "square");
			supervizors.push_back(p);
		}
	}


}

void RedLightGreenLight(vector<Player>& p, int g[][12], vector<Supervizor>& s) {
	int x = 0;
	for (int i = 0; i < 9; i++)
	{
		for (int j = 1; j < 12; j++)
			if (p[g[i][j] - 1].getNumber() % 2 == 0)
			{
				totalMoney += p[i * 10 + j - 1].getMoneyDet();
				s[i].addEarnedMoney(p[i * 10 + j - 1].getMoneyDet());
				g[i][j] = 0;
			}
	}

	for (int i = 1; i < p.size(); i++)
	{
		p.erase(p.begin() + i);
	}

}
void removeLoosingTeam(int groupNumber, vector<Player>& p, int g[][12], vector<Supervizor>& s, Player* m[4][12])
{
	for (Player* player : m[groupNumber])
	{
		for (int i = 0; i < 9; i++)
			for (int j = 1; j < 12; j++)
				if (player->getNumber() == g[i][j])
				{
					s[i].addEarnedMoney(player->getMoneyDet());
					totalMoney += player->getMoneyDet();
					int index = -1;
					for (int k = 0; k < p.size(); k++)
					{
						if (p[k].getNumber() == player->getNumber())
							index = k;
					}
					p.erase(p.begin() + index - 1);
				}
	}
}
void removeLoosingPlayer(vector<Player>& p, int g[][12], vector<Supervizor>& s, Player player)
{
	for (int i = 0; i < 9; i++)
		for (int j = 1; j < 12; j++)
			if (player.getNumber() == g[i][j])
			{
				s[i].addEarnedMoney(player.getMoneyDet());
				totalMoney += player.getMoneyDet();
				int index = -1;
				for (int k = 0; k < p.size(); k++)
				{
					if (p[k].getNumber() == player.getNumber())
						index = k;
				}
				p.erase(p.begin() + index);
			}
}
void ThugOfWar(vector<Player>& p, int g[][12], vector<Supervizor>& s)
{//group A is from 0 to 11, B from 12 to 23, C from 24 to 35, D from 36 to 47( the tags are the index of the vector +1)
	// player 48 and 49 will automatically go to the next round

	Player* m[4][12];
	int i = 0, j = 0, weightVector[4] = { 0 }, winner1, winner2;

	for (Player player : p)
	{
		if (j == 12)
		{
			i++;
			j = 0;
		}
		if (i == 4)
			break;

		m[i][j] = &player;
		j++;
	}

	for (int i = 0; i < 4; i++)
		for (Player* player : m[i])
			weightVector[i] += player->getWeight();

	if (weightVector[0] > weightVector[1])
	{
		removeLoosingTeam(1, p, g, s, m);
		winner1 = 0;
	}
	else
	{
		removeLoosingTeam(0, p, g, s, m);
		winner1 = 1;
	}

	if (weightVector[2] > weightVector[3])
	{
		removeLoosingTeam(3, p, g, s, m);
		winner2 = 2;
	}
	else
	{
		removeLoosingTeam(2, p, g, s, m);
		winner2 = 3;
	}

	if (weightVector[winner1] > weightVector[winner2])
	{
		removeLoosingTeam(winner2, p, g, s, m);
	}
	else
	{
		removeLoosingTeam(winner1, p, g, s, m);
	}

}

void Marbles(vector<Player>& p, int g[][12], vector<Supervizor>& s)
{
	for (int i = 0; i < p.size(); i++) //the players play first with second, third with fourth etc.
	{
		int x, y;
		x = rand() % 100;
		y = rand() % 100;
		if (x < y)
		{
			removeLoosingPlayer(p, g, s, p[i + 1]);
		}
		else
		{
			removeLoosingPlayer(p, g, s, p[i]);
		}
	}

}
int rockPaperScissors(int x, int y)
{

	if (x == 1 && y == 2) {
		return 2;
	}
	else if (x == 2 && y == 3) {
		return 2;
	}
	else if (x == 3 && y == 1) {
		return 2;
	}
	else if (x == 1 && y == 3) {
		return 1;
	}
	else if (x == 2 && y == 1) {
		return 1;
	}
	else if (x == 3 && y == 2) {
		return 1;
	}
	else {
		return 3;
	}
}

void Genken(vector<Player>& p, int g[][12], vector<Supervizor>& s)
{
	int x, y;
	for (int j = 0; j < p.size() - 1; j++)
		for (int i = 0; i < p.size() - 1; i++)//the players play first with second, third with fourth etc.
		{
		startingLabel: {}
			x = rand() % 3 + 1, y = rand() % 3 + 1;
			if (rockPaperScissors(x, y) == 1) {
				removeLoosingPlayer(p, g, s, p[i + 1]);
			}
			else if (rockPaperScissors(x, y) == 2) {
				removeLoosingPlayer(p, g, s, p[i]);
			}
			else goto startingLabel; // play again if they are equal.
		}

startingLabel2: {}
	x = rand() % 3 + 1, y = rand() % 3 + 1;
	cout << x << " " << y << endl;
	if (rockPaperScissors(x, y) == 1) {
		removeLoosingPlayer(p, g, s, p[1]);
	}
	else if (rockPaperScissors(x, y) == 2) {
		removeLoosingPlayer(p, g, s, p[0]);
	}
	else goto startingLabel2;

}

int main()
{
	vector<Supervizor> supervizors;
	vector<Player> players;
	ifstream f;
	getUsers(players, supervizors);
	srand(time(NULL));


	int groupVector[9][12];
	buildGroupVector(groupVector);

	for (int i = 0; i < 9; i++) // afisarea matricei asociate userilor.
	{
		for (int j = 0; j < 12; j++)
			cout << groupVector[i][j] << " ";
		cout << endl;
	}

	for (int i = 0; i < 99; i++)
	{
		cout << i << " ";
		players[i].print();
	}

	for (Supervizor s : supervizors)
		s.print();


	cout << "<<<<<<<<<<<<<<<<<LET THE GAMES BEGIN>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << endl;

	cout << " <<< Red Light Green Light >>>" << endl;

	RedLightGreenLight(players, groupVector, supervizors);

	cout << " <<< Remaining players: >>>" << endl;

	for (Player i : players)
		i.print();

	cout << "<Remaining players> : " << players.size() << endl;
	cout << "<The Pot> : " << totalMoney << "$" << endl;

	ThugOfWar(players, groupVector, supervizors);

	cout << endl;
	cout << endl;
	for (Player i : players)
		i.print();

	cout << "<Remaining players> : " << players.size() << endl;
	cout << "<The Pot> : " << totalMoney << "$" << endl;

	Marbles(players, groupVector, supervizors);
	for (Player i : players)
		i.print();
	cout << "<Remaining players> : " << players.size() << endl;
	cout << "<The Pot> : " << totalMoney << "$" << endl;

	Genken(players, groupVector, supervizors);

	for (Player i : players)
		i.print();
	cout << "<Remaining players> : " << players.size() << endl;
	cout << "<The Pot> : " << totalMoney << "$" << endl;

	for (int i = 0; i < 9; i++)
		for (int j = 1; j < 12; j++)
			if (players[0].getNumber() == groupVector[i][j])
				supervizors[i].addEarnedMoney(supervizors[i].getMoneyDet() * 10);; //Supraveghetorul care îl are pe câștigător va primi datoria lui înmulțită cu 10

	for (Supervizor sup : supervizors)
	{
		sup.substractDet(sup.getMoneyDet());
	}
	cout << endl;

	for (int i = 0; i < supervizors.size() - 1; i++)
		for (int j = 0; j < supervizors.size() - 1; j++)
		{
			if (supervizors[j].getMoneyEarned() < supervizors[j + 1].getMoneyEarned())
			{
				Supervizor aux;
				aux = supervizors[j];
				supervizors[j] = supervizors[j + 1];
				supervizors[j + 1] = aux;
			}
		}

	players[0].addEarnedMoney(totalMoney);
	cout <<"<<<< THE WINNER HAS JUST EARNED >>>>: "<< players[0].getEarnedMoney()<<"$" << endl << endl;

	for (Supervizor s : supervizors)
		s.print();
	int max = 0,poz=0;
	for (int i = 0; i < 9; i++)
		if (max > supervizors[i].getMoneyEarned())
		{
			max = supervizors[i].getMoneyEarned();
			poz = i;
		}
	cout << "The supervizor team that earned the most money is " << poz+1;
}


