#include "filemanager.h"

bool Filemanager::checkForInit()
{
    if(!init)
    {
        #ifdef J54J6_LOGGING_H //use logging Libary if included
            logger::SFLog(className, "checkForInit", "Filesystem not mounted!", 2);
        #endif
        if(autoFix)
        {
            #ifdef J54J6_LOGGING_H //use logging Libary if included
                logger::SFLog(className, "checkForInit", "AutoFix enabled - try to mount Filesystem");
            #endif
            if(mount())
            {
                #ifdef J54J6_LOGGING_H //use logging Libary if included
                    logger::SFLog(className, "checkForInit", "Filesystem successfully mounted");
                #endif
                return true;
            }
            else
            {
                #ifdef J54J6_LOGGING_H //use logging Libary if included
                    logger::SFLog(className, "checkForInit", "Can't mount Filesyste", 2);
                #endif
                return false;
            }
            
        }
        else
        {
            #ifdef J54J6_LOGGING_H //use logging Libary if included
                    logger::SFLog(className, "checkForInit", "AutoFix disabled - SKIP");
            #endif
            return false;
        }
        
    }
    return true;
}


Filemanager::Filemanager(bool tryAutoFix)
{
    #ifdef J54J6_LOGGING_H //use logging Libary if included
        logger::SFLog(className, "Filemanager", "Start Filemanager");
    #endif
    
    if(tryAutoFix)
    {
        #ifdef J54J6_LOGGING_H //use logging Libary if included
        logger::SFLog(className, "Filemanager", "Autofix enabled");
        #endif
        this->autoFix = true;
    }
    else
    {
        #ifdef J54J6_LOGGING_H //use logging Libary if included
        logger::SFLog(className, "Filemanager", "Autofix disabled");
        #endif
        this->autoFix = false;
    }
    
}

bool Filemanager::begin()
{
    #ifdef J54J6_LOGGING_H //use logging Libary if included
        logger::SFLog(className, "begin", "Mount Filesystem");
    #endif

    if(this->init)
    {
        #ifdef J54J6_LOGGING_H //use logging Libary if included
            logger::SFLog(className, "begin", "Filesystem already mounted - SKIP", 1);
        #endif
        return true;
    }
    else
    {
        #ifdef J54J6_LOGGING_H //use logging Libary if included
            logger::SFLog(className, "begin", "Try to mount Filesystem");
        #endif
        if(LittleFS.begin())
        {
            #ifdef J54J6_LOGGING_H //use logging Libary if included
                logger::SFLog(className, "begin", "Try to mount Filesystem");
            #endif
            this->init = true;
            return true;
        }
        else
        {
            #ifdef J54J6_LOGGING_H //use logging Libary if included
                logger::SFLog(className, "begin", "Can't init Filesystem!", 2);
            #endif
            this->init = false;
            return false;
        }
        
    }
    #ifdef J54J6_LOGGING_H //use logging Libary if included
        logger::SFLog(className, "begin", "undefined - unexcepted Jump! \n further assistance needed", 2);
    #endif
    return false;
}

bool Filemanager::mount()
{
    return begin();
}

