#include "util.h"

#define SYS_OPEN 5
#define SYS_LSEEK 19
#define SEEK_CUR 1
#define SYS_CLOSE 6 
#define SYS_WRITE 4
#define EXIT 1
#define STDOUT 1
#define SYS_WRITE 4
#define STDOUT 1
#define STDIN 0
#define STDERR 2
#define SYS_READ 3

extern int system_call();
void printDebug(int id, int returnCode);

int main (int argc , char* argv[], char* envp[])
{
  	char c[1];
	int debugMode=0;
	int givenInput=0;
	int givenOutput=0;
	int inputID=0;
	int outputID=0;
	int charMode=0;
	char* inputChar;
	int i;
	int x, y ,z;
	for(i=0; i< argc; i++){
		if(strncmp(argv[i],"-D",2)==0)
			debugMode=1;
		if(strncmp(argv[i],"-i",2)==0)
			givenInput=i;
		if(strncmp(argv[i],"-o",2)==0)
			givenOutput=i;
		if(strncmp(argv[i],"-c",2)==0){
			charMode=1;
			inputChar=&argv[i][2];
		}
	}
	if(givenInput==0){
		inputID= STDIN;
		if(debugMode==1){
			system_call(SYS_WRITE,STDERR,"input: stdin\n",13);
		}
	}
	if(givenOutput==0){
		outputID= STDOUT;
		if(debugMode==1){
			system_call(SYS_WRITE,STDERR,"output: stdout\n",15);
		}
	}
	if(givenInput!=0){
		inputID= system_call(SYS_OPEN,&argv[givenInput][2],0,0777);
		if(inputID<0){
			system_call(SYS_WRITE,STDERR,"Error in opening input file\n",28);
			system_call(EXIT,0x55,1,1);
		}
		if(debugMode==1){
			system_call(SYS_WRITE,STDERR,"input: ",7);
			system_call(SYS_WRITE,STDERR,&argv[givenInput][2],sizeof(&argv[givenInput][2]));
			system_call(SYS_WRITE,STDERR,"\n",1);
			printDebug(SYS_OPEN,inputID);
		}
	}
	if(givenOutput!=0){
		outputID= system_call(SYS_OPEN,&argv[givenOutput][2],64,0777);
		outputID= system_call(SYS_OPEN,&argv[givenOutput][2],1,0777);
		if(outputID<0){
			system_call(SYS_WRITE,STDERR,"Error in opening output file\n",29);
			system_call(EXIT,0x55,1,1);
		}
		if(debugMode==1){
			system_call(SYS_WRITE,STDERR,"output: ",8);
			system_call(SYS_WRITE,STDERR,&argv[givenOutput][2],sizeof(&argv[givenOutput][2]));
			system_call(SYS_WRITE,STDERR,"\n",1);
			printDebug(SYS_OPEN,outputID);
		}
	}
	int count=0;
	int inWord=0;
	char* countStr;
	while((x=(system_call(SYS_READ,inputID,c,1))) >0){
		if(debugMode==1){
			printDebug(SYS_READ,x);
		}
		if(charMode!=0){
			if(c[0]==inputChar[0])
				count= count+1;
		}
		if(charMode==0){
			if((c[0]==' ' || c[0]=='\n') && inWord==1){
				inWord= 0;
			}
			else if(c[0]!=' ' && c[0]!='\n' && inWord==0){
				inWord=1;
				count= count+1;
			}
		}
	}
	countStr= itoa(count);
	y= system_call(SYS_WRITE,outputID, countStr, strlen(countStr));
	z= system_call(SYS_WRITE,outputID,"\n", 1);
	if(debugMode==1){
		printDebug(SYS_WRITE,x);
		printDebug(SYS_WRITE,y);
		printDebug(SYS_WRITE,z);
	}
	if(givenInput!=0)
		y= system_call(SYS_CLOSE, inputID);
	if(givenOutput!=0)
		z= system_call(SYS_CLOSE, outputID);
	if(debugMode==1){
		if(givenInput!=0)
			printDebug(SYS_CLOSE,y);
		if(givenOutput!=0)
			printDebug(SYS_CLOSE,z);
	}
  	return 0;
}

void printDebug(int id, int returnCode) {
	char* output=itoa(id);
        system_call(SYS_WRITE,STDERR,"ID- ",4);
        system_call(SYS_WRITE,STDERR,output,strlen(output));
        output=itoa(returnCode);
        system_call(SYS_WRITE,STDERR," ,Return code- ",15);
        system_call(SYS_WRITE,STDERR,output,strlen(output));
        system_call(SYS_WRITE,STDERR,"\n",1);
}
