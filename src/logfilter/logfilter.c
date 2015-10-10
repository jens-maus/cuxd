/*
filter for cuxd-devlogfiles
(C) '2015 by Uwe Langhammer
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define VERSION "1.0"
#define BUFSIZE 400000
#define MAXLINE 150

char verbose = 0;

int is_number(char c) {
  if ((c >= '0') && (c <= '9')) return 1;
  return 0;
}

int is_space(char c) {
  if (c && ((c == ' ') || (c == '\t') || (c == '\r') || (c == '\n'))) return 1;
  return 0;
}

char *chop(char *line) {
  if (line) {
    char *p1 = line + strlen(line);
    if (!*p1) p1--;
    while ((p1 > line) && is_space(*p1)) p1--;
    *(++p1) = 0;
  }
  return line;
}

// 01234567890123456789
// 2015-02-15T00:51:48
int checkts(short idx,char c) {
  switch (idx) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 5:
    case 6:
    case 8:
    case 9:
    case 11:
    case 12:
    case 14:
    case 15:
    case 17:
    case 18:
      return is_number(c);
    case 4:
    case 7:
      return (c == '-');
    case 10:
      return (c == 'T');
    case 13:
    case 16:
      return (c == ':');
  }
  return 0;
}

int parseline(char *line) {
  int ret = 0;
  char *read_line = line;
  
  if (read_line) {
    short idx = 0;
// check timestamp
    while (*read_line && !is_space(*read_line)) {
      if (!checkts(idx++,*(read_line++))) return 0;
    }
// 1 space
    if (*read_line) {
      line = read_line;
      *line++ = ' ';
    } else return 0;    
//printf("ts\n");

// skip spaces
    while (*read_line && is_space(*read_line)) read_line++;
    if (!*read_line) return 0;
// dp name
    while (*read_line && !is_space(*read_line)) {
      if (line != read_line) *line = *read_line;
      line++;
      read_line++;
    }
// 1 space
    if (*read_line) {
      line = read_line;
      *line++ = ' ';
    } else return 0;    
//printf("dp\n");

// skip spaces
    while (*read_line && is_space(*read_line)) read_line++;
    if (!*read_line) return 0;
// check number
    idx = 0;
    while (*read_line && !is_space(*read_line)) {
//printf("%d ",idx);
      switch (idx) {
        case 0:
          if ((*read_line == '-')||(*read_line == '+')) idx = 1;
          else if (is_number(*read_line)) idx = 2;
          else return 0;
          break;
        case 1:
          if (is_number(*read_line)) idx = 2;
          else return 0;
          break;
        case 2:
          if (*read_line == '.') idx = 3;
          else if (!is_number(*read_line)) return 0;
          break;
        case 3:
          if (!is_number(*read_line)) {
//printf("[%d] ",*read_line);
            return 0;
          }  
          break;
      }
      if (line != read_line) *line = *read_line;
      line++;
      read_line++;
    }
//printf("value\n");

    if (*line) *line = 0;
    ret = 1;
  }
  return ret;
}

int main(int argc, char *argv[]) {
  int c,ret = 0;
  char invert = 0;
//  char *argv0 = argv[0];

  while ((c = getopt(argc, argv, "vi")) != -1) {
    switch (c) {
      case 'v':
        verbose++;
        break;
      case 'i':
        invert++;
        break;
    }
  }
  optind--;
// printf("%d %d %d\n",optind,argc,verbose);
  argc -= optind;
  argv += optind;
  if (argc > 1) {
    FILE *fd;
    if ((fd = fopen(argv[1],"rb"))) {
      int len;
      char buffer[BUFSIZE+1];
      char line[MAXLINE+1];
      line[MAXLINE] = 0;
      *buffer = 0;
      *line = 0;      
      while ((len = fread(buffer,1,BUFSIZE,fd))) {
        char *p1,*p2;
        p1 = buffer;
        buffer[len] = 0;
        while ((p2 = strchr(p1,'\n'))) {
          *p2 = 0;
          if (*line) {
            int llen = strlen(line);
            if (llen < MAXLINE) strncpy(line+llen,p1,MAXLINE-llen);
            p1 = line;
          }         
// puts(p1);
          if (parseline(p1)) {
            if (!invert) puts(p1);
          } else {
            if (invert) puts(chop(p1));
          }
          p1 = p2 + 1;
          *line = 0;          
        }
        if (strlen(p1)) strncpy(line,p1,MAXLINE);
        else *line = 0;
      }      
      fclose(fd);
    } else {
//      printf("error open(%s) %s",argv[1],strerror(errno));
      ret = 1;
    }
  } else {
    puts("logfilter [-i] <logfile>");
    printf("Version %s, (C) by Uwe Langhammer\n",VERSION);
    puts("options:");
    puts("  -i  invert filter");
  }

  return ret;
}
