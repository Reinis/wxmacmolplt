/*
 *  (c) 2004-2008 Iowa State University
 *      see the LICENSE file in the top level directory
 */

/*
	BFiles.h

	Buffered File class definitions

	Brett Bode - June 1996
	Adapted for Std C IO for Rhapsody - 2/98 BMB
*/
#ifndef __BFILE__	//make sure this header is not already included
#define __BFILE__
#include <stdio.h>
#include "Globals.h"

#define	kBufferSize		262144		///<default file buffer size (for BufferFile internal buffer)
#define kMaxLineLength	180			///<arbitrary Max line length (should handle 132 col lines), used throughout the program

/// enum of file types supported/returned by BufferFile::GetFileType.
typedef enum TextFileType {
	kUnknown,
	kMolType,
	kGAMESSlogType,
	kGAMESSIRCType,
	kGAMESSDRCType,
	kGAMESSInputType,
	kXYZType,
	kPDBType,
	kMDLMolFile,
	CMLFile,
	MolDenFile,
	MolekelFile,
	MOPACarcFile,
	MOPACmopFile
} TextFileType;

/** case insensitive search for the keyword in the char string.
 * The return value is the position of the first character of the keyword on success or -1 on failure.
 * @param buffer A null-terminated character string to search in.
 * @param keyword The string to search for.
 * @param numbyte The number of bytes in the keyword to match.
 */
long FindKeyWord(const char *buffer, const char keyword[], long numbyte);
/** Search the null-terminated string for "Keyword = string".
 * The return value is 1 for success 0 for failure.
 * @param Line A null-terminated string to search in.
 * @param KeyWord A null-terminated string to search for in the line followed by an '='.
 * @param Value The resulting string, which must be preallocated by the caller.
 */
long ReadStringKeyword(const char * Line, const char * Keyword, char * Value);
/** Search the null-terminated string for "Keyword = long int value".
 * The return value is 1 for success 0 for failure.
 * @param Line A null-terminated string to search in.
 * @param KeyWord A null-terminated string to search for in the line followed by an '='.
 * @param Value The long value to set.
 */
long ReadLongKeyword(const char * Line, const char * Keyword, long * Value);
/** Search the null-terminated string for "Keyword = float value".
 * The return value is 1 for success 0 for failure.
 * @param Line A null-terminated string to search in.
 * @param KeyWord A null-terminated string to search for in the line followed by an '='.
 * @param Value The float value to set.
 */
long ReadFloatKeyword(const char * Line, const char * Keyword, float * Value);
/** Search the null-terminated string for "Keyword = true/false/t/f".
 * The return value is 1 for success 0 for failure.
 * @param Line A null-terminated string to search in.
 * @param KeyWord A null-terminated string to search for in the line followed by an '='.
 * @param Value The bool value to set.
 */
long ReadBooleanKeyword(const char * Line, const char * Keyword, bool * Value);
/** Search the null-terminated string for "Keyword = value".
 * The return value is the position after the '='.
 * @param Line A null-terminated string to search in.
 * @param KeyWord A null-terminated string to search for in the line followed by an '='.
 */
long LocateForValue(const char * Line, const char * KeyWord);
///Search the provided, null-terminated, string for any non-white space characters.
bool IsBlank(const char * Line);
/** Convert old fortran style exponents using d or D into c style e or E.
 * The routine simply replaces all 'd' or 'D's with 'e'.
 * @param Line A null-terminated string that will be modified in place.
 */
void ConvertExponentStyle(char * Line);

