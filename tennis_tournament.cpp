// TENNIS TOURNAMENT - JAVIER JEREZ REINOSO - GROUP I

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <cmath>		// For the absolute value...
#include <fstream>

using namespace std;

// CONSTANTS
const int MIN_ABILITY = 2;
const int MAX_ABILITY = 4;
const int MIN_SPEED = 2;
const int MAX_SPEED = 4;

const int COURT_WIDTH = 7;
const int COURT_LENGTH = 5;
const int POINTS_TO_WIN = 5;
const int GAMES_TO_WIN = 3;

const int DIM_HIT_ARRAY = COURT_WIDTH + 2;

// ASCII symbols...
const char  VERTICAL = char(179);
const char  HORIZONTAL = char(196);
const char  TOP_LEFT_CORNER = char(218);
const char  TOP_RIGHT_CORNER = char(191);
const char  BOT_LEFT_CORNER = char(192);
const char  BOT_RIGHT_CORNER = char(217);
const char  T_UP = char(193);
const char  T_DOWN = char(194);
const char  CROSS = char(197);

const bool DEBUG_MODE = true;
//const bool DEBUG_MODE = false;

const int DIM_PLAYERS = 10;
const string FileName = "players.txt";

// ENUMRATIONS
typedef enum { NOBODY, PLAYER1, PLAYER2 } tPlayer;
typedef enum { NONE, FIFTEEN, THIRTY, FORTY, ADVANTAGE } tPoints;

// ARRAYS + STRUCTURES
typedef int tHitCounter[DIM_HIT_ARRAY];		// For the statistics...

struct tPlayerData {
	string initials;
	int ability_level;
	int speed_level;
	int games_won = 0;
	int games_lost = 0;
	tPoints points;
	int games = 0;
	int position;
	int win_hits = 0;
	tHitCounter hits = { 0 };
};

typedef tPlayerData tPlayer_Array[DIM_PLAYERS];

struct tPlayerList {
	tPlayer_Array player_array;
	int counter = 0;
};

// PROTOTYPES
int menu();
void welcome();
void load(tPlayerList& playerList);

void display(const tPlayerList& playerList);

void newPlayer(tPlayerList& playerList);
int askData(string ability, int min_value, int max_value);

void selectPlayers(const tPlayerList& playerList, int& p1Idx, int& p2Idx);
int selectPlayer(const tPlayerList& playerList);
int findPlayer(const tPlayerList& playerList, string initials);
void initMatchData(tPlayerData& player);
void playMatch(tPlayerData& player1, tPlayerData& player2);

void selectFinalists(const tPlayerList& playerList, int& p1Idx, int& p2Idx);

void printScore(tPlayerData player1, tPlayerData player2, tPlayer service);
tPlayer updateScore(tPlayer pointWinner, tPlayerData& player1, tPlayerData& player2);
string pointsToStr(tPoints points);

tPlayer playGame(tPlayerData& player1, tPlayerData& player2, tPlayer service);
tPlayer playPoint(tPlayerData& player1, tPlayerData& player2, tPlayer service);
tPlayer playBall(tPlayer hitterPlayer, tPlayerData& hitter, tPlayerData& receiver, int& ballPos);

void displayStats(tPlayerData player);
int hit(int playerPosition, int hitAbility);
int run(int playerPosition, int runAbility, int ballPosition);

void displayCourt(string name1, string name2, int player1Pos, int player2Pos, tPlayer playerBall, int ballPos);
void vertical_borders(int ballPos, tPlayer playerBall);
void horizontal_borders1();
void horizontal_borders2();
void net();

void save(const tPlayerList& playerList);

//***************************************************/ MAIN /************************************************************

