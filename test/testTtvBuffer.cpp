#include "include/TtvBuffer.h"
#include "include/common.h"
#include <iostream>
#include <string>

using namespace ttv;

/*****************************************
   Unit Testing for ttv buffer class
*****************************************/

int main(int argc, char const *argv[]) {
  {
    TtvBox box;
    std::string cfgFileTxt = "../demo/modelPreCfg.txt";
    box.parse(cfgFileTxt);
    box.getValue();

    TtvBuffer ttvBuffer;
    std::string cfgFileBin = "../demo/modelPreCfg.bin";
    ttvBuffer.serialize(cfgFileBin, box);
  }

  {
    TtvBuffer ttvBuffer;
    TtvBox box;
    std::string cfgFileBin = "../demo/modelPreCfg.bin";
    ttvBuffer.deserialize(cfgFileBin, box);
  }

  return 0;
}