/** BufferFile is a class to abstract out file handling calls.
	It was originally written to get around file buffering issues on MacOS 9 and earlier,
	but still is handy to deal with the issues of searching through files for keywords
	while a text file is parsed.
*/
class BufferFile {
	private:
#ifdef UseHandles
		Handle		Buffer;
#else
		Ptr			Buffer;
#endif
		long		BufferStart;
		long		ByteCount;
						//Blocks allow the user to restrict reads to a given
						//size below that of the total file length
		long		BlockCount;
		long *		BlockLengths;
		long		BlockArrayAllocation;
		long		BufferPos;
		long		BufferSize;		//Size (in bytes) of IO buffer
		long		LinePos;
#ifdef UseMacIO
		short		FileRefNum;
#else
		FILE *		FilePtr;
#endif
 		short		ColsPerLine;
 		short		IOType;
 		char		EOLchar;
 		bool		DoIt;
			///Internal routine which takes care of the actual reading/writing to disk
 		void AdvanceBuffer(void);
 	public:
 			//Build a read buffer on the specified (already open!) file
#ifdef UseMacIO
 		BufferFile(short TargetFileRef, bool Write);
#else
		///Contructor used to operate on a pre-opened file.
 		BufferFile(FILE * TargetFileRef, bool Write);
#endif
#ifdef UseHandles
 		BufferFile(Handle	TargetHandle, long HandleSize);
#else
		/// Contructor used to operate on a preallocated character buffer
 		BufferFile(Ptr TargetPtr, long PtrSize);
#endif
 		~BufferFile(void);
		/// Obtain the current file position
		long GetFilePos(void);
		///Set the file position
		void SetFilePos(long NewPos);
		///returns the current file length (valid for read and write opporations)
		inline long GetFileLength(void) {return ((ByteCount>GetFilePos())?ByteCount:GetFilePos());};
		/** Attempt to discern the file type (format) based on the filename and the contents.
		 * @param fileName The name of the file, used to examine the file extension
		 */
		TextFileType GetFileType(const char * fileName);
		///Find the specified GAMESS group name " $GroupName ...". 
		long FindGroup(const char * GroupName);
		/** Read NumBytes bytes from the file into the target string.
		 * Note attempting to read past the end of the file will cause an exception to be thrown.
		 * @param Target The pre-allocated buffer to read into which must be at least NumBytes+1 bytes long.
		 * @param NumBytes The number of bytes to read.
		 */
 		long Read(Ptr Target, long NumBytes);
		/** Read characters up to the next end of line.
		 * The EOL char is consumed, but not included in the Line. At most kMaxLineLength-1 chars will
		 * be read so the pre-allocated Line buffer should be at least kMaxLineLength long.
		 * @param Line The pre-allocated storage to read the line into.
		 */
 		long GetLine(char * Line);
		/** Write out the null-terminated text string (minus the null terminator).
		 * returns the actual number of characters written.
		 * @param Text The null-terminated string to output.
		 */
		long PutText(const char * Text);
		/** Back up the specified number of lines in the file.
		 * The position will be at the beginning of the line. A value of 1 will 
		 * move to the beginning of the current line if you are not already there.
		 * @param nBack The number of lines to back up.
		 */
		void BackupnLines(long nBack);
		/// Skip nSkip lines in the file.
		void SkipnLines(long nSkip);
		/* inline bool LocateKeyWord(char Keyword[], long NumByte) {return LocateKeyWord(Keyword, NumByte, -1);}; */
		/** Search the file for the specified keyword until found, EOF, or the limit is reached.
		 * Returns true or false, the file position upon exit will be the start of the keyword,
		 * or the starting position if the keyword is not found.
		 * @param Keyword The literal text string to search for
		 * @param NumByte The length of the Keyword string
		 * @param Limit (optional) The file position limit (in bytes) to limit the search. -1 will
		 *							search to the end of the file.
		 */
		bool LocateKeyWord(const char Keyword[], long NumByte, long Limit = -1);
		/** Obtain the position of the next blank line.
		 * The return value is the position. The file position is unchanged upon exit.
		 */
		long FindBlankLine(void);
		/** Count the number of lines in the next size bytes.
		 * return value is the line count. The file position is unchanged upon exit.
		 * @param size Bytecount to search through
		 */
		long GetNumLines(long size);
		///Move the file position forward NumBytes.
 		long BufferSkip(long NumBytes);
		///Set the column count for line wrapping in the WriteLine routine
 		inline void SetColsPerLine(short newVal) {ColsPerLine = newVal;};
		/// Obtain the current file position as a percentage of the file size.
 		inline float GetPercentRead(void) {return (float) 100.0*GetFilePos()/GetFileLength();};
		/// Simply write Numbytes from Source to the file.
 		long Write(const char * Source, long NumBytes);
		/** Output the specified text, wrapping the lines intelligently if needed.
		 * @param text The text to output, this normally will not contain any EOL chars
		 * @param newLine Flag to force an EOL char after this text.
		 */
		long WriteLine(const char *text, bool newline);
		/** SetOutput allows you to turn off actual writing and just count the bytes output
		 * to obtain the accurate output size (in a brute force fashion).
		 * @param State true for normal output, false to just count bytes.
		 */
 		bool SetOutput(bool State);
		///Accessor for the output flag.
 		bool GetOutput(void);
		///Call to zero out buffers avoiding normal file cleanup/flushing
 		void AbnormalCleanup(void);
		/** Call to close the file and delete this object.
		 * This will flush the buffers and close the file pointer.
		 */
 		void CloseFile(void);
		///Set a block length to restrict searchs to a subset of the file.
 		void SetBlockLength(long length);
		///Pop the current block length restriction off the list.
 		void FinishBlock(void);
		///obtain the file size
		inline long GetFileSize(void) const {return ByteCount;};
		///How many bytes are left in the current search block.
 		long BytesLeftInBlock(void) {return (BlockLengths[0]-GetFilePos());};
 };
 
#endif
