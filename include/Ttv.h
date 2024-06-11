/*
 *  @file     Ttv.h
 *  @brief    TTV class, the blueprint to create an ttv object 
 *  Author:   Simon (dewen.wu@gmail.com)
 */
#pragma once

#include <stdint.h>
#include <string>
#include <memory>
#include "common.h"

namespace ttv {

/* TTV class */
class TTV_PUBLIC Ttv {
public:
    /*
     * @brief construct an ttv object
     * support bool/uint8/int8/uint16/int16/uint32/int32/uint64/int64/float/double/string/char*
     * @param tag     tag id of ttv object
     * @param type    the type of ttv object
     * @param length  the length of ttv object
     * @param value   the value of ttv object
     */
    Ttv(const uint8_t tag, const uint8_t type, const uint32_t length = 0, const void* value = nullptr);
    /*
     * @brief construct an ttv object where data type is TTV
     * @param tag     tag id of ttv object
     * @param type    the type of ttv object
     * @param value   the value of ttv object
     */
    Ttv(const uint8_t tag, const uint8_t type, const Ttv& value);

    /*
     * @brief deconstruct an ttv object
     */
    ~Ttv() = default;
    
    /*
     * @brief get the tag of an ttv object
     */
    uint8_t getTag() const;

    /*
     * @brief get the type of an ttv object
     * @see enum::TtvTypeDefinition
     */
    uint8_t getType() const;

    /*
     * @brief get the length of an ttv object
     */
    uint32_t getLength() const;

    /*
     * @brief get the value of an ttv object
     */
    uint8_t* getValue() const;

private:
    void initialize(const void* value, const uint32_t length);

private:
    /* the tag id of ttv object */
    uint8_t mTag;

    /* the type of ttv object */
    uint8_t mType;

    /* the buffer length of ttv object */
    uint32_t mLength;

    /* the pointer which points to the value of ttv object */
    std::unique_ptr<uint8_t[]> mValue;
};

} // namespace ttv
