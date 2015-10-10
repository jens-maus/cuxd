/*
update_addon (C) '2013 by Uwe Langhammer
./update_addon <addon_id> [<addon_file>]
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <malloc.h>
#include <errno.h>
#include <sys/syslog.h>
#include <sys/socket.h>
#include <time.h>

#define VERSION "1.0"
#define HM_CONFIG "/usr/local/etc/config/hm_addons.cfg"

char *read_configfile(char *configfile) {
  char *buffer = NULL;
  FILE *f;
//printf("read(%s)\n",configfile);
  if (configfile) {
    if ((f = fopen(configfile,"rb"))) {
      int size,len;
      fseek(f,0,SEEK_END);
      size = ftell(f);
      rewind(f);
      buffer = malloc(size+2);
      len = fread(buffer,1,size,f);
      fclose(f);
      if (len == size) {
        char c1,c2,c,ws = 1;
        int i,j = 0;
        buffer[len] = 0;
//printf("in: (%s)\n",buffer);
        for (i=0;i<=len;i++) {
          c1 = buffer[i];
          c2 = buffer[i+1];

          if (c1 > ' ') {
            if (c1 == '{') ws=1;
            else ws=0;
            c=1;
          } else {
            c1 = ' ';
            if (ws || (c2 <= ' ') || (c2 == '}')) c = 0;
            else c = 1;
            if (c) ws = 1;
          }

          if (c) buffer[j++] = c1;
        }
        while (j && (buffer[j-1] == ' ')) j--;
        buffer[j] = 0;
//printf("out: (%s) %d:%d\n",buffer,size,j);
      } else {
        fprintf(stderr,"ERR: read(%s) len(%d)\n",configfile,len);
        free(buffer);
        buffer = NULL;
      }
    } else {
      fprintf(stderr,"ERR: open(%s) %s\n",configfile,strerror(errno));
    }
  } else {
    buffer = malloc(1);
    *buffer = 0;
  }
  return buffer;
}

void del_addon(char *hmconfig,char *id) {
  char *p3,*p2,*p1 = hmconfig;
  char fid[30];
  while (*p1) {
    while (*p1 && (*p1 <= ' ')) p1++;
    p2 = p1;
    while ((*p2 > ' ') && (*p2 != '{')) p2++;
    memcpy(fid,p1,p2-p1);
    fid[p2-p1] = 0;	  
//printf("del1(%s)?\n",fid);
    p3 = p2;
    while (*p3 && (*p3 <= ' ')) p3++;
    if (*p3 == '{') {
      int n = 1;
      p3++;
      while (n && *p3) {
        switch (*p3) {
          case '{':
            n++;
            break;
          case '}':
            n--;
            break;
        }
        p3++;
      }
      while (*p3 && (*p3 <= ' ')) p3++;
      if (!strcmp(fid,id)) {
printf("del(%s)\n",fid);
        memmove(p1,p3,strlen(p3)+1);
        break;
      }
    }
    p1 = p3;
  }
}

char *add_addon(char *hmconfig,char *id,char *addon) {
  hmconfig = realloc(hmconfig,strlen(hmconfig)+strlen(id)+strlen(addon)+10);
  if (strlen(hmconfig) && (hmconfig[strlen(hmconfig)-1] > ' ')) strcat(hmconfig," ");
  strcat(hmconfig,id);
  strcat(hmconfig," ");
  strcat(hmconfig,addon);
printf("set(%s)\n",id);
  return hmconfig;
}

int write_hmconfig(char *cfgfile,char *hmconfig) {
  FILE *f;
  if ((f = fopen(cfgfile,"wb"))) {
    int len = strlen(hmconfig);
    if (len && (hmconfig[len-1] == ' ')) len--;
    fwrite(hmconfig,1,len,f);
//    fwrite("\n",1,1,f);
    fclose(f);
  } else return 1;
  return 0;
}

int set_addon(char *id,char *addon) {
  int ret = 0;	
  char *buffer = read_configfile(addon);
  if (buffer) {
    char *hmconfig = read_configfile(HM_CONFIG);
    if (hmconfig) {
//printf("del(%s): %s\n",id,hmconfig);
      del_addon(hmconfig,id);
//printf("add(%s): %s\n",id,hmconfig);
      if (strlen(buffer)) hmconfig = add_addon(hmconfig,id,buffer);
//printf("write(%s): %s\n",HM_CONFIG,hmconfig);
      ret = write_hmconfig(HM_CONFIG,hmconfig);
      free(hmconfig);
    } else ret = 1;
    free(buffer);
  } else ret = 1;
  return ret;
}

int main(int argc, char *argv[]) {
  int ret = 0;
  if (argc > 2) {
    ret = set_addon(argv[1],argv[2]);
  } else if (argc > 1) {
    ret = set_addon(argv[1],NULL);
  } else {
    fprintf(stderr,"update_addon Version %s (C) by Uwe Langhammer\n",VERSION);
    fprintf(stderr,"usage: %s <id> [<descr>]\n",argv[0]);
    ret = 1;
  }
  return ret;
}
