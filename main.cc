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
  string output;
  string userInput;
  string suggested;
  int oldf;
  
  stringstream ss;
  ifstream infile;
//http://stackoverflow.com/questions/5419356/redirect-stdout-stderr-to-a-string  
  
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
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
  
      infile.open("outputfile");
      string word;
      int size;
  while(1){
    
    
    
    ch = getchar();
    if(ch == -1) continue;
    else if (ch == 10) {
      cout << "\nDone: " << output << endl;
      //word = output; 
      ch=-1;
      break;
    }

    else if(ch == 49){
    	output = "";
    	cout << endl << "Cleared" << endl;
    }
    else if(ch == -1) {
      continue;
    }
    
    else {
    	size = 20;
    	string spaces = string(size, ' ');
    	string bspaces = string(size, '\b');
    	cout << "\r" << spaces << "\r";
        if(ch == 127 && output.size() > 0) {
    		output.erase(output.size() - 1);
    	}
    	else{
    		
    		output += ch;
    	}
    	cout << output;
    	if (output.size() > 0)
      for(int i=0; i < MAX_SIZE; i++){
      	word = array[i];

   		string prefix(output);
   		if (!word.compare(0, prefix.size(), prefix)){
      		int foo_value = atoi(word.substr(prefix.size()).c_str());
      		//string mult = "\b";
      		//string asdf = string(output.length(), '\b');
      		cout << "\r" << word << "\033["<< word.size()-output.size() <<"D";
      		//output = word;
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


















