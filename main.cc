#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <algorithm>


//http://stackoverflow.com/questions/5897319/how-to-use-stdsort-to-sort-an-array-in-c
template<class Cont>
typename Cont::iterator begin(Cont& c){
  return c.begin();
}

template<class Cont>
typename Cont::iterator end(Cont& c){
  return c.end();
}

// const version
template<class Cont>
typename Cont::const_iterator begin(Cont const& c){
  return c.begin();
}

template<class Cont>
typename Cont::const_iterator end(Cont const& c){
  return c.end();
}

// overloads for C style arrays
template<class T, std::size_t N>
T* begin(T (&arr)[N]){
  return &arr[0];
}

template<class T, std::size_t N>
T* end(T (&arr)[N]){
  return arr + N;
}





using namespace std;
static const int MAX_SIZE = 10000;
int main(){
	struct termios oldt, newt;
	char ch, command[20];
	string userInput;
	
	int cursorPosition = 0;
  
	stringstream ss;
	ifstream infile;

	//Reference: http://stackoverflow.com/questions/5419356/redirect-stdout-stderr-to-a-string  
  
	system("bash -c 'compgen -A function -abck > output'");
	infile.open("output");
	ss << infile.rdbuf() << "\n";
	infile.close();
  
	string array[MAX_SIZE];
	int i = 0;
	while(ss.good() && i < MAX_SIZE){
		ss >> array[i];
		i++;
	}
	sort(begin(array), end(array));
  
  
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	int oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
  
	string word;
	
	while(1){

		ch = getchar(); //Does not block
		if(ch == -1) continue;
		
		if(ch == 27){
		
			stringstream escape;
			escape << "\033";
			ch = getchar();
			escape << ch;
			ch = getchar();
			
			if(ch != 65 && ch != 66){
			
				int diff = word.size() - userInput.size();
				
				if(ch == 67 && diff > 0){
					escape << diff;
					
					//cursorPosition += diff;
				}
				
				
				escape << ch;
				cout << escape.str();
			}
		}
		
		else if (ch == 10) { //If Enter is pressed...
			cout << "\nDone: " << userInput << endl;
			ch = -1;
			break;
		}

		else if(ch == 49){ //If the 1 key is pressed
			userInput = "";
			cout << "\nCleared" << endl;
		}

	else {
		//cout << "\n pressed: "<< int(ch) << endl;
		int size = 10;
		string spaces = string(size, ' ');
		string bspaces = string(size, '\b');
		cout << "\r" << spaces << "\r";
		
		if(ch == 127 && userInput.size() > 0) { //if backspace is pressed
			userInput.erase(userInput.size() - 1);
		}
		
		else userInput += ch;
		
		cout << userInput;
		if (userInput.size() > 0)
			for(int i=0; i < MAX_SIZE; i++){
				word = array[i];

				string prefix(userInput);
				if (!word.compare(0, prefix.size(), prefix)){
					int foo_value = atoi(word.substr(prefix.size()).c_str());
					int diff = word.size() - userInput.size();
					
					if(diff > 0) cout << "\r\033[1;36m" << word << "\r\033[0m" << userInput;
					
					else cout << "\r" << word;
					
					break;
				}
			}
	}
}
  
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	fcntl(STDIN_FILENO, F_SETFL, oldf);
	cout << "Output: " << word << "\n";

	system(word.c_str());
	
	if(ch != EOF){
		ungetc(ch,stdin);
		putchar(ch);
		scanf("%s",command);
		return 1;
  	}
	return 0;
}


  /*
  ifstream infile;
    
  cout << "Enter a command\n";
  stringstream ss;
   	
  char string[MAX_STRING_LENGTH];
   	
  cin >> string;
    
  system(ss.str().c_str());
   
  infile.open("outputfile");
  cout << infile.rdbuf() << "\n";
  infile.close();
       
  return 0;
}*/


















