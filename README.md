# ESP8266_LittleFSFileHandler
A very Little Libary to create and use JSON Config(s)

Ver. Beta - 0.2
This is an Upgrade of B0.1 - Much more functionalities and auto Tested with 1.2M write / rewrite types - "auto syntax fix" not working perfectly but helps with "hard" mistakes like forget mounting or things like this.

ConfigReader and Creator fixed - now both functions work without problems

Req. 
  - ArduinoJSON (tested with: version 6.15.2)
  - LittleFS (part of Arduino Core)
  
 Useable like POSIX or Unix Commandline (e.g move, mkdir, etc.) - not completly implemented
 
 How to use:
 
 Auto generated instance of Filemanager is "FM" - you can redefine it in filemanager.h...
 

Most functionalities i describe as comment in filemanager.h - and most of the functions are self-explaining
If you need some help or want to give some feedback - open Issue or visit me on TS -> ts.microcont.de
