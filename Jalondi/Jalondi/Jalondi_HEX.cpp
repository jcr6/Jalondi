// License:
// 
// Jalondi
// HEX viewer
// 
// 
// 
// 	(c) Jeroen P. Broks, 2024, 2025
// 
// 		This program is free software: you can redistribute it and/or modify
// 		it under the terms of the GNU General Public License as published by
// 		the Free Software Foundation, either version 3 of the License, or
// 		(at your option) any later version.
// 
// 		This program is distributed in the hope that it will be useful,
// 		but WITHOUT ANY WARRANTY; without even the implied warranty of
// 		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// 		GNU General Public License for more details.
// 		You should have received a copy of the GNU General Public License
// 		along with this program.  If not, see <http://www.gnu.org/licenses/>.
// 
// 	Please note that some references to data like pictures or audio, do not automatically
// 	fall under this licenses. Mostly this is noted in the respective files.
// 
// Version: 25.01.04
// End License
// Lic:
// Jalondi
// HEX viewer
// 
// 
// 
// (c) Jeroen P. Broks, 2024
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
// 
// Please note that some references to data like pictures or audio, do not automatically
// fall under this licenses. Mostly this is noted in the respective files.
// 
// Version: 24.10.03
// EndLic

#include "Jalondi_Class.hpp"
#include <SlyvString.hpp>
#include <SlyvQCol.hpp>
#include <JCR6_Core.hpp>
#include <SlyvStream.hpp>
#include <SlyvArgParse.hpp>

using namespace Slyvina;
using namespace Units;
using namespace JCR6;

namespace Slyvina {
	namespace Jalondi {

		static void _HEX_Explain() {
			QCol->White("Will show all the contents of a JCR6 resource (including all external files it's been linked to)\n\n");
			QCol->Magenta("Usage: ");
			QCol->Yellow(StripAll(Jalondi_Exe));
			QCol->LGreen(" hex ");
			QCol->LCyan(" [<switches>] ");
			QCol->Pink(" <JCR6 file> ");
			QCol->Grey(" <entries>\n");
		}

		static int _HEX_Action(int car, char** arg){
			int ret{ 0 };
			FlagConfig f{};
			auto PA{ ParseArg(car,arg,f) };
			auto fjcr{ ChReplace(PA.arguments[1],'\\','/')};
			if (car < 4) { QCol->Error("No entries to be HEX viewed!"); return 4; }
			QCol->Doing("Reading", fjcr);
			if (!FileExists(fjcr)) {
				if (DirectoryExists(fjcr)) {
					QCol->Error(fjcr + " is a directory!");					
					return 2;
				} else {
					QCol->Error(fjcr + "has not been found");
					return 404; // Hey, let's remain in 'style' here.					
				}
			}
			auto rjcr{ JCR6_Dir(fjcr) };
			if (Last()->Error) {
				QCol->Error(Last()->ErrorMessage);
				QCol->Doing("- Main", Last()->MainFile);
				QCol->Doing("- Entry", Last()->Entry);
				return 3;				
			}
			for (int i = 2; i < PA.arguments.size(); i++) {
				QCol->Doing(TrSPrintF("%d/%d: Hexing", i-1 , PA.arguments.size() - 2), PA.arguments[i]);
				auto BT = rjcr->B(PA.arguments[i]);
				if (Last()->Error) {
					QCol->Error(Last()->ErrorMessage);
					QCol->Doing("- Main", Last()->MainFile);
					QCol->Doing("- Entry", Last()->Entry);
					ret=3;
					break;
				}
				if (!BT) { QCol->Error("Input buffer null pointer for unknown reasons!"); ret = 6; break; }
				std::cout << ST("", 10);
				for (int j = 0; j < 16; j++) {
					if (j % 4 == 0) std::cout << " ";
					QCol->White(TrSPrintF("0%X ", j));
				}
				std::cout << "\n";
				BT->Position(0);
				byte c{ 0 };
				std::string dmp{ "" };
				auto pos{ 0L };
				while (!BT->AtEnd()) {
					if (pos % 16 == 0) {
						if (pos > 0) QCol->LGreen(dmp);
						QCol->White(TrSPrintF("\n%08X  ", BT->Position()));
						c = 0;
						dmp = "";
					} else if (pos % 4 == 0) {
						std::cout << " ";
						c = (++c) % 2;
					}
					auto B{ BT->ReadByte() }; ++pos;
					switch (c) {
					case 0: QCol->LCyan(TrSPrintF("%02X ", B)); break;
					case 1: QCol->Cyan(TrSPrintF("%02X ", B)); break;
					}
					if (B >= 32 && B < 127) dmp += TrSPrintF("%c",(char)B); else dmp += ".";
				}
				for (auto p = pos; p % 16 != 0; p++) {
					if (p % 4 == 0) std::cout << " ";
					QCol->Dark(".. ");
				}
				QCol->LGreen(dmp+"\n\n");
			}

			return ret;
		}


		void Jal_Jalondi_HEX() {
			SJB("Jalondi_HEX.cpp");
			J_Action J{ "Hex",_HEX_Action,_HEX_Explain,"HEX dump of one of more entries inside a JCR6 file" };
		}
	}
}
