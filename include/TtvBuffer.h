#pragma once

#include "TtvBox.h"

namespace ttv {

class TtvBox;

/* TTV buffer, similiar to Protobuf and flatbuffer, 
   is a lightweight data communication tool and 
   can be used to serlize or deserialize data with multiple data types. 
*/
class TTV_PUBLIC TtvBuffer {
public:
    TtvBuffer() = default;
    ~TtvBuffer() = default;

public:
    /*
    * @brief encode a ttv box and write it to file
    * @param file      file name
    * @param ttvbox    the ttv box to be encoded
    * @return true if encoding sucessfully, false otherwise
    */
    bool serialize(const std::string file, TtvBox& ttvbox);

    /*
    * @brief decode a ttv box from file (already open) and write it to a ttv box
    * @param file          file handler
    * @param ttvbox     the ttv box to be decoded
    * @return true if encoding sucessfully, false otherwise
    */
    bool deserialize(FILE* file, TtvBox& ttvbox);

    /*
    * @brief decode a ttv box from file (not open yet) and write it to a ttv box
    * @param file          file handler
    * @param ttvbox     the ttv box to be decoded
    * @return true if encoding sucessfully, false otherwise
    */
    bool deserialize(const std::string& file, TtvBox& ttvbox);

    /*
    * @brief decode a ttv box from buffer and write it to a ttv box
    * @param file          file handler
    * @param ttvbox     the ttv box to be decoded
    * @return true if encoding sucessfully, false otherwise
    */
    bool deserialize(const void* buffer, TtvBox& ttvbox);
};

} // namespace ttv
