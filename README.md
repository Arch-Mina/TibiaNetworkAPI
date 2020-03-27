# TibiaNetworkAPI
 A network packet simulation tool for Tibia 10.0

# Important
 I'm not available to update or fix this tool, just to answer questions in issues.<br/>
 Feel free to report bugs and do pull requests.<br/>
 This tool is available for TESTING purposes only.<br/>
 Last thing, the code for now is missing organization, feel free to fix this if you want.

# How to compile?
[Requirements]
 - Visual Studio 2019 (install C++)
 - Lua 5.1 libs (include & lib, you can use the vcpkg to install and download easier)

[Downloads]
 - You can find the binaries compiled inside 'binaries' folder (faster way if you don't wanna compile the project).

[Compiling]
 - Open TibiaNetworkAPI.sln with Visual Studio 2019
 - Change the project build to Release x86
 - Configure the includes & libs diretories
 - Add lua51.lib into lib inputs
 - Compile and enjoy

[Usage]
 - Create a file called 'main.lua' into 'C:/tnapi/' directory
 - Move lua51.dll into Tibia folder
 - Inject the TibiaNetworkAPI.dll compiled into Tibia.exe
 - Write some script inside main.lua
 - Press insert to reload script
 - enjoy

# Script Sample
```
-- add friend in vip list
addPacketHead(0xDC) -- 0xDC = parseVipAddPacket
addString("Friend") -- friend name
send() -- send the packet
```

# Available functions
```
addPacketHead(packetHead) -- parameter type: number from 0 to 255
addU8(byte) -- parameter type: number from 0 to 255
addU16(number) -- parameter type: number from 0 to 65535
addU32(number)  -- parameter type: number from 0 to 4294967295
addDouble(number) -- not tested and not done
addString(string) -- parameter type: string
send() -- after all packet has builded, just use this function to send to server
debugPrint(string) -- print something to console openned with dll injection
```