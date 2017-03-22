#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <algorithm>

#define MAX_STRING_LENGTH 100



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
static const int MAX_SIZE = 100;
int main(){
  struct termios oldt, newt;
  char ch, command[20];
  string output;
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
  while(1){
    
    ch = getchar();
    if (ch == '\033') {
      printf("End\n"); 
      ch=-1;
      break;
    }
    else if(ch == -1) {
      continue;
    }
    else {
      for(int i=0; i < MAX_SIZE; i++){
      	string word = array[i];

   		string prefix("ca");
   		if (!word.compare(0, prefix.size(), prefix)){
      		int foo_value = atoi(word.substr(prefix.size()).c_str());
      		cout << word << endl;
      		break;
      		}
      }
      cout << ch;
      output += ch;
      ss << "bash -c 'compgen -A function -abck > outputfile";
      system(ss.str().c_str());
   	  cout << ss;
      //cout << infile.rdbuf() << "\n";
    }
  }
  
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);
  cout << "Output: " << output << "\n";
  if(ch != EOF){
    ungetc(ch,stdin);
    putchar(ch);
    scanf("%s",command);
    return 1;
  }
      infile.close();
  return 0;
}

string* streamToStringArray(stringstream ss){
	
	return NULL;
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


















