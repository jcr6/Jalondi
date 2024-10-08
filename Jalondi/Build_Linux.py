#!/usr/bin/python
from glob import glob
from os import system
from os import remove

global incdir
incdir = "../../../Libs/JCR6/Headers:../../../Libs/Units/Headers:../../../Libs/Lunatic"

def gcc(name,dir):
    print("Building: ",name)
    print("Dir: ",dir)
    d=glob(dir+"/*.c")
    for fp in d:
        print("Compiling: ",fp)
        sfp=fp.split("/")
        o = sfp[len(sfp)-1]
        o = o[:-1]+"o"
        return_code = system("gcc -c -o Jalondi/Linux/Objects/%s \"%s\""%(o,fp))
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
        cmd =  "g++ -Woverflow -c -o Jalondi/Linux/Objects/%s "%o
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

os.chdir("Jalondi")
print("Init data generation")
system("python3 Jalondi_MkInit.py")
os.chdir("..")

newdir("Jalondi/Linux")
newdir("Jalondi/Linux/Objects")
newdir("Jalondi/Linux/Exe")

gcc("Lua","../../../Libs/Lunatic/Lua/Raw/src/")  
delf("Jalondi/Linux/Objects/lua.o")  # lua.o and luac.o are objects for stand alone executables
delf("Jalondi/Linux/Objects/luac.o") # leaving them be will spook up the linking process later.
gcc("zlib","../../../Libs/JCR6/3rdParty/zlib/src/")
gpp("Slyvina JCR6","../../../Libs/JCR6/Source")
gpp("Slyvina Lunatic","../../../Libs/Lunatic")
gpp("Slyvina Units",glijst("../../../Libs/Units/Source",("SlyvQCol","SlyvAsk","SlyvBank","SlyvDir","SlyvDirry","SlyvMD5","SlyvOpenURL","SlyvRoman","SlyvSTOI","SlyvStream","SlyvString","SlyvTime","SlyvVolumes")),True,True)
gpp("Jalondi","Jalondi")