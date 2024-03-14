#include <iostream>
#include <array>
#include <vector>
#include <string>
#include <stdlib.h> 
#include <Windows.h>
#include "server_connect.h"
using namespace std;

void PROCESS_INIT(int active) {
	system("cls");
	int y, activeA;
	string process_select, term_logo;
	term_logo = "HackRoids~>";
	cout << "g4st 2018*GNU LICENSE\n";
	cout << "exploding servers since 10 years old\n";
	cout << R"(
__          __  _                          
\ \        / / | |                         
 \ \  /\  / /__| | ___ ___  _ __ ___   ___ 
  \ \/  \/ / _ \ |/ __/ _ \| '_ ` _ \ / _ \
   \  /\  /  __/ | (_| (_) | | | | | |  __/
    \/  \/ \___|_|\___\___/|_| |_| |_|\___|
)" << '\n';
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	for (activeA == 1; activeA<y; activeA++, y++) {
		string input_tr;
		SetConsoleTextAttribute(hConsole, 2);
		char commands[8][20] = {"help", "servers", "ip", "hack", "clear", "server_create"};
		cout << term_logo;
		cin >> input_tr;
		 if (input_tr == commands[0]) {
			SetConsoleTextAttribute(hConsole, 3);
			cout << commands[1] << "\n" << commands[2] << "\n" << commands[3] << "\n" << commands[4] << "\n" << commands[5] <<endl;
		} else if (input_tr == commands[1]) {
			SetConsoleTextAttribute(hConsole, 3);
			cout << "Reference to creating server in server_create\n";
		} else if (input_tr == commands[2]) {
			SetConsoleTextAttribute(hConsole, 3);
			cout << "127.0.0.1\n";
		} else if (input_tr == commands[3]) {
			SetConsoleTextAttribute(hConsole, 3);
			cout << "HAXX0R LMAO HACKING NASA 2020\n";
		} else if (input_tr == commands[4]) {
			system("cls");
		} else if (input_tr == commands[5]) {
			SetConsoleTextAttribute(hConsole, 3);
			string name_Server;
			cout << "please input server name=>>";
			cin >> name_Server;
			// add commands to server_connect.h
			ServerCreate(name_Server);
			
		} 
		else {
			SetConsoleTextAttribute(hConsole, 3);
			cout << "command non-existent\n";
		};
	}
}

int main() {
	string start;
	cout << "HACKERSPLOIT TOOL 2018\n";
	cout << "made by g4st(Lk10)\n";
	cout << "start program? y/n\n";
	cin >> start;
	if (start == "y" || start == "Y") {
		int active = 1;
		PROCESS_INIT(active);
	} 
	else if (start == "n" || start == "N") {
		int active = 0;
		cout << "bye!";
	}
	else {
		cout << "BUG!";
	}
	return 0;
}
