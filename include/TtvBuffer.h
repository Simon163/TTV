#pragma once

#include "include/TtvBox.h"

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
  bool serialize(const std::string &file, TtvBox &ttvbox);

  /*
   * @brief decode a ttv box from file (already open) and write it to a ttv box
   * @param file       file stream object
   * @param ttvbox     the ttv box to be decoded
   * @return true if encoding sucessfully, false otherwise
   */
  void deserialize(std::ifstream &file, TtvBox &ttvbox);

  /*
   * @brief decode a ttv box from file (not open yet) and write it to a ttv box
   * @param file       file name
   * @param ttvbox     the ttv box to be decoded
   * @return none
   */
  bool deserialize(const std::string &file, TtvBox &ttvbox);

  /*
   * @brief decode a ttv box from buffer and write it to a ttv box
   * @param buffer     pointer that points to the begining of contents of the
   * file
   * @param ttvbox     the ttv box to be decoded
   * @return none
   */
  void deserialize(const void *buffer, TtvBox &ttvbox);
};

} // namespace ttv