int main() {
	tPlayerList playerList;
	int p1Idx, p2Idx;
	int option = -1;													

	if (DEBUG_MODE) {
		srand(0);	// Seed is equal to 0 in DEBUG_MODE = true...
	}
	else {
		srand(time(NULL));
	}

	load(playerList);	// Loading players...

	// SWITCH
	while (option != 0) {
		option = menu();
		switch (option) {
		case 1:
			display(playerList);
			break;
		case 2:		// Asking for new player information...
			newPlayer(playerList);
			break;
		case 3:
			selectPlayers(playerList, p1Idx, p2Idx);
			playMatch(playerList.player_array[p1Idx], playerList.player_array[p2Idx]);	// The match starts...
			break;
		case 4:
			selectFinalists(playerList, p1Idx, p2Idx);
			playMatch(playerList.player_array[p1Idx], playerList.player_array[p2Idx]);	// The match starts...
			break;
		case 0:
			break;
		}
	}
	save(playerList);	// Saving players...

	return 0;
}

//***********************************************/ MENU + TITLE /********************************************************

void load(tPlayerList& playerList) {
	ifstream read_file;
	tPlayerData AuxPlayer;
	int i = 0;

	playerList.counter = 0;
	read_file.open(FileName);

	if (read_file.is_open()) {
		read_file >> AuxPlayer.initials; 

		while ((playerList.counter < DIM_PLAYERS) && (AuxPlayer.initials != "XXX")) {
			read_file >> AuxPlayer.ability_level >> AuxPlayer.speed_level >> AuxPlayer.games_won >> AuxPlayer.games_lost;
			
			playerList.player_array[i] = AuxPlayer;
			playerList.counter++;
			i++;
			read_file >> AuxPlayer.initials;
		}
		read_file.close();
	}
	else {
		cout << endl << "ERROR: File could not be found!" << endl;
	}
}

int menu() {
	int option = -1;
	
	welcome();

	do {
		cout << endl << "<1> Player list" << endl;
		cout << "<2> New player" << endl;
		cout << "<3> New match" << endl;
		cout << "<4> Final" << endl;
		cout << "<0> Exit" << endl;
		cout << "Option: ";
		cin >> option;

		if (option > 4 || option < 0) {
			cout << endl << "ERROR: That is not a valid option!" << endl;
		}

	} while (option > 4 || option < 0);

	return option;
}

void welcome() {
	cout << endl << TOP_LEFT_CORNER;
	for (int i = 0; i < 19; i++) {
		cout << HORIZONTAL;
	}
	cout << TOP_RIGHT_CORNER;

	cout << endl << VERTICAL << " TENNIS TOURNAMENT " << VERTICAL << endl;

	cout << BOT_LEFT_CORNER;
	for (int i = 0; i < 19; i++) {
		cout << HORIZONTAL;
	}
	cout << BOT_RIGHT_CORNER << endl;
}

//****************************************************/ SWITCH /*********************************************************

// CASE 1 - PRINT PLAYERS INFORMATION
void display(const tPlayerList& playerList) {
	cout << endl << setw(10) << "Initials" << setw(10) << "Ability" << setw(10) << "Speed" << setw(10) << "Won" << setw(10) << "Lost" << endl;
	for (int i = 0; i < playerList.counter; i++) {
		cout << setw(10) << playerList.player_array[i].initials << setw(10) << playerList.player_array[i].ability_level
			<< setw(10) << playerList.player_array[i].speed_level << setw(10) << playerList.player_array[i].games_won
			<< setw(10) << playerList.player_array[i].games_lost << endl;
	}
}

// CASE 2 - PLAYERS INFORMATION
void newPlayer(tPlayerList& playerList) {
	int skill = 0, index;
	string initials;
	bool initials_repeated = true;

	if (playerList.counter < DIM_PLAYERS) {
		cout << endl << "Player data..." << endl;
		// ASKING INITIALS...
		while ((initials.size() != 3) || (initials_repeated)) { // If initials are repeated...
			initials_repeated = false;

			cout << "- Initials: ";
			cin >> initials;

			if (initials.size() != 3) {
				cout << setw(2) << endl << "ERROR: There must be exactly 3 initials!" << endl << endl;
			}

			index = findPlayer(playerList, initials);	// We call findPlayer()...

			if (index != -1) {
				initials_repeated = true;
				cout << endl << "ERROR: A player with those initials already exists!" << endl << endl;
			}
		}

		playerList.player_array[playerList.counter].initials = initials;
		// ASKING FOR ABILITY...
		playerList.player_array[playerList.counter].ability_level = askData("Ability", MIN_ABILITY, MAX_ABILITY);
		// ASKING FOR SPEED...
		playerList.player_array[playerList.counter].speed_level = askData("Speed", MIN_SPEED, MAX_SPEED);
		// 0 games...
		playerList.player_array[playerList.counter].games_won = 0;
		playerList.player_array[playerList.counter].games_lost = 0;

		cout << endl << "New player added!" << endl;
		playerList.counter++;
	}
	else {
		cout << endl << "ERROR: There is not enough space for a new player!" << endl;
	}
}