void Filemanager::end() //Stop/unmount Filesystem
{
    #ifdef J54J6_LOGGING_H //use logging Libary if included
        logger::SFLog(className, "end", "Stop LitteFS Filesystem");
    #endif
    if(init)
    {
        init = false;
        LittleFS.end();
        return;
    }
    else
    {
        #ifdef J54J6_LOGGING_H //use logging Libary if included
            logger::SFLog(className, "end", "Just for dev. - init state maybe inconsistent - init = false! - please check...");
        #endif
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
            #ifdef J54J6_LOGGING_H //use logging Libary if included
            logger::SFLog("Filemanager", "lambda - formatFS", "filesystem successfully formatted");
            #endif
            return true;
        }
        else
        {
            #ifdef J54J6_LOGGING_H //use logging Libary if included
            logger::SFLog("Filemanager", "lambda - formatFS", "filesystem can't be formatted - please check...");
            #endif
            return false;
        }
    }; 

    #ifdef J54J6_LOGGING_H //use logging Libary if included
            logger::SFLog("Filemanager", "format", "try to format Filesystem");
    #endif
    if(init)
    {
        #ifdef J54J6_LOGGING_H //use logging Libary if included
            logger::SFLog("Filemanager", "end", "FS isn't initialized...");
        #endif

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

    #ifdef J54J6_LOGGING_H //use logging Libary if included
        logger::SFLog(className, "mkdir", "try to create Folder");
    #endif
    if(!init)
    { 
        #ifdef J54J6_LOGGING_H //use logging Libary if included
            logger::SFLog(className, "mkdir", "can't create Directory - no Filesystem mounted!", 2);
        #endif
        if(!autoFix || alreadyTriedToFix)
        {
            #ifdef J54J6_LOGGING_H //use logging Libary if included
                logger::SFLog(className, "mkdir", "AutoFix disabled or already tried - End with Fail", 2);
            #endif
            return false;
        }
        else
        {
            alreadyTriedToFix = true;
            #ifdef J54J6_LOGGING_H //use logging Libary if included
                logger::SFLog(className, "mkdir", "AutoFix anabled - mount Filesystem");
            #endif
            if(this->begin())
            {
                #ifdef J54J6_LOGGING_H //use logging Libary if included
                    logger::SFLog(className, "mkdir", "Filesystem successfully mounted");
                #endif
                goto startPoint; //only to restart the function - i ddon't want to call this function recursive
            }
            else
            {
                #ifdef J54J6_LOGGING_H //use logging Libary if included
                    logger::SFLog(className, "mkdir", "Can't mount Filesystem - please check...", 2);
                #endif
            }
        }
    }

    if(LittleFS.mkdir(path))
    {
        #ifdef J54J6_LOGGING_H //use logging Libary if included
            logger::SFLog(className, "mkdir", "Directory created");
        #endif
        return true;
    }
    else
    {
        #ifdef J54J6_LOGGING_H //use logging Libary if included
            logger::SFLog(className, "mkdir", "Directory created");
        #endif
        return false;
    }
    #ifdef J54J6_LOGGING_H //use logging Libary if included
        logger::SFLog(className, "mkdir", "undefined - unexcepted Jump! \n further assistance needed", 2);
    #endif
    return false;
}

bool Filemanager::openDir(const char* path)
{
    #ifdef J54J6_LOGGING_H //use logging Libary if included
        String message = "open Dir '";
        message += path;
        message += "'";
        logger::SFLog(className, "openDir", message.c_str());
    #endif
    if(LittleFS.exists(path))
    {
        #ifdef J54J6_LOGGING_H //use logging Libary if included
            logger::SFLog(className, "openDir", "Dir opened");
        #endif
        internOpenDir = true;
        actualDir = LittleFS.openDir(path);
        return true;
    }
    else
    {
        #ifdef J54J6_LOGGING_H //use logging Libary if included
            logger::SFLog(className, "openDir", "Can't open Dir", 1);
        #endif
        return false;
    }
    return false;
}

