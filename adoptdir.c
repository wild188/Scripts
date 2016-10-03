#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

/*
 * This script should move all the contents of a subfolder to the current
 * directory and delete the subfolder.
 */

int copydir(char * curPath, char * subdir){
  //TODO
  return 1;
}

void cleardir(char * dir){
  //TODO
}

//main method to capture arguments
int main(int argc, char** argv){
  char * curPath;
  char * subdir = "test";


  printf("Hello world!\n");
  char temp[1024];
  if(getcwd(temp, sizeof(temp))){
    printf("Current path is %s\n", temp);
    curPath = malloc(sizeof(temp));
    strcpy(curPath, temp);
  }else{
    printf("Path error.\n");
  }
  int result = copydir(curPath, subdir);
  if(result){
    cleardir(strcat(curPath, subdir));
  }
}