int askData(string ability, int min_value, int max_value) {
	int skill = 0;

	while (skill < min_value || skill > max_value) {
		cout << "- " << ability << " (value between " << min_value << " and " << max_value << ") : ";
		cin >> skill;
		if (skill < min_value || skill > max_value) {
			cout << endl << "ERROR: Invalid value! Please try again... " << endl << endl;
		}
	}

	return skill;
}

// CASE 3 - NEW MATCH
void selectPlayers(const tPlayerList& playerList, int& p1Idx, int& p2Idx) {
	cout << endl << "Select the first player..." << endl;
	p1Idx = selectPlayer(playerList);
	cout << endl << "Select the second player..." << endl;
	do {		// If the player is chosen twice...
		p2Idx = selectPlayer(playerList);
		if (p1Idx == p2Idx) {
			cout << endl << "ERROR: A player cannot be selected twice!" << endl;
		}
	} while (p2Idx == p1Idx);

	cout << endl << "The match begins..." << endl;
}

int selectPlayer(const tPlayerList& playerList) {
	int player_index = -1;
	string initials;

	while (player_index == -1) {

		display(playerList);
		cout << endl << "Please enter the initials of the player: ";
		cin >> initials;

		player_index = findPlayer(playerList, initials);

		if (player_index == -1) {
			cout << endl << "ERROR: There is no player with that name!" << endl;
		}
	}

	return player_index;
}

int findPlayer(const tPlayerList& playerList, string initials) {
	int index = 0;
	bool found = false;

	while ((index < playerList.counter) && (!found)) {
		if (initials == playerList.player_array[index].initials) {
			found = true;
		}
		else {
			index++;
		}
	}

	if (!found) {
		index = -1;
	}

	return index;
}

void initMatchData(tPlayerData& player) {
	for (int i = 0; i < DIM_HIT_ARRAY; i++) {
		player.hits[i] = 0;
	}
	player.games = 0;
	player.win_hits = 0;
	player.points = NONE;
}

void playMatch(tPlayerData& player1, tPlayerData& player2) {
	bool set_winner = false;
	string name_of_match_winner;
	tPlayer server = NOBODY, receiver = NOBODY, pointWinner = NOBODY, gameWinner = NOBODY;

	// We choose randomly the player who serves, between 1-2...
	server = tPlayer(rand() % 2 + 1);

	cin.ignore(80, '\n');

	initMatchData(player1);
	initMatchData(player2);

	while (!set_winner) {
		if (server == PLAYER1) {
			cout << endl << "Service for " << player1.initials << endl;	// Player 1 will serve
			receiver = PLAYER2;
		}
		else {
			cout << endl << "Service for " << player2.initials << endl;	// Player 2 will serve
			receiver = PLAYER1;
		}

		printScore(player1, player2, server);

		gameWinner = playGame(player1, player2, server);

		if (gameWinner == PLAYER1) {
			cout << endl << player1.initials << " wins the game!" << endl;
		}
		else {
			cout << endl << player2.initials << " wins the game!" << endl;
		}

		displayStats(player1);
		displayStats(player2);

		// To win a match there must exist a difference of two games...
		if ((player1.games >= GAMES_TO_WIN) && (player1.games - 2 >= player2.games)) {
			set_winner = true;
			name_of_match_winner = player1.initials;
			player1.games_won++;
			player2.games_lost++;
		}
		else if ((player2.games >= GAMES_TO_WIN) && (player2.games - 2 >= player1.games)) {
			set_winner = true;
			name_of_match_winner = player2.initials;
			player2.games_won++;
			player1.games_lost++;
		}

		// CHANGING PLAYER'S ROLE..
		server = receiver;
		receiver = tPlayer(3 - int(server));
	}

	cout << endl << "Press Enter to continue... ";
	cin.get();

	cout << endl << "    " << player1.initials << setw(3) << player1.games << endl;
	cout << "    " << player2.initials << setw(3) << player2.games << endl;
	cout << endl << name_of_match_winner << " wins the match!" << endl;
}

