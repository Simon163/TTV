/*
 *  @file     TtvBox.h
 *  @brief    TTV box class, the blueprint to create an ttv box object which can contain multiple ttv obejcts
 *  Author:   Simon (dewen.wu@gmail.com)
 */

#pragma once

#include <map>
#include <string>
#include <vector>
#include "common.h"
#include "Ttv.h"

namespace ttv {

class Ttv;

/* TTV box class */
class TTV_PUBLIC TtvBox {
public:
    /*
     * @brief construct an ttv box object and do the initilization
     * @param none
     * @return none
     */
    TtvBox();

    /*
     * @brief construct an ttv box object and free the memory
     * @param none
     * @return none
     */
    ~TtvBox();

    /*
     * @brief put the start or end tag to the ttv box indicating the begining and end of ttv box object
     * @param tag     tag id of ttv object
     * @param type    the type of ttv object
     * @return true if putting sucessfully, false otherwise
     */
    bool putStartEndTag(const uint8_t tag, const uint8_t type);

    /*
     * @brief put a numberical value into the ttv box,
     * support bool/uint8/int8/uint16/int16/uint32/int32/uint64/int64/float/double
     * @param tag     tag id of ttv object
     * @param type    the type of ttv object
     * @param value   the value of ttv object
     * @return true if putting sucessfully, false otherwise
     */
    template <typename T>
    bool putNumbericalValue(const uint8_t tag, const uint8_t type, const T value);

    /*
     * @brief put a non numberical value into the ttv box,
     * support string/array defined using char *
     * @param tag     tag id of ttv object
     * @param type    the type of ttv object
     * @param value   the value of ttv object
     * @return true if putting sucessfully, false otherwise
     */
    bool putNonNumbericalValue(const uint8_t tag, const uint8_t type, const uint32_t length, const void* value);

    /*
     * @brief put another ttv box into this tv box,
     * please note put the start/end tag to the another ttv box
     * @param tag     tag id of ttv object
     * @param type    the type of ttv object
     * @param value   the pointer which points to another ttv object
     * @return true if putting sucessfully, false otherwise
     */
    bool putTtvValue(const uint8_t tag, const uint8_t type, const TtvBox *value);

    /*
     * @brief pack a ttv box after putting all the wanted values
     * after packing, an value with the basic data type is stored as ttv (tag + type + value),\
     * an value with other data type is stored as ttv(tag + length + value)
     * the start tag and the end tag is used to indicate the beginning and the end of the output buffer
     * @param none
     * @return true if packing sucessfully, false otherwise
     */
    bool pack();

    /*
     * @brief parse the input file and put all the value into a ttv box,
     * the contents of the input file should be given in the format splitted by space like:
     * tag_id data_type value
     * for example:
     * 1 uint8 224
     * 2 uint8 224
     * 3 float 127.0
     * 4 double 0.128
     * 5 int8 -127
     * @param file  the pure text file for parsing
     * @return true if parsing sucessfully, false otherwise
     */
    bool parse(const std::string& file);

    /*
     * @brief  get the pointer of packed buffer
     * @param none
     * @return return the pointer of packed buffer
     */
    uint8_t* getPackedBuffer() const;

    /*
     * @brief  get the length of packed buffer (net length excluding the header size)
     * @param none
     * @return return the length of packed buffer
     */
    uint32_t getPackedBytes() const;

    /*
     * @brief  get the length of packed buffer (net length including the header size)
     * @param none
     * @return return the length of packed buffer
     */
    uint32_t getStorageBytes() const;

    /*
     * @brief  get the number of values in TtvBox, along with a vector of the tags
     * @param list      tag list
     * @return return the length of packed buffer
     */
    uint8_t getTagList(std::vector<uint8_t> &list) const;

    /*
     * @brief  unpack a ttv box
     * after unpacking, all the tags are stored into the map mTtvMap so we can get their values
     * by the function get_xx_value()
     * @param buffer      the pointer which points to a buffer contains ttv box
     * @param buffer      the size of the ttv box
     * @return true if unpacking sucessfully, false otherwise
     */
    bool unpack(const uint8_t* buffer, const uint32_t buffersize);

    /*
     * @brief  get the values of all tags from the minimum tag to the maximum tag
     * @param none
     * @return true if getting sucessfully, false otherwise
     */
    bool getValue() const;

    /*
     * @brief get a numberical value from the ttv box,
     * support bool/uint8/int8/uint16/int16/uint32/int32/uint64/int64/float/double
     * @param tag     tag id of ttv object
     * @param value   the value of ttv object
     * @return true if getting sucessfully, false otherwise
     */
    template <typename T>
    bool getNumbericalValue(const uint8_t tag, T& value) const;

    /*
     * @brief get a string value from the ttv box, stored in the variable with std::string type
     * @param tag     tag id of ttv object
     * @param value   the value of ttv object
     * @return true if getting sucessfully, false otherwise
     */
    bool getStringValue(const uint8_t tag, std::string& value) const;

    /*
     * @brief get a string value from the ttv box, stored in the variable with char* type  * 
     * @param tag     tag id of ttv object
     * @param value   the value of ttv object
     * @return true if getting sucessfully, false otherwise
     */
    bool getBytesValue(const uint8_t tag, char** value) const;

    /*
     * @brief get another ttv box from the tv box
     * @param tag     tag id of ttv object
     * @param type    the type of ttv object
     * @param value   the pointer which points to another ttv object
     * @return true if getting sucessfully, false otherwise
     */
    bool getTtvValue(const uint8_t tag, TtvBox& value) const;

