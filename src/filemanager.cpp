#include "filemanager.h"

bool Filemanager::checkForInit()
{
    if(!init)
    {
        if(autoFix)
        {
            if(mount())
            {
                return true;
            }
            else
            {
                return false;
            }   
        }
        else
        {
            return false;
        }   
    }
    return true;
}


Filemanager::Filemanager(bool tryAutoFix)
{    
    if(tryAutoFix)
    {
        this->autoFix = true;
    }
    else
    {
        this->autoFix = false;
    }
}

bool Filemanager::begin()
{
    if(this->init)
    {
        return true;
    }
    else
    {
        if(LittleFS.begin())
        {
            this->init = true;
            return true;
        }
        else
        {
            this->init = false;
            return false;
        }
    }
    return false;
}

bool Filemanager::mount()
{
    return begin();
}

void Filemanager::end() //Stop/unmount Filesystem
{
    if(init)
    {
        init = false;
        LittleFS.end();
        return;
    }
}

void Filemanager::unmount()
{
    return end();
}

bool Filemanager::format()
{
    auto formatFS = [] () 
    { //simple helper so i don't need to write it two times ^^
        if(LittleFS.format())
        {
            return true;
        }
        else
        {
            return false;
        }
    }; 

    if(init)
    {
        return formatFS();       
    }
    else
    {
        return formatFS();
    }
    return false;
}


bool Filemanager::mkdir(const char* path) //create Directory with $path - first symbol of $path must be "/"!
{
    bool alreadyTriedToFix = false;
    startPoint:

    if(!init)
    { 
        if(!autoFix || alreadyTriedToFix)
        {
            return false;
        }
        else
        {
            alreadyTriedToFix = true;
            if(this->begin())
            {
                goto startPoint; //only to restart the function - i ddon't want to call this function recursive
            }
        }
    }

    if(LittleFS.mkdir(path))
    {
        return true;
    }
    else
    {
        return false;
    }
    return false;
}

bool Filemanager::openDir(const char* path)
{
    if(LittleFS.exists(path))
    {
        internOpenDir = true;
        actualDir = LittleFS.openDir(path);
        return true;
    }
    else
    {
        return false;
    }
    return false;
}

Dir Filemanager::aOpenDir(const char* path)
{
    if(LittleFS.exists(path))
    {
        return LittleFS.openDir(path);
    }
    else
    {
        return {};
    }
    return {};
}

bool Filemanager::cd(const char* path)
{
    return openDir(path);
}

Dir Filemanager::aCd(const char* path)
{
   return aOpenDir(path);
}

bool Filemanager::rename(const char* oldName, const char* newName)
{
    if(!LittleFS.exists(oldName))
    {
        return false;
    }
    else
    {
        if(LittleFS.rename(oldName, newName))
        {
            return true;
        }
        else
        {
            /*
                Feature Upgrade #202011x
                    Return ErrorVal RCx from FS.h 
                    ->AutoFix enabled: use int Code to fix problem (checked: isMounted, isPath/[0])
            */
            return false;
        }
    }
}

bool Filemanager::move(const char* oldPath, const char* newPath)
{
    return rename(oldPath, newPath);
}

FSInfo Filemanager::info()
{
    FSInfo info;
    LittleFS.info(info);
    return info;
}

FSInfo64 Filemanager::info64()
{
    FSInfo64 info;
    LittleFS.info64(info);
    return info;
}

/*
    File control section
        Internal only Data Mode
*/

bool Filemanager::fOpen(const char* path, String mode)
{
    if(!init)
    {
        if(autoFix)
        {
            if(!mount())
            {
                return false;
            } 
        }
        else
        {
            return false;
        } 
    }

    if(mode != "r" && mode != "r+" && mode != "w" && mode != "w+" && mode != "a" && mode != "a+")
    {
        return false;
    }
    if(LittleFS.exists(path))
    {
        internFileOpen = true;
        internalFileHandle = LittleFS.open(path, mode.c_str());
        return true;
    }
    return false;
}

