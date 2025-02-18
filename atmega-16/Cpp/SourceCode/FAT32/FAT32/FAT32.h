#ifndef F32
#define F32

#ifndef F_CPU
#define F_CPU 20000000UL
#endif

#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <USART.h>
#include <SPI.h>
#include <MicroSD.h>

#define ReseveClust  500
#define SectorEndingTag 0x0FFFFFFF
#define FATEntryMask    0xFFFFFFFF

//************************************************************//
class FAT32_File{
//************************************************************//
	public:
	uint8_t DataBuff[512];
	//default constructor
	FAT32_File();

	FAT32_File(char *Fname, char *type);
	FAT32_File(const char *Fname, const char *type);
	//Accessors & modifiers  
	char* getFname();
	char* getType();
	void setFname(char *FileName);
	void setType(char *FileType);
	//prototypes
	/*
	* Reads the FAT32 MBR for allocation info
	* @param partionNum - The partition being accessed
	*/
	void Init(char partitionNUM);
	/*
	* Finds the first cluster and location in the root directory of a file
	* @param file - A File to be opened (typedef)
	* @return 0 - No error, file found
	* @return 2 - File not Found
	*/
	uint8_t Open();
	/*
	* Checks if the File exists
	* @param file - A File to be opened (typedef)
	*/
	void Check();
	/*
	* Reports MBR information
	* @param file - A File to be opened (typedef)
	*/
	void Report();
	/*
	* Reads the entire contents of a File and outputs the contents through USART
	* @param file - A File to be opened (typedef)
	* @return 0 - No error, file found
	* @return 2 - File not Found
	*/uint8_t Read();
	/*
	* Deletes a file from the FAT
	* @param file - A File to be opened (typedef)
	* @return 0 - No error, file found
	* @return 2 - File not Found
	*/
	void Delete();
	/*
	* Appends a new cluster to a file
	* @param file - A File to be opened (typedef)
	* @return 0 - No error, file found
	* @return 2 - File not Found
	*/
	uint8_t Append();
	/*
	* Appends a new cluster to the root dir
	* @return 0 - No error, file found
	* @return 2 - Error
	*/
	uint8_t Append_Cluster_RootDir();
	/*
	* Appends a new cluster to the root directory
	* @param file - A File to be opened (typedef)
	* @return 0 - No error, file found
	* @return 2 - File not Found
	*/
	uint8_t RootDir();
	/*
	* Creates a file
	* @param file - A File to be opened (typedef)
	* @param tempstr - an array that stores the current time and date, or write as 0
	* @return 0 - No error, file found
	* @return 2 - File not Found
	*/
	uint8_t Create(char tempstr[]);
	/*
	* Updates the size of the file in the root directory entry
	* @param file - A File to be opened (typedef)
	* @return 0 - No error, file found
	* @return 2 - File not Found
	*/
	uint8_t UpdateFileSize();
	/*
	* Reads a desired sector in a desired cluster of a file, stores in DataBuff
	* @param file - A File to be opened (typedef)
	* @param Clust - the cluster relative to the file, begins at 0
	* @param Sect -the sector relative to the cluster, begins at 0
	* @return 0 - No error, file found
	* @return 2 - File not Found
	*/
	uint8_t RSIC(long Clust,uint8_t Sect);
	/*
	* Writes a desired sector in a desired cluster of a file, writes DataBuff
	* @param file - A File to be opened (typedef)
	* @param Clust - the cluster relative to the file, begins at 0
	* @param Sect -the sector relative to the cluster, begins at 0
	* @return 0 - No error, file found
	* @return 2 - File not Found
	*/
	uint8_t WSIC(long Clust,uint8_t Sect);
	//************************************************************//
	private:
	// Private members
	uint32_t RootDirSec, FATStartSec, FatSecs,LBA,SecsPerClust,BytesPerSec, FSInfo, FILELocationInRootDir, FirstClustAddr;
	char *Fname;
	char *type;
	MicroSD_Card card;
	USART usart; 
	//Private prototypes
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
	uint8_t Delete_Contents();
	/*
	* Clears a files entry in the FAT
	* @param file - A File to be opened (typedef)
	* @return 0 - No error, file found
	* @return 2 - File not Found
	*/
	uint8_t Clear_From_FAT();
	/*
	* Clears a files entry form the root directory
	* @param file - A File to be opened (typedef)
	* @return 0 - No error, file found
	* @return 2 - File not Found
	*/
	uint8_t Clear_From_Dir();
	/*
	* Returns the next open cluster
	* @return CurrentClust - The next cluster that is unallocated
	*/
	uint32_t RETURNING_NEXT_OPEN_CLUST();
	/*
	* Updates the FAT ending tag
	* @param Clust - Address of new allocated cluster
	*/
	void Update_FATWithNewEndingTAG(long Clust);
	/*
	* Finds the location of the last cluster of a file
	* @param file - A File to be opened (typedef)
	* @return 0 - No error, file found
	* @return 2 - File not Found
	*/
	uint32_t LocationOfLastClustInFile();
	/*
	* Updates the FAT with new allocated cluster location
	* @param file - A File to be opened (typedef)
	* @param NewLastclust -The location of the last cluster
	* @param flag - reserved
	* @return 0 - No error, file found
	* @return 2 - File not Found
	*/
	char UpdateFatWithNewClustLoc(uint32_t NewLastclust, char flag);

	//************************************************************//
};
//************************************************************//
#endif