#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>

/*
 * This script should move all the contents of a subfolder to the current
 * directory and delete the subfolder.
 */

char** copied;
int numFiles;

void copyFile(char *source, char *dest){
     int childExitStatus;
    pid_t pid;
    //int status = 0;
    if (!source || !dest) {
        /* handle as you wish */
        exit(1);
    }

    pid = fork();

    if (pid == 0) { /* child */
        execl("/bin/cp", "/bin/cp", source, dest, (char *)0);
        _exit(EXIT_FAILURE);
        //exit(0);
    }
    else if (pid < 0) {
        /* error - couldn't start process - you decide how to handle */

    }
    else {
        /* parent - wait for child - this has all error handling, you
         * could just call wait() as long as you are only expecting to
         * have one child process at a time.
         */
        pid_t ws = waitpid( pid, &childExitStatus, WNOHANG);
        if (ws == -1)
        { /* error - handle as you wish */
          exit(1);
        }

        if( WIFEXITED(childExitStatus)) /* exit code in childExitStatus */
        {
            int status = WEXITSTATUS(childExitStatus); /* zero is normal exit */
            /* handle non-zero as you wish */
            if(status){
              printf("Exited with error number %i\n", status);
            }else{
              printf("Successful copy of %s\n", source);
            }
        }
        else if (WIFSIGNALED(childExitStatus)) /* killed */
        {
          printf("Process killed\n");
          //exit(1);
        }
        else if (WIFSTOPPED(childExitStatus)) /* stopped */
        {
          printf("Process stoped\n");
          exit(1);
        }
    }
}

int copydir(char * curPath, char * subdir){
  //TODO
  printf("Copying directory at %s\n", subdir);
  int fileCount = 0;
  DIR* dir;
  struct dirent *ent;
  if ((dir = opendir (subdir)) != NULL) {
    /* print all the files and directories within directory */
    while ((ent = readdir (dir)) != NULL) {
      printf ("%s ", ent->d_name);
      if(!(strcmp(ent->d_name, ".")) || !strcmp(ent->d_name, "..")){
        printf("- skipping\n");
      }else{
        char * file = malloc((strlen(subdir) + strlen(ent->d_name)) * sizeof(char));
        strcpy(file, subdir);
        strcat(file, ent->d_name);
        copyFile(file, curPath);
        printf("- coppied\n");
        fileCount++;
        copied = realloc(copied, (fileCount * sizeof(char *)));
        copied[fileCount - 1] = file;
      }
    }
    numFiles = fileCount;
    printf("Trying to close direcory %s at %p\n", subdir, dir);
    closedir(dir);
  } else {
    /* could not open directory */
    perror ("Couldnt open dir \n");
    numFiles = 0;
    return EXIT_FAILURE;
  }
  
  return 1;
}

void cleardir(char * dir){
  //TODO

  sleep(2);

  int i;
  for(i = 0; i < numFiles; i++){
    printf("Unlinking %s\n", copied[i]);
    if(unlink(copied[i])){
      //Error
      printf("Unlinking error with file %s\n", copied[i]);
    }
    free(copied[i]);
  }
  free(copied);
  //error check?
   rmdir(dir);
  printf("removed directory %s\n", dir);
}

int confirmDelete(){
  //TODO
  return 1;
}

//main method to capture arguments
int main(int argc, char** argv){
  char * curPath;
  char * subdir;

  printf("Hello world!\n");
  char temp[1024];
  if(getcwd(temp, sizeof(temp))){
    printf("Current path is %s\n", temp);
    curPath = malloc(sizeof(temp));
    strcpy(curPath, temp);
  }else{
    printf("Path error.\n");
  }

  if(argc > 0){
    subdir = malloc((strlen(curPath) + strlen(argv[1]) + 1) * sizeof(char));
    strcpy(subdir, curPath);
    strcat(subdir, "/");
    strcat(subdir, argv[1]);
    printf("added /%s to current path resulting in %s\n", argv[1], subdir);
  }

  if(!subdir){
    return 1;
  }
  int result = copydir(curPath, subdir);
  
  if(result && confirmDelete()){
    cleardir(subdir);
  }
  printf("freeing memory\n");
  free(subdir);
  free(curPath);
  printf("Task complete\n");
  exit(0);
}