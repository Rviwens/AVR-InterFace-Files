



// In Progress ---->>> 

// Set create File to use  FAT32_Append_Cluster_RootDir(); to create a new cluster in the Root Dir if current Cluster is full.
// Set to Update File Size


//<<<--------




#define F_CPU 20000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <math.h>
#include <string.h>

#define ReseveClust  500
#define SectorEndingTag 0x0FFFFFFF
#define FATEntryMask    0xFFFFFFFF





// For reading and writing files in FAT32
#if defined(FAT32)
// Init vars

uint32_t RootDirSec;
uint32_t FATStartSec;
uint32_t FatSecs;
uint32_t LBA;
uint32_t SecsPerClust;
uint32_t BytesPerSec;
uint32_t FirstClustAddr;
uint32_t FSInfo;
uint32_t FILELocationInRootDir; 



// Find FAT32 Root Directory
uint8_t FAT32_Init(char partionNUM){
	partionNUM++;
	
	
	SD_RSB(DataBuff,0);
	
if(DataBuff[446+(16*partionNUM)-12] ==0xC ||DataBuff[446+(16*partionNUM)-12] == 0xb){
LBA = (DataBuff[446+(16*partionNUM)-8]);
LBA += (long)(DataBuff[446+(16*partionNUM)-7]* 256);
LBA += (long)(DataBuff[446+(16*partionNUM)-6]* 65536);
LBA += (long)(DataBuff[446+(16*partionNUM)-5]* 16777216);
} else LBA=0; 
	
	SD_RSB(DataBuff,LBA);
	if(DataBuff[511]==0xaa && DataBuff[510] == 0x55){	
	SecsPerClust =(DataBuff[13]);
	FATStartSec= (DataBuff[15]*256+DataBuff[14])+LBA;
	FatSecs = (((DataBuff[38]*65536) + ((unsigned) DataBuff[37]*256) + (DataBuff[36])) *DataBuff[16]);
	RootDirSec = FATStartSec+FatSecs;
	BytesPerSec= (DataBuff[12]*256+DataBuff[11]);
	}


// RootDirSec=40960;
// BytesPerSec=512;
// FATStartSec =11470;
// SecsPerClust=32;
// USART_Send("\r\n Secs Per Clust = ");
// USART_Long_Str(SecsPerClust,0);
// USART_Send("\r\n FAT Start Sec = ");
// USART_Long_Str(FATStartSec,0);
// USART_Send("\r\n FATSecs = ");
// USART_Long_Str(FatSecs,0);
// USART_Send("\r\n RootDir = ");
// USART_Long_Str(RootDirSec,0);
// USART_Send("\r\n Bytes Per Sec = ");
// USART_Long_Str(BytesPerSec,0);
// USART_Send("\r\n");

return 0; 
}















//Find File location if Root Directory and First Data cluster of file

char FAT32_FILE_Open(char str[]){
	static uint16_t locX;
	short flag=1;
    uint32_t loci;
	for (uint16_t i=0; i<SecsPerClust; i++){
		SD_RSB(DataBuff,RootDirSec+i);
		for(uint16_t x=0; x<= 479; x++){
			flag=1;		

			if(DataBuff[x]==str[0]){							
				for(int y=0; y <strlen(str); y++)					
				if (DataBuff[x+y]!=str[0+y])flag=0;				
				if(flag!=0){
					locX=x;
					loci=i;
					//USART_Send("\r\n Found");
					flag =2;
					break;									
				}if(flag==2)break;
			}if(flag==2)break;
		}if(flag==2)break;
		
flag=0;
	}

	if(flag ==2){
		//Attr=DataBuff[locX+0x0B];
		FILELocationInRootDir = (loci<<9)+locX; 
		FirstClustAddr = (65536*(DataBuff[locX+0x15])) + (4096*(DataBuff[locX+0x14]))+(256*(DataBuff[locX+0x1B]))+(DataBuff[locX+0x1a]);
return 0;
	}

return 1;
}










