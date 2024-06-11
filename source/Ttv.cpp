/*
 *  @file     Ttv.cpp
 *  @brief    TTV class, the blueprint to create an ttv object 
 *  Author:   Simon (dewen.wu@gmail.com)
 */

#include <string.h>
#include "ttv.h"

namespace ttv {

Ttv::Ttv(const uint8_t tag, const uint8_t type, const uint32_t length, const void* value) : mTag(tag), mType(type) {
    initialize(value, length);
};

Ttv::Ttv(const uint8_t tag, const uint8_t type, const Ttv &value) : mTag(tag), mType(type) {
    initialize((const void *)value.getValue(), (uint32_t)value.getLength());
}

void Ttv::initialize(const void* value, const uint32_t length) {
    mLength = length;
    mValue.reset(new uint8_t[length]);
    ::memcpy((void *)(mValue.get()), (void *)value, static_cast<size_t>(length));
}

uint8_t Ttv::getTag() const {
    return mTag; 
}

uint8_t Ttv::getType() const {
    return mType;
}

uint32_t Ttv::getLength() const {
    return mLength;
}

uint8_t* Ttv::getValue() const {
    return mValue.get();
}

} // namespace ttv
