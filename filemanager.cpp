#include "filemanager.h"

bool fileManagerAlreadyInit(short mode)
{
    static bool init = false;
    switch(mode)
    {
        case 1:
            init = true;
            return true;
            break;
        case 2:
            init = false;
            return false;
            break;
        default:
            return init;
            break;
    }
    return false;;
}
bool fileManager() //controlKey only for internal useage!  dont pass any parameters on call
{
    
  #ifdef J54J6_LOGGING_H //use logging Libary if included
        SFLog::log("test");
  #endif
    //If filemanager is already initialized, skip and return
    if(fileManagerAlreadyInit())
    {
        return true;
    }

    bool fsState = LittleFS.begin();
    if(!fsState)
    {
        return false;
    }
    fileManagerAlreadyInit(1); //set already init to true
    return true;
}

bool initFileSystem()
{
    LittleFS.end();
    fileManagerAlreadyInit(2);
    if(LittleFS.format())
    {
        return true;
    }
    else
    {
        return false;
    }
    
}

bool checkForFile(const char* Filename)
{
    fileManager();
    if(LittleFS.exists(Filename))
    {
        return true;
    }
    else
    {
        return false;
    }
    
}

bool delFile(const char* Filename)
{
    fileManager();
    LittleFS.remove(Filename);
     if(!LittleFS.exists(Filename))
    {
        return true;
    }
    return false;
}

bool createFile(const char* Filename)
{
    fileManager();
    if(LittleFS.exists(Filename))
    {
        return true;
    }

    File newFile = LittleFS.open(Filename, "w");
    newFile.close();

    return LittleFS.exists(Filename);
}

bool writeInFile(const char* Filename, const char* pattern, const char* writeMode)
{
    fileManager();
    uint32_t bytesWritten;
    if(strcmp(writeMode, "a") && !LittleFS.exists(Filename))
    {
        File writeFile = LittleFS.open(Filename, "w");
        bytesWritten = writeFile.println(pattern);
        writeFile.close();
    }
    else
    {
        File writeFile = LittleFS.open(Filename, writeMode);
        bytesWritten = writeFile.println(pattern);
        writeFile.close();
    }
    
    if(bytesWritten == 0)
    {
        return false;
    }
    return true; 
}

bool writeJsonFile(const char* Filename,  const char* jsonPattern[][2], int amountOfData, const char* writeMode)
{
    fileManager();

    const size_t capacity = JSON_OBJECT_SIZE(25) + 400;
    DynamicJsonDocument jsonDocument(capacity);
    String jsonOutput;
    
    serializeJsonPretty(jsonDocument, jsonOutput);
    File writeFile = LittleFS.open(Filename, writeMode);
    uint32_t bytesWritten = writeFile.println(jsonOutput);
    writeFile.close();

    if(bytesWritten == 0)
    {
        return false;
    }
    return true; 
}

bool writeJsonFile(const char* Filename, DynamicJsonDocument jsonFile, const char* writeMode)
{
    fileManager();
    if(!LittleFS.exists(Filename))
    {
        bool newFile = createFile(Filename);
        if(!newFile)
        {
            return false;
        }
    }
    String jsonOutput;
    serializeJsonPretty(jsonFile, jsonOutput);

    File writeFile = LittleFS.open(Filename, writeMode);
    uint32_t bytesWritten = writeFile.println(jsonOutput);
    writeFile.close();

    if(bytesWritten == 0)
    {
        return false;
    }
    return true; 
}

bool changeJsonValueFile(const char* Filename, const char* key, const char* newValue)
{
    fileManager();
    DynamicJsonDocument jsonFile = readJsonFile(Filename);
    jsonFile[key] = newValue;
    bool val1 = writeJsonFile(Filename, jsonFile);
    if(!val1)
    {
        return false;
    }
    jsonFile = readJsonFile(Filename);
    if(newValue == jsonFile[key])
    {
        return true;
    }
    return false;

}

String readFile(const char* Filename)
{
    fileManager();
    if(!LittleFS.exists(Filename))
    {
        return {};
    }
    File readFile = LittleFS.open(Filename, "r");
    if(!readFile)
    {
        return {};
    }
    String output = readFile.readString();
    readFile.close();
    return output;
}

const char* readJsonFileValue(const char* Filename, const char* pattern)
{ 
    fileManager();
    const size_t capacity = JSON_OBJECT_SIZE(25) + 400;
    DynamicJsonDocument jsonDocument(capacity);
    if(!LittleFS.exists(Filename))
    {
        
        return {};
    }

    File readFile = LittleFS.open(Filename, "r"); //Open File

    if(!readFile)
    { 
        return {};
    }

    String output = readFile.readString();
    readFile.close();

    DeserializationError error = deserializeJson(jsonDocument, output);
    if(error)
    {
      Serial.print("Filemanager: ERROR -> ");
      Serial.println(error.c_str());
        return {};
    }
    const char* returnVal = jsonDocument[pattern]; //pattern need quotes too! e.g pattern = "\"id\""
    return returnVal;
}

DynamicJsonDocument readJsonFile(const char* Filename)
{
    fileManager();
    const size_t capacity = JSON_OBJECT_SIZE(25) + 400;
    DynamicJsonDocument jsonDocument(capacity);

    if(!LittleFS.exists(Filename))
    {
        return jsonDocument;
    }
    File readFile = LittleFS.open(Filename, "r");
    if(!readFile)
    {
        return jsonDocument;
    }

    String output = readFile.readString();
    readFile.close();

    DeserializationError error = deserializeJson(jsonDocument, output);
    if(error)
    {
        return jsonDocument;
    }
    return jsonDocument;
}

float getFreeSpace(short mode)
{
    FSInfo fs_info;
    fileManager();
    LittleFS.info(fs_info);
    float freeSpace;
    switch(mode)
    {
        case 1:
            float val1;
            float val2;
            val1 = fs_info.totalBytes;
            val2 = fs_info.usedBytes;
            
            freeSpace = val2/val1;
            freeSpace = freeSpace * 100;
            freeSpace = 100 - freeSpace;

            return freeSpace;
            break;
        default:
            freeSpace = fs_info.totalBytes - fs_info.usedBytes;
            return freeSpace;
            break;
    }
    return 0;
}

FSInfo getFilesystemInfo()
{ 
    FSInfo fs_info;
    fileManager();
    LittleFS.info(fs_info);
    return fs_info;
}