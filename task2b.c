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
#define SYS_GETDENTS 141
enum
  {
    DT_UNKNOWN = 0,
# define DT_UNKNOWN        DT_UNKNOWN
    DT_FIFO = 1,
# define DT_FIFO        DT_FIFO
    DT_CHR = 2,
# define DT_CHR                DT_CHR
    DT_DIR = 4,
# define DT_DIR                DT_DIR
    DT_BLK = 6,
# define DT_BLK                DT_BLK
    DT_REG = 8,
# define DT_REG                DT_REG
    DT_LNK = 10,
# define DT_LNK                DT_LNK
    DT_SOCK = 12,
# define DT_SOCK        DT_SOCK
    DT_WHT = 14
# define DT_WHT                DT_WHT
  };

extern int system_call();
void printDebug(int id, int returnCode);

typedef struct ent{
    int inode;
    int offset;
    short len;
    char buff[];
} ent;

int main (int argc , char* argv[], char* envp[])
{
	int debugMode=0;
	int prefixMode=0;
	int i;
	for(i=0; i< argc; i++){
		if(strncmp(argv[i],"-D",2)==0)
			debugMode=1;
		if(strncmp(argv[i],"-p",2)==0)
			prefixMode=i;
	}
	char* prefix;
	if(prefixMode!=0)
        	prefix= &argv[prefixMode][2];
	char buf[8192];
	int fd;
	ent *entp= (ent*) (buf);
	fd= system_call(SYS_OPEN, ".",0,0777);
	if(fd<0){
		system_call(SYS_WRITE,STDERR,"Error in opening directory\n",27);
		system_call(EXIT,0x55,1,1);
	}
	if(debugMode==1)
		printDebug(SYS_OPEN,fd);
	int count=system_call(SYS_GETDENTS,fd,buf,8192);
	if(count<0){
		system_call(SYS_WRITE,STDERR,"Error in getdents\n",18);
		system_call(EXIT,0x55,1,1);
	}
	if(debugMode==1)
		printDebug(SYS_GETDENTS,count);
	int x=0;
	char* length;
	while(x<count){
		entp = (ent*) (buf+x);
		if((prefixMode==0) || strncmp(prefix,entp->buff,strlen(prefix))==0)   {
			char d_type = *(buf + x + entp->len- 1);
			char* fileType="?";
			if(d_type==DT_BLK)
				fileType= "block device";
			else if(d_type==DT_CHR)
				fileType= "character device";
			else if(d_type==DT_BLK)
				fileType= "block device";
			else if(d_type==DT_DIR)
				fileType= "directory";
			else if(d_type==DT_FIFO)
				fileType= "pipe";
			else if(d_type==DT_LNK)
				fileType= "symbolic link";
			else if(d_type==DT_REG)
				fileType= "regular file";
			else if(d_type==DT_SOCK)
				fileType= "unix domain socket";
			else if(d_type==DT_UNKNOWN)
				fileType= "unknown";
			int code1=system_call(SYS_WRITE,STDOUT,entp->buff,strlen(entp->buff));
			int code2=system_call(SYS_WRITE,STDOUT,"\n",1);
			int code3=system_call(SYS_WRITE,STDOUT,"type of file: ",14);
            		int code4=system_call(SYS_WRITE,STDOUT,fileType,strlen(fileType));
            		int code5=system_call(SYS_WRITE,STDOUT,"\n",1);
			if(debugMode==1){
				printDebug(SYS_WRITE,code1);
				printDebug(SYS_WRITE,code2);
				printDebug(SYS_WRITE,code3);
				printDebug(SYS_WRITE,code4);
				printDebug(SYS_WRITE,code5);
				length=itoa(entp->len);
				system_call(SYS_WRITE,STDERR,"length: ",8);
				system_call(SYS_WRITE,STDERR,length,sizeof(length));
				system_call(SYS_WRITE,STDERR,"\n",1);
			}
		}
		x= x+entp->len;
	}
	int closed= system_call(SYS_CLOSE,fd);
	if(closed<0){
		system_call(SYS_WRITE,STDERR,"Error in closing directory\n",27);
		system_call(EXIT,0x55,1,1);
	}
	if(debugMode==1)
		printDebug(SYS_CLOSE,closed);
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
