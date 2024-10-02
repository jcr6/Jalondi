"""   -- Start License block
Jalondi
Init Header Generator



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

Version: 24.10.02
-- End License block   """
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