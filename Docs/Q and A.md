# Questions and Answers


Question | Answer
---|---
Why do Westwood PAK files always require the -w switch | Because Westwood PAK files are a downright drama to support. The format is downright terrible and due to this it easily the detection of other kinds of resources. Westwood has no kind of headers whatsoever to make them fully detectable. So not loading the Westwood driver unless needed to handle Westwood PAK files is the better way to go.
Why the Jalondi project? The original tools weren't good enough? | I have one tool written in Go, and I am not really game to get much more done in Go. The NJCR tool will be discontinued as soon as Jalondi is stable. I want to get rid of the .NET for my projects. On top op that NJCR has a few nasty bugs I could never get fixed (in other words COCKROACHES), and hopefully Jalondi will get me rid of those as well.
Why the name "Jalondi"? | I just needed a name. Jalondi is a woman who plays a role in my fantasy novel "De Vleugels Des Oorlogs" (The Wings of War). I named this tool after her. There are more projects of mine that got their name in similar ways.
Will Jalondi be able to pack in other formats but JCR6? | No, it will only be able to create JCR6 files, although it can read and extract from other formats. That was easy to implement and only required a minimum amount of extra code. Allowing Jalondi to write in these formats will only get me a lot of extra work and Jalondi's stability will only be undermined in the process. If you really want me to create a packer for the other formats, I can write one, but they won't be part of Jalondi.
