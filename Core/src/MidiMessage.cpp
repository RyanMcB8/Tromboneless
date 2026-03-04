// src/MidiMessage.cpp
#include "MidiMessage.hpp"

#include <utility> // std::move

MidiMessage::MidiMessage(std::vector<Byte> bytes)
    : data_(std::move(bytes))
{
}

MidiMessage::MidiMessage(std::initializer_list<Byte> bytes)
    : data_(bytes)
{
}

const std::vector<MidiMessage::Byte>& MidiMessage::bytes() const noexcept
{
    return data_;
}

std::size_t MidiMessage::size() const noexcept
{
    return data_.size();
}

bool MidiMessage::empty() const noexcept
{
    return data_.empty();
}