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
void autocomplete();
void clearLine();
int main(){
	autocomplete();
	return 0;
}

void autocomplete(){
	struct termios oldt, newt;
	char ch, command[256];
	string userInput = "";
	string whole = "";
	string curCommands[MAX_SIZE];
	int commandIndex = 0;
	int cursorPosition = 0;

	stringstream ss;
	ifstream infile;

	//Reference: http://stackoverflow.com/questions/5419356/redirect-stdout-stderr-to-a-string  
  	//Reference: https://superuser.com/questions/977693/how-can-i-make-unix-sort-work-properly-using-the-underscore-as-a-field-separator
  	//Reference: http://www.liamdelahunty.com/tips/linux_remove_duplicate_lines_with_uniq.php
	system("bash -c '(find *; compgen -acdf) | sort | uniq > /home/jason/Desktop/COMP3000/output'");
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

		/*else if(ch == '1'){
			userInput = "";
			cout << "\ncleared\n";
		}*/
		//else 

		else {

			//cout << "\npressed: "<< int(ch) << endl;
		
			//the following lines update the suggestion text
			//int cursorPosition = whole.size() - userInput.size();
			
			clearLine();
			
		
			if(ch == 127) { //if backspace is pressed
				cout << "\r" << whole;
				if (userInput.size() > 0){
					userInput.erase(userInput.size() - 1);
					
				}
				
				if(whole.size() > 0){
					whole.erase(whole.size() - 1);
					
				}
				//cout << "\n\"" <<userInput<<"\""<<endl;
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
			//cout << "\nUserInputSize: " << userInput.size() << endl;
			cout << whole;
		}
	}
  
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	fcntl(STDIN_FILENO, F_SETFL, oldf);
	
	cout << endl;
	system(whole.c_str());
	//cout << "Output: " << whole << endl;
	
	if(ch != EOF){
		ungetc(ch,stdin);
		putchar(ch);
		scanf("%s",command);
		//return 1;
  	}
	//autocomplete();
}
void clearLine(){
	cout << "\33[2K\r";
}


