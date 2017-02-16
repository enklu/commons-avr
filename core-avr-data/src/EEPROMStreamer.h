#ifndef EEPROMSTREAMER_H
#define EEPROMSTREAMER_H

#include "Streamer.h"

#include <PinConfiguration.h>
#include <dataflash.h>

/**
 * @brief      Abstracts SPI + EEPROM paging into working with a single
 * block of contiguous bytes. All calls are blocking.
 */
class EEPROMStreamer : public Streamer
{
private:
	/**
	 * SPI interface.
	 */
	Dataflash _dataFlash;

	/**
	 * Page currently in buffer.
	 */
	int _page = -1;

	/**
	 * Dataflash buffer to use.
	 */
	int _bufferIndex = -1;

	/**
	 * Index into EEPROM stream.
	 */
	int _absoluteByteIndex = 0;

public:
	/**
	 * @brief      Constructor.
	 */
	EEPROMStreamer();

	/**
	 * @brief      Destructor.
	 */
	~EEPROMStreamer();

	/**
	 * @brief      Initializes the stream with pin info.
	 *
	 * @param[in]  pins      The configuration for each pin.
	 */
	bool init(PinConfiguration pins) override;

	/**
	 * @brief      Reads a single byte from the stream and advances the index.
	 *
	 * @return     Returns the byte read.
	 */
	char read() override;

	/**
	 * @brief      Reads a block of bytes into the input buffer.
	 *
	 * @param      buffer  The buffer to read values into.
	 * @param[in]  offset  The starting index.
	 * @param[in]  count   The maximum number of bytes to read.
	 *
	 * @return     The number of bytes actually read.
	 */
	int read(char* const buffer, const int offset, const int count) override;

	/**
	 * @brief      Writes a single byte to the stream and advances the index.
	 *
	 * @param[in]  value  The value to write.
	 *
	 * @return     Returns true if the byte could be successfully written.
	 */
	bool write(const char value) override;

	/**
	 * @brief      Writes a block of bytes to the stream.
	 *
	 * @param[in]  buffer  The buffer to copy values from.
	 * @param[in]  offset  The starting byte offset.
	 * @param[in]  count   The number of bytes to write from buffer.
	 *
	 * @return     Returns true if the write was successful.
	 */
	bool write(char* const buffer, const int offset, const int count) override;

	/**
	 * @brief      Moves the index to a specific place. This is used when
	 * reading and writing single values.
	 *
	 * @param[in]  offset  The offset to count from.
	 * @param[in]  origin  The number of bytes to add to the offset.
	 *
	 * @return     The current absolute index.
	 */
	int seek(const int offset, const int origin) override;
};

#endif