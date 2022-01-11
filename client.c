#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/sendfile.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
  struct sockaddr_in server;
  struct stat ob;
  int sock, c, choice, status;
  char buf[20], command[20], *f;
  int open_flags, open_perms, fd, size;
  
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if(sock == -1)
  {
    perror("Socket creation failed");
    return -1;
  }
  server.sin_family = AF_INET;
  server.sin_port = atoi(argv[1]);
  server.sin_addr.s_addr = 0;
  printf("Connecting\n");
  c = connect(sock, (struct sockaddr*)&server, sizeof(server));
  if(c == -1)
  {
    perror("Connection Failed");
    return -1;
  }
  printf("Connected\n");
  open_flags = O_RDWR | O_CREAT | O_TRUNC;
  open_perms = 0777;
  while(1)
  {
    printf("Enter 1 for ls, 2 for pwd, 3 for df, 4 for cd, and 5 to quit");
    scanf("%d",&choice);
    if(choice == 1)
    {
      strcpy(buf,"ls");
      send(sock,buf,20,0);
      recv(sock,&size,sizeof(int),0);
      f = malloc(size);
      recv(sock,f,size,0);
      fd = open("temp.txt",open_flags,open_perms);
      write(fd,f,size);
      close(fd);
      system("cat temp.txt");
      free(f);
    }
    else if(choice == 2)
    {
      strcpy(buf,"pwd");
      send(sock,buf,20,0);
      recv(sock,&size,sizeof(int),0);
      f = malloc(size);
      fd = open("output.txt", O_RDWR);
      recv(sock, f, size, 0);
      write(fd,f,size);
      close(fd);
      free(f);
      system("cat output.txt");
    }
    else if(choice == 3)
    {
      strcpy(buf,"df");
      send(sock,buf,20,0);
      recv(sock,&size,sizeof(int),0);
      f = malloc(size);
      fd = open("output.txt", O_RDWR);
      recv(sock, f, size, 0);
      write(fd,f,size);
      close(fd);
      free(f);
      system("cat output.txt");
    }
    else if(choice == 4)
    {
      strcpy(buf,"cd ");
      printf("Enter the directory you want to change to\n");
      scanf("%s",buf+3);
      send(sock,&status,20,0);
      recv(sock,&status,sizeof(int),0);
      if(status)
        printf("Directory successfully changed\n");
      else
        printf("Directory not successfully changed\n");
    }
    else if(choice == 5)
    {
      strcpy(buf, "quit");
      send(sock,buf,20,0);
      exit(1);
    }
  }
  return 0;
}
