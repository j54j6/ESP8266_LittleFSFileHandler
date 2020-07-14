# ESP8266_LittleFSFileHandler
A very Little Libary to create and use JSON Config(s)

Ver. Beta - 0.1

This Libary is in general for one of my Projects -

Req. 
  - ArduinoJSON (tested with: version 6.15.2)
  - LittleFS (part of Arduino Core)
  
 What does it do:
  - create JSON Config
  - readout JSON Config (complete or per Value)
  - del Files
  - read Files
  - write Files
  - extra:
    - show Free Space (in Bytes or percent)
    - get Filesystem Info (only fs_info from LittleFS)
    
 of course this is not a big Libary, but it saves a bit of work ^^ 
 
 
 How to Use:
 
 Read comments in "filemanager.h" - i think it's very simple to use this Lib ^^
 
 Bucket List:
  - Filesystem configurator (for e.g multiple Partitions (keyword OTA))
  - internal Cache for often used files
  - u tell me^^
