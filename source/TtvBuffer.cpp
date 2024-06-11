/*
 *  @file     TtvBuffer.cpp
 *  @brief    TTVBuffer class, the blueprint to create an ttv buffer object to serialize/deserialize data
 *  Author:   Simon (dewen.wu@gmail.com)
 */

#include <string>
#include "common.h"
#include "TtvBuffer.h"

namespace ttv {

bool TtvBuffer::serialize(const std::string file, TtvBox& ttvbox) {
    TTV_LOGI("serialize...");
    if (ttvbox.getPackedBytes() == 0) {
        TTV_LOGE("Error: this is an empty ttv box! please create an non-empty box first.");
    }
    if (!ttvbox.pack()) {
        TTV_LOGI("serialize failed.");
        return false;
    }
    ttvbox.write(file);

    TTV_LOGI("serialize succeeded, the total size is %d bytes.", ttvbox.getPackedBytes());

    return true;
}

bool TtvBuffer::deserialize(const std::string& file, TtvBox& ttvbox) {
    FILE* fp = fopen(file.c_str(), "rb");
    if (!fp) {
        TTV_LOGE("Error: cann't open file %s.", file.c_str());
        return false;
    }
    deserialize(fp, ttvbox);
    fclose(fp);

    return true;
}

bool TtvBuffer::deserialize(FILE* file, TtvBox& ttvbox) {
    TTV_LOGI("Deserialize...");

    ttvbox.read(file);
    if (!ttvbox.unpack(ttvbox.getPackedBuffer(), ttvbox.getPackedBytes())) {
        TTV_LOGI("Deserialize succeeded, the total size is %d bytes", ttvbox.getPackedBytes());
    }
    {
        std::vector<uint8_t> tagList;
        uint8_t numTags = ttvbox.getTagList(tagList);
        TTV_LOGI("The deserialized ttv box contains %d ttv objects as follows:", numTags);
        for (size_t ii = 0; ii < (size_t)numTags; ii++) {
            TTV_LOGI("ii = %d, Tag = 0x%X", (int)ii, tagList[ii]);
        }
        // get contents of ttv box
        ttvbox.getValue();
    }

    return true;
}

bool TtvBuffer::deserialize(const void* buffer, TtvBox& ttvbox) {
    TTV_LOGI("Deserialize...");

    ttvbox.read(buffer);
    if (!ttvbox.unpack(ttvbox.getPackedBuffer(), ttvbox.getPackedBytes())) {
        TTV_LOGI("Deserialize succeeded, the total size is %d bytes", ttvbox.getPackedBytes());
    }

    {
        std::vector<uint8_t> tagList;
        uint8_t numTags = ttvbox.getTagList(tagList);
        TTV_LOGI("The deserialized ttv box contains %d ttv objects as follows:", numTags);
        for (size_t ii = 0; ii < (size_t)numTags; ii++) {
            TTV_LOGI("ii = %d, Tag = 0x%X", (int32_t)ii, tagList[ii]);
        }
        // get contents of ttv box
        ttvbox.getValue();
    }

    return true;
}

} // namespace ttv

