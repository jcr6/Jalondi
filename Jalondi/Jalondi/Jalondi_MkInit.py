#!/usr/bin/python


print("Setting right Jalondi's init functions")
from glob import glob
import datetime

dir = glob("*.cpp");
OutPut = "\n\n// Output Generated: "+datetime.date.today().strftime('%Y-%m-%d')+"\n\n\nnamespace Slyvina {\nnamespace Jalondi {\n\n"
Initfun = "inline void InitAll() {\n"
for file in dir:
	print("Found:",file) # debug!
	short=file[0:len(file)-4];
	fun="Jal_%s"%short
	print("=> ",fun)
	OutPut+="void %s();\n"%fun
	Initfun+="\t%s();\n"%fun
Initfun+="}\n\n"
OutPut+="\n\n%s\n\n}}\n\n"%Initfun
with open("Jalondi_Init.hpp", "w") as text_file:
    text_file.write(OutPut)
print("Jalondi_Init.hpp -- Updated")