// CASE 4 - FINAL
void selectFinalists(const tPlayerList& playerList, int& p1Idx, int& p2Idx) {
	int aux1 = -1, aux2 = -1; // We initialize the games_won of each player to -1

	for (int i = 0; i < playerList.counter; i++) {
		if (playerList.player_array[i].games_won > aux1) {	// The games won MUST be strictly bigger (not equal) so as to select the first player [...]
			aux2 = aux1;									// [...] when two or more players have the same score
			aux1 = playerList.player_array[i].games_won;
			p2Idx = p1Idx;
			p1Idx = i;
		}
		else if (playerList.player_array[i].games_won > aux2) {
			aux2 = playerList.player_array[i].games_won;
			p2Idx = i;
		}
	}
}

// SAVE
void save(const tPlayerList& playerList) {
	ofstream write_file;
	int i = 0;

	write_file.open(FileName);	// We do not have to check if it is open (it creates it instead)

	while (i < playerList.counter) {
		write_file << playerList.player_array[i].initials << " " << playerList.player_array[i].ability_level << " " << playerList.player_array[i].speed_level << " "
			<< playerList.player_array[i].games_won << " " << playerList.player_array[i].games_lost << " " << endl;
		i++;
	}
	write_file << "XXX";

	write_file.close();
}

//****************************************************/ SCORE /***********************************************************

string pointsToStr(tPoints points) {
	string score = "";

	if (points == NONE) {
		score = "00";
	}
	else if (points == FIFTEEN) {
		score = "15";
	}
	else if (points == THIRTY) {
		score = "30";
	}
	else if (points == FORTY) {
		score = "40";
	}
	else if (points == ADVANTAGE) {
		score = "Ad";
	}

	return score;
}

tPlayer updateScore(tPlayer pointWinner, tPlayerData& player1, tPlayerData& player2) {
	tPlayer returned_game_winner = NOBODY;

	if (pointWinner == PLAYER1) {
		player1.points = tPoints(int(player1.points) + 1);	// We use a cast in order to add points
	}
	else {
		player2.points = tPoints(int(player2.points) + 1);		// We use a cast in order to add points
	}

	// ALL POSSIBLE SPECIAL POINTS CASES...
	// Bear in mind that POINT_WINNER = 5
	if (player1.points == ADVANTAGE && player2.points == ADVANTAGE) {	// If both players are in Advantage...
		player1.points = FORTY;
		player2.points = FORTY;
	}
	else if (player1.points == ADVANTAGE && player2.points < FORTY) {	// If player1 is Advanatage, he wins if player2 is lower than 40
		returned_game_winner = PLAYER1;
		player1.games++;
		player1.points = NONE;
		player2.points = NONE;
	}
	else if (player1.points < FORTY && player2.points == ADVANTAGE) {	// If player2 is Advantage, he wins if player1 is lower than forty
		returned_game_winner = PLAYER2;
		player2.games++;
		player1.points = NONE;
		player2.points = NONE;
	}
	else if (player1.points == ADVANTAGE && pointWinner == PLAYER2 && player2.points == FORTY) { // If one is in Ad and the other wins with 40
		player1.points = FORTY;
	}
	else if (player2.points == ADVANTAGE && pointWinner == PLAYER1 && player1.points == FORTY) { // If one is in Ad and the other wins with 40
		player2.points = FORTY;
	}
	else if (player1.points == POINTS_TO_WIN) {	// If one player reaches 5 points
		returned_game_winner = PLAYER1;
		player1.games++;
		player1.points = NONE;
		player2.points = NONE;
	}
	else if (player2.points == POINTS_TO_WIN) {	// If one player reaches 5 points
		returned_game_winner = PLAYER2;
		player2.games++;
		player1.points = NONE;
		player2.points = NONE;
	}

	return returned_game_winner;
}

