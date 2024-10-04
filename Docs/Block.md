# To block or not to block, that's the question

## What is a block?

JCR6 will normally compress all files one by one if a compression method was chosen. Similar to how .zip files work.
However, JCR6 does support to put multiple files together and compress them all together, and these groups packed together are called a "block".
The idea is similar to solid archives which rar supports and 7z even does by default.

## Is it advicable to put files together in blocks?

If you don't know what you are doing, the answer is simply "no".
In other cases, well, there are some pros and some cons to consider.

Pros | Cons
---|---
Lower compression ratios | More memory needed for packing and unpacking
When multiple files in one block are loaded together loading times can increase | If not loaded together, loading times can be significantly longer

Well, I guess there you have it.



When it comes to my own projects, loads of files are often loaded all at once. Some assets loaded when a game is being started up for example. The multiple files of which Kthura maps are
composed or animated pictures in .jpbf (JCR6 Picture Bundle File) files. Having those in blocks will increase loading speed. If JCR6 reads multiple files from the same block it will just
keep the block unpacked in the memory so data can be extracted from it as asked for UNTIL the moment a different block is accessed. 

Dumping ALL data into one block is rarely advicable, as it consumes loads of RAM for no purpose. JCR6 is NOT an archiver, it's a resource manager for games, although some ulities can also make great use of this.


Please note that Jalondi doesn't know which files you plan to use together, and as such you will have to decide for yourself what to put together and what not. RAR and 7Z will even sort out if a different order of files will get better results. Jalondi will not, as Jalondi ain't a backup tool or something like that. Although JCR6 always shows filenames in alphabetic order in file lists, the order in which the data is stored inside a block is always the same order as you instruct JCR6 (and as such Jalondi as well) to add the data to the block. The same actually applies to files that are not in a block. The File Table will tell JCR6 where to find the data, so this is quite safe.