File Filemanager::fdOpen(const char* path, String mode)
{
    if(!init)
    {
        if(autoFix)
        {
            if(!mount())
            {
                return {};
            } 
        }
        else
        {
            return {};
        } 
    }

    if(mode != "r" && mode != "r+" && mode != "w" && mode != "w+" && mode != "a" && mode != "a+")
    {
        return {};
    }
    if(LittleFS.exists(path))
    {
        return LittleFS.open(path, mode.c_str());
    }
    return {};
}

bool Filemanager::fClose()
{
    if(!checkForInit())
    {
        return false;
    }
    internalFileHandle.close();
    internFileOpen = false;
    return true;
}

void Filemanager::fClose(File actualFile)
{
    if(!checkForInit())
    {
        return;
    }
    actualFile.close();
}

bool Filemanager::fDelete(const char* path)
{
    if(!checkForInit())
    {
        return false;
    }

    if(LittleFS.exists(path))
    {
        if(LittleFS.remove(path))
        {
            return true;
        }
        else
        {   
            return false;
        } 
    }
    else
    {
        return false;
    }
    return false;
}

bool Filemanager::rm(const char* path)
{
    return fDelete(path);
}

bool Filemanager::remove(const char* path)
{
    return fDelete(path);
}

bool Filemanager::fRm(const char* path)
{
    return fDelete(path);
}

bool Filemanager::fRemove(const char* path)
{
    return fDelete(path);
}

bool Filemanager::fWrite(const char* content)
{
    if(!checkForInit())
    {
        return false;
    }

    if(!internFileOpen)
    {
        return false;
    }
    else
    {
        internalFileHandle.write(content);
        return true;
    }
    return false;
}

bool Filemanager::fWrite(File actualFile, const char* content)
{
    if(!checkForInit())
    {
        return false;
    }
    
    actualFile.write(content);
    return true;
}

bool Filemanager::fExist(const char* path)
{
    if(!checkForInit())
    {
        return false;
    }
    bool res = LittleFS.exists(path);
    return res;
}

/*
    Dir control
*/
bool Filemanager::dNext()
{
    if(!checkForInit())
    {
        return false;
    }

    if(internOpenDir)
    {
        return actualDir.next();
    }
    else
    {
        return false;
    }
    return false;
}

bool Filemanager::dNext(Dir usedDir)
{
    if(!checkForInit())
    {
        return false;
    }
    return usedDir.next();
}

const char* Filemanager::getFName()
{
    if(!checkForInit())
    {
        return {};
    }

    if(internOpenDir)
    {
        return actualDir.fileName().c_str();
    }
    else
    {
        return {};
    }
    return {};
}

const char* Filemanager::getFName(Dir usedDir)
{
    if(!checkForInit())
    {
        return {};
    }
    return usedDir.fileName().c_str();
}

uint16 Filemanager::getFileSize()
{
    if(!checkForInit())
    {
        return -1;
    }

    if(internOpenDir)
    {
        return actualDir.fileSize();
    }
    else
    {
        return -1;
    }
    return -1;
}

uint16 Filemanager::getFileSize(Dir usedDir)
{
    if(!checkForInit())
    {
        return false;
    }
    return usedDir.fileSize();
}

bool Filemanager::rewind()
{
    if(!checkForInit())
    {
        return false;
    }

    if(internOpenDir)
    {
        return actualDir.rewind();
    }
    else
    {
        return false;
    }
    return false;
}

bool Filemanager::rewind(Dir usedDir)
{
    if(!checkForInit())
    {
        return false;
    }
    return usedDir.rewind();
}

bool Filemanager::dClose()
{
    if(!checkForInit())
    {
        return false;
    }

    if(internOpenDir)
    {
        internOpenDir = false;
        return true;
    }
    else
    {
        return false;
    }
    return false;
}

bool Filemanager::seek(uint8 offset, const char* mode)
{
    if(!checkForInit())
    {
        return false;
    }
    
    if(!internFileOpen)
    {
        return false;
    }
    return internalFileHandle.seek(offset);
}

bool Filemanager::seek(File usedFile, uint8 offset, const char* mode)
{
    if(!checkForInit())
    {
        return false;
    }
    
    return usedFile.seek(offset);
}

