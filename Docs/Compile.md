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
- Open Jalondi.sln in Visual Studio
- Build the solution
  - If this doesn't work, please note the errors and send them to me, so I can try to figure out what went wrong.
- The file "S:\Slyvina\Tools\Jalondi\Jalondi\Jalondi\Exe\Jalondi.exe" should be present along with "S:\Slyvina\Tools\Jalondi\Jalondi\Jalondi\Exe\zlib.dll". Please note Jalondi will NOT work without the zlib.dll file! You may find Jalondi.pdb, which is merely a file that helps Visual Studio with debugging, but it's not required for distribution or general operation.







# GCC

I have not yet worked out a proper way to compile Jalondi in GCC. Since Jalondi is supposed to work in Linux in the nearby future, this will be high on my agenda, but please note that I can only run Linux in a Virtual Machine at this moment. That does not make things impossible, but it does make things harder to work around.





