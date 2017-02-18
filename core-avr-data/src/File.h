#ifndef FILE_H
#define FILE_H

#include "Streamer.h"

#include <string.h>
#include <Converter.h>
#include <Logger.h>

#define FILE_HEADER_SIZE 8

/**
 * @brief      Description of file, stored at the start of the file buffer.
 */
class FileHeader
{

public:
	/**
	 * Version of the file.
	 */
	int version;

	/**
	 * Number of bytes, excluding the header.
	 */
	short size;

	/**
	 * Number of records total.
	 */
	short numRecords;

	/**
	 * @brief      Reads header information.
	 *
	 * @param      stream          The stream to read from.
	 * @param[in]  absoluteOffset  The absolute offset of the file.
	 *
	 * @return     True if the header was successfully read.
	 */
	bool read(
		Streamer* stream,
		const int absoluteOffset)
	{
		char buffer[FILE_HEADER_SIZE];

		// read file header
		if (FILE_HEADER_SIZE != stream->read(buffer, absoluteOffset, FILE_HEADER_SIZE))
		{
			return false;
		}

		// read in version
		IntUnion intConverter;
		intConverter.charValue[0] = buffer[0];
		intConverter.charValue[1] = buffer[1];
		intConverter.charValue[2] = buffer[2];
		intConverter.charValue[3] = buffer[3];
		version = intConverter.intValue;

		// size
		ShortUnion converter;
		converter.charValue[0] = buffer[4];
		converter.charValue[1] = buffer[5];
		size = converter.shortValue;

		// numrecords
		converter.charValue[0] = buffer[6];
		converter.charValue[1] = buffer[7];
		numRecords = converter.shortValue;

		return true;
	}

	/**
	 * @brief      Writes the header.
	 *
	 * @param      stream  The stream to write with.
	 * @param      absoluteOffset  The absolute offset to write the header.
	 *
	 * @return     True if the header was successfully writtern.
	 */
	bool write(
		Streamer* stream,
		const int absoluteOffset)
	{
		// prepare buffer
		char buffer[FILE_HEADER_SIZE];

		memcpy(buffer, &version, 4);
		memcpy(buffer + 4, &size, 2);
		memcpy(buffer + 6, &numRecords, 2);

		int bytesWritten = stream->write(buffer, absoluteOffset, FILE_HEADER_SIZE);

		return FILE_HEADER_SIZE == bytesWritten;
	}
};

/**
 * @brief      Entry point for adding data to + updating fixed size files.
 */
class File
{
private:

	/**
	 * Logger.
	 */
	Logger* _logger;

	/**
	 * Stream to read/write with.
	 */
	Streamer* _stream;

	/**
	 * Byte offset into stream.
	 */
	int _offset;

	/**
	 * Buffer used internally to move floats around.
	 */
	char _scratchBuffer[4];

public:

	/**
	 * Header information.
	 */
	FileHeader header;

	/**
	 * @brief      Constructor.
	 */
	File();

	/**
	 * @brief      Destructor.
	 */
	~File();

	/**
	 * @brief      Loads header into memory. This is for Files that already
	 * exist.
	 *
	 * @param      stream  The stream to read/write from.
	 * @param[in]  offset  The byte offset this file starts at.
	 */
	bool load(Streamer* stream, const int offset);

	/**
	 * @brief      Writes header to disk. This is for new Files.
	 *
	 * @param      stream  The stream to write to.
	 * @param[in]  offset  The byte offset this file starts at.
	 * @param[in]  size    The size, in bytes, of the file.
	 *
	 * @return     { description_of_the_return_value }
	 */
	bool init(Streamer* stream, const int offset, const short size);

	/**
	 * @brief      Retrieves the size of the file.
	 *
	 * @return     Returns the size of the file, in bytes.
	 */
	int size();

	/**
	 * @brief      Flushes any changes to the stream, in case the File decides
	 * to cache.
	 *
	 * @return     { description_of_the_return_value }
	 */
	bool flush();

	/**
	 * @brief      Adds a value to the file.
	 *
	 * @param[in]  value	The float to add to the file.
	 *
	 * @return     Returns true iff the add was successful.
	 */
	bool add(float value);

	/**
	 * @brief      Total number of stored values.
	 *
	 * @return     Total number of float values stored.
	 */
	float numValues();

	/**
	 * @brief      Retrieves values.
	 *
	 * @param[in]  buffer   Buffer to copy values into.
	 * @param[in]  recordOffset  The starting index of values.
	 * @param[in]  recordCount   The number of values to copy into the buffer.
	 *
	 * @return     Returns the number of values copied.
	 */
	int values(float* buffer, const int recordDffset, const int recordCount);
};

#endif