uint8 Filemanager::position()
{
    if(!checkForInit())
    {
        return false;
    }
    
    if(!internFileOpen)
    {
        return false;
    }
    return internalFileHandle.position();
}

uint8 Filemanager::position(File usedFile)
{
    if(!checkForInit())
    {
        return false;
    }
    
    return usedFile.position();
}

uint8 Filemanager::size()
{
    if(!checkForInit())
    {
        return false;
    }
    
    if(!internFileOpen)
    {
        return false;
    }
    return internalFileHandle.size();
}

uint8 Filemanager::size(const char* filename)
{
    if(!checkForInit())
    {
        return 0;
    }

    if(this->fExist(filename))
    {
        File getSizeOfFile = LittleFS.open(filename, "r");
        
        return getSizeOfFile.size();
    }
    return 0;
}

uint8 Filemanager::size(File usedFile)
{
    if(!checkForInit())
    {
        return false;
    }
    
    return usedFile.size();
}

const char* Filemanager::name()
{
    if(!checkForInit())
    {
        return {};
    }
    
    if(!internFileOpen)
    {
        return {};
    }
    return internalFileHandle.name();
}

const char* Filemanager::name(File usedFile)
{
    if(!checkForInit())
    {
        return {};
    }
    
    return usedFile.name();
}

const char* Filemanager::fullName()
{
    if(!checkForInit())
    {
        return {};
    }
    
    if(!internFileOpen)
    {
        return {};
    }
    return internalFileHandle.fullName();
}

const char* Filemanager::fullName(File usedFile)
{
    if(!checkForInit())
    {
        return {};
    }
    return usedFile.fullName();
}

long Filemanager::getLastWrite()
{
    if(!checkForInit())
    {
        return false;
    }
    
    if(!internFileOpen)
    {
        return false;
    }
    return internalFileHandle.getLastWrite();
}

long Filemanager::getLastWrite(File usedFile)
{
    if(!checkForInit())
    {
        return false;
    }
    
    return usedFile.getLastWrite();
}

long Filemanager::getCreationTime()
{
    if(!checkForInit())
    {
        return false;
    }
    
    if(!internFileOpen)
    {
        return false;
    }
    return internalFileHandle.getCreationTime();
}

long Filemanager::getCreationTime(File usedFile)
{
    if(!checkForInit())
    {
        return false;
    }
    return usedFile.getCreationTime();
}