    /*
     * @brief write the contents of the ttv box to a file
     * @param file    file name
     * @return true if writing sucessfully, false otherwise
     */
    bool write(const std::string& file);
    /*
     * @brief read the contents of the ttv box frome a file
     * @param file    file name
     * @return true if reading sucessfully, false otherwise
     */
    bool read(const std::string& file);
    /*
     * @brief read the contents of the ttv box frome a file
     * @param file    file handler
     * @return true if readding sucessfully, false otherwise
     */
    bool read(FILE *file);
    /*
     * @brief read the contents of the ttv box frome a buffer
     * @param buffer    the pointer which points to a buffer
     * @return true if readding sucessfully, false otherwise
     */
    bool read(const void* buffer);

public:
    TtvBox(const TtvBox&)  = delete;
    TtvBox(const TtvBox&&) = delete;
    TtvBox& operator=(const TtvBox&) = delete;
    TtvBox& operator=(const TtvBox&&) = delete;

private:
    bool putValue(const Ttv* value);
    void freeMem();

private:
    // store pairs of tag and ttv object
    std::map<uint8_t, const Ttv *> mTtvMap;
    // pointer which points to the ttv box object
    std::unique_ptr<uint8_t[]> mPackedBuffer;
    // total length of ttv box object
    uint32_t mPackedBytes     = 0;
};

template <typename T>
bool TtvBox::putNumbericalValue(const uint8_t tag, const uint8_t type, const T value) {
    if ((std::is_same<typename std::decay<T>::type, typename std::decay<bool>::type>::value) ||
        (std::is_same<typename std::decay<T>::type, typename std::decay<uint8_t>::type>::value) ||
        (std::is_same<typename std::decay<T>::type, typename std::decay<int8_t>::type>::value)) {
        // TTV_LOGD("tag = %d, type = %d, value = %d\n", tag, type, value);
        return putValue((const Ttv*) new Ttv(tag, type, sizeof(uint8_t), (const void* )&value));
    } else if ((std::is_same<typename std::decay<T>::type, typename std::decay<uint16_t>::type>::value) ||
               (std::is_same<typename std::decay<T>::type, typename std::decay<int16_t>::type>::value)) {
        uint16_t newvalue = htons(value);
        return putValue((const Ttv*) new Ttv(tag, type, sizeof(uint16_t), (const void* )&newvalue));
    } else if ((std::is_same<typename std::decay<T>::type, typename std::decay<uint32_t>::type>::value) ||
               (std::is_same<typename std::decay<T>::type, typename std::decay<int32_t>::type>::value)) {
        uint32_t newvalue = htonl(value);
        return putValue((const Ttv*) new Ttv(tag, type, sizeof(uint32_t), (const void* )&newvalue));
    } else if ((std::is_same<typename std::decay<T>::type, typename std::decay<uint64_t>::type>::value) ||
               (std::is_same<typename std::decay<T>::type, typename std::decay<int64_t>::type>::value)) {
        uint64_t newvalue = htobe64(value);
        return putValue((const Ttv*) new Ttv(tag, type, sizeof(uint64_t), (const void* )&newvalue));
    } else if ((std::is_same<typename std::decay<T>::type, typename std::decay<float>::type>::value)) {
        float newvalue = swapFloat(value);
        return putValue((const Ttv*) new Ttv(tag, type, sizeof(float), (const void* )&newvalue));
    } else if ((std::is_same<typename std::decay<T>::type, typename std::decay<double>::type>::value)) {
        double newvalue = swapDouble(value);
        return putValue((const Ttv*) new Ttv(tag, type, sizeof(double), (const void* )&newvalue));
    } else {
        TTV_LOGE("Error: unsupported data type.");
        return false;
    }
    return true;
}

template <typename T>
bool TtvBox::getNumbericalValue(const uint8_t tag, T& value) const {
    auto iter = mTtvMap.find(tag);
    if (iter != mTtvMap.end()) {
        if ((std::is_same<typename std::decay<T>::type, typename std::decay<bool>::type>::value) ||
            (std::is_same<typename std::decay<T>::type, typename std::decay<uint8_t>::type>::value) ||
            (std::is_same<typename std::decay<T>::type, typename std::decay<int8_t>::type>::value)) {
            value = (*(T *)(iter->second->getValue()));
            return true;
        } else if ((std::is_same<typename std::decay<T>::type, typename std::decay<uint16_t>::type>::value) ||
                   (std::is_same<typename std::decay<T>::type, typename std::decay<int16_t>::type>::value)) {
            value = ntohs((*(uint16_t *)(iter->second->getValue())));
            return true;
        } else if ((std::is_same<typename std::decay<T>::type, typename std::decay<uint32_t>::type>::value) ||
                   (std::is_same<typename std::decay<T>::type, typename std::decay<int32_t>::type>::value)) {
            value = (uint32_t)ntohl((*(uint32_t *)(iter->second->getValue())));
            return true;
        } else if ((std::is_same<typename std::decay<T>::type, typename std::decay<uint64_t>::type>::value) ||
                   (std::is_same<typename std::decay<T>::type, typename std::decay<int64_t>::type>::value)) {
            value = be64toh((*(uint64_t *)(iter->second->getValue())));
            return true;
        } else if ((std::is_same<typename std::decay<T>::type, typename std::decay<float>::type>::value)) {
            value = swapFloat((*(float *)(iter->second->getValue())));
            return true;
        } else if ((std::is_same<typename std::decay<T>::type, typename std::decay<double>::type>::value)) {
            value = swapDouble((*(double *)(iter->second->getValue())));
            return true;
        } else {
            TTV_LOGE("Error: tag = %d is found, but its type mismatch.", tag);
            return false;
        }
    } else {
        TTV_LOGE("Error: tag = %d is not found.", tag);
        return false;
    }

    return true;
}

} // namespace ttv

