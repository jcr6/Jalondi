# Scripting

Jalondi can be scripted for the creation of new JCR6 files. This is handy for when the building of a JCR6 file requires a rather specific approach.
Scripts are just Lua scripts (Lua 5.4.0)

Now the scripts can be read in both source form and compiled form (be prepared that the latter option may get incompatible if I ever decide to upgrade Lua within Jalondi).


The scripts will get access to the "Jaloni" module. Nice to note is that although "Jalondi" must be typed with the case set here, it's methods and properties are case insensitive.



## Command reference
The next members are available

### void Jalondi.Add(string Source,string Target=<see explanation>,string Storage="Store",string Author="", string Notes="",string Block="")
Adds a file or directory to a JCR6 file. 
Now please note, this is only a data collection function, nothing will happen until Jalondi.Start() has actually been executed.

- *Source* can be the file or directory to be added. When it's a directory it will add all files inside that directory recursively.
- *Target* will be the name to store the file in, or in case of a directory the directory name. When not set the source name (without path) will be used.
- *Storage* will be the storage method. The first version of Jalondi only supports "Store" and "zlib" (mind the case, as it matters). Hopefully more methods can be added later.
- *Author* could contain the author's name. Particularly when using with 3rd party stuff this can be a good way to go.
- *Notes* are simple notes. I mostly put in copyright notices in there myself, but it's up to the user how this argument is used.
- *Block* can be used to group files into a [block](https://github.com/jcr6/Jalondi/blob/master/Docs/Block.md). Please note that the name you pick is only for your own convenience, as side from putting all stuff with the same name in the same block, the names will NOT be stored inside the JCR6 file, so this name has only value during the creation stage. If you do not know how to use this feature properly, it's better not to use it.

 ### void Jalondi.Alias(string ori,string alias)
Creates an alias of a file you add to this JCR6 file.
An 'alias' is in JCR6 just a file that has the same referrence pointer data than the original file. So basically they refer to the same chunk of data. 
In game creation it can sometimes come in handy, trust me. 
You should NOT confuse this with the alias system in Unix/Linux, as that works slightly different, I'm afraid.

### property ing Args
Contains the number of arguments given with the script when starting Jalondi to run it.
You can use the Jalondi module itself to get the arguments. 
Please note, this is Lua, so arrays start at 1. Hey, I didn't come up with that silly thing about Lua, it's just the way Lua works.
So if you run: 
~~~sh
Jalondi script myscript.lua I am Jeroen
~~~
Jalondi.Args will contain the number 3, and Jalondi[1] will contain the string "I", Jalondi[2] will contain the string "am" and Jalondi[3] will contain the string "Jeroen".


### table Jalondi.ChopFile(string file)
Will return a table with three elements. 
*Dir* will contain the path
*File* will contain the file without path or extension
*Ext* will contain the extension

### void Jalondi.Clear()
Clears all data for package creation


### table Jalondi.Dir(string path)
Returns an array with all files within a requested path. This happens recursively.
Please note, Jalondi (and basically nearly all tools of mine) will deem files prefixed with a period as hidden and ignore them (yes, even in Windows).

### void Jalondi.Doing(string whatamidoing,string withwhatisitdone)
This feature will just print the whatamidoing variable in yellow letters followed with a colon, and withwhatisitdone will then appear in cyan letters.
It's basically just a wrapper to my C++ function I used a lot in Jalondi's C++ code (and loads of other CLI tools of mine for that matter).

### void Jalondi.Import(string externaljcr6)
When the created JCR6 contains an import, JCR6 will when see if a file with that name exists, and if it does load and merge its contents to the JCR6 directory.
This can be a handy feature for auto-patching for bug fixing or to allow optional added content.
Please note, any file JCR6 can support can be imported as long as the program in question has the specific drivers for that file type loaded.

### void Jalondi.Require(string externaljcr6)
Basically the same as import, but when the file that should be imported does not exist the JCR6 resource can not be loaded.
I never used this myself, but it was just easy to implement, so what the heck. I guess it can be handy for dependencies, who knows.


### property string Jalondi.PWD
Contains the current path

### void Jalondi.Run(string outputfile,string storage="Store",string Signature="*AUTO")
Alias for Jalondi.Start()

### property string Jalondi.Script
Contains the name of the Lua main script you are now running

### void Jalondi.Start(string outputfile,string storage="Store",string Signature="*AUTO")
Will create the JCR6 with the data collected before. Please note, the data will NOT be cleared, so you can create multiple files based on the same data... if you see a need for that.
- *outputfile* = the file to write it all to
- *storage* = the storage used for the file table (yeah, you can compress it with zlib)
- *signature* = a signature... if you see a need for that.

### void printf(string fmt,...)
Will print a formatted string. Same as printf() function in C, however unlike C it will go to the next line.
Please note, printf is not a member of the Jalondi module.

### sting sprintf(string fmt,...)
Same as string.format. Please note that this is not a member of Jalondi, but Jalondi adds it anyway