char FAT32_FILE_Read(char str[]){
	
	if(	FAT32_FILE_Open(str) ==0){

	uint32_t CurrentClust = FirstClustAddr;

	// FAT32[First Data Cluster]--> Next Data Cluster of Data;
	//I.E In the FAT32 Primary FAT, The index of (FirstDataCluster*4) (4 for 4 bytes) gives the index of the next cluster(bits 0-6)and the cluster location (bits 0-511);

	while(1){
		long ConstMin = 512*(int)(CurrentClust/128);
		long loc = RootDirSec+(32*(CurrentClust-2));
		char c=-1;
		uint32_t AddrData;
		
		while(++c<SecsPerClust){
			SD_RSB(DataBuff,loc+c);
			for(short i =0; i<512;i++)
			USART_Int_StrHEXRAW(DataBuff[i],0);
		}

		SD_RSB(DataBuff,FATStartSec+((int)CurrentClust/128));
		
		AddrData = (DataBuff[(0)+(CurrentClust*4) - ConstMin]);
        AddrData+= (long)(DataBuff[(1)+(CurrentClust*4)- ConstMin])*256;
 		AddrData+= (long)(DataBuff[(2)+(CurrentClust*4)- ConstMin]*65536);
		AddrData+= (long)(DataBuff[(3)+(CurrentClust*4)- ConstMin]*16777216);		
		CurrentClust =  (AddrData&FATEntryMask);

	
		
	if(AddrData >= SectorEndingTag)break;
	}
	return 0;
}
return 1;
	}








void ClearClust(uint32_t ClustAddr){
	memset(DataBuff,0,512);
	char c=-1;
	while(++c<SecsPerClust){
		SD_WSB(DataBuff,ClustAddr+c);
		_delay_ms(10);
	}

}










char FAT32_FILE_Delete_Contents(char str[]){
	if(	FAT32_FILE_Open(str) ==0){
	uint32_t CurrentClust = FirstClustAddr;	//
    uint32_t AddrData=0;
	while(FirstClustAddr!=0){
		long loc = RootDirSec+(32*(CurrentClust-2));
		long ConstMin = 512*(int)(CurrentClust/128);

		ClearClust(loc);

		SD_RSB(DataBuff,FATStartSec+((int)CurrentClust/128));

		
				AddrData = (DataBuff[(0)+(CurrentClust*4) - ConstMin]);
				AddrData+= (long)(DataBuff[(1)+(CurrentClust*4)- ConstMin])*256;
				AddrData+= (long)(DataBuff[(2)+(CurrentClust*4)- ConstMin]*65536);
				AddrData+= (long)(DataBuff[(3)+(CurrentClust*4)- ConstMin]*16777216);
		
		
		
		CurrentClust =  (AddrData&FATEntryMask);

		if(AddrData>= SectorEndingTag||AddrData== 0x0)break; 
		
		
		
	//	USART_Send("\r\n Clearing Cluster:");
	//	USART_Long_Str(CurrentClust,0);
	
	
	}
	
	return 0;
}
return 1;

}












char FAT32_Clear_From_FAT(char str[]){
	if(FAT32_FILE_Open(str)==0){
	uint32_t CurrentClust = FirstClustAddr;
    uint32_t AddrData =0;
	while(1){
		long ConstMin = 512*(int)(CurrentClust/128);
	

		SD_RSB(DataBuff,FATStartSec+((int)CurrentClust/128));				
		AddrData = (DataBuff[(0)+(CurrentClust*4) - ConstMin]);
		AddrData+= (long)(DataBuff[(1)+(CurrentClust*4)- ConstMin])*256;
		AddrData+= (long)(DataBuff[(2)+(CurrentClust*4)- ConstMin]*65536);
		AddrData+= (long)(DataBuff[(3)+(CurrentClust*4)- ConstMin]*16777216);
	
										
		DataBuff[(0)+(CurrentClust*4) - ConstMin] =0;
		DataBuff[(1)+(CurrentClust*4) - ConstMin] =0;
		DataBuff[(2)+(CurrentClust*4) - ConstMin] =0;
		DataBuff[(3)+(CurrentClust*4) - ConstMin] =0;
		SD_WSB(DataBuff,FATStartSec+((int)CurrentClust/128));

		CurrentClust =  (AddrData&FATEntryMask);
		
		if(AddrData>= SectorEndingTag||AddrData==0x0){    
				DataBuff[(0)+(CurrentClust*4) - ConstMin*4] =0;
				DataBuff[(1)+(CurrentClust*4) - ConstMin*4] =0;
				DataBuff[(2)+(CurrentClust*4) - ConstMin*4] =0;
				DataBuff[(3)+(CurrentClust*4) - ConstMin*4] =0;
				SD_WSB(DataBuff,FATStartSec+((int)CurrentClust/128));	
				break;}

	}
	return 0;
}
return 1;
}