Dir Filemanager::aOpenDir(const char* path)
{
    #ifdef J54J6_LOGGING_H //use logging Libary if included
        String message = "Open Dir '";
        message += path;
        message += "'";
        logger::SFLog(className, "aOpenDir", message.c_str());
    #endif
    if(LittleFS.exists(path))
    {
        return LittleFS.openDir(path);
    }
    else
    {
        #ifdef J54J6_LOGGING_H //use logging Libary if included
            logger::SFLog(className, "aOpenDir", "Can't open Dir  ", 1);
        #endif
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
    #ifdef J54J6_LOGGING_H //use logging Libary if included
        logger::SFLog(className, "rename", "rename File", 0);
    #endif
    if(!LittleFS.exists(oldName))
    {
        #ifdef J54J6_LOGGING_H //use logging Libary if included
            String message = "Can't rename/move File - ";
            message += oldName;
            message += " to ";
            message += newName;
            message += " - File does not exist!";
            logger::SFLog(className, "rename", message.c_str(), 1);
        #endif
        return false;
    }
    else
    {
        if(LittleFS.rename(oldName, newName))
        {
            #ifdef J54J6_LOGGING_H //use logging Libary if included
                logger::SFLog(className, "rename", "File successfully renamed/moved ", 0);
            #endif
            return true;
        }
        else
        {
            /*
                Feature Upgrade #202011x
                    Return ErrorVal RCx from FS.h 
                    ->AutoFix enabled: use int Code to fix problem (checked: isMounted, isPath/[0])
            */
            #ifdef J54J6_LOGGING_H //use logging Libary if included
                String message = "Can't rename/move File - ";
                message += oldName;
                message += " to ";
                message += newName;
                message += " - function return 'false'!";
                logger::SFLog(className, "rename", message.c_str(), 1);
            #endif
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
    #ifdef J54J6_LOGGING_H //use logging Libary if included
        String message = "try to open File - ";
        message += path;
        message += " with mode: ";
        message += mode;
        logger::SFLog(className, "fOpen", message.c_str(), 0);
    #endif
    if(!init)
    {
        if(autoFix)
        {
            #ifdef J54J6_LOGGING_H //use logging Libary if included
                logger::SFLog(className, "fOpen", "Filesystem not mounted - try to mount it...", 0);
            #endif
            if(mount())
            {
                #ifdef J54J6_LOGGING_H //use logging Libary if included
                logger::SFLog(className, "fOpen", "Filesystem mounted... - continue", 0);
                #endif
            }
            else
            {
                #ifdef J54J6_LOGGING_H //use logging Libary if included
                    logger::SFLog(className, "fOpen", "Can't mount Filesystem", 2);
                #endif
                return false;
            } 
        }
        else
        {
            #ifdef J54J6_LOGGING_H //use logging Libary if included
            logger::SFLog(className, "fOpen", "Filesystem not mounted and autoFix disabled", 2);
            #endif
            return false;
        } 
    }

    if(mode != "r" && mode != "r+" && mode != "w" && mode != "w+" && mode != "a" && mode != "a+")
    {
        #ifdef J54J6_LOGGING_H //use logging Libary if included
            message = "Mode ";
            message += mode;
            message += "is not supported. Please only use : 'w(+) (writing) - r(+) reading - a(+) (append)";
            message += "for further information please check Link: https://arduino-esp8266.readthedocs.io/en/latest/filesystem.html#open";
            logger::SFLog(className, "fOpen", message.c_str(), 1);
        #endif
        return false;
    }
    if(LittleFS.exists(path))
    {
        #ifdef J54J6_LOGGING_H //use logging Libary if included
            logger::SFLog(className, "fOpen", "File opened", 0);
        #endif
        internFileOpen = true;
        internalFileHandle = LittleFS.open(path, mode.c_str());
        return true;
    }
    else
    {
        #ifdef J54J6_LOGGING_H //use logging Libary if included
            message = "File ";
            message += path;
            message += "doesn't exist!";
            logger::SFLog(className, "fOpen", message.c_str(), 2);
        #endif
    }
    return false;
}

File Filemanager::fdOpen(const char* path, String mode)
{
    #ifdef J54J6_LOGGING_H //use logging Libary if included
        String message = "try to open File - ";
        message += path;
        message += " with mode: ";
        message += mode;
        logger::SFLog(className, "fOpen", mode.c_str(), 0);
    #endif
    if(!init)
    {
        if(autoFix)
        {
            #ifdef J54J6_LOGGING_H //use logging Libary if included
                logger::SFLog(className, "fOpen", "Filesystem not mounted - try to mount it...", 0);
            #endif
            if(mount())
            {
                #ifdef J54J6_LOGGING_H //use logging Libary if included
                logger::SFLog(className, "fOpen", "Filesystem mounted... - continue", 0);
                #endif
            }
            else
            {
                #ifdef J54J6_LOGGING_H //use logging Libary if included
                    logger::SFLog(className, "fOpen", "Can't mount Filesystem", 2);
                #endif
                return {};
            } 
        }
        else
        {
            #ifdef J54J6_LOGGING_H //use logging Libary if included
            logger::SFLog(className, "fOpen", "Filesystem not mounted and autoFix disabled", 2);
            #endif
            return {};
        } 
    }

    if(mode != "r" && mode != "r+" && mode != "w" && mode != "w+" && mode != "a" && mode != "a+")
    {
        #ifdef J54J6_LOGGING_H //use logging Libary if included
            message = "Mode ";
            message += mode;
            message += "is not supported. Please only use : 'w(+) (writing) - r(+) reading - a(+) (append)";
            message += "for further information please check Link: https://arduino-esp8266.readthedocs.io/en/latest/filesystem.html#open";
            logger::SFLog(className, "fOpen", message.c_str(), 1);
        #endif
        return {};
    }
    if(LittleFS.exists(path))
    {
        #ifdef J54J6_LOGGING_H //use logging Libary if included
            logger::SFLog(className, "fOpen", "File opened", 0);
        #endif
        return LittleFS.open(path, mode.c_str());
    }
    else
    {
        #ifdef J54J6_LOGGING_H //use logging Libary if included
            message = "File ";
            message += path;
            message += "doesn't exist!";
            logger::SFLog(className, "fOpen", message.c_str(), 2);
        #endif
    }
    return {};
}

bool Filemanager::fClose()
{
    if(!checkForInit())
    {
        #ifdef J54J6_LOGGING_H //use logging Libary if included
            logger::SFLog(className, "fClose", "init check failed!");
        #endif
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
        #ifdef J54J6_LOGGING_H //use logging Libary if included
            logger::SFLog(className, "fClose", "init check failed!");
        #endif
        return;
    }
    actualFile.close();
    return;
}

bool Filemanager::fDelete(const char* path)
{
    if(!checkForInit())
    {
        #ifdef J54J6_LOGGING_H //use logging Libary if included
            logger::SFLog(className, "fClose", "init check failed!");
        #endif
        return false;
    }

    if(LittleFS.exists(path))
    {
        if(LittleFS.remove(path))
        {
            #ifdef J54J6_LOGGING_H //use logging Libary if included
                logger::SFLog(className, "fDelete", "File removed!");
            #endif
            return true;
        }
        else
        {   
            #ifdef J54J6_LOGGING_H //use logging Libary if included
                logger::SFLog(className, "fDelete", "can't remove File - undefined Error - implemented soon :)");
            #endif
            return false;
        }
        
    }
    else
    {
        #ifdef J54J6_LOGGING_H //use logging Libary if included
            logger::SFLog(className, "fDelete", "File doesn't exist!", 2);
        #endif
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
        #ifdef J54J6_LOGGING_H //use logging Libary if included
            logger::SFLog(className, "fWrite", "Error Init. Check - writing failed!", 2);
        #endif
        return false;
    }

    if(!internFileOpen)
    {
        #ifdef J54J6_LOGGING_H //use logging Libary if included
            logger::SFLog(className, "fWrite", "There is no opened File!", 2);
        #endif
        return false;
    }
    else
    {
        internalFileHandle.write(content);
        #ifdef J54J6_LOGGING_H //use logging Libary if included
            logger::SFLog(className, "fWrite", "Content is written in File!", 0);
        #endif
        return true;
    }
    return false;
}

bool Filemanager::fWrite(File actualFile, const char* content)
{
    if(!checkForInit())
    {
        #ifdef J54J6_LOGGING_H //use logging Libary if included
            logger::SFLog(className, "fWrite", "Error Init. Check - writing failed!", 2);
        #endif
        return false;
    }
    
    actualFile.write(content);
    #ifdef J54J6_LOGGING_H //use logging Libary if included
        logger::SFLog(className, "fWrite", "Content is written in File!", 0);
    #endif
    return true;
}

bool Filemanager::fExist(const char* path)
{
    if(!checkForInit())
    {
        #ifdef J54J6_LOGGING_H //use logging Libary if included
            logger::SFLog(className, "fWrite", "Error Init. Check - writing failed!", 2);
        #endif
        return false;
    }
    bool res = LittleFS.exists(path);
    #ifdef J54J6_LOGGING_H //use logging Libary if included
            String message = "File ";
            message += path;
            message += "exists?: ";
            message += res;
            logger::SFLog(className, "fExist", message.c_str(), 0);
    #endif
    return res;
}

/*
    Dir control
*/

bool Filemanager::dNext()
{
    if(!checkForInit())
    {
        #ifdef J54J6_LOGGING_H //use logging Libary if included
            logger::SFLog(className, "dNext", "Error Init. Check - writing failed!", 2);
        #endif
        return false;
    }

    if(internOpenDir)
    {
        #ifdef J54J6_LOGGING_H //use logging Libary if included
            logger::SFLog(className, "dNext", "pointer to next File in Dir or end reached...", 0);
        #endif
        return actualDir.next();
    }
    else
    {
        #ifdef J54J6_LOGGING_H //use logging Libary if included
            logger::SFLog(className, "dNext", "No Dir opened - please use (a)openDir()", 2);
        #endif
        return false;
    }
    return false;
}

bool Filemanager::dNext(Dir usedDir)
{
    if(!checkForInit())
    {
        #ifdef J54J6_LOGGING_H //use logging Libary if included
            logger::SFLog(className, "dNext", "Error Init. Check - writing failed!", 2);
        #endif
        return false;
    }
    return usedDir.next();
}

const char* Filemanager::getFName()
{
    if(!checkForInit())
    {
        #ifdef J54J6_LOGGING_H //use logging Libary if included
            logger::SFLog(className, "getFName", "Error Init. Check - writing failed!", 2);
        #endif
        return false;
    }

    if(internOpenDir)
    {
        #ifdef J54J6_LOGGING_H //use logging Libary if included
            logger::SFLog(className, "getFName", "pointer to next File in Dir or end reached...", 0);
        #endif
        return actualDir.fileName().c_str();
    }
    else
    {
        #ifdef J54J6_LOGGING_H //use logging Libary if included
            logger::SFLog(className, "getFName", "No Dir opened - please use (a)openDir()", 2);
        #endif
        return false;
    }
    return false;
}

const char* Filemanager::getFName(Dir usedDir)
{
    if(!checkForInit())
    {
        #ifdef J54J6_LOGGING_H //use logging Libary if included
            logger::SFLog(className, "getFName", "Error Init. Check - writing failed!", 2);
        #endif
        return false;
    }
    return usedDir.fileName().c_str();
}

uint Filemanager::getFileSize()
{
    if(!checkForInit())
    {
        #ifdef J54J6_LOGGING_H //use logging Libary if included
            logger::SFLog(className, "getFileSize", "Error Init. Check - writing failed!", 2);
        #endif
        return false;
    }

    if(internOpenDir)
    {
        #ifdef J54J6_LOGGING_H //use logging Libary if included
            logger::SFLog(className, "getFileSize", "pointer to next File in Dir or end reached...", 0);
        #endif
        return actualDir.fileSize();
    }
    else
    {
        #ifdef J54J6_LOGGING_H //use logging Libary if included
            logger::SFLog(className, "getFileSize", "No Dir opened - please use (a)openDir()", 2);
        #endif
        return false;
    }
    return false;
}

uint Filemanager::getFileSize(Dir usedDir)
{
    if(!checkForInit())
    {
        #ifdef J54J6_LOGGING_H //use logging Libary if included
            logger::SFLog(className, "getFName", "Error Init. Check - writing failed!", 2);
        #endif
        return false;
    }
    return usedDir.fileSize();
}

bool Filemanager::rewind()
{
    if(!checkForInit())
    {
        #ifdef J54J6_LOGGING_H //use logging Libary if included
            logger::SFLog(className, "getFileSize", "Error Init. Check - writing failed!", 2);
        #endif
        return false;
    }

    if(internOpenDir)
    {
        #ifdef J54J6_LOGGING_H //use logging Libary if included
            logger::SFLog(className, "getFileSize", "pointer to next File in Dir or end reached...", 0);
        #endif
        return actualDir.rewind();
    }
    else
    {
        #ifdef J54J6_LOGGING_H //use logging Libary if included
            logger::SFLog(className, "getFileSize", "No Dir opened - please use (a)openDir()", 2);
        #endif
        return false;
    }
    return false;
}

bool Filemanager::rewind(Dir usedDir)
{
    if(!checkForInit())
    {
        #ifdef J54J6_LOGGING_H //use logging Libary if included
            logger::SFLog(className, "getFName", "Error Init. Check - writing failed!", 2);
        #endif
        return false;
    }
    return usedDir.rewind();
}

bool Filemanager::dClose()
{
    if(!checkForInit())
    {
        #ifdef J54J6_LOGGING_H //use logging Libary if included
            logger::SFLog(className, "getFileSize", "Error Init. Check - writing failed!", 2);
        #endif
        return false;
    }

    if(internOpenDir)
    {
        #ifdef J54J6_LOGGING_H //use logging Libary if included
            logger::SFLog(className, "getFileSize", "pointer to next File in Dir or end reached...", 0);
        #endif
        internOpenDir = false;
        return true;
    }
    else
    {
        #ifdef J54J6_LOGGING_H //use logging Libary if included
            logger::SFLog(className, "getFileSize", "No Dir opened - please use (a)openDir()", 2);
        #endif
        return false;
    }
    return false;
}

bool Filemanager::seek(uint8 offset, const char* mode)
{
    if(!checkForInit())
    {
        #ifdef J54J6_LOGGING_H //use logging Libary if included
            logger::SFLog(className, "seek", "Error Init. Check!", 2);
        #endif
        return false;
    }
    
    if(!internFileOpen)
    {
        #ifdef J54J6_LOGGING_H //use logging Libary if included
            logger::SFLog(className, "seek", "Error Init. Check!", 2);
        #endif
        return false;
    }
    return internalFileHandle.seek(offset);
}

bool Filemanager::seek(File usedFile, uint8 offset, const char* mode)
{
    if(!checkForInit())
    {
        #ifdef J54J6_LOGGING_H //use logging Libary if included
            logger::SFLog(className, "seek", "Error Init. Check!", 2);
        #endif
        return false;
    }
    
    return usedFile.seek(offset);
    
}

uint8 Filemanager::position()
{
    if(!checkForInit())
    {
        #ifdef J54J6_LOGGING_H //use logging Libary if included
            logger::SFLog(className, "position", "Error Init. Check!", 2);
        #endif
        return false;
    }
    
    if(!internFileOpen)
    {
        #ifdef J54J6_LOGGING_H //use logging Libary if included
            logger::SFLog(className, "position", "Error Init. Check!", 2);
        #endif
        return false;
    }
    return internalFileHandle.position();
    
}

uint8 Filemanager::position(File usedFile)
{
    if(!checkForInit())
    {
        #ifdef J54J6_LOGGING_H //use logging Libary if included
            logger::SFLog(className, "position", "Error Init. Check!", 2);
        #endif
        return false;
    }
    
    return usedFile.position();
}

uint8 Filemanager::size()
{
    if(!checkForInit())
    {
        #ifdef J54J6_LOGGING_H //use logging Libary if included
            logger::SFLog(className, "size", "Error Init. Check!", 2);
        #endif
        return false;
    }
    
    if(!internFileOpen)
    {
        #ifdef J54J6_LOGGING_H //use logging Libary if included
            logger::SFLog(className, "size", "Error Init. Check!", 2);
        #endif
        return false;
    }
    return internalFileHandle.size();

}

uint8 Filemanager::size(File usedFile)
{
    if(!checkForInit())
    {
        #ifdef J54J6_LOGGING_H //use logging Libary if included
            logger::SFLog(className, "size", "Error Init. Check!", 2);
        #endif
        return false;
    }
    
    return usedFile.size();
}

const char* Filemanager::name()
{
    if(!checkForInit())
    {
        #ifdef J54J6_LOGGING_H //use logging Libary if included
            logger::SFLog(className, "name", "Error Init. Check!", 2);
        #endif
        return false;
    }
    
    if(!internFileOpen)
    {
        #ifdef J54J6_LOGGING_H //use logging Libary if included
            logger::SFLog(className, "name", "Error Init. Check!", 2);
        #endif
        return false;
    }
    return internalFileHandle.name();
}

const char* Filemanager::name(File usedFile)
{
    if(!checkForInit())
    {
        #ifdef J54J6_LOGGING_H //use logging Libary if included
            logger::SFLog(className, "name", "Error Init. Check!", 2);
        #endif
        return false;
    }
    
    return usedFile.name();

}

const char* Filemanager::fullName()
{
    if(!checkForInit())
    {
        #ifdef J54J6_LOGGING_H //use logging Libary if included
            logger::SFLog(className, "fullName", "Error Init. Check!", 2);
        #endif
        return false;
    }
    
    if(!internFileOpen)
    {
        #ifdef J54J6_LOGGING_H //use logging Libary if included
            logger::SFLog(className, "fullName", "Error Init. Check!", 2);
        #endif
        return false;
    }
    return internalFileHandle.fullName();
}

const char* Filemanager::fullName(File usedFile)
{
    if(!checkForInit())
    {
        #ifdef J54J6_LOGGING_H //use logging Libary if included
            logger::SFLog(className, "fullName", "Error Init. Check!", 2);
        #endif
        return false;
    }
    
    return usedFile.fullName();

}

long Filemanager::getLastWrite()
{
    if(!checkForInit())
    {
        #ifdef J54J6_LOGGING_H //use logging Libary if included
            logger::SFLog(className, "getLastWrite", "Error Init. Check!", 2);
        #endif
        return false;
    }
    
    if(!internFileOpen)
    {
        #ifdef J54J6_LOGGING_H //use logging Libary if included
            logger::SFLog(className, "getLastWrite", "Error Init. Check!", 2);
        #endif
        return false;
    }
    return internalFileHandle.getLastWrite();
}

long Filemanager::getLastWrite(File usedFile)
{
    if(!checkForInit())
    {
        #ifdef J54J6_LOGGING_H //use logging Libary if included
            logger::SFLog(className, "getLastWrite", "Error Init. Check!", 2);
        #endif
        return false;
    }
    
    return usedFile.getLastWrite();

}

long Filemanager::getCreationTime()
{
    if(!checkForInit())
    {
        #ifdef J54J6_LOGGING_H //use logging Libary if included
            logger::SFLog(className, "getCreationTime", "Error Init. Check!", 2);
        #endif
        return false;
    }
    
    if(!internFileOpen)
    {
        #ifdef J54J6_LOGGING_H //use logging Libary if included
            logger::SFLog(className, "getCreationTime", "Error Init. Check!", 2);
        #endif
        return false;
    }
    return internalFileHandle.getCreationTime();
}

long Filemanager::getCreationTime(File usedFile)
{
    if(!checkForInit())
    {
        #ifdef J54J6_LOGGING_H //use logging Libary if included
            logger::SFLog(className, "getCreationTime", "Error Init. Check!", 2);
        #endif
        return false;
    }
    
    return usedFile.getCreationTime();
}

bool Filemanager::writeInFile(const char* Filename, const char* pattern, const char* writeMode)
{
    if(!checkForInit())
    {
        #ifdef J54J6_LOGGING_H //use logging Libary if included
            logger::SFLog(className, "getCreationTime", "Error Init. Check!", 2);
        #endif
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

bool Filemanager::writeJsonFile(const char* Filename,  const char* jsonPattern[][2], int amountOfData, const char* writeMode)
{
    if(!checkForInit())
    {
        #ifdef J54J6_LOGGING_H //use logging Libary if included
            logger::SFLog(className, "getCreationTime", "Error Init. Check!", 2);
        #endif
        return false;
    }

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

bool Filemanager::writeJsonFile(const char* Filename, DynamicJsonDocument jsonFile, const char* writeMode)
{
    if(!checkForInit())
    {
        #ifdef J54J6_LOGGING_H //use logging Libary if included
            logger::SFLog(className, "getCreationTime", "Error Init. Check!", 2);
        #endif
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
        #ifdef J54J6_LOGGING_H //use logging Libary if included
            logger::SFLog(className, "getCreationTime", "Error Init. Check!", 2);
        #endif
        return false;
    }
    DynamicJsonDocument jsonFile = readJsonFile(Filename);
    jsonFile[key] = newValue;
    bool val1 = writeJsonFile(Filename, jsonFile, "w");
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

String Filemanager::readFile(const char* Filename)
{
    if(!checkForInit())
    {
        #ifdef J54J6_LOGGING_H //use logging Libary if included
            logger::SFLog(className, "getCreationTime", "Error Init. Check!", 2);
        #endif
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
    return output;
}

const char* Filemanager::readJsonFileValue(const char* Filename, const char* pattern)
{ 
    if(!checkForInit())
    {
        #ifdef J54J6_LOGGING_H //use logging Libary if included
            logger::SFLog(className, "getCreationTime", "Error Init. Check!", 2);
        #endif
        return {};
    }
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

DynamicJsonDocument Filemanager::readJsonFile(const char* Filename)
{
    const size_t capacity = JSON_OBJECT_SIZE(25) + 400;
    DynamicJsonDocument jsonDocument(capacity);
    if(!checkForInit())
    {
        #ifdef J54J6_LOGGING_H //use logging Libary if included
            logger::SFLog(className, "getCreationTime", "Error Init. Check!", 2);
        #endif
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
        return jsonDocument;
    }
    return jsonDocument;
}

float Filemanager::getFreeSpace(short mode)
{
    FSInfo fs_info;
    if(!checkForInit())
    {
        #ifdef J54J6_LOGGING_H //use logging Libary if included
            logger::SFLog(className, "getCreationTime", "Error Init. Check!", 2);
        #endif
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
    Serial.print("   ");
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
      Serial.print("|- ");
      Serial.print(dir.fileName());
      Serial.print(" -> ");
      Serial.print(dir.fileSize());
      Serial.println("Byte");
    }
    if(dir.isDirectory())
    {
      insideDirs++;
      String cPath = mainPath + "/" + dir.fileName();
      Serial.print("|--| ");
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
        #ifdef J54J6_LOGGING_H //use logging Libary if included
            logger::SFLog(className, "getCreationTime", "Error Init. Check!", 2);
        #endif
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
