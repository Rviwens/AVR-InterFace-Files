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
	SD_RSB(DataBuff,0);
        SecsPerClust =(DataBuff[13]);
		FATStartSec= (DataBuff[15]*256+DataBuff[14]);
		FatSecs = (((DataBuff[38]*65536) + ((unsigned) DataBuff[37]*256) + (DataBuff[36])) *DataBuff[16]);	
		RootDirSec = FATStartSec+FatSecs;
		return 1;
	}



	//Find File location if Root Directory and First Data cluster of file
void FAT32_Open_File(char str[]){
static uint16_t locX;
static uint16_t locI;
short flag=1;

for (int i=0; i<0xFFFF; i++){		
	SD_RSB(DataBuff,RootDirSec+i);	
		for(uint16_t x=0; x<= 511; x++){
			flag=1;
				if(DataBuff[x]==str[0]){			
					for(int y=0; y <strlen(str); y++)
						if (DataBuff[x+y]!=str[0+y])flag=0;
				if(flag!=0){
					USART_Send("\r\n Found\r\n");
				    locX=x;
					locI=i;
					flag =2;
					break;
   }if(flag==2)break;
  }if(flag==2)break;
 }if(flag==2)break;
}
if(flag !=0){		
//Attr=DataBuff[locX+0x0B];
FirstClustAddr = (65536*(DataBuff[locX+0x15])) + (4096*(DataBuff[locX+0x14]))+(256*(DataBuff[locX+0x1B]))+(DataBuff[locX+0x1a]);	



}
}



void FAT32_Read_File(char str[]){
FAT32_Open_File(str);
uint32_t CurrentClust = FirstClustAddr;	

// FAT32[First Data Cluster]--> Next Data Cluster of Data; 	
//I.E In the FAT32 Primary FAT, The index of (FirstDataCluster*4) (4 for 4 bytes) gives the index of the next cluster(bits 0-6)and the cluster location (bits 0-511); 

while(1){
long ConstMin = 128*(int)(CurrentClust/128);
long loc = RootDirSec+(32*(CurrentClust-2));		
char c=-1;
while(++c<SecsPerClust){		
	SD_RSB(DataBuff,loc+c);
	
	for(short i =0; i<512;i++)
		USART_Int_StrHEXRAW(DataBuff[i],0);
}

SD_RSB(DataBuff,FATStartSec+((int)CurrentClust/128));	
long long AddrData =(DataBuff[(0)+(CurrentClust*4) - ConstMin]+(DataBuff[(1)+(CurrentClust*4)- ConstMin]*256)+(DataBuff[(2)+(CurrentClust*4)- ConstMin]*65536)+(DataBuff[(3)+(CurrentClust*4)- ConstMin]*16777216));
CurrentClust = (AddrData&0b1111111);
if(AddrData== 0xffefff8){break;} // If not working check ending flag. I set this for the file I was working on and might nbot be right.
}
}




void ClearClust(uint32_t ClustAddr){
memset(DataBuff,0,512);
char c=-1;
while(++c<SecsPerClust){
SD_WSB(DataBuff,ClustAddr+c);	
_delay_ms(10);
}

}



void FAT32_Delete_File_Contents(char str[]){
FAT32_Open_File(str);
uint32_t CurrentClust = FirstClustAddr;	//

while(FirstClustAddr!=0){
long loc = RootDirSec+(32*(CurrentClust-2));
long ConstMin = 128*(int)(CurrentClust/128);

ClearClust(loc);

SD_RSB(DataBuff,FATStartSec+((int)CurrentClust/128));

long long AddrData =(DataBuff[(0)+(CurrentClust*4) - ConstMin]+(DataBuff[(1)+(CurrentClust*4)- ConstMin]*256)+(DataBuff[(2)+(CurrentClust*4)- ConstMin]*65536)+(DataBuff[(3)+(CurrentClust*4)- ConstMin]*16777216));
CurrentClust = (AddrData&0b1111111);

if(AddrData== 0xffefff8||AddrData== 0x0)break; // If not working check ending flag. I set this for the file I was working on and might nbot be right.
USART_Send("\r\n Clearing Cluster:");
USART_Long_Str(CurrentClust,0);
}
}

void FAT32_Clear_From_FAT(char str[]){
FAT32_Open_File(str);	
uint32_t CurrentClust = FirstClustAddr;	

while(1){
long ConstMin = 128*(int)(CurrentClust/128);	
long long AddrData =(DataBuff[(0)+(CurrentClust*4) - ConstMin]+(DataBuff[(1)+(CurrentClust*4)- ConstMin]*256)+(DataBuff[(2)+(CurrentClust*4)- ConstMin]*65536)+(DataBuff[(3)+(CurrentClust*4)- ConstMin]*16777216));

SD_RSB(DataBuff,FATStartSec+((int)CurrentClust/128));

DataBuff[(0)+(CurrentClust*4) - ConstMin] =0;
DataBuff[(1)+(CurrentClust*4) - ConstMin] =0;
DataBuff[(2)+(CurrentClust*4) - ConstMin] =0;
DataBuff[(3)+(CurrentClust*4) - ConstMin] =0;

SD_WSB(DataBuff,FATStartSec+((int)CurrentClust/128));	

CurrentClust = (AddrData&0b1111111);
if(AddrData== 0xffefff8||AddrData==0x0)break;	

}
}