char FAT32_Clear_From_Dir(char str[]){
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
		return 0;
	} else return 1;
}












void FAT32_FILE_Delete(char str[]){
	FAT32_FILE_Delete_Contents(str);
	_delay_ms(100);
	FAT32_Clear_From_FAT(str);
	_delay_ms(100);
	FAT32_Clear_From_Dir(str);
}










uint32_t FAT32_RETURNING_NEXT_OPEN_CLUST(){
//Assigns First Cluster Address.
uint32_t CurrentClust=2;
uint32_t ConstMin;
uint32_t AddrData;
uint32_t OverFlow;
while(1){
CurrentClust++;
OverFlow = ((int)(CurrentClust/128));
ConstMin = 128*OverFlow;
SD_RSB(DataBuff,FATStartSec+OverFlow);
				
AddrData = (DataBuff[(CurrentClust*4) - ConstMin*4]);
AddrData+= (long)(DataBuff[(1)+(CurrentClust*4)- ConstMin*4])*256;
AddrData+= (long)(DataBuff[(2)+(CurrentClust*4)- ConstMin*4]*65536);
AddrData+= (long)(DataBuff[(3)+(CurrentClust*4)- ConstMin*4]*16777216);

if(AddrData==0) break;

}
	
	
// USART_Send("\r\n FAT32_return_next_clust = ");
// USART_Int_Str(CurrentClust,0);
	
	
return CurrentClust;
}











void FAT32_Update_FATWithNewEndingTAG(long Clust){

 	uint32_t OverFlow = (int)(Clust/128);
	long ConstMin = 512*OverFlow;
	Clust *=4;	
	
	SD_RSB(DataBuff,FATStartSec+OverFlow);
	DataBuff[Clust-ConstMin]= 0xFF;
	DataBuff[(Clust-ConstMin)+1]=0xFF;
	DataBuff[(Clust-ConstMin)+2]=0xFF;
	DataBuff[(Clust-ConstMin)+3] = 0xFF;    // Have a |= here so the top 4bits are reserved
	SD_WSB(DataBuff,FATStartSec+OverFlow);
	
	SD_RSB(DataBuff,FATStartSec+OverFlow);
}












uint32_t FAT32_LocationOfLastClustInFile(char *str){
if(FAT32_FILE_Open(str)==0){
	
uint32_t CurrentClust = FirstClustAddr;	
uint32_t AddrData=0;

while(FirstClustAddr!=0){
	
uint32_t OverFlow = ((int)CurrentClust/128);
uint32_t ConstMin = 512*OverFlow;


SD_RSB(DataBuff,FATStartSec+OverFlow);

AddrData = (DataBuff[(0)+(CurrentClust*4) - ConstMin]);
AddrData+= (long)(DataBuff[(1)+(CurrentClust*4)- ConstMin])*256;
AddrData+= (long)(DataBuff[(2)+(CurrentClust*4)- ConstMin]*65536);
AddrData+= (long)(DataBuff[(3)+(CurrentClust*4)- ConstMin]*16777216);
	
		
		
	if(AddrData >= SectorEndingTag||AddrData== 0x0)break; 

	
	CurrentClust = (AddrData&FATEntryMask);
	

}
// USART_Send("\r\n Location of last clust CHeck 2= ");
// USART_Int_Str(CurrentClust,0);

if(AddrData== 0x0)return 2; else return CurrentClust; 

}



return 1;	
}












// set the OriginalLasClust to Point to NewLastClust 