void printScore(tPlayerData player1, tPlayerData player2, tPlayer service) {
	// Player 1...
	cout << endl << setw(7) << player1.initials << setw(3) << player1.games << " : " << pointsToStr(player1.points);
	if (service == PLAYER1) {
		cout << " *";
	}
	// Player 2...
	cout << endl << setw(7) << player2.initials << setw(3) << player2.games << " : " << pointsToStr(player2.points);
	if (service == PLAYER2) {
		cout << " *";
	}
	cout << endl;
}

//*************************************************/ HIT FUNCTION /*******************************************************

int hit(int playerPosition, int hitAbility) {
	int ball_position, distance_to_destination, probability_of_success, random_number, desired_destination, left_or_right;

	desired_destination = rand() % COURT_WIDTH + 1;
	if (DEBUG_MODE) {
		cout << " Desired destination: row " << desired_destination << endl;
	}
	distance_to_destination = abs(playerPosition - desired_destination);

	if (distance_to_destination <= hitAbility) {
		ball_position = desired_destination;			// ball reaches destination
		if (DEBUG_MODE) {
			cout << " Easy hit!" << endl;
		}
	}
	else {
		if (DEBUG_MODE) {
			cout << " Complicated hit... ";
		} 
		
		// GIVEN FORMULA
		probability_of_success = 100 - (double(distance_to_destination - hitAbility) / ((COURT_WIDTH - 1) - MIN_ABILITY)) * 100;
		random_number = rand() % 100;	// n-1 is equal to 100-1, which is equal to 99. Hence, the number is between 0-99
		
		if (DEBUG_MODE) {
			cout << " Success probability: " << probability_of_success << " - " << "Value: " << random_number;
		}

		if (random_number < probability_of_success) {
			if (DEBUG_MODE) {
				cout << " Success!" << endl;
			}
			ball_position = desired_destination;	// ball reaches destination
		}
		else {
			if (DEBUG_MODE) {
				cout << " Not a precise hit!" << endl;	// If not a precise hit, we do not output the Ballrow.
			}
			left_or_right = rand() % 2;			//0 -> left, 1 -> right
			if (left_or_right == 0) {
				ball_position = desired_destination - 1;			// Ball goes left
			}
			else {
				ball_position = desired_destination + 1;			// Ball goes right
			}
		}
	}

	return ball_position;
}

//************************************************/ SPEED FUNCTION /******************************************************

int run(int playerPosition, int runAbility, int ballPosition) {
	int new_player_position = playerPosition, distance_player_ball;

	distance_player_ball = abs(playerPosition - ballPosition);
	if (runAbility >= distance_player_ball) {
		new_player_position = ballPosition;
		if (DEBUG_MODE) {
			cout << " The receiver got the ball" << endl;
		}
	}
	else {
		if (DEBUG_MODE) {
			cout << " The receiver missed the ball" << endl;		//To say the row of each player
		}
		if (ballPosition > playerPosition) {
			new_player_position = playerPosition + runAbility;	//Moves to the right	
		}
		else {
			new_player_position = playerPosition - runAbility;	//Moves to the left
		}
	}

	return new_player_position;
}

//**************************************************/ STATISTICS /********************************************************

void displayStats(tPlayerData player) {
	int total_hits = 0;
	double division = 0;

	cout << endl << "Statistics for " << player.initials << endl;
	for (int i = 0; i < DIM_HIT_ARRAY; i++) {
		total_hits += player.hits[i];
	}
	cout << "   Total hits: " << total_hits << endl;
	cout << "   Winning hits: " << player.win_hits << endl;
	cout << "   Out: " << player.hits[0] + player.hits[DIM_HIT_ARRAY - 1] << endl;		// Out
	cout << "   Hit distribution on the court" << endl;
	cout << setw(10) << "Row ";
	for (int i = 0; i < DIM_HIT_ARRAY; i++) {
		cout << setw(6) << i;
	}
	cout << endl << setw(12) << "% ";
	for (int i = 0; i < DIM_HIT_ARRAY; i++) {
		if (total_hits != 0) {
			division = double(player.hits[i] * 100) / total_hits;		// hit of the row divided by total hits * 100 (%)	---> cast
		}
		else {
			division = 0;
		}
		cout << setw(6) << fixed << setprecision(1) << division;
	}
	cout << endl;
}