void FAT32_Clear_From_Dir(char str[]){
static uint16_t locX;
static uint16_t locI;
short flag=1;


for (int i=0; i<0x20; i++){
	SD_RSB(DataBuff,RootDirSec+i);
	for(uint16_t x=0; x<= 511; x++){
		flag=1;
		if(DataBuff[x]==str[0]){
			for(int y=0; y <strlen(str); y++)
			if (DataBuff[x+y]!=str[0+y])flag=0;
			if(flag!=0){
				USART_Send("\r\n Found\r\n");
				locX=x;
				locI=i;
				flag =2;
				break;
				
			}if(flag==2)break;
		}if(flag==2)break;
	}if(flag==2)break;
}


if(flag !=0){
SD_RSB(DataBuff,RootDirSec+locI);
DataBuff[locX]=0xe5;
SD_WSB(DataBuff,RootDirSec+locI);
}
}

void FAT32_Delete_File(char str[]){
FAT32_Delete_File_Contents(str);
_delay_ms(100);
FAT32_Clear_From_FAT(str);
_delay_ms(100);
FAT32_Clear_From_Dir(str);
}


unsigned long FAT32_RETURNING_NEXT_OPEN_CLUST(){
//Assigns First Cluster Address.
long CurrentClust=2;
long ConstMin = 128*(int)(CurrentClust/128);
long long AddrData=0;
while(1){
CurrentClust++;
ConstMin = 128*(int)(CurrentClust/128);
SD_RSB(DataBuff,FATStartSec+((int)CurrentClust/128));

AddrData =(DataBuff[(0)+(CurrentClust*4) - ConstMin]+(DataBuff[(1)+(CurrentClust*4)- ConstMin]*256)+(DataBuff[(2)+(CurrentClust*4)- ConstMin]*65536)+(DataBuff[(3)+(CurrentClust*4)- ConstMin]*16777216));
if(AddrData==0)break;
}
	
return CurrentClust;
}




void FAT32_Update_FAT(long Clust){
Clust *=4;
long ConstMin = 128*(int)(Clust/128);
SD_RSB(DataBuff,FATStartSec+((int)Clust/128));
DataBuff[Clust-ConstMin]=0xff;
DataBuff[Clust-ConstMin+1]=0xef;
DataBuff[Clust-ConstMin+2]=0xff;
DataBuff[Clust-ConstMin+3]=0xf8;
SD_WSB(DataBuff,FATStartSec+((int)Clust/128));
	
}




void FAT32_Create_File(char str[],char type[]){

uint16_t Count=4;
uint16_t CountOvrFlw=0; 

int Addr=0;
uint32_t out = FAT32_RETURNING_NEXT_OPEN_CLUST();

SD_RSB(DataBuff,RootDirSec+CountOvrFlw);

//Finds new Entry in Dir
while((DataBuff[(Count*32)-(512*CountOvrFlw)]!= 0xe5) && (DataBuff[(Count*32)-(512*CountOvrFlw)]!= 0x0))
{		
SD_RSB(DataBuff,RootDirSec+CountOvrFlw);	
CountOvrFlw =(int)(Count/16);
Count++;
}
Addr =(Count*32)-(512*CountOvrFlw);

//Writes New Entry Name, type, and ATTR in Dir
for (int i=0; i<(11-strlen(type)); i++){
if(i>=strlen(str)) DataBuff[Addr+i]=0x20; else DataBuff[Addr+i]=str[i];
}
//Type
for (int i=11-strlen(type); i<11; i++)
	DataBuff[Addr+i]=type[i-(11-strlen(type))];

//Attr
DataBuff[Addr+11]=0x20;
//Upper Case or lowercase
DataBuff[Addr+12]=0x00;

//Date created
for (int i=13; i<20; i++)
DataBuff[Addr+i]=0x0;

// Declare Size
for (int i=28; i<32; i++)
DataBuff[Addr+i]=0;


// Date Modified
for (int i=22; i<26; i++)
DataBuff[Addr+i]=0x1;


//Assigning First Cluster
DataBuff[Addr+21]= out & 0xFF000000;
DataBuff[Addr+20]=out & 0xFF0000;
DataBuff[Addr+27]= out & 0xFF00;
DataBuff[Addr+26]=out & 0xFF;
SD_WSB(DataBuff,RootDirSec+CountOvrFlw);


USART_Send("\r\n X =");
USART_Int_Str(Addr, 0);
USART_Send("\r\n Free Clust at =");
USART_Long_Str(out,0);

 //FAT32_Update_FAT(out);
  
}

FAT32_Append_Sector(char str[]){
	FAT32_Open_File(str);
		 USART_Send("\r\n ClustAddr =");
		 USART_Long_Str(FirstClustAddr,0);
	 USART_Send("\r\n ClustAddr at =");
	 USART_Long_Str(RootDirSec+(32*(FirstClustAddr-2)),0);
	memset(DataBuff,0,512);
	DataBuff[0]=0x45;
		DataBuff[1]=0x35;
			DataBuff[2]=0x42;
				DataBuff[3]=0x23;
					DataBuff[4]=0x52;
						DataBuff[5]=0x49;
	 SD_WSB(DataBuff,RootDirSec+(32*(FirstClustAddr-2)));
}

void FAT32_Read_Sector_In_Cluster(long Clust,uint8_t Sect){
SD_RSB(RootDirSec+(32*(Clust-2))+Sect,0);	

	 USART_Send("\r\n");
for(int t = 0; t<512; t++)
	USART_Int_StrHEXRAW(DataBuff[t],0);


}
	
#endif
