#include "include/TtvBox.h"
#include "include/common.h"
#include <iostream>
#include <string>

using namespace ttv;

/*****************************************
   Take preprocessing configuration parameters as an example,
   give a demo of:
   1. how to serialize parameters to .bin file
   2. how to deserialize parameters from .bin file
   place the contents of serialized .bin file to the model file before releasing
and then ensure preprocessing is self-explanatory when performing deep learning
inference
*****************************************/

int main(int argc, char const *argv[]) {
  {
    TtvBox box;
    // create ttv box object
    {
      std::string cfgFileTxt = "../demo/modelPreCfg.txt";
      box.parse(cfgFileTxt);
      box.getValue();
    }

    // serialize/encode
    box.pack();
    std::string cfgFileBin = "../demo/modelPreCfg.bin";
    box.write(cfgFileBin);
  }

  {
    // deserialize/decode
    TtvBox box;
    std::string cfgFileBin = "../demo/modelPreCfg.bin";
    box.read(cfgFileBin);
    box.unpack(box.getPackedBuffer(), box.getPackedBytes());
    box.getValue();
  }

  return 0;
}
