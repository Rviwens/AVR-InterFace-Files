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
uint32_t ClustAddr;



// Find FAT32 Root Directory
char FAT32_Init(){
	SD_RSB(StatusBuff,0,&token);
	if((StatusBuff[510]==0x55)&&(StatusBuff[511]==0xAA)){
		USART_Send("\r\n Valid");
		}else{return 0;}
		FATStartSec= (StatusBuff[15]*256+StatusBuff[14]);
		FatSecs = (((StatusBuff[38]*65536) + ((unsigned) StatusBuff[37]*256) + (StatusBuff[36])) *StatusBuff[16]);	
		RootDirSec = FATStartSec+FatSecs;
		return 1;
	}

	//Find File location if Root Directory and First Data cluster of file
	void FAT32_Open_File(char str[]){
		char locX =0;
		char locI =0;
		short flag=1;
		char Attr =0;
		//uint32_t Clust;
		RDS = RootDirSec;
		//Find Location of file
		for (int i=0; i<0xFFFF; i++){
			
			SD_RSB(StatusBuff,RootDirSec+i,&token);
			for(int x=0; x<= 511; x++){
				
				if(StatusBuff[x]==str[0]){
					flag=1;
					
					for(int y=0; y <strlen(str); y++){
						if (StatusBuff[x+y]!=str[0+y]){flag=0;}
					}
					
					if(flag!=0){
						locX=x;
						locI=i;
						USART_Send("\r\nFound\r\n");
						flag =2;
						break;
					}
					if(flag==2){break;}
				}
				if(flag==2){break;}
			}
			if(flag==2){break;}
		}

		if(flag !=0){
			
			SD_RSB(StatusBuff,RootDirSec+locI,&token);
			Attr=StatusBuff[locX+0x0B];
			ClustAddr = (65536*(StatusBuff[locX+0x15])) + (4096*(StatusBuff[locX+0x14])) +(256*(StatusBuff[locX+0x1B])) + (StatusBuff[locX+0x1a]);

		}
	}



	//
	void FAT32_Read_File(char str[]){
		FAT32_Open_File(str);
        
		
// 	SD_RSB(StatusBuff,FATStartSec,&token);	
// 	
// 	 for(int t = 0; t<512; t++){
// 	 	USART_Int_StrHEXRAW(StatusBuff[t],0);
// 	 }



		for (long Sc = RootDirSec+(8*(ClustAddr-2));Sc<=(RootDirSec+(8*(ClustAddr-2)))+5 ;Sc++)
		{
			SD_RSB(StatusBuff,Sc,&token);
			
			for (int rd =0; rd<=512; rd++)
			USART_Int_StrHEXRAW(StatusBuff[rd],0);

		}
	}


	void FAT32_Create_File(){
		
	}


	void FAT32_Delete_File(){
		
	}
	#endif




	// USART_Int_StrHEXRAW(StatusBuff[locX+0x14+1],0);
	// USART_Int_StrHEXRAW(StatusBuff[locX+0x14],0);
	// USART_Int_StrHEXRAW(StatusBuff[locX+0x1a+1],0);
	// USART_Int_StrHEXRAW(StatusBuff[locX+0x1a],0);
	// USART_Send("\r\n");
	// USART_Send("ClustAddr =");
	// USART_Long_Str(ClustAddr,0);
	// USART_Send("\r\n");

	// for(int t = locX; t<locX+32; t++){
	// 	USART_Send("\r\nIndex =");
	// 	USART_Int_Str(t,0);
	// 	USART_Send("=| ");
	// 	USART_Int_StrHEXRAW(StatusBuff[t],0);
	// }
	//
	// for(int t = 0; t<512; t++){
	// 	USART_Send("\r\nIndex =");
	// 	USART_Int_Str(t,0);
	// 	USART_Send("=| ");
	// 	USART_Int_StrHEXRAW(StatusBuff[t],0);
	// }





	// 	 USART_Send("\r\n |");
	// 	 USART_Send("Index =");
	// 	 USART_Int_Str(i,0);
	// 	 USART_Send("| ");