bool Filemanager::writeInFile(const char* Filename, const char* pattern, const char* writeMode)
{
    if(!checkForInit())
    {
        return false;
    }

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

bool Filemanager::writeInFileStr(const char* Filename, String* pattern, const char* writeMode)
{
    if(!checkForInit())
    {
        return false;
    }
    uint32_t bytesWritten;
    if(strcmp(writeMode, "a") && !LittleFS.exists(Filename))
    {
        File writeFile = LittleFS.open(Filename, "w");
        bytesWritten = writeFile.print(pattern->c_str());
        writeFile.close();
    }
    else
    {
        File writeFile = LittleFS.open(Filename, writeMode);
        bytesWritten = writeFile.print(pattern->c_str());
        writeFile.close();
    }
    
    if(bytesWritten == 0)
    {
        return false;
    }
    return true; 
}

bool Filemanager::writeJsonFile(const char* Filename,  const char* jsonPattern[][2], int amountOfData, const char* writeMode)
{

    if(!checkForInit())
    {
        return false;
    }

    if(amountOfData == 0)
    {
        Serial.println("NO DATA PASSED - SKIP");
        return true;
    }
    const size_t capacity = JSON_OBJECT_SIZE(25) + 400;
    DynamicJsonDocument jsonDocument(capacity);

    for(int i = 0; i < amountOfData; i++)
    {
        jsonDocument[jsonPattern[i][0]] = jsonPattern[i][1];
    }

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

bool Filemanager::writeJsonFile(const char* Filename, DynamicJsonDocument jsonFile, const char* writeMode)
{
    if(!checkForInit())
    {
        return false;
    }

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

bool Filemanager::changeJsonValueFile(const char* Filename, const char* key, const char* newValue)
{
    if(!checkForInit())
    {
        return false;
    }
    DynamicJsonDocument jsonFile = readJsonFile(Filename);
    if(!checkForKeyInJSONFile(Filename, key))
    {
        return appendJsonKey(Filename, key, newValue);
    }
    else
    {
        jsonFile[key] = newValue;
        bool val1 = writeJsonFile(Filename, jsonFile, "w");
        if(!val1)
        {
            return false;
        }
    }

    jsonFile = readJsonFile(Filename);
    if(newValue == jsonFile[key])
    {
        return true;
    }
    return false;
}

String Filemanager::readFile(const char* Filename)
{
    if(!checkForInit())
    {
        return "noInit";
    }
    if(!LittleFS.exists(Filename))
    {
        return "notExist";
    }
    File readFile = LittleFS.open(Filename, "r");
    if(!readFile)
    {
        return "Can't read!";
    }
    String output = readFile.readString();
    readFile.close();
    return output;
}

/*
String* Filemanager::readFilePointed(const char* Filename)
{
    if(!checkForInit())
    {
        return {};
    }
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
    return &output;
}
*/
const char* Filemanager::readJsonFileValue(const char* Filename, const char* pattern)
{ 
    if(!checkForInit())
    {
        Serial.println(F("notInit - return NULL"));
        return {};
    }
    const size_t capacity = JSON_OBJECT_SIZE(25) + 400;
    DynamicJsonDocument jsonDocument(capacity);
    if(!LittleFS.exists(Filename))
    {
        Serial.println("File " + String(Filename) + String(" doesn't exist!"));
        return {};
    }

    File readFile = LittleFS.open(Filename, "r"); //Open File

    if(!readFile)
    { 
        Serial.println(F("Can't read File!"));
        return {};
    }
    String output = this->readFile(Filename);

    DeserializationError error = deserializeJson(jsonDocument, output);
    if(error)
    {
        Serial.println(F("Error while reading File!"));
        return {};
    }
    if(jsonDocument.containsKey(pattern))
    {
        const char* returnVal = jsonDocument[pattern]; //pattern need quotes too! e.g pattern = "\"id\""
        //Serial.println("Readed from FS: " + String(returnVal) + ", Key: " + String(pattern) + ", File: " + String(Filename));
        return returnVal;
    }
    else
    {
        return "";
    }
    
}

String Filemanager::readJsonFileValueAsString(const char* Filename, const char* pattern)
{ 
    if(!checkForInit())
    {
        Serial.println(F("notInit - return NULL"));
        return {};
    }
    const size_t capacity = JSON_OBJECT_SIZE(25) + 400;
    DynamicJsonDocument jsonDocument(capacity);
    if(!LittleFS.exists(Filename))
    {
        Serial.println(F("File doesn't exist!"));
        return {};
    }

    File readFile = LittleFS.open(Filename, "r"); //Open File

    if(!readFile)
    { 
        Serial.println(F("Can't read File!"));
        return {};
    }
    String output = this->readFile(Filename);

    DeserializationError error = deserializeJson(jsonDocument, output);
    if(error)
    {
        Serial.println(F("Error while reading File!"));
        return {};
    }
    if(jsonDocument.containsKey(pattern))
    {
        String returnVal = jsonDocument[pattern]; //pattern need quotes too! e.g pattern = "\"id\""
        //Serial.println("Readed from FS: " + String(returnVal));
        return returnVal;
    }
    else
    {
        return "";
    }
    
}

DynamicJsonDocument Filemanager::readJsonFile(const char* Filename)
{
    const size_t capacity = JSON_OBJECT_SIZE(25) + 400;
    DynamicJsonDocument jsonDocument(capacity);
    if(!checkForInit())
    {
        return jsonDocument;
    }
    

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
        this->error = true;
        return jsonDocument;
    }
    return jsonDocument;
}

float Filemanager::getFreeSpace(short mode)
{
    FSInfo fs_info;
    if(!checkForInit())
    {
        return 0;
    }
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

void Filemanager::space(int number)
{
  if(number == 0)
  {
    return;
  }
  for(int i = 0; i < number;  i++)
  {
    Serial.print(F("   "));
  }
}

int Filemanager::getNumberOfFiles(String path)
{
  int amountFilesInRoot = 0;
  Dir dir = LittleFS.openDir(path);
  while(dir.next())
  {
    amountFilesInRoot++;
  }
  return amountFilesInRoot;
}

void Filemanager::recursive(String path)
{
  static int insideDirs = 0;
  static String mainPath = "";
  String oldPath = mainPath;
  mainPath = path;
  Dir dir = LittleFS.openDir(path);
  int number = getNumberOfFiles(path);
  int actual = 0;
  while(actual <= number)
  {
    dir.next();
    space(insideDirs);
    if(dir.isFile())
    {
      Serial.print(F("|- "));
      Serial.print(dir.fileName());
      Serial.print(F(" -> "));
      Serial.print(dir.fileSize());
      Serial.println(F("Byte"));
    }
    if(dir.isDirectory())
    {
      insideDirs++;
      String cPath = mainPath + "/" + dir.fileName();
      Serial.print(F("|--| "));
      Serial.println(dir.fileName());
      recursive(cPath);
    }
    actual++;
    if(actual >= number)
    {
      if(insideDirs > 0)
      {
        insideDirs--;
      }
    }
  }  
  mainPath = oldPath;
}

void Filemanager::getSerialFileStructure(const char* path)
{
    recursive(path);
}

bool Filemanager::createFile(const char* Filename)
{
    if(!checkForInit())
    {
        return false;
    }
    if(LittleFS.exists(Filename))
    {
        return true;
    }

    File newFile = LittleFS.open(Filename, "w");
    newFile.close();

    return LittleFS.exists(Filename);
}

bool Filemanager::returnAsBool(const char* val)
{
    String format = val;
    format.replace(" ", "");
    if(format == "False" || format == "false" || format == "FALSE")
    {
        return false;
    }
    else if(format == "True" || format == "true" || format == "TRUE")
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Filemanager::checkForKeyInJSONFile(const char* filename, const char* key)
{
    if(!fExist(filename))
    {
        return false;
    }

    const size_t capacity = JSON_OBJECT_SIZE(25) + 400;
    DynamicJsonDocument cacheDocument(capacity);

    cacheDocument = readJsonFile(filename);
        
    if(cacheDocument.containsKey(key))
    {
        return true;
    }
    else
    {
        return false;
    }
}


bool Filemanager::appendJsonKey(const char* filename, const char* newKey, const char* newVal)
{
    begin();
    if(fExist(filename))
    {
        if(checkForKeyInJSONFile(filename, newKey))
        {
            return changeJsonValueFile(filename, newKey, newVal);
        }

        const size_t capacity = JSON_OBJECT_SIZE(25) + 400;
        StaticJsonDocument<capacity> cacheDocument;

        String data = readFile(filename);

        //for debug
        /*
        Serial.println("--------------------------------");
        Serial.println(data);
        Serial.println("--------------------------------");
        */

        if(!data.isEmpty())
        {
            deserializeJson(cacheDocument, data);
        }
        cacheDocument[newKey] = newVal;

        writeJsonFile(filename, cacheDocument);
        if(checkForKeyInJSONFile(filename, newKey))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        if(createFile(filename))
        {
            return appendJsonKey(filename, newKey, newVal);
        }
        else
        {
            return false;
        }
    }
}

bool Filemanager::delJsonKeyFromFile(const char* filename, const char* key)
{
    if(fExist(filename))
    {
        return false;
    }

    if(!checkForKeyInJSONFile(filename, key))
    {
        return true;
    }

    const size_t capacity = JSON_OBJECT_SIZE(25) + 400;
    DynamicJsonDocument cacheDocument(capacity);

    cacheDocument = readJsonFile(filename);

    cacheDocument.remove(key);

    if(checkForKeyInJSONFile(filename, key))
    {
        return false;
    }
    return true;
}