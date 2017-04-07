#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

using namespace std;
static const int MAX_SIZE = 10000;

void clearLine(){
	cout << "\33[2K\r";
}

void autocomplete(){
	struct termios oldt, newt;
	char ch, command[256];
	
	string userInput = "";
	string whole = "";
	string curCommands[MAX_SIZE];
	
	int commandIndex = 0;

	stringstream ss;
	ifstream infile;

	//Reference: http://www.liamdelahunty.com/tips/linux_remove_duplicate_lines_with_uniq.php
	system("bash -c '(find *; compgen -cdf) | sort | uniq > /home/jason/Desktop/COMP3000/output'");
	infile.open("/home/jason/Desktop/COMP3000/output");
	ss << infile.rdbuf() << "\n";
	infile.close();

	string array[MAX_SIZE];
	int i = 0;
	while(ss.good() && i < MAX_SIZE){
		ss >> array[i];
		i++;
	}

	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	int oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
  
	string word;
	cout << "Enter a command\n";
	while(1){
	
		ch = getchar(); //Does not block
		if(ch == -1) continue;
		
		
		if(ch == 27){ //if a 'special' character is entered...
		
			stringstream escape;
			escape << "\033";
			ch = getchar();
			escape << ch;
			ch = getchar();
			
			if(ch != 65 && ch != 66){
			
				int diff = word.size() - userInput.size();
				
				if(ch == 67 && diff > 0){
					escape << diff;
					whole.erase(whole.size() - userInput.size());
					userInput = word;
					whole += word;
				}
				
				
				escape << ch;
				cout << escape.str();
			}
		}
		
		else if (ch == 10) { //If Enter is pressed...
			if (word.size() > userInput.size()) {
				whole.erase(whole.size() - userInput.size());
				userInput = word;
				whole += word;
			}
			//cout << "\nDone: " << whole << endl;
			ch = -1;
			
			break;
		}


		else {

		
			//the following lines update the suggestion text
			
			clearLine();
			
		
			if(ch == 127) { //if backspace is pressed
				if (userInput.size() > 0){
					userInput.erase(userInput.size() - 1);
					
				}
				
				if(whole.size() > 0){
					whole.erase(whole.size() - 1);
					
				}
				if (userInput.size() == 0){
					if(commandIndex > 0){
						commandIndex--;
						userInput = curCommands[commandIndex];
						curCommands[commandIndex] = "";
					}
					if(commandIndex == 0){
						word = "";
					}
					
					
				}
				
			}
			else {
				userInput += ch;
				whole += ch;
				
				if(ch == '|' || ch == ' '){

					curCommands[commandIndex] = userInput;
					userInput = "";
					commandIndex++;
			
				}
				
				
				if(userInput[0] == ' '){
					userInput.erase(0,1);
					whole.erase(whole.size() - userInput.size(), whole.size() - (userInput.size() - 1));
				}
			}
		
		
			if (userInput.size() > 0){
				for(int i = 0; i < MAX_SIZE; i++){
					word = array[i];

					if (word.compare(0, userInput.size(), userInput) == 0){ //if the user's input matches a program...
					
						int cursorPosition = whole.size() - userInput.size();
						if(cursorPosition > 0)
							cout << "\r\033["<< cursorPosition <<"C\033[1;36m" << word << "\033[0m\r";
						else
							cout << "\r\033[1;36m" << word << "\033[0m\r";
						
						break;
					}
				}
			}
			else{
				clearLine();
			}
			cout << whole;
		}
	}
  
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	fcntl(STDIN_FILENO, F_SETFL, oldf);
	
	cout << endl;
	system(whole.c_str());
	
	if(ch != EOF){
		ungetc(ch,stdin);
		putchar(ch);
		scanf("%s",command);
  	}
	//autocomplete(); //for looping forever
}

int main(){
	autocomplete();
	return 0;
}



