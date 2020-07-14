#ifdef J54J6_FILEMANAGER_H
#undef J54J6_FILEMANAGER_H
#endif
#ifndef J54J6_FILEMANAGER_H
    #define J54J6_FILEMANAGER_H
    #include <Arduino.h>
    #include <ArduinoJson.h>
    #include <FS.h>
    #include <SPI.h>
    #include <LittleFS.h>

    /*
        filesystem control
    */
    bool fileManagerAlreadyInit(short mode = 0); //internal  function - not for normal use...
    bool fileManager(); //Start / Mount Filesystem
    bool initFileSystem(); //Re-Init Filesystem - delete All Files 
    bool checkForFile(const char* Filename); //check for File (exist(Filename))
    bool delFile(const char* Filename); //delete File
    bool createFile(const char* Filename); //create new File

    /*
        write in Files
    */
    bool writeInFile(const char* Filename, const char* pattern, const char* writeMode = "w"); //writes a plain Text into a File - writeModes (w (write in File), a (append to File))
    bool writeJsonFile(const char* Filename, const char* jsonPattern[][2],  int amountOfData, const char* writeMode = "w"); //writes JSON in File - use 2. Dim Array with key - Value e.g const char* new Array[3][2] = {{"myKey", "myValue"}, {"mySeondKey", mySecondValue}, {"key", "val"}}
    bool writeJsonFile(const char* Filename, DynamicJsonDocument jsonFile, const char* writeMode = "w"); //writes JSON in File - with DynamicJSONDocument - check ArduinoJSON Documentation for further information
    bool changeJsonValueFile(const char* Filename, const char* key, const char* newValue); //change only one special Value in pre. Generated File and save it

    /*
        readout Files
    */
    String readFile(const char* Filename); //read complete File in one String
    const char* readJsonFileValue(const char* Filename, const char* pattern); //read only one value from given key 
    DynamicJsonDocument readJsonFile(const char* Filename); //return complete JSON File

    /*
        Information about Filesystem
    */
   float getFreeSpace(short mode = 0); //mode 0 = in bytes, 1 = as percentage e.g 57.3% free disk Space
   FSInfo getFilesystemInfo(); //return normal LittleFS FSInfo struct for cont. usage
#endif
