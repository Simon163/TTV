#include <iostream>
#include <string>
#include "common.h"
#include "TtvBox.h"

using namespace ttv;

/***************************************** 
   Unit Testing for ttv box class.
*****************************************/

int main(int argc, char const *argv[])
{
    uint8_t tag;
    TtvBox box;
    
    // ===============put values===============
    // put start tag and type
    box.putStartEndTag(START_TAG, START_TYPE);   
    tag = 1;   
    box.putNumbericalValue<bool>(tag++, BOOL_T, true);    
    box.putNumbericalValue<uint8_t>(tag++, UINT8_T, (uint8_t)123);    
    box.putNumbericalValue<int8_t>(tag++, INT8_T, (int8_t)-123);   
    box.putNumbericalValue<uint16_t>(tag++, UINT16_T, (uint16_t)1234);    
    box.putNumbericalValue<int16_t>(tag++, INT16_T, (int16_t)-1234);
    box.putNumbericalValue<uint32_t>(tag++, UINT32_T, (uint32_t)123456);    
    box.putNumbericalValue<int32_t>(tag++, INT32_T, (int32_t)-123456);
    box.putNumbericalValue<uint64_t>(tag++, UINT64_T, (uint64_t)1234567890);    
    box.putNumbericalValue<int64_t>(tag++, INT64_T, (int64_t)-1234567890);
    box.putNumbericalValue<float>(tag++, FLOAT_T, (float)1234.5);    
    box.putNumbericalValue<double>(tag++, DOUBLE_T, (double)1234.5);  
    std::string str1 = "xyz";
    box.putNonNumbericalValue(tag++, STRING_T, str1.size(), str1.c_str());
    char str2[] = "abcde";
    box.putNonNumbericalValue(tag++, BYTES_T, sizeof(str2), str2);
    // put end tag and type
    box.putStartEndTag(END_TAG, END_TYPE);

    // ===============get values===============
    tag = 1;
    {
        bool value;
        if (!box.getNumbericalValue(tag, value)) {   
            TTV_LOGE("Error: getNumbericalValue() failed.");
            return -1;
        }
        TTV_LOGI("getNumbericalValue() succeded, tag [%d], value [%d].", tag, value);
        tag++;
    }
    {
        uint8_t value;
        if (!box.getNumbericalValue(tag, value)) {
            TTV_LOGE("Error: getNumbericalValue() failed.");      
            return -1;
        }
        TTV_LOGI("getNumbericalValue() succeded, tag [%d], value [%d].", tag, value);
        tag++;
    }
    {
        int8_t value;
        if (!box.getNumbericalValue(tag, value)) {
            TTV_LOGE("Error: getNumbericalValue() failed.");      
            return -1;
        }
        TTV_LOGI("getNumbericalValue() succeded, tag [%d], value [%d].", tag, value);
        tag++;
    }

    {
        uint16_t value;
        if (!box.getNumbericalValue(tag, value)) {
            TTV_LOGE("Error: getNumbericalValue() failed.");      
            return -1;
        }
        TTV_LOGI("getNumbericalValue() succeded, tag [%d], value [%d].", tag, value);
        tag++;
    }
    {
        int16_t value;
        if (!box.getNumbericalValue(tag, value)) {
            TTV_LOGE("Error: getNumbericalValue() failed.");      
            return -1;
        }
        TTV_LOGI("getNumbericalValue() succeded, tag [%d], value [%d].", tag, value);
        tag++;
    }

    {
        uint32_t value;
        if (!box.getNumbericalValue(tag, value)) {
            TTV_LOGE("Error: getNumbericalValue() failed.");      
            return -1;
        }
        TTV_LOGI("getNumbericalValue() succeded, tag [%d], value [%d].", tag, value);
        tag++;
    }
    {
        int32_t value;
        if (!box.getNumbericalValue(tag, value)) {
            TTV_LOGE("Error: getNumbericalValue() failed.");      
            return -1;
        }
        TTV_LOGI("getNumbericalValue() succeded, tag [%d], value [%d].", tag, value);
        tag++;
    }

    {
        uint64_t value;
        if (!box.getNumbericalValue(tag, value)) {
            TTV_LOGE("Error: getNumbericalValue() failed.");      
            return -1;
        }
        TTV_LOGI("getNumbericalValue() succeded, tag [%d], value [%ld]", tag, value);
        tag++;
    }
    {
        int64_t value;
        if (!box.getNumbericalValue(tag, value)) {
            TTV_LOGE("Error: getNumbericalValue() failed.");      
            return -1;
        }
        TTV_LOGI("getNumbericalValue() succeded, tag [%d], value [%ld]", tag, value);
        tag++;
    }

    {
        float value;
        if (!box.getNumbericalValue(tag, value)) {
            TTV_LOGE("Error: getNumbericalValue() failed.");      
            return -1;
        }
        TTV_LOGI("getNumbericalValue() succeded, tag [%d], value [%f]", tag, value);
        tag++;
    }
    {
        double value;
        if (!box.getNumbericalValue(tag, value)) {
            TTV_LOGE("Error: getNumbericalValue() failed.");      
            return -1;
        }
        TTV_LOGI("getNumbericalValue() succeded, tag [%d], value [%lf]", tag, value);
        tag++;
    }

    {
        std::string value;
        if (!box.getStringValue(tag, value)) {
            TTV_LOGE("Error: getStringValue() failed.");
            return -1;
        }
        TTV_LOGI("getStringValue() succeded, tag [%d], value [%s]", tag, value.c_str());
        tag++;
    }

    {
        char* value = nullptr;
        if (!box.getBytesValue(tag, &value)) {
            TTV_LOGE("Error: getBytesValue() failed.");
            return -1;
        }
        TTV_LOGI("getBytesValue succeded, tag [%d], value [%s]", tag, value);
        tag++;
    }

    return 0;
}
