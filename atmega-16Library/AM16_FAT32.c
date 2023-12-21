#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <math.h>
#include <string.h>


// For reading and writing files in FAT32
#if defined(FAT32)


// Init vars

long RootDirSec;
uint16_t FATStartSec;
uint16_t FatSecs;
char MBR=0;
char FSInfo = 1;
uint8_t SecsPerClust;

uint32_t FirstClustAddr;



// Find FAT32 Root Directory
char FAT32_Init(){
	SD_RSB(StatusBuff,0);
	
        SecsPerClust =(StatusBuff[13]);
		FATStartSec= (StatusBuff[15]*256+StatusBuff[14]);
		FatSecs = (((StatusBuff[38]*65536) + ((unsigned) StatusBuff[37]*256) + (StatusBuff[36])) *StatusBuff[16]);	
		RootDirSec = FATStartSec+FatSecs;
		return 1;
		
		
	}






	//Find File location if Root Directory and First Data cluster of file
void FAT32_Open_File(char str[]){
static uint16_t locX;
static uint16_t locI;
short flag=1;


for (int i=0; i<0xFFFF; i++){		
	SD_RSB(StatusBuff,RootDirSec+i);	
		for(uint16_t x=0; x<= 511; x++){
			flag=1;
				if(StatusBuff[x]==str[0]){			
					for(int y=0; y <strlen(str); y++)
						if (StatusBuff[x+y]!=str[0+y])flag=0;
				if(flag!=0){
					//USART_Send("\r\n Found\r\n");
				    locX=x;
					locI=i;
					flag =2;
					break;
					
   }if(flag==2)break;
  }if(flag==2)break;
 }if(flag==2)break;
}


if(flag !=0){		
	
SD_RSB(StatusBuff,RootDirSec+locI);
//Attr=StatusBuff[locX+0x0B];
FirstClustAddr = (65536*(StatusBuff[locX+0x15])) + (4096*(StatusBuff[locX+0x14])) +(256*(StatusBuff[locX+0x1B])) + (StatusBuff[locX+0x1a]);	
}
}



void FAT32_Read_File(char str[]){
FAT32_Open_File(str);
uint32_t CurrentClustAddr = FirstClustAddr;
uint32_t CurrentClust = FirstClustAddr;	//
// FAT32[First Data Cluster]--> Next Data Cluster of Data; 	
//I.E In the FAT32 Primary FAT, The index of (FirstDataCluster*4) (4 for 4 bytes) gives the index of the next cluster(bits 0-6)and the cluster location (bits 0-511); 
while(1){
long loc = RootDirSec+(32*(CurrentClust-2));		
char c=0;
while(++c<SecsPerClust){		
	SD_RSB(StatusBuff,loc+c);
	
	for(short i =0; i<512;i++)
		USART_Int_StrHEXRAW(StatusBuff[i],0);
}

SD_RSB(StatusBuff,FATStartSec);	

long long AddrData =(StatusBuff[(0)+(CurrentClust*4)]+(StatusBuff[(1)+(CurrentClust*4)]*256)+(StatusBuff[(2)+(CurrentClust*4)]*65536)+(StatusBuff[(3)+(CurrentClust*4)]*16777216));
		
CurrentClustAddr = RootDirSec+(SecsPerClust*((AddrData&0b1111111)-2));
CurrentClust = (AddrData&0b1111111);

if(AddrData== 0xffefff8){break;} // If not working check ending flag. I set this for the file I was working on and might nbot be right.
}
}




void ClearClust(uint32_t ClustAddr){
memset(StatusBuff,0,512);
	
char c=0;
while(++c<SecsPerClust){
SD_WSB(StatusBuff,ClustAddr+c);	
}

}



void Clear(uint32_t StartAddr, uint32_t EndAddr){
memset(StatusBuff,0,512);	

for(uint32_t i =StartAddr; i<=EndAddr;i++)
SD_WSB(StatusBuff,i);	
}



void FAT32_Delete_File(char str[]){
//clear_DIR(str);

uint32_t CurrentClustAddr = 32800;
SD_RSB(StatusBuff,FATStartSec);

while( ( StatusBuff[CurrentClustAddr*4]+ ( (StatusBuff[(CurrentClustAddr*4)+1])*256) ) !=0xFFFF) {
	
CurrentClustAddr =  StatusBuff[CurrentClustAddr*4]; 
StatusBuff[CurrentClustAddr*4]=0; 
ClearClust(RootDirSec+(8*(CurrentClustAddr-2)));
}

}





void clear_DIR(char str[]){
static uint16_t locX;
static uint16_t locI;
short flag=1;

for (int i=0; i<0xFFFF; i++){
	SD_RSB(StatusBuff,RootDirSec+i);
	for(uint16_t x=0; x<= 511; x++){
		flag=1;
		if(StatusBuff[x]==str[0]){
			for(int y=0; y <strlen(str); y++)
			if (StatusBuff[x+y]!=str[0+y])flag=0;
			if(flag!=0){
				locX=x;
				locI=i;
				flag =2;
				break;
				
			}if(flag==2)break;
		}if(flag==2)break;
	}if(flag==2)break;
}



if(flag !=0){
	SD_RSB(StatusBuff,RootDirSec+locI);
	FirstClustAddr = (65536*(StatusBuff[locX+0x15])) + (4096*(StatusBuff[locX+0x14])) +(256*(StatusBuff[locX+0x1B])) + (StatusBuff[locX+0x1a]);
	StatusBuff[locX]=0xe5;
	SD_WSB(StatusBuff,RootDirSec+locI);
}	
}






	
#endif

