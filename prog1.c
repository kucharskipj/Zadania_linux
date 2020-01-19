#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(){
int ret;
int kanal[2];
int liczba;
printf("Podaj liczbe: \n");
scanf("%d",&liczba);

ret = pipe(kanal);
if (ret!=0){
printf("prog1: Nie udalo sie stworzyc unnamed pipe!\n");
}

if (fork() == 0){
close(kanal[0]);
write(kanal[1],&liczba,sizeof(liczba));
printf("prog1 parent: Wyslano liczbe %d przez unnamed pipe\n",liczba);
}
else{
int lodbior, fif;
close(kanal[1]);
read(kanal[0],&lodbior,sizeof(lodbior));
printf("prog1 child: Odebrano liczbe %d, wynik: %d\n",lodbior,2*lodbior);
lodbior=2*lodbior;

char *myfifo = "/tmp/myfifo";
ret =mkfifo(myfifo,0666);
if (ret != 0) printf("prog1 child: Nie udalo sie stworzyc named pipe!\n");
fif = open(myfifo,O_WRONLY);
write(fif, &lodbior, sizeof(lodbior));
printf("prog1 child: Wyslano przez myfifo liczbe %d\n", lodbior);
close(fif);
}

}
