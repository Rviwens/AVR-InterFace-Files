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



static struct File{
	char*Fname;
	char *type;
	uint32_t FILELocationInRootDir;
	uint32_t FirstClustAddr;
};

//prototypes


/*
* Reads the FAT32 MBR for allocation info
* @param partionNum - The partition being accessed
*/
void FAT32_Init(char partitionNUM);

/*
* Finds the first cluster and location in the root directory of a file
* @param partionNum - The partition being accessed
* @return
*/

uint8_t FAT32_FILE_Open(struct File file);

void FAT32_FILE_Check(struct File file);

void FAT32_FILE_Report(struct File file);

uint8_t FAT32_FILE_Read(struct File file);

void ClearClust(uint32_t ClustAddr);

uint8_t FAT32_FILE_Delete_Contents(struct File file);

uint8_t FAT32_Clear_From_FAT(struct File file);

uint8_t FAT32_Clear_From_Dir(struct File file);

void FAT32_FILE_Delete(struct File file);

uint32_t FAT32_RETURNING_NEXT_OPEN_CLUST();

void FAT32_Update_FATWithNewEndingTAG(long Clust);

uint32_t FAT32_LocationOfLastClustInFile(struct File file);

char FAT32_UpdateFatWithNewClustLoc(struct File file, uint32_t NewLastclust, char flag);

uint8_t FAT32_Append(struct File file);

uint8_t FAT32_Append_Cluster_RootDir();

uint8_t FAT32_FILE_Create(struct File file, char tempstr[]);

uint8_t FAT32_FILE_UpdateFileSize(struct File file);

uint8_t FAT32_FILE_Read_Sector_In_Cluster(struct File file, long Clust,uint8_t Sect);

uint8_t FAT32_FILE_Write_Sector_In_Cluster(struct File Wfile, long Clust,uint8_t Sect);