//****************************************************/ POINT /***********************************************************

tPlayer playPoint(tPlayerData& player1, tPlayerData& player2, tPlayer service) {
	tPlayer return_point_winner = NOBODY, hitter = NOBODY, receiver = NOBODY, PlayerBall = NOBODY, pointWinner = NOBODY;
	int ball_row;
	bool winner_of_the_point = false;

	player1.position = (COURT_WIDTH / 2 + 1);
	player2.position = (COURT_WIDTH / 2 + 1);
	ball_row = (COURT_WIDTH / 2 + 1);

	if (service == PLAYER1) {
		hitter = PLAYER1;
		receiver = PLAYER2;
		PlayerBall = PLAYER1;
	}
	else {
		hitter = PLAYER2;
		receiver = PLAYER1;
		PlayerBall = PLAYER2;
	}

	// Displaying court...
	displayCourt(player1.initials, player2.initials, player1.position, player2.position, PlayerBall, ball_row);

	cout << endl << "Press Enter to continue... ";
	cin.get();

	while (!winner_of_the_point) {
		if (hitter == PLAYER1) {
			pointWinner = playBall(hitter, player1, player2, ball_row);
		}
		else {
			pointWinner = playBall(hitter, player2, player1, ball_row);
		}
		PlayerBall = tPlayer(3 - int(hitter));

		// Displaying court...
		displayCourt(player1.initials, player2.initials, player1.position, player2.position, PlayerBall, ball_row);

		if (pointWinner != NOBODY) {	// If there is a winner...
			winner_of_the_point = true;
			return_point_winner = pointWinner;
		}

		// CHANGING PLAYER'S ROLE...
		if (!winner_of_the_point) {
			hitter = receiver;	// 1 or 2
			receiver = tPlayer(3 - hitter);		// We use a "cast" to change player's roles...
			PlayerBall = tPlayer(3 - PlayerBall);		// We use a "cast" to change the playerBall... 

			cout << endl << "Press Enter to continue... ";
			cin.get();
		}
	}

	return return_point_winner;
}

tPlayer playBall(tPlayer hitterPlayer, tPlayerData& hitter, tPlayerData& receiver, int& ballPos) {
	tPlayer return_point_winner = NOBODY;

	cout << endl << hitter.initials << " hits..." << endl;
	ballPos = hit(hitter.position, hitter.ability_level);
	hitter.hits[ballPos]++;
	
	if (ballPos < 1 || ballPos >= COURT_WIDTH + 1) {		// BALL OUT!
		if (DEBUG_MODE) {
			cout << " Out!" << endl;
		}
		return_point_winner = tPlayer(3 - int(hitterPlayer));
	}
	else {
		receiver.position = run(receiver.position, receiver.speed_level, ballPos);
		if (receiver.position != ballPos) {
			return_point_winner = hitterPlayer;
			hitter.win_hits++;
		}
	}

	return return_point_winner;
}

//****************************************************/ GAME /************************************************************

tPlayer playGame(tPlayerData& player1, tPlayerData& player2, tPlayer service) {
	tPlayer return_game_winner = NOBODY, pointWinner = NOBODY, gameWinner = NOBODY;
	bool winner_of_the_game = false;

	while (!winner_of_the_game) {
		cout << endl << "Press Enter to continue... ";
		cin.get();

		// WE PLAY A POINT...
		pointWinner = playPoint(player1, player2, service);
		gameWinner = updateScore(pointWinner, player1, player2);

		if (pointWinner == PLAYER1) {
			cout << endl << "Point for " << player1.initials << "!!!" << endl;
		}
		else {
			cout << endl << "Point for " << player2.initials << "!!!" << endl;
		}

		if (gameWinner == PLAYER1) {
			winner_of_the_game = true;
			return_game_winner = PLAYER1;
		}
		else if (gameWinner == PLAYER2) {
			winner_of_the_game = true;
			return_game_winner = PLAYER2;
		}

		if (!winner_of_the_game) {
			printScore(player1, player2, service);
		}
	}

	return return_game_winner;
}

