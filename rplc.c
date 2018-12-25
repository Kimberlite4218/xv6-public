#include "types.h"
#include "stat.h"
#include "user.h"

#define BUFF_SIZE 1024

// /*
char *buff;
int match(char*, char*);
// */

void
rplc(char *pattern, char *rplc_word, int fd) {
 //  /*
  buff = malloc(sizeof(char)*BUFF_SIZE);

  int n, m;
  char *p, *q;    

  m = 0;
  while((n = read(fd, buff+m, BUFF_SIZE-m-1)) > 0){
    m += n;
    buff[m] = '\0';
    p = buff;
    while((q = strchr(p, '\n')) != 0){
      *q = 0;
      if(match(pattern, p)){
        *q = '\n';
        for (int i = 0; i < (q+1 - p); i++){
          int isPattern = 1;
          for (int j = 0; j < strlen(pattern); j++){
            if (p[i+j]!=pattern[j]){
              isPattern = 0;
              break;
            }
          }
          if (isPattern){
            printf(1, "%s", rplc_word);
            i += strlen(pattern) - 1;
          }
          else
            printf(1, "%c", p[i]);
        }
      }
      p = q+1;
    }
    if(p == buff)
      m = 0;
    if(m > 0){
      m -= p - buff;
      memmove(buff, p, m);
    }
  }

  free(buff);
  // */
}

int
main(int argc, char *argv[]) {
// /*
  int fd, i;
  char *pattern, *rplc_word;

  if(argc <= 2){
    printf(2, "usage: replace pattern [file ...]\n");
    exit();
  }
  pattern = argv[1];
  rplc_word = argv[2];

  if(argc <= 3){
    rplc(pattern, rplc_word, 0);
    exit();
  }

  for(i = 3; i < argc; i++){
    if((fd = open(argv[i], 0)) < 0){
      printf(1, "replace: cannot open %s\n", argv[i]);
      exit();
    }
    rplc(pattern, rplc_word, fd);
    close(fd);
  }
  // */
  exit();
}

// Regexp matcher from Kernighan & Pike,
// The Practice of Programming, Chapter 9.
// /*
int matchhere(char*, char*);
int matchstar(int, char*, char*);

int
match(char *re, char *text)
{
  if(re[0] == '^')
    return matchhere(re+1, text);
  do{  // must look at empty string
    if(matchhere(re, text))
      return 1;
  }while(*text++ != '\0');
  return 0;
}

// matchhere: search for re at beginning of text
int matchhere(char *re, char *text)
{
  if(re[0] == '\0')
    return 1;
  if(re[1] == '*')
    return matchstar(re[0], re+2, text);
  if(re[0] == '$' && re[1] == '\0')
    return *text == '\0';
  if(*text!='\0' && (re[0]=='.' || re[0]==*text))
    return matchhere(re+1, text+1);
  return 0;
}

// matchstar: search for c*re at beginning of text
int matchstar(int c, char *re, char *text)
{
  do{  // a * matches zero or more instances
    if(matchhere(re, text))
      return 1;
  }while(*text!='\0' && (*text++==c || c=='.'));
  return 0;
}
// */
