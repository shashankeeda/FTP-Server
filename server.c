#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/sendfile.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
  struct sockaddr_in server, client;
  struct stat ob;
  int sock1, sock2, sckt;
  char buffer[20], command[20];
  int lis, len, size, c, status;
  int open_flags, open_perms, fd;
  char *ags[] = {"ls", NULL};
  
  sock1 = socket(AF_INET, SOCK_STREAM, 0);
  if(sock1 < 0)
  {
    perror("Socket creation failed");
    return -1;
  }
  server.sin_port = atoi(argv[1]);
  server.sin_addr.s_addr = 0;
  server.sin_family = AF_INET;
  
  sckt = bind(sock1, (struct sockaddr *)&server, sizeof(server));
  if(sckt < 0)
  {
    perror("Binding error");
    return -1;
  }
  while(1)
  {
    printf("listening\n");
    lis = listen(sock1, 2);
    if(lis < 0)
    {
      perror("Listen failed");
      return -1;
    }
    len = sizeof(client);
    sock2 = accept(sock1, (struct sockaddr*)&client, &len);
    printf("recieved a connection request\n");
    if(fork()==0)
    {
      break;
    }
  }
  while(1)
  {
    recv(sock2,buffer,20,0);
    sscanf(buffer, "%s\n", command);
    
    printf("command is %s\n", command);
    
    if(strcmp(command, "ls") == 0)
    {
      system("ls >output.txt");
      stat("output.txt", &ob);
      size = ob.st_size;
      send(sock2, &size, sizeof(int), 0);
      fd = open("output.txt", O_RDONLY);
      sendfile(sock2, fd, NULL, size);
      close(fd);
    }
    else if(strcmp(command, "pwd") == 0)
    {
      system("df >output.txt");
      stat("output.txt", &ob);
      size = ob.st_size;
      send(sock2, &size, sizeof(int), 0);
      fd = open("output.txt", O_RDONLY);
      sendfile(sock2, fd, NULL, size);
      close(fd);
    }
    else if(strcmp(command, "df") == 0)
    {
      system("df >output.txt");
      stat("output.txt", &ob);
      size = ob.st_size;
      send(sock2, &size, sizeof(int), 0);
      fd = open("output.txt", O_RDONLY);
      sendfile(sock2, fd, NULL, size);
      close(fd);
    }
    else if(strcmp(command, "cd") == 0)
    {
      if(chdir(buffer+3) == 0)
        c=1;
      else
        c=0;
      send(sock2,&c,sizeof(int),0);
    }
    else if(strcmp(command,"quit") == 0)
    {
      printf("Quitting server");
      break;
    }
  }
  return 0;
}