char FAT32_UpdateFatWithNewClustLoc(char str[], uint32_t NewLastclust, char flag){
uint32_t OriginalLastCl =0;


	if(flag==0)
   OriginalLastCl = FAT32_LocationOfLastClustInFile(str); 
 if(flag ==1) OriginalLastCl=2;
 
if (  OriginalLastCl !=1){


uint32_t OverFlow = (int)(OriginalLastCl/128);
 uint32_t ConstMin = 512*OverFlow;
 

 SD_RSB(DataBuff,FATStartSec+OverFlow);
 DataBuff[OriginalLastCl*4-ConstMin]= NewLastclust&0xff;
 DataBuff[(OriginalLastCl*4-ConstMin)+1]=(NewLastclust&0xff00)>>8;
 DataBuff[(OriginalLastCl*4-ConstMin)+2]=(NewLastclust&0xff0000)>>16;
 DataBuff[(OriginalLastCl*4-ConstMin)+3] = (NewLastclust&0xff000000)>>24; // Have a |= here so the top 4bits are reserved
 SD_WSB(DataBuff,FATStartSec+OverFlow);

	return 0;	
} else return 1; 
return 2;
}









uint8_t FAT32_Append(char str[]){	
uint16_t clust= (FAT32_RETURNING_NEXT_OPEN_CLUST());
if(FAT32_UpdateFatWithNewClustLoc(str,clust,0)==0)
FAT32_Update_FATWithNewEndingTAG(clust);
return 1; 

}





uint8_t FAT32_Append_Cluster_RootDir(){
uint16_t clust= (FAT32_RETURNING_NEXT_OPEN_CLUST());
FAT32_UpdateFatWithNewClustLoc(0,clust,1);
FAT32_Update_FATWithNewEndingTAG(clust);
	return 0;
}
 







// EXAMPLE
 
//Create File
// RTC_Clock_Read(CurrentTimeRegister);
// RTC_Calendar_Read(CurrentTimeRegister);
// FAT32_FILE_Create("DARKEN","txt",CurrentTimeRegister);