//****************************************************/ COURT /***********************************************************

void displayCourt(string name1, string name2, int player1Pos, int player2Pos, tPlayer playerBall, int ballPos) {
	tPlayer auxiliar = playerBall;

	cout << endl;
	cout << setw((player1Pos + 1) * 2) << name1 << endl;	// We print the name in the court...
	horizontal_borders1();
	if (playerBall != PLAYER1) {
		playerBall = NOBODY;
	}
	vertical_borders(ballPos, playerBall);
	net();
	playerBall = PLAYER2;
	if (auxiliar != PLAYER2) {
		playerBall = NOBODY;
	}
	vertical_borders(ballPos, playerBall);
	horizontal_borders2();
	cout << setw((player2Pos + 1) * 2) << name2 << endl;	// We print the name in the court...
}

// VERTICAL BORDERS
void vertical_borders(int ballPos, tPlayer playerBall) {
	for (int i = 0; i < COURT_LENGTH; i++) {
		for (int j = 0; j < COURT_WIDTH + 1; j++) {

			if ((ballPos == j) && (playerBall == PLAYER1) && (i == 0)) {	// 0 because we print the ball in the first line...
				cout << "o" << VERTICAL;
			}
			else if ((ballPos == j) && (playerBall == PLAYER2) && (i == COURT_LENGTH - 1)) {
				cout << "o" << VERTICAL;
			}
			else {
				cout << " " << VERTICAL;
			}
		}

		if ((ballPos == COURT_WIDTH + 1) && (playerBall == PLAYER1) && (i == 0)) {
			cout << "o";
		}
		else if ((ballPos == COURT_WIDTH + 1) && (playerBall == PLAYER2) && (i == COURT_LENGTH - 1)) {
			cout << "o";
		}
		cout << endl;
	}
}

// HORIZONTAL TOP
void horizontal_borders1() {

	cout << " " << TOP_LEFT_CORNER;
	for (int i = 0; i < COURT_WIDTH; i++) {
		cout << HORIZONTAL;
		if (i < COURT_WIDTH - 1) {
			cout << T_DOWN;
		}
	}
	cout << TOP_RIGHT_CORNER << endl;
}

// HORIZONTAL BOT
void horizontal_borders2() {
	cout << " " << BOT_LEFT_CORNER;
	for (int i = 0; i < COURT_WIDTH; i++) {
		cout << HORIZONTAL;
		if (i < COURT_WIDTH - 1) {
			cout << T_UP;
		}
	}
	cout << BOT_RIGHT_CORNER << endl;
}

// NET
void net() {
	int position = 1;

	cout << HORIZONTAL;
	for (int i = 0; i < COURT_WIDTH; i++) {
		cout << CROSS << position;
		position++;
	}
	cout << CROSS << HORIZONTAL;
	cout << endl;
}

/* WITH THE FORMULA: setw((playerPos + 1) * 2), THIS CAN BE SUBSTITUDE:

if (playerPos == 1) {
		cout << setw(4) << name << endl;
	}
	else if (playerPos == 2) {
		cout << setw(6) << name << endl;
	}
	else if (playerPos == 3) {
		cout << setw(8) << name << endl;
	}
	else if (playerPos == 4) {
		cout << setw(10) << name << endl;
	}
	else if (playerPos == 5) {
		cout << setw(12) << name << endl;
	}
	else if (playerPos == 6) {
		cout << setw(14) << name << endl;
	}
	else if (playerPos == 7) {
		cout << setw(16) << name << endl;
	}
	else if (playerPos == 8) {
		cout << setw(18) << name << endl;
	}
	else if (playerPos == 9) {
		cout << setw(20) << name << endl;
	}
*/