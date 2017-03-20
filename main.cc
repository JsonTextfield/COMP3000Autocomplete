#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

#define MAX_STRING_LENGTH 100

using namespace std;

int main(){
  struct termios oldt, newt;
  char ch, command[20];
  string output;
  int oldf;
  
  stringstream ss;
  ifstream infile;
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
      cout << ch;
      output += ch;
      ss << "bash -c 'compgen -A function -abck " << output << " > outputfile'";
      system(ss.str().c_str());
   
      cout << infile.rdbuf() << "\n";
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
