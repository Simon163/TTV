/*
 *  @file     TtvBuffer.cpp
 *  @brief    TTVBuffer class, the blueprint to create an ttv buffer object to serialize/deserialize data
 *  Author:   Simon (dewen.wu@gmail.com)
 */

#include <string>
#include <fstream>
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
    std::ifstream in(file, std::ios::binary);
    if (!in) {
        TTV_LOGE("Error: failed to open file.");
        return false;
    }

    deserialize(in, ttvbox);
    in.close();

    return true;
}

void TtvBuffer::deserialize(std::ifstream& file, TtvBox& ttvbox) {
    TTV_LOGI("Deserialize...");

    ttvbox.read(file);
    if (!ttvbox.unpack(ttvbox.getPackedBuffer(), ttvbox.getPackedBytes())) {
        TTV_LOGI("Deserialize succeeded, the total size is %d bytes", ttvbox.getPackedBytes());
    }

    ttvbox.printTagList();

    return;
}

void TtvBuffer::deserialize(const void* buffer, TtvBox& ttvbox) {
    TTV_LOGI("Deserialize...");

    ttvbox.read(buffer);
    if (!ttvbox.unpack(ttvbox.getPackedBuffer(), ttvbox.getPackedBytes())) {
        TTV_LOGI("Deserialize succeeded, the total size is %d bytes", ttvbox.getPackedBytes());
    }

    ttvbox.printTagList();

    return;
}

} // namespace ttv

