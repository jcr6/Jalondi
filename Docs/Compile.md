To build

# Microsoft Visual Studio

## 3rd party dependencies
- zlib (included in JCR6 repository, so don't worry)
- Lua

## Preparation instructions
I will assume you do this from Windows PowerShell and that you have git installed. Since the building configuration uses a Python script to generate two header files that are not part of the repository, Python must be installed. I used Python 3.12.6, but I guess any version of Python 3 should do.
- You need a folder dedicated to Slyvina projects, of which Jalondi is one. I will call it S:\Slyvina for this documentation, so you can substitute that with your own directory.
- Type: cd S:\Slyvina
- Create the following folders "Libs", "Tools" (you may create "Apps" if you like to go for my Apps in the Slyvina framework, but for Jalondi you don't need it).
- Type: git clone https://github.com/jcr6/Jalondi.git Tools/Jalondi
- Type: git clone https://github.com/Slyvina/Units.git Libs/Units
- Type: git clone https://github.com/Slyvina/JCR6.git Libs/JCR6
- Type: git clone https://github.com/Slyvina/Lunatic.git Libs/Lunatic
- If you check the S:\Slyvina\Libs\Lunatic directory you will notice a folder named "Lua" with only a "Readme.md" file inside. Create a folder named 'Raw' here in 'Raw' all the source code of Lua should be placed, that is all the .c and .h files
  - The Lua version used for the original version of Jalondi is lua 5.4.0 (Other versions should work fine, but no guarantees. As Jalondi can load pre-compiled Lua scripts (if you need them), you should take in mind that they will not load if they were written for a different Lua version).

## Compiling
- Open Jalondi.sln in Visual Studio
- Build the solution
  - If this doesn't work, please note the errors and send them to me, so I can try to figure out what went wrong.
- The file "S:\Slyvina\Tools\Jalondi\Jalondi\Jalondi\Exe\Jalondi.exe" should be present along with "S:\Slyvina\Tools\Jalondi\Jalondi\Jalondi\Exe\zlib.dll". Please note Jalondi will NOT work without the zlib.dll file! You may find Jalondi.pdb, which is merely a file that helps Visual Studio with debugging, but it's not required for distribution or general operation.







# GCC (SCons)

I only recommed going this way on Linux and NOT on Windows. 

## 3rd Party dependencies
- zlib
- Lua

## Tools needed
- Python 3
- SCons
- GCC
- git

## Preparation instructions
I will assume you are using bash.
- You will need a folder dedicated to Slyvina projects, of which Jalondi is one. I will call it /home/Jeroen/Slyvina for this documentation, but you can substituate that with your own directory.
- Type: cd /home/Jeroen/Slyvina
- Type: mkdir Libs
- Type: mkdir Tools
- Type: mkdir Apps # As far as Jalondi is concerned optional, but handy to have for other Slyvina apps I suppose.
- Type: git clone https://github.com/jcr6/Jalondi.git Tools/Jalondi  
- Type: git clone https://github.com/Slyvina/Units.git Libs/Units    
- Type: git clone https://github.com/Slyvina/JCR6.git Libs/JCR6      
- Type: git clone https://github.com/Slyvina/Lunatic.git Libs/Lunatic
- Make sure you put the source code of Lua in the directory /home/Jeroen/Slyvina/Lunatic/Lua/Raw/src
  - Please check the version note about Lua in the Windows compile instructions (same apply here, after all)

## Compiling
- Type: cd Tools/Jalondi/Jalondi # Note Jalondi goes TWICE here!
- Type: scons
- Now in exe/linux the file "jalondi" should have been created. That's your executable.
- Optional: Now if you want to easily use jalondi like any other console app just type: sudo cp exe/linux/jalondi /usr/bin
  - It goes of course without saying that I do not take any responsibility for bad results of that, though none are expected
  - And it also goes without saying that Linux may be asking for your password





# Known trouble
- The "scons" script is specifically made for Linux, yet it should also work in Windows. Doing so will cause some non-existing errors. When compiling Jalondi in Visual Studio, no problem (after all, there are no issues), but from SCons it does, and SCons calls the same compiler with the same source files, plus the error says that stuff that is even a downright lie (I checked to code multiple times).
- Mac is not yet supported, nor do I have any plans to support it (at least NOT on the short term). If you wanna make Jalondi compatible with Mac, you're free to make it happen though.




