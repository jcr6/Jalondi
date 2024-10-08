"""   -- Start License block
Jalondi
Linux Compile Script



(c) Jeroen P. Broks, 2024

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

Please note that some references to data like pictures or audio, do not automatically
fall under this licenses. Mostly this is noted in the respective files.

Version: 24.10.08
-- End License block   """
#!/usr/bin/python
from glob import glob
from os import system
from os import remove
import os
import platform

system("clear")
print("Jalondi Linux Compilation Script")
print("(c) Jeroen P. Broks\n\n")

# This script is ONLY meant for Linux!
print("OS: ",platform.system(),"; release: ",platform.release())
if platform.system()!="Linux":
    print("I'm sorry, but this script has been designed for Linux Only!")
    quit()

global incdir
incdir = "../../../Libs/JCR6/Headers:../../../Libs/Units/Headers:../../../Libs/Lunatic:../../../Libs/Kitty/Headers"

def gcc(name,dir):
    print("Building: ",name)
    print("Dir: ",dir)
    d=glob(dir+"/*.c")
    for fp in d:
        print("Compiling: ",fp)
        sfp=fp.split("/")
        o = sfp[len(sfp)-1]
        o = o[:-1]+"o"
        return_code = system("gcc -Wno-trigraphs -c -o Jalondi/Linux/Objects/%s \"%s\""%(o,fp))
        if return_code>0:
            print("Compilation failed! (%d)"%return_code)
            quit()
          
def delf(file):
        print("Deleting: ",file)
        remove(file)
        
        
def gpp(name,dir,full=True,dirislist=False):
    global incdir
    print("Building: ",name)
    print("Dir: ",dir)
    if full: print("Include dirs:",incdir)
    if dirislist: 
        d=dir
    else:
        d=glob(dir+"/*.cpp")
    for fp in d:
        print("Compiling: ",fp)
        sfp=fp.split("/")
        o = sfp[len(sfp)-1]
        o = o[:-3]+"o"
        cmd =  "g++ -Woverflow -Wno-trigraphs -c -o Jalondi/Linux/Objects/%s "%o
        if full:
            for id in incdir.split(":"):
                cmd+="\"-I%s\" "%id
        cmd += " \"%s\""%fp
        # print cmd # debug (Python 2)
        return_code = system(cmd)
        if return_code>0:
            print("Compilation failed! (%d)"%return_code)
            quit()          

def glijst(dir,lst):
    ret = []
    for l in lst: ret.append("%s/%s.cpp"%(dir,l))
    return ret
    
def newdir(d):
    if not os.path.exists(d):
        print("Creating directory: ",d)
        os.makedirs(d)

#os.chdir("Jalondi")
#print("Init data generation")
#system("python3 Jalondi_MkInit.py")
#os.chdir("..")

newdir("Jalondi/Linux")
newdir("Jalondi/Linux/Objects")
newdir("Jalondi/Linux/Exe")

#gcc("Lua","../../../Libs/Lunatic/Lua/Raw/src/")  
#delf("Jalondi/Linux/Objects/lua.o")  # lua.o and luac.o are objects for stand alone executables
#delf("Jalondi/Linux/Objects/luac.o") # leaving them be will spook up the linking process later.
#gcc("zlib","../../../Libs/JCR6/3rdParty/zlib/src/")
gpp("Slyvina JCR6","../../../Libs/JCR6/Source"); delf("Jalondi/Linux/Objects/JCR6_zlib.o")
#gpp("Slyvina Lunatic","../../../Libs/Lunatic")
gpp("Slyvina Kitty","../../../Libs/Kitty/Source")
gpp("Slyvina Units",glijst("../../../Libs/Units/Source",("SlyvQCol","SlyvArgParse","SlyvAsk","SlyvBank","SlyvDir","SlyvDirry","SlyvMD5","SlyvOpenURL","SlyvRoman","SlyvSTOI","SlyvStream","SlyvString","SlyvTime","SlyvVolumes")),True,True)
gpp("Jalondi","Jalondi")
delf("Jalondi/Linux/Objects/Jalondi_Script.o") # temporary

print("Linking: Jalondi/Linux/Exe/jalondi")
rc = system("g++ -o Jalondi/Linux/Exe/jalondi Jalondi/Linux/Objects/*.o")
if rc!=0:
    print("Error in linking (%d): "%rc)