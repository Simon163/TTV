/*
 *  @file     TtvBox.cpp
 *  @brief    TTV box class, the blueprint to create an ttv box object which can contain multiple ttv obejcts
 *  Author:   Simon (dewen.wu@gmail.com)
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include "string.h"
#include "common.h"
#include "TtvBox.h"

namespace ttv {

TtvBox::TtvBox() : mPackedBuffer(nullptr), mPackedBytes(0) {}

TtvBox::~TtvBox() { freeMem(); }

void TtvBox::freeMem() {
    for (auto& ttvmap : mTtvMap) {
        delete ttvmap.second;
        ttvmap.second = nullptr;
    }

    mTtvMap.clear();
}

bool TtvBox::putNonNumbericalValue(const uint8_t tag, const uint8_t type, const uint32_t length, const void* value) {
    if ((type > BASIC_TYPE_MAX) && (type <= COMPLEX_TYPE_MAX)) {
        putValue((const Ttv*) new Ttv(tag, type, length, value));
    }
    else {
        TTV_LOGE("Error: unsupported data type.");
        return false;
    }
    return true;
}


bool TtvBox::getStringValue(const uint8_t tag, std::string& value) const {
    auto iter = mTtvMap.find(tag);
    if (iter != mTtvMap.end()) {
        if (STRING_T != iter->second->getType()) {
            TTV_LOGE("Error: tag = %d is found, but its type mismatch.", tag);
            return false;
        }
        value = std::string((char *)(iter->second->getValue()), iter->second->getLength());
    } else {
        TTV_LOGE("Error: tag = %d is not found.", tag);
        return false;
    }
    return true;
}

bool TtvBox::getBytesValue(const uint8_t tag, char** value) const {
    auto iter = mTtvMap.find(tag);
    if (iter != mTtvMap.end()) {
        if (BYTES_T != iter->second->getType()) {
            TTV_LOGE("Error: tag = %d is found, but its type mismatch.", tag);
            return false;
        }
        *value = (char *)(iter->second->getValue());
    } else {
        TTV_LOGE("Error: tag = %d is not found.", tag);
        return false;
    }
    return true;
}

bool TtvBox::pack() {
    if (mPackedBuffer) {
        mPackedBuffer.reset();
        freeMem();
    }

    uint32_t offset = 0;
    mPackedBuffer.reset(new uint8_t[mPackedBytes]);

    auto iter = mTtvMap.begin();
    for (; iter != mTtvMap.end(); iter++) {
        uint8_t tag = (uint8_t)iter->second->getTag();
        ::memcpy((void *)(mPackedBuffer.get() + offset), (void *)&tag, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        uint8_t type = iter->second->getType();
        ::memcpy((void *)(mPackedBuffer.get() + offset), (void *)&type, sizeof(uint8_t));
        offset += sizeof(uint8_t);

        // the tag and type of start and end is to indicate the start and the end to store data
        // for the start and the end, store the tag and type only.
        if (!(((START_TAG == tag) && (START_TYPE == type)) || ((END_TAG == tag) && (END_TYPE == type)))) {
            uint32_t length = iter->second->getLength();

            // for basice types like char, int, float, the storage format is tag + type + value
            // for other non-basice types like string, char *, class, structure,
            // the storage format is tag + type + length + value
            if (type > BASIC_TYPE_MAX) {
                uint32_t newlength = htonl(length);
                ::memcpy(mPackedBuffer.get() + offset, &newlength, sizeof(uint32_t));
                offset += sizeof(uint32_t);
            }

            ::memcpy((void *)(mPackedBuffer.get() + offset), (void *)(iter->second->getValue()), static_cast<size_t>(length));
            offset += length;
        }
    }

    return true;
}

bool TtvBox::parse(const std::string& file) {
    TTV_LOGI("Parse the input file %s...", file.c_str());
    std::ifstream fin(file, std::ios::in);
    if (!fin.is_open()) {
        TTV_LOGE("Error: failed to open the input file: %s!", file.c_str());
        return false;
    }
    std::string tagstr   = "";
    std::string typestr  = "";
    std::string valuestr = "";
    // support 1024 chars at most each line
    char line[1024] = {0};

    putValue((const Ttv *)new Ttv((uint8_t)START_TAG, (uint8_t)START_TYPE));

    while (fin.getline(line, sizeof(line))) {
        std::stringstream word(line);
        word >> tagstr;
        word >> typestr;
        word >> valuestr;

        const uint8_t tag = (uint8_t)atoi(tagstr.c_str());
        if ((tag <= START_TAG) || (tag >= END_TAG)) {
            TTV_LOGE("Error: the range of tag value should be (0, 255), please check the value you configure.");
            return false;
        }

        if ((strcmp(typestr.c_str(), "uint8") == 0) || (strcmp(typestr.c_str(), "bool") == 0)) {
            const uint8_t type  = (uint8_t)UINT8_T;
            const uint8_t value = (uint8_t)atoi(valuestr.c_str());
            putNumbericalValue(tag, type, value);
        } else if ((strcmp(typestr.c_str(), "int8") == 0)) {
            uint8_t type = (uint8_t)INT8_T;
            int8_t value = (int8_t)atoi(valuestr.c_str());
            putNumbericalValue(tag, type, value);
        } else if ((strcmp(typestr.c_str(), "uint16") == 0)) {
            uint8_t type   = (uint8_t)UINT16_T;
            uint16_t value = (uint16_t)atoi(valuestr.c_str());
            putNumbericalValue(tag, type, value);
        } else if ((strcmp(typestr.c_str(), "int16") == 0)) {
            uint8_t type  = (uint8_t)INT16_T;
            int16_t value = (int16_t)atoi(valuestr.c_str());
            putNumbericalValue(tag, type, value);
        }
#if defined(__x86_64__) || defined(__aarch64__)
        else if ((strcmp(typestr.c_str(), "uint32") == 0)) {
            uint8_t type   = (uint8_t)UINT32_T;
            uint32_t value = (uint32_t)atol(valuestr.c_str());
            putNumbericalValue(tag, type, value);
        }
#else
        else if ((strcmp(typestr.c_str(), "uint32") == 0)) {
            uint8_t type   = (uint8_t)UINT32_T;
            uint32_t value = (uint32_t)atoll(valuestr.c_str());
            putNumbericalValue(tag, type, value);
        }
#endif
        else if ((strcmp(typestr.c_str(), "int32") == 0)) {
            uint8_t type  = (uint8_t)INT32_T;
            int32_t value = (int32_t)atol(valuestr.c_str());
            putNumbericalValue(tag, type, value);
        } else if ((strcmp(typestr.c_str(), "uint64") == 0)) {
            uint8_t type   = (uint8_t)UINT64_T;
            uint64_t value = (uint64_t)atoll(valuestr.c_str());
            putNumbericalValue(tag, type, value);
        } else if ((strcmp(typestr.c_str(), "int64") == 0)) {
            uint8_t type  = (uint8_t)INT64_T;
            int64_t value = (int64_t)atoll(valuestr.c_str());
            putNumbericalValue(tag, type, value);
        } else if ((strcmp(typestr.c_str(), "float") == 0)) {
            uint8_t type = (uint8_t)FLOAT_T;
            float value  = (float)atof(valuestr.c_str());
            putNumbericalValue(tag, type, value);
        } else if ((strcmp(typestr.c_str(), "double") == 0)) {
            uint8_t type = (uint8_t)DOUBLE_T;
            double value = (double)atof(valuestr.c_str());
            putNumbericalValue(tag, type, value);
        } else if ((strcmp(typestr.c_str(), "string") == 0)) {
            uint8_t type      = (uint8_t)STRING_T;
            const char* value = valuestr.c_str();
            uint32_t length = valuestr.size();
            putNonNumbericalValue(tag, type, length, value);
        } else if ((strcmp(typestr.c_str(), "char*") == 0)) {
            uint8_t type      = (uint8_t)BYTES_T;
            const char* value = valuestr.c_str();
            uint32_t length = valuestr.size();
            putNonNumbericalValue(tag, type, length, value);
        }
         else {
            TTV_LOGE("Error: unsupported data type.");
            return false;
        }
    }

    putValue((const Ttv *)new Ttv((uint8_t)END_TAG, (uint8_t)END_TYPE));

    fin.close();
    return true;
}

bool TtvBox::unpack(const uint8_t* buffer, const uint32_t buffersize) {
    // unpack from another ttvbox
    if ((0 == mPackedBytes) && (nullptr != buffer) && (buffersize > 0)) {
        mPackedBuffer.reset(new uint8_t[buffersize]);
        ::memcpy((void *)mPackedBuffer.get(), (void *)buffer, static_cast<size_t>(buffersize));
    }

    uint32_t offset = 0;
    while (offset < buffersize) {
        uint8_t tag = (*(uint8_t* )(mPackedBuffer.get() + offset));
        offset += sizeof(uint8_t);
        uint8_t type = (*(uint8_t* )(mPackedBuffer.get() + offset));
        offset += sizeof(uint8_t);
        // the tag and type of start and end is to indicate the start and the end to store data
        // for the start and the end, store the tag and type only.
        if (((START_TAG == tag) && (START_TYPE == type)) || ((END_TAG == tag) && (END_TYPE == type))) {
            putValue((const Ttv *)new Ttv(tag, type));
        } else {
            // for basice types like char, int, float, the storage format is tag + type + value
            // for other non-basice types like string, char *, class, structure,
            // the storage format is tag + type + length + value
            if (type <= BASIC_TYPE_MAX) {
                uint8_t* value = mPackedBuffer.get() + offset;
                switch (type) {
                    case BOOL_T: {
                        putValue((const Ttv *)new Ttv(tag, type, sizeof(bool), (const void*)value));
                        offset += sizeof(bool);
                    } break;
                    case UINT8_T: {
                        putValue((const Ttv *)new Ttv(tag, type, sizeof(uint8_t), (const void*)value));
                        offset += sizeof(uint8_t);
                    } break;
                    case INT8_T: {
                        putValue((const Ttv *)new Ttv(tag, type, sizeof(int8_t), (const void*)value));
                        offset += sizeof(int8_t);
                    } break;
                    case UINT16_T: {
                        putValue((const Ttv *)new Ttv(tag, type, sizeof(uint16_t), (const void*)value));
                        offset += sizeof(uint16_t);
                    } break;
                    case INT16_T: {
                        putValue((const Ttv *)new Ttv(tag, type, sizeof(int16_t), (const void*)value));
                        offset += sizeof(int16_t);
                    } break;
                    case UINT32_T: {
                        putValue((const Ttv *)new Ttv(tag, type, sizeof(uint32_t), (const void*)value));
                        offset += sizeof(uint32_t);
                    } break;
                    case INT32_T: {
                        putValue((const Ttv *)new Ttv(tag, type, sizeof(int32_t), (const void*)value));
                        offset += sizeof(int32_t);
                    } break;
                    case UINT64_T: {
                        putValue((const Ttv *)new Ttv(tag, type, sizeof(uint64_t), (const void*)value));
                        offset += sizeof(uint64_t);
                    } break;
                    case INT64_T: {
                        putValue((const Ttv *)new Ttv(tag, type, sizeof(int64_t), (const void*)value));
                        offset += sizeof(int64_t);
                    } break;
                    case FLOAT_T: {
                        putValue((const Ttv *)new Ttv(tag, type, sizeof(float), (const void*)value));
                        offset += sizeof(float);
                    } break;
                    case DOUBLE_T: {
                        putValue((const Ttv *)new Ttv(tag, type, sizeof(double), (const void*)value));
                        offset += sizeof(double);
                    } break;
                    default: {
                        TTV_LOGE("Error: unsupported data type.");;
                    } break;
                }
            } else if ((type > BASIC_TYPE_MAX) && (type <= COMPLEX_TYPE_MAX)) {
                uint32_t length = ntohl((*(uint32_t *)(mPackedBuffer.get() + offset)));
                offset += sizeof(uint32_t);
                uint8_t* value = mPackedBuffer.get() + offset;
                putValue((const Ttv *)new Ttv(tag, type, length, (const void*)value));
                offset += length;
            } else {
                TTV_LOGE("Error: unsupported data type.");;
            }
        }
    }

    if (offset != buffersize) {
        TTV_LOGE("Error: buffer size doesn't match.");
    }

    mPackedBytes = buffersize;

    return true;
}

bool TtvBox::write(const std::string& file) {
    std::fstream out(file, std::ios::binary | std::ios::out);
    if (!out) {
        TTV_LOGE("Error: failed to open file");
        return false;
    }

    if (nullptr == mPackedBuffer.get()) {
        TTV_LOGE("Error: the packed buffer cannot be null when writing");
        TTV_LOGE("Please pack the ttv box first.");
        freeMem();
        return false;
    }
    TTV_LOGI("The size of ttv box is: mPackedBytes = %d bytes", mPackedBytes);
    // write the number of bytes frist and then the contents of the buffer
    uint32_t newlength = htonl(mPackedBytes);
    // write the size of ttv box object first
    out.write(reinterpret_cast<const char *>(&newlength), sizeof(mPackedBytes));
    // write the contents of ttv box next
    out.write(reinterpret_cast<const char *>(mPackedBuffer.get()), mPackedBytes);
    out.close();

    return true;
}

bool TtvBox::read(const std::string& file) {
    std::fstream in(file, std::ios::binary | std::ios::ate | std::ios::in);
    if (!in) {
        TTV_LOGE("Error: failed to open file.");
        return false;
    }

    in.seekg(0, std::ios::beg);

    uint32_t newlength = 0;
    // read the number of bytes first and then the contents of the buffer
    in.read(reinterpret_cast<char *>(&newlength), sizeof(uint32_t));
    newlength    = ntohl(newlength);
    mPackedBytes = newlength;
    TTV_LOGI("The size of ttv box is: mPackedBytes = %d bytes", mPackedBytes);
    // in.read(reinterpret_cast<char *>(&mPackedBytes), sizeof(mPackedBytes));
    mPackedBuffer.reset(new uint8_t[mPackedBytes]);

    // read the TTV data buffer
    in.read(reinterpret_cast<char *>(mPackedBuffer.get()), mPackedBytes);
    in.close();

    return true;
}

bool TtvBox::read(FILE *file) {
    if (!file) {
        TTV_LOGE("Error: error file handler");
        return false;
    }

    fseek(file, 0, SEEK_SET);

    uint32_t newlength = 0;
    // read the number of bytes frist and then the contents of the buffer
    fread(&newlength, sizeof(uint8_t), sizeof(uint32_t), file);

    newlength    = ntohl(newlength);
    mPackedBytes = newlength;
    // in.read(reinterpret_cast<char *>(&mPackedBytes), sizeof(mPackedBytes));
    mPackedBuffer.reset(new uint8_t[mPackedBytes]);

    // read the TTV data buffer
    fread(mPackedBuffer.get(), sizeof(uint8_t), mPackedBytes, file);

    TTV_LOGI("The size of ttv box is: mPackedBytes = %d bytes", mPackedBytes);

    return true;
}

bool TtvBox::read(const void* buffer) {
    uint32_t newlength = 0;
    // read the number of bytes frist and then the contents of the buffer
    ::memcpy((void *)&newlength, (void *)buffer, sizeof(uint32_t));

    uint8_t* newbuffer = (uint8_t* )buffer + sizeof(uint32_t);

    newlength    = ntohl(newlength);
    mPackedBytes = newlength;

    mPackedBuffer.reset(new uint8_t[mPackedBytes]);

    // read the TTV data buffer
    ::memcpy((void *)mPackedBuffer.get(), (void *)newbuffer, static_cast<size_t>(mPackedBytes));

    TTV_LOGI("The size of ttv box is: mPackedBytes = %d bytes", mPackedBytes);
    return true;
}

uint8_t* TtvBox::getPackedBuffer() const {
    return mPackedBuffer.get();
}

uint32_t TtvBox::getPackedBytes() const {
    return mPackedBytes;
}

uint32_t TtvBox::getStorageBytes() const {
    return mPackedBytes + sizeof(uint32_t);
}

bool TtvBox::putValue(const Ttv* ttv) {
    uint8_t tag  = ttv->getTag();
    uint8_t type = ttv->getType();

    auto iter = mTtvMap.find(tag);
    if (iter != mTtvMap.end()) {
        delete iter->second;
        iter->second = nullptr;
        freeMem();
        TTV_LOGE("Error: the tag %d has been put before, please check.", tag);
        return false;
    } else {
        mTtvMap.insert(std::pair<uint8_t, const Ttv *>(tag, ttv));
    }

    // the tag and type of start and end is to indicate the start and the end to store data
    // for the start and the end, store the tag and type only.
    if (((START_TAG == tag) && (START_TYPE == type)) || ((END_TAG == tag) && (END_TYPE == type))) {
        mPackedBytes += sizeof(uint8_t) + sizeof(uint8_t);
        return true;
    }

    // for basice types like char, int, float, the storage format is tag + type + value
    uint8_t length = ttv->getLength();
    mPackedBytes += sizeof(uint8_t) + sizeof(uint8_t) + length;

    // for other non-basice types like string, char *, class, structure,
    // the storage format is tag + type + value + length
    if (type > BASIC_TYPE_MAX) {
        mPackedBytes += sizeof(uint32_t);
    }

    return true;
}

bool TtvBox::putStartEndTag(const uint8_t tag, const uint8_t type) {
    return putValue((const Ttv*) new Ttv(tag, type));
}

bool TtvBox::putTtvValue(const uint8_t tag, const uint8_t type, const TtvBox *value) {
    const uint8_t* const buffer = value->getPackedBuffer();

    if (!buffer) {                                                                 
        TTV_LOGE("Error: input buffer is null ptr.");
        return false;                                                                 
    }

    return putValue((const Ttv*) new Ttv(tag, type, value->getPackedBytes(), buffer));
}

bool TtvBox::getValue() const {
    for (const auto& ttvmap : mTtvMap) {
        uint8_t tag  = ttvmap.first;
        uint8_t type = ttvmap.second->getType();

        if ((START_TAG == tag) && (START_TYPE == type)) {
            TTV_LOGI("Start parsing ttv box... ");
        } else if ((END_TAG == tag) && (END_TYPE == type)) {
            TTV_LOGI("Finish parsing ttv box...");
        } else {
            switch (type) {
                case BOOL_T: {
                    bool value;
                    if (!getNumbericalValue(tag, value)) {
                        TTV_LOGE("Failed to get the value of the tag 0x%X", tag);
                        return false;
                    }
                    TTV_LOGI("Get the value of the tag 0x%X sucessfully, value = %d", tag, (int32_t)value);
                } break;
                case UINT8_T: {
                    uint8_t value;
                    if (!getNumbericalValue(tag, value)) {
                        TTV_LOGE("Failed to get the value of the tag 0x%X", tag);
                        return false;
                    }
                    TTV_LOGI("Get the value of the tag 0x%X sucessfully, value = %d", tag, (int32_t)value);
                } break;
                case INT8_T: {
                    int8_t value;
                    if (!getNumbericalValue(tag, value)) {
                        TTV_LOGE("Failed to get the value of the tag 0x%X", tag);
                        return false;
                    }
                    TTV_LOGI("Get the value of the tag 0x%X sucessfully, value = %d", tag, (int32_t)value);
                } break;
                case UINT16_T: {
                    uint16_t value;
                    if (!getNumbericalValue(tag, value)) {
                        TTV_LOGE("Failed to get the value of the tag 0x%X", tag);
                        return false;
                    }
                    TTV_LOGI("Get the value of the tag 0x%X sucessfully, value = %d", tag, (int32_t)value);
                } break;
                case INT16_T: {
                    int16_t value;
                    if (!getNumbericalValue(tag, value)) {
                        TTV_LOGE("Failed to get the value of the tag 0x%X", tag);
                        return false;
                    }
                    TTV_LOGI("Get the value of the tag 0x%X sucessfully, value = %d", tag, (int32_t)value);
                } break;
                case UINT32_T: {
                    uint32_t value;
                    if (!getNumbericalValue(tag, value)) {
                        TTV_LOGE("Failed to get the value of the tag 0x%X", tag);
                        return false;
                    }
                    TTV_LOGI("Get the value of the tag 0x%X sucessfully, value = %d", tag, value);
                } break;
                case INT32_T: {
                    int32_t value;
                    if (!getNumbericalValue(tag, value)) {
                        TTV_LOGE("Failed to get the value of the tag 0x%X", tag);
                        return false;
                    }
                    TTV_LOGI("Get the value of the tag 0x%X sucessfully, value = %d", tag, value);
                } break;
                case UINT64_T: {
                    uint64_t value;
                    if (!getNumbericalValue(tag, value)) {
                        TTV_LOGE("Failed to get the value of the tag 0x%X", tag);
                        return false;
                    }
                    TTV_LOGI("Get the value of the tag 0x%X sucessfully, value = %lld", tag, (long long int)value);
                } break;
                case INT64_T: {
                    int64_t value;
                    if (!getNumbericalValue(tag, value)) {
                        TTV_LOGE("Failed to get the value of the tag 0x%X", tag);
                        return false;
                    }
                    TTV_LOGI("Get the value of the tag 0x%X sucessfully, value = %lld", tag, (long long int)value);
                } break;
                case FLOAT_T: {
                    float value;
                    if (!getNumbericalValue(tag, value)) {
                        TTV_LOGE("Failed to get the value of the tag 0x%X", tag);
                        return false;
                    }
                    TTV_LOGI("Get the value of the tag 0x%X sucessfully, value = %f", tag, value);
                } break;
                case DOUBLE_T: {
                    double value;
                    if (!getNumbericalValue(tag, value)) {
                        TTV_LOGE("Failed to get the value of the tag 0x%X", tag);
                        return false;
                    }
                    TTV_LOGI("Get the value of the tag 0x%X sucessfully, value = %lf", tag, value);
                } break;
                case STRING_T: {
                    std::string value;
                    value.resize(ttvmap.second->getLength());     
                    if (!getStringValue(tag, value)) {
                        TTV_LOGE("Failed to get the value of the tag 0x%X", tag);
                        return false;
                    }
                    TTV_LOGI("Get the value of the tag 0x%X sucessfully, value = %s", tag, value.c_str());
                } break;
                case BYTES_T: {
                    char* value = nullptr;
                    if (!getBytesValue(tag, &value)) {
                        TTV_LOGE("Failed to get the value of the tag 0x%X", tag);
                        return false;
                    }
                    TTV_LOGI("Get the value of the tag 0x%X sucessfully, value = %s", tag, value); 
                  } break;
                default: {
                    TTV_LOGE("Error: unsupported data type.");;
                } break;
            }
        }
    }

    return true;
}

bool TtvBox::getTtvValue(const uint8_t tag, TtvBox& value) const {
    auto iter = mTtvMap.find(tag);
    if (iter == mTtvMap.end()) {
        return false;
    }

    return value.unpack(iter->second->getValue(), iter->second->getLength());
}

uint8_t TtvBox::getTagList(std::vector<uint8_t>& list) const {
    for (const auto& ttvmap : mTtvMap) {
        list.push_back(ttvmap.first);
    }

    return list.size();
}

} // namespace ttv

