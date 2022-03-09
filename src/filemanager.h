#pragma once
    #include <Arduino.h>
    #include <ArduinoJson.h>
    #include <SPI.h>
    #include <LittleFS.h>


    class Filemanager {
        private:
            const char* className = "Filemanager";
            bool init = false;
            bool internOpenDir = false;
            bool internFileOpen = false;
            bool autoFix = false;

            bool checkForInit();
            void space(int number);
            int getNumberOfFiles(String path);
            void recursive(String path);
            
        protected:
            File internalFileHandle;
            Dir actualDir;

        public: 
            bool error = false;
            
            Filemanager(bool tryAutoFix = false); //Autofix is at this time intended to help prevent syntax fails - e.g open file without mounted FS 
            bool begin(); //start/mount Filesystem - same like mount()
            bool mount(); //link to begin()
            void end(); //unmount / stop LittleFS Filesystem
            void unmount(); //link to end()
            bool format(); //format LitteFS Filesystem useable before and after call begin()
            bool mkdir(const char* path); //create Directory with $path - first symbol of $path must be "/" if you want to use a subdirectory!
            bool openDir(const char* path); //opens a folder at $path, if you want to use a subdirectory you need to use the complete path from root (e.g /datafolder/config/)
            Dir aOpenDir(const char* path);
            bool cd(const char* path); //link to openDir
            Dir aCd(const char* path);
            bool rename(const char* oldName, const char* newName); //rename/move a File or Direcotry (Directory needs to be empty!) 
            bool move(const char* oldPath, const char* newPath); //link to rename
            // bool gc(); //garbagecollector - normally you don't need to use this function - not implemented yet...
            FSInfo info(); //used on storage-sizes < 4GB - returns the FSInfo struct (e.g with capacity of FS, used Bytes etc.
            FSInfo64 info64(); //used with storage-sizes > 4GB - returns the FSInfo struct (e.g with capacity of FS, used Bytes etc.

            /*
                File control
            */
            bool fOpen(const char* path, String mode); //internal Filehandling (much easier but less oportuinities)
            File fdOpen(const char* path, String mode); //direct Filehandling - Fileobnject returned
            bool fClose();
            void fClose(File actualFile);
            bool fDelete(const char* path);  //remove File with $path - can't remove Folder!
            bool rm(const char* path); //remove File with $path - can't remove Folder!  - link to fDelete
            bool remove(const char* path); //link to rm
            bool fWrite(const char* content);
            bool fWrite(File actualFile, const char* content);
            bool fExist(const char* path);
            bool fRm(const char* file); //link to rm
            bool fRemove(const char* file); //link to rm

            /*
                Dir operations
            */
            bool dNext(); //true if there are files to iteratate in Directory
            bool dNext(Dir usedDir);
            const char* getFName(); //return Filename of pointed Object
            const char* getFName(Dir usedDir);
            uint16 getFileSize(); //return fileSize of pointed Object
            uint16 getFileSize(Dir usedDir);
            bool isFile(); //return true if pointed object is a File
            bool isFile(Dir usedDir);
            bool isDirectory(); //return true if pointed object is a Directory
            bool isDirectory(Dir usedDir);
            bool rewind(); //reset the pointer to start of directory
            bool rewind(Dir usedDir);
            bool dClose();
            /*
                File operations
            */
            bool seek(uint8 offset, const char* mode);
            bool seek(File usedFile, uint8 offset, const char* mode);
            uint8 position();
            uint8 position(File usedFile);
            uint8 size(const char* filename);
            uint8 size();
            uint8 size(File usedFile);
            const char* name();
            const char* name(File usedFile);
            const char* fullName();
            const char* fullName(File usedFile);
            long getLastWrite();
            long getLastWrite(File usedFile);
            long getCreationTime();
            long getCreationTime(File usedFile);

            /*
                 Special functionalities
             */
            bool createFile(const char* Filename);
            bool writeInFile(const char* Filename, const char* pattern, const char* writeMode = "w");
            bool writeInFileStr(const char* Filename, String* pattern, const char* writeMode = "w");
            bool writeJsonFile(const char* Filename,  const char* jsonPattern[][2], int amountOfData, const char* writeMode ="w");
            bool writeJsonFile(const char* Filename, DynamicJsonDocument jsonFile, const char* writeMode = "w");
            bool changeJsonValueFile(const char* Filename, const char* key, const char* newValue);
            String readFile(const char* Filename);
            //String* readFilePointed(const char* Filename);
            const char* readJsonFileValue(const char* Filename, const char* pattern);
            String readJsonFileValueAsString(const char* Filename, const char* pattern);
            DynamicJsonDocument readJsonFile(const char* Filename);
            float getFreeSpace(short mode);
            void getSerialFileStructure(const char* path = "/");
            bool returnAsBool(const char* val);
            bool checkForKeyInJSONFile(const char* filename, const char* key);
            bool appendJsonKey(const char* filename, const char* newKey, const char* newVal);
            bool delJsonKeyFromFile(const char* filename, const char* key);
    };