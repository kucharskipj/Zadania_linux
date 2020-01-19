#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#define BUFFER_LENGTH 256
static char receive[BUFFER_LENGTH];

int main(){
int ret, fd;
char stringToSend[4] = {"1024"};
fd = open("/dev/my_device", O_RDWR);
printf("%d\n",fd);

ret = write(fd,stringToSend, strlen(stringToSend));


printf("Press ENTER to read back from the device...\n");
getchar();

ret = read(fd, receive, BUFFER_LENGTH);
printf("[%s]\n", receive);

close(fd);

return 0;
}
