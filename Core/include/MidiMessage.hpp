#pragma once

#include <cstddef>
#include <initializer_list>
#include <vector>

/**
 * @brief Represents a raw MIDI message as a sequence of bytes.
 *
 * This class is a lightweight value type used to store MIDI messages
 * before transmission. It does not perform validation beyond basic
 * structural construction.
 *
 * A MIDI message consists of:
 * - A status byte
 * - Zero, one, or two data bytes (for standard channel messages)
 * - Or an arbitrary number of bytes (e.g. SysEx)
 *
 * The underlying storage uses std::vector<unsigned char> to allow
 * variable-length messages for future extensibility.
 */
class MidiMessage
{
public:
    using Byte = unsigned char;

    /**
     * @brief Default constructor.
     *
     * Creates an empty MIDI message.
     */
    MidiMessage() = default;

    /**
     * @brief Construct a MIDI message from an existing byte vector.
     *
     * @param bytes Vector containing raw MIDI bytes.
     */
    explicit MidiMessage(std::vector<Byte> bytes);

    /**
     * @brief Construct a MIDI message from a list of bytes.
     *
     * Example:
     * @code
     * MidiMessage msg{0x90, 0x3C, 0x64};
     * @endcode
     *
     * @param bytes Initialiser list of raw MIDI bytes.
     */
    MidiMessage(std::initializer_list<Byte> bytes);

    /**
     * @brief Access the raw byte buffer.
     *
     * @return Const reference to internal byte vector.
     */
    const std::vector<Byte>& bytes() const noexcept;

    /**
     * @brief Get number of bytes in the message.
     *
     * @return Size of the message in bytes.
     */
    std::size_t size() const noexcept;

    /**
     * @brief Check whether the message contains any bytes.
     *
     * @return True if empty, false otherwise.
     */
    bool empty() const noexcept;

private:
    std::vector<Byte> data_;
};