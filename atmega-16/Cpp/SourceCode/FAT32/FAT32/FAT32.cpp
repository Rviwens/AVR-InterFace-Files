//************************************************************//
#include "FAT32.h"
//************************************************************//
// For reading and writing files in FAT32
FAT32_File::FAT32_File(char *FileName, char *Filetype){
	Fname=FileName;
	type=Filetype;
}
//************************************************************//
FAT32_File::FAT32_File(const char *FileName, const char *Filetype){
	Fname=(char*)FileName;
	type=(char*)Filetype;
}
//************************************************************//
char* FAT32_File::getFname(){return Fname;}
char* FAT32_File::getType(){return type;}
void FAT32_File::setFname(char *FileName){Fname=FileName;}
void FAT32_File::setType(char *FileType){type=FileType;}
//************************************************************//
// Find FAT32 Root Directory
void FAT32_File::Init(char partitionNUM){
	partitionNUM++;
	card.RSB(DataBuff,0);
	
	if(DataBuff[446+(16*partitionNUM)-12] ==0xC ||DataBuff[446+(16*partitionNUM)-12] == 0xb){
		LBA = (DataBuff[446+(16*partitionNUM)-8]);
		LBA += (long)(DataBuff[446+(16*partitionNUM)-7]* 256);
		LBA += (long)(DataBuff[446+(16*partitionNUM)-6]* 65536);
		LBA += (long)(DataBuff[446+(16*partitionNUM)-5]* 16777216);
	} else LBA=0;
	
	card.RSB(DataBuff,LBA);
	if(DataBuff[511]==0xaa && DataBuff[510] == 0x55){
		SecsPerClust =(DataBuff[13]);
		FATStartSec= (DataBuff[15]*256+DataBuff[14])+LBA;
		FatSecs = (((DataBuff[38]*65536) + ((unsigned) DataBuff[37]*256) + (DataBuff[36])) *DataBuff[16]);
		RootDirSec = FATStartSec+FatSecs;
		BytesPerSec= (DataBuff[12]*256+DataBuff[11]);
	}
}
//************************************************************//
uint8_t FAT32_File::Open(){
	static uint16_t locX;
	short flag=1;
	uint32_t loci;
	for (uint16_t i=0; i<SecsPerClust; i++){
		card.RSB(DataBuff,RootDirSec+i);
		for(uint16_t x=0; x<= 479; x++){
			flag=1;

			if(DataBuff[x]==Fname[0]){
				for(uint8_t y=0; y <strlen(Fname); y++)
				if (DataBuff[x+y]!=Fname[0+y])flag=0;
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
		//Attr=DataBuff[locX+0x0B]
		FILELocationInRootDir = (loci<<9)+locX;
		FirstClustAddr = (65536*(DataBuff[locX+0x15])) + (4096*(DataBuff[locX+0x14]))+(256*(DataBuff[locX+0x1B]))+(DataBuff[locX+0x1a]);
		return 0;
	}
	return 1;
}
//************************************************************//
void FAT32_File::Check(){
	if(Open()==0)usart.Send("\r\nFile Found");else usart.Send("\r\nFile not Found");
}

void FAT32_File::Report(){
	usart.Send("\r\n Secs Per Clust = ");
	usart.Long_Str(SecsPerClust,0);
	usart.Send("\r\n FAT Start Sec = ");
	usart.Long_Str(FATStartSec,0);
	usart.Send("\r\n FATSecs = ");
	usart.Long_Str(FatSecs,0);
	usart.Send("\r\n RootDir = ");
	usart.Long_Str(RootDirSec,0);
	usart.Send("\r\n Bytes Per Sec = ");
	usart.Long_Str(BytesPerSec,0);
	usart.Send("\r\n");
	Check();
	usart.Send("\r\n Name of File is: ");
	usart.Send(Fname);
	usart.Send(" | Type of File is: ");
	usart.Send(type);
	usart.Send(" | FCA: ");
	usart.Long_Str(FirstClustAddr,0);
	usart.Send(" | FLIRD: ");
	usart.Long_Str(FILELocationInRootDir,0);
	usart.Send("\r\n");
}
//************************************************************//
uint8_t FAT32_File::Read(){
	
	if(Open() ==0){

		uint32_t CurrentClust = FirstClustAddr;

		// FAT32[First Data Cluster]--> Next Data Cluster of Data;
		//I.E In the FAT32 Primary FAT, The index of (FirstDataCluster*4) (4 for 4 bytes) gives the index of the next cluster(bits 0-6)and the cluster location (bits 0-511);

		while(1){
			long ConstMin = 512*(int)(CurrentClust/128);
			long loc = RootDirSec+(32*(CurrentClust-2));
			char c=-1;
			uint32_t AddrData;
			
			while(++c<SecsPerClust){
				
				
				usart.Send("\r\n Sector: ");
				usart.Int_Str(c,0);
				usart.Send("\r\n");
				
				card.RSB(DataBuff,loc+c);
				for(short i =0; i < 512;i++)
				usart.Int_StrHEXRAW(DataBuff[i],0);
			}

			card.RSB(DataBuff,FATStartSec+((int)CurrentClust/128));
			
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
//************************************************************//
void FAT32_File::ClearClust(uint32_t ClustAddr){
	memset(DataBuff,0,512);
	char c=-1;
	while(++c<SecsPerClust){
		card.WSB(DataBuff,ClustAddr+c);
		_delay_ms(10);
	}

}
//************************************************************//
uint8_t FAT32_File::Delete_Contents(void){
	if(Open()==0){
		uint32_t CurrentClust = FirstClustAddr;	//
		uint32_t AddrData=0;
		while(FirstClustAddr!=0){
			long loc = RootDirSec+(32*(CurrentClust-2));
			long ConstMin = 512*(int)(CurrentClust/128);

			ClearClust(loc);

			card.RSB(DataBuff,FATStartSec+((int)CurrentClust/128));

			
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
//************************************************************//
uint8_t FAT32_File::Clear_From_FAT(){
	if(Open()==0){
		uint32_t CurrentClust = FirstClustAddr;
		uint32_t AddrData =0;
		while(1){
			long ConstMin = 512*(int)(CurrentClust/128);
			

			card.RSB(DataBuff,FATStartSec+((int)CurrentClust/128));
			AddrData = (DataBuff[(0)+(CurrentClust*4) - ConstMin]);
			AddrData+= (long)(DataBuff[(1)+(CurrentClust*4)- ConstMin])*256;
			AddrData+= (long)(DataBuff[(2)+(CurrentClust*4)- ConstMin]*65536);
			AddrData+= (long)(DataBuff[(3)+(CurrentClust*4)- ConstMin]*16777216);
			
			
			DataBuff[(0)+(CurrentClust*4) - ConstMin] =0;
			DataBuff[(1)+(CurrentClust*4) - ConstMin] =0;
			DataBuff[(2)+(CurrentClust*4) - ConstMin] =0;
			DataBuff[(3)+(CurrentClust*4) - ConstMin] =0;
			card.WSB(DataBuff,FATStartSec+((int)CurrentClust/128));

			CurrentClust =  (AddrData&FATEntryMask);
			
			if(AddrData>= SectorEndingTag||AddrData==0x0){
				DataBuff[(0)+(CurrentClust*4) - ConstMin*4] =0;
				DataBuff[(1)+(CurrentClust*4) - ConstMin*4] =0;
				DataBuff[(2)+(CurrentClust*4) - ConstMin*4] =0;
				DataBuff[(3)+(CurrentClust*4) - ConstMin*4] =0;
				card.WSB(DataBuff,FATStartSec+((int)CurrentClust/128));
			break;}

		}
		return 0;
	}
	return 1;
}
//************************************************************//
uint8_t FAT32_File::Clear_From_Dir(){
	static uint16_t locX;
	static uint16_t locI;
	short flag=1;

	for (int i=0; i<0x20; i++){
		card.RSB(DataBuff,RootDirSec+i);
		for(uint16_t x=0; x<= 511; x++){
			flag=1;
			if(DataBuff[x]==Fname[0]){
				for(uint8_t y=0; y <strlen(Fname); y++)
				if (DataBuff[x+y]!=Fname[0+y])flag=0;
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
		card.RSB(DataBuff,RootDirSec+locI);
		DataBuff[locX]=0xe5;
		card.WSB(DataBuff,RootDirSec+locI);
		return 0;
	} else return 1;
}
//************************************************************//
void FAT32_File::Delete(){
	if (Delete_Contents()==0){
		_delay_ms(100);
		Clear_From_FAT();
		_delay_ms(100);
		Clear_From_Dir();
	}
}
//************************************************************//
uint32_t FAT32_File::RETURNING_NEXT_OPEN_CLUST(){
	//Assigns First Cluster Address.
	uint32_t CurrentClust=2;
	uint32_t ConstMin;
	uint32_t AddrData;
	uint32_t OverFlow;
	while(1){
		CurrentClust++;
		OverFlow = ((int)(CurrentClust/128));
		ConstMin = 128*OverFlow;
		card.RSB(DataBuff,FATStartSec+OverFlow);
		
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
//************************************************************//
void FAT32_File::Update_FATWithNewEndingTAG(long Clust){

	uint32_t OverFlow = (int)(Clust/128);
	long ConstMin = 512*OverFlow;
	Clust *=4;
	
	card.RSB(DataBuff,FATStartSec+OverFlow);
	DataBuff[Clust-ConstMin]= 0xFF;
	DataBuff[(Clust-ConstMin)+1]=0xFF;
	DataBuff[(Clust-ConstMin)+2]=0xFF;
	DataBuff[(Clust-ConstMin)+3] = 0xFF;    // Have a |= here so the top 4bits are reserved
	card.WSB(DataBuff,FATStartSec+OverFlow);
	
	card.RSB(DataBuff,FATStartSec+OverFlow);
}
//************************************************************//
uint32_t FAT32_File::LocationOfLastClustInFile(){
	if(Open()==0){
		
		uint32_t CurrentClust = FirstClustAddr;
		uint32_t AddrData=0;

		while(FirstClustAddr!=0){
			
			uint32_t OverFlow = ((int)CurrentClust/128);
			uint32_t ConstMin = 512*OverFlow;


			card.RSB(DataBuff,FATStartSec+OverFlow);

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
//************************************************************//
// set the OriginalLasClust to Point to NewLastClust
char FAT32_File::UpdateFatWithNewClustLoc(uint32_t NewLastclust, char flag){
	uint32_t OriginalLastCl =0;


	if(flag==0)
	OriginalLastCl =LocationOfLastClustInFile();
	if(flag ==1) OriginalLastCl=2;
	
	if (  OriginalLastCl !=1){


		uint32_t OverFlow = (int)(OriginalLastCl/128);
		uint32_t ConstMin = 512*OverFlow;
		

		card.RSB(DataBuff,FATStartSec+OverFlow);
		DataBuff[OriginalLastCl*4-ConstMin]= NewLastclust&0xff;
		DataBuff[(OriginalLastCl*4-ConstMin)+1]=(NewLastclust&0xff00)>>8;
		DataBuff[(OriginalLastCl*4-ConstMin)+2]=(NewLastclust&0xff0000)>>16;
		DataBuff[(OriginalLastCl*4-ConstMin)+3] = (NewLastclust&0xff000000)>>24; // Have a |= here so the top 4bits are reserved
		card.WSB(DataBuff,FATStartSec+OverFlow);

		return 0;
	} else return 1;
	return 2;
}
//************************************************************//
uint8_t FAT32_File::Append(){
	uint16_t clust= (RETURNING_NEXT_OPEN_CLUST());
	if(UpdateFatWithNewClustLoc(clust,0)==0)
	Update_FATWithNewEndingTAG(clust);
	return 1;

}
//************************************************************//
uint8_t FAT32_File::Append_Cluster_RootDir(){
	uint16_t clust= (RETURNING_NEXT_OPEN_CLUST());
	UpdateFatWithNewClustLoc(clust,1);
	Update_FATWithNewEndingTAG(clust);
	return 0;
}
//************************************************************//
// EXAMPLE

//Create File
// RTC_Clock_Read(CurrentTimeRegister);
// RTC_Calendar_Read(CurrentTimeRegister);
// FAT32_FILE_Create("DARKEN","txt",CurrentTimeRegister);

//************************************************************//
uint8_t FAT32_File::Create(char tempstr[]){

	if(Open()==1){
		
		uint16_t Count=4;
		uint16_t CountOvrFlw=0;

		int Addr=0;
		uint32_t out = RETURNING_NEXT_OPEN_CLUST();
		FirstClustAddr =out;
		
		card.RSB(DataBuff,RootDirSec);

		//Finds new Entry in Dir
		while((DataBuff[(Count*32)-(512*CountOvrFlw)]!= 0xe5) && (DataBuff[(Count*32)-(512*CountOvrFlw)]!= 0x0))
		{
			card.RSB(DataBuff,RootDirSec+CountOvrFlw);
			CountOvrFlw =(int)(Count/16);
			Count++;
			// Set so that The loop breaks if Count exceeds the amount of SecsPerClust and in that instance a new cluster is created for rootdir. Use  --->  FAT32_Append_Cluster_RootDir();
		}
		
		Addr =(Count*32)-(512*CountOvrFlw);

		//Writes New Entry Name, type, and ATTR in Dir
		for (uint8_t i=0; i<(11-strlen(type)); i++){
			if(i>=strlen(Fname)) DataBuff[Addr+i]=0x20; else DataBuff[Addr+i]=Fname[i];
		}
		//Type
		for (int i=11-strlen(type); i<11; i++)
		DataBuff[Addr+i]=type[i-(11-strlen(type))];

		//Attr
		DataBuff[Addr+11]=0x20;
		
		//Upper Case or lowercase
		DataBuff[Addr+12]=0x00;
		
		if (tempstr!=0){
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
		}
		
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
		card.WSB(DataBuff,RootDirSec+CountOvrFlw);

		Update_FATWithNewEndingTAG(out);
		return 0;
	}
	else return 1;
}
//************************************************************//
uint8_t FAT32_File::UpdateFileSize(){
	
	if(Open()==0){
		card.RSB(DataBuff,RootDirSec+(FILELocationInRootDir>>9));
		uint16_t temp= FILELocationInRootDir&511;
		uint32_t  Size = FirstClustAddr-LocationOfLastClustInFile();
		DataBuff[temp+27]= Size&0xFF;
		DataBuff[temp+28]= (Size& 0xff00)>>8;
		DataBuff[temp+29]= (Size&0xFF0000)>>16;
		DataBuff[temp+30]= (Size&0xFF000000)>>24;
	}
	return 1;
}
//************************************************************//
uint8_t FAT32_File::RSIC(long Clust,uint8_t Sect){
	
	if(Sect<SecsPerClust){
		
		if(Open() ==0){
			
			uint32_t CurrentClust = FirstClustAddr;
			uint32_t AddrData =0;
			uint16_t TempCount=0;
			
			while(Clust!=0){
				
				long ConstMin = 512*(int)(CurrentClust/128);
				card.RSB(DataBuff,FATStartSec+((int)CurrentClust/128));
				
				AddrData = (DataBuff[(0)+(CurrentClust*4) - ConstMin]);
				AddrData+= (long)(DataBuff[(1)+(CurrentClust*4)- ConstMin])*256;
				AddrData+= (long)(DataBuff[(2)+(CurrentClust*4)- ConstMin]*65536);
				AddrData+= (long)(DataBuff[(3)+(CurrentClust*4)- ConstMin]*16777216);
				
				
				if(AddrData >= SectorEndingTag||AddrData== 0x0||TempCount>Clust)break; // If not working check ending flag. I set this for the file I was working on and might nbot be right.
				CurrentClust =  (AddrData&FATEntryMask);
				TempCount++;
			}
			
			
			if(TempCount<Clust){memset(DataBuff,0xFF,512); return 2;}// Cluster Dose not exist
			
			card.RSB(DataBuff,(RootDirSec+(32*(CurrentClust-2))+Sect));
			// 			USART_Send("\r\n Cluster Addr |||=  ");
			// 			USART_Long_StrHEX(CurrentClust,0);
			// 			USART_Send("\r\n TempCount |||=  ");
			// 			USART_Long_StrHEX(TempCount,0);
			return 0; // Complete With No errors
			
		}
		return 1; // File Dose not Exist
	}
	return 3; // Sector Called Outside Cluster Size (Over Sectors Per Clust)
}
//************************************************************//
uint8_t FAT32_File::WSIC(long Clust,uint8_t Sect){
	card.WSB(DataBuff,(RootDirSec+(32*(ReseveClust-2))+Sect));
	_delay_ms(50);
	if(Sect<SecsPerClust){

		if(Open()==0){
			
			uint32_t CurrentClust = FirstClustAddr;
			char TempCount=0;


			uint32_t AddrData =0;

			while(Clust!=0){
				
				long ConstMin = 512*(int)(CurrentClust/128);
				card.RSB(DataBuff,FATStartSec+((int)CurrentClust/128));
				
				AddrData = (DataBuff[(0)+(CurrentClust*4) - ConstMin]);
				AddrData+= (long)(DataBuff[(1)+(CurrentClust*4)- ConstMin])*256;
				AddrData+= (long)(DataBuff[(2)+(CurrentClust*4)- ConstMin]*65536);
				AddrData+= (long)(DataBuff[(3)+(CurrentClust*4)- ConstMin]*16777216);
				
				
				if(AddrData >= SectorEndingTag||AddrData== 0x0||TempCount>Clust)break;
				CurrentClust =  (AddrData&FATEntryMask);
				TempCount++;
			}
			
			if(TempCount<Clust){memset(DataBuff,0xFF,512);return 2; }// Cluster Dose not exist

			card.RSB(DataBuff,(RootDirSec+(32*(ReseveClust-2))+Sect));
			card.WSB(DataBuff,(RootDirSec+(32*(CurrentClust-2))+Sect));
			
			return 0; // Complete With No errors
			
		}
		return 1; // File Dose not Exist
	}
	//returned 1 - Outside Cluster Size (Over Sectors Per Clust)
return 0;
}
//************************************************************//