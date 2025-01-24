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



typedef struct{
	char *Fname;
	char *type;
} File;

#if defined(FAT32)

// Init vars
uint32_t RootDirSec;
uint32_t FATStartSec;
uint32_t FatSecs;
uint32_t LBA;
uint32_t SecsPerClust;
uint32_t BytesPerSec;
uint32_t FSInfo;
uint32_t FILELocationInRootDir;
uint32_t FirstClustAddr;
#endif
//prototypes


/*
* Reads the FAT32 MBR for allocation info
* @param partionNum - The partition being accessed
*/
void FAT32_Init(char partitionNUM);
/*
* Finds the first cluster and location in the root directory of a file
* @param file - A File to be opened (typedef) 
* @return 0 - No error, file found
* @return 2 - File not Found
*/
uint8_t FAT32_FILE_Open( File file);
/*
* Checks if the File exists
* @param file - A File to be opened (typedef) 
*/
void FAT32_FILE_Check(File file);
/*
* Reports MBR information
* @param file - A File to be opened (typedef)
*/
void FAT32_FILE_Report( File file);

#if defined(USARTTX)
/*
* Reads the entire contents of a File and outputs the contents through USART
* @param file - A File to be opened (typedef)
* @return 0 - No error, file found
* @return 2 - File not Found
*/
	uint8_t FAT32_FILE_Read(File file);
#endif
/*
* Clears the contents in a cluster
* @param ClustAddr - The address of the cluster 
*/
void ClearClust(uint32_t ClustAddr);
/*
* Deletes the contents of a File
* @param file - A File to be opened (typedef)
* @return 0 - No error, file found
* @return 2 - File not Found
*/
uint8_t FAT32_FILE_Delete_Contents( File file);
/*
* Clears a files entry in the FAT
* @param file - A File to be opened (typedef)
* @return 0 - No error, file found
* @return 2 - File not Found
*/
uint8_t FAT32_Clear_From_FAT(File file);
/*
* Clears a files entry form the root directory 
* @param file - A File to be opened (typedef)
* @return 0 - No error, file found
* @return 2 - File not Found
*/
uint8_t FAT32_Clear_From_Dir( File file);
/*
* Deletes a file from the FAT
* @param file - A File to be opened (typedef)
* @return 0 - No error, file found
* @return 2 - File not Found
*/
void FAT32_FILE_Delete(File file);
/*
* Returns the next open cluster
* @return CurrentClust - The next cluster that is unallocated
*/
uint32_t FAT32_RETURNING_NEXT_OPEN_CLUST();
/*
* Updates the FAT ending tag
* @param Clust - Address of new allocated cluster
*/
void FAT32_Update_FATWithNewEndingTAG(long Clust);
/*
* Finds the location of the last cluster of a file
* @param file - A File to be opened (typedef)
* @return 0 - No error, file found
* @return 2 - File not Found
*/
uint32_t FAT32_LocationOfLastClustInFile( File file);
/*
* Updates the FAT with new allocated cluster location
* @param file - A File to be opened (typedef)
* @param NewLastclust -The location of the last cluster
* @param flag - reserved
* @return 0 - No error, file found
* @return 2 - File not Found
*/
char FAT32_UpdateFatWithNewClustLoc(File file, uint32_t NewLastclust, char flag);
/*
* Appends a new cluster to a file
* @param file - A File to be opened (typedef)
* @return 0 - No error, file found
* @return 2 - File not Found
*/
uint8_t FAT32_Append( File file);
/*
* Appends a new cluster to the root directory
* @param file - A File to be opened (typedef)
* @return 0 - No error, file found
* @return 2 - File not Found
*/
uint8_t FAT32_Append_Cluster_RootDir();
/*
* Creates a file 
* @param file - A File to be opened (typedef)
* @param tempstr - an array that stores the current time and date, or write as 0
* @return 0 - No error, file found
* @return 2 - File not Found
*/
uint8_t FAT32_FILE_Create( File file, char tempstr[]);
/*
* Updates the size of the file in the root directory entry
* @param file - A File to be opened (typedef)
* @return 0 - No error, file found
* @return 2 - File not Found
*/
uint8_t FAT32_FILE_UpdateFileSize( File file);
/*
* Reads a desired sector in a desired cluster of a file, stores in DataBuff
* @param file - A File to be opened (typedef)
* @param Clust - the cluster relative to the file, begins at 0
* @param Sect -the sector relative to the cluster, begins at 0
* @return 0 - No error, file found
* @return 2 - File not Found
*/
uint8_t FAT32_FILE_Read_Sector_In_Cluster( File file, long Clust,uint8_t Sect);
/*
* Writes a desired sector in a desired cluster of a file, writes DataBuff
* @param file - A File to be opened (typedef)
* @param Clust - the cluster relative to the file, begins at 0
* @param Sect -the sector relative to the cluster, begins at 0
* @return 0 - No error, file found
* @return 2 - File not Found
*/
uint8_t FAT32_FILE_Write_Sector_In_Cluster( File Wfile, long Clust,uint8_t Sect);