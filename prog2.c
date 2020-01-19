#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

int main(){
char *myfifo = "/tmp/myfifo";
int y, fd, fif;
char stringToSend[256];

fif = open(myfifo,O_RDONLY);
read(fif,&y,sizeof(y));
close(fif);
printf("prog2: Odebrano od prog1 liczbe: %d\n", y);
unlink(myfifo);

sprintf(stringToSend,"%d",y);
printf("prog2: Otwarto urzadzenie my_device\n");
fd = open("/dev/my_device", O_RDWR);
write(fd, stringToSend, strlen(stringToSend));
printf("prog2: Wyslano liczbe %d do sterownika",y);
getchar();
close(fd);

return 0;
}
