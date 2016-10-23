# Custom 3ds home menu

Requires [citrus](https://github.com/Steveice10/citrus) and its dependencies to build.

All files stored in romfs can be read from SD card too by just copying the contents of the romfs folder to the folder hbhmenu on the root of the sd card. The file locations can be changed by modifying romfs:/config.cfg.

##Playing audio in background:

-Copy config.cfg from romfs to the hbhmenu folder
-change playBGAudio to true
-change BGAudioPath to a path inside of the hbhmenu folder containing the music
-change BGAudioFormat to whatever type your soundfile is in. (Supports DSPADPCM and raw 16 bits PCM)

##Configuration file variables:
|Name|Value|Default|
|----|-----|-------|
|msgFileLoc|Contains the path to the greeting message. To be replaced by the file containing all messages|msgs/msg.txt|
|topScreenFlash|Flash screen for the top screen. 400*240 picture rotated by 90° and then saved as raw RGBA8888 data|images/top.bin|
|playBGAudio|Detemines if the menu should play audio or not|false|
|BGAudioPath|Path to the audio file. Ignored if playBGAudio is set to false|none|
|BGAudioFormat|Determines the audio file format. Can be any of PCM and DSPADPCM|PCM|