void FAT32_FILE_Create(char str[],char type[], char tempstr[]){


	uint16_t Count=4;
	uint16_t CountOvrFlw=0;

	int Addr=0;
	uint32_t out = FAT32_RETURNING_NEXT_OPEN_CLUST();

	SD_RSB(DataBuff,RootDirSec);

	//Finds new Entry in Dir
	while((DataBuff[(Count*32)-(512*CountOvrFlw)]!= 0xe5) && (DataBuff[(Count*32)-(512*CountOvrFlw)]!= 0x0))
	{
		SD_RSB(DataBuff,RootDirSec+CountOvrFlw);
		CountOvrFlw =(int)(Count/16);
		Count++;
		
		
		
		
		
		
		// Set so that The loop breaks if Count exceeds the amount of SecsPerClust and in that instance a new cluster is created for rootdir. Use  --->  FAT32_Append_Cluster_RootDir();
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
uint16_t temp =   ( (0b1111111&((2000+tempstr[7])-1980))<<9) + ((0b1111&tempstr[6])<<5) + (0b11111&tempstr[5]);
	DataBuff[Addr+16]=  temp & 0xFF;
	DataBuff[Addr+17]= (temp>>8);


// convert to military time 
if(tempstr[2]>12) tempstr[2]+=12 ;	
temp =	((0b11111&tempstr[2])<<12) + ((0b111111&tempstr[1])<<6) + (0b111111&tempstr[0]);
	DataBuff[Addr+13]= temp&0b1;
	DataBuff[Addr+14]=  (temp>>1)&0xFF;
	DataBuff[Addr+15]= (temp>>9);



	// Declare Size
	for (int i=28; i<32; i++)
	DataBuff[Addr+i]=0;

	// Date Modified
	for (int i=22; i<26; i++)
	DataBuff[Addr+i]=0x1;



	//Assigning First Cluster
	DataBuff[Addr+21]= (out & 0xFF000000)>>24;
	DataBuff[Addr+20]= (out & 0xFF0000)>>16;
	DataBuff[Addr+27]= (out & 0xFF00)>>8;
	DataBuff[Addr+26]=out & 0xFF;
	SD_WSB(DataBuff,RootDirSec+CountOvrFlw);

	FAT32_Update_FATWithNewEndingTAG(out);
}









char FAT32_FILE_UpdateFileSize(char str[]){
	
	if(FAT32_FILE_Open(str)==0){
		SD_RSB(DataBuff,RootDirSec+(FILELocationInRootDir>>9));
		uint16_t temp= FILELocationInRootDir&511;
		uint32_t  Size = FirstClustAddr-FAT32_LocationOfLastClustInFile(str);
		DataBuff[temp+27]= Size&0xFF;
		DataBuff[temp+28]= (Size& 0xff00)>>8;
		DataBuff[temp+29]= (Size&0xFF0000)>>16;
		DataBuff[temp+30]= (Size&0xFF000000)>>24;
	}
	return 1;
}







	
uint8_t FAT32_FILE_Read_Sector_In_Cluster(char str[], long Clust,uint8_t Sect){
	
if(Sect<SecsPerClust){
		
if(	FAT32_FILE_Open(str) ==0){
		
uint32_t CurrentClust = FirstClustAddr;
uint32_t AddrData =0;
uint16_t TempCount=0;
	
while(Clust!=0){
		
long ConstMin = 512*(int)(CurrentClust/128);
SD_RSB(DataBuff,FATStartSec+((int)CurrentClust/128));
		
AddrData = (DataBuff[(0)+(CurrentClust*4) - ConstMin]);
AddrData+= (long)(DataBuff[(1)+(CurrentClust*4)- ConstMin])*256;
AddrData+= (long)(DataBuff[(2)+(CurrentClust*4)- ConstMin]*65536);
AddrData+= (long)(DataBuff[(3)+(CurrentClust*4)- ConstMin]*16777216);
		
		
if(AddrData >= SectorEndingTag||AddrData== 0x0||TempCount>Clust)break; // If not working check ending flag. I set this for the file I was working on and might nbot be right.
CurrentClust =  (AddrData&FATEntryMask);
TempCount++;
}
		
		
if(TempCount<Clust){memset(DataBuff,0xFF,512); return 2;}// Cluster Dose not exist
	
SD_RSB(DataBuff,(RootDirSec+(32*(CurrentClust-2))+Sect));	
USART_Send("\r\n Cluster Addr |||=  ");
USART_Long_StrHEX(CurrentClust,0);
USART_Send("\r\n TempCount |||=  ");
USART_Long_StrHEX(TempCount,0);
return 0; // Complete With No errors 
	
}
return 1; // File Dose not Exist 
}
return 3; // Sector Called Outside Cluster Size (Over Sectors Per Clust)
}












char FAT32_FILE_Write_Sector_In_Cluster(char str[], long Clust,uint8_t Sect){

		SD_WSB(DataBuff,(RootDirSec+(32*(ReseveClust-2))+Sect));
	
	if(Sect<SecsPerClust){	
	if(	FAT32_FILE_Open(str) ==0){
		
	uint32_t CurrentClust = FirstClustAddr;
	char TempCount=0;


uint32_t AddrData =0;

	while(Clust!=0){
		
		long ConstMin = 512*(int)(CurrentClust/128);
		SD_RSB(DataBuff,FATStartSec+((int)CurrentClust/128));
		
		AddrData = (DataBuff[(0)+(CurrentClust*4) - ConstMin]);
		AddrData+= (long)(DataBuff[(1)+(CurrentClust*4)- ConstMin])*256;
		AddrData+= (long)(DataBuff[(2)+(CurrentClust*4)- ConstMin]*65536);
		AddrData+= (long)(DataBuff[(3)+(CurrentClust*4)- ConstMin]*16777216);
		
		
		if(AddrData >= SectorEndingTag||AddrData== 0x0||TempCount>Clust)break; 
		CurrentClust =  (AddrData&FATEntryMask);
		TempCount++;
	}
	
	if(TempCount<Clust){memset(DataBuff,0xFF,512);   return 2; }// Cluster Dose not exist

	SD_RSB(DataBuff,(RootDirSec+(32*(ReseveClust-2))+Sect));
	SD_WSB(DataBuff,(RootDirSec+(32*(CurrentClust-2))+Sect));
	
	return 0; // Complete With No errors 
	
}
return 1; // File Dose not Exist 
	}
return 3; // Sector Called Outside Cluster Size (Over Sectors Per Clust)
	}
#endif
