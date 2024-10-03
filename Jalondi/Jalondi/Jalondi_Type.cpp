// Lic:
// Jalondi
// Type
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
#include <SlyvQCol.hpp>
#include <JCR6_Core.hpp>
#include <SlyvStream.hpp>
#include <SlyvArgParse.hpp>
using namespace Slyvina::JCR6;
using namespace Slyvina::Units;

namespace Slyvina {
	namespace Jalondi {

		union ChB {
			char ch; Byte Bt;
		};

		static void _Type_Explain() {
			QCol->White("Will show all the contents of a JCR6 resource (including all external files it's been linked to)\n\n");
			QCol->Magenta("Usage: ");
			QCol->Yellow(StripAll(Jalondi_Exe));
			QCol->LGreen(" type ");
			QCol->LCyan(" [<switches>] ");
			QCol->Pink(" <JCR6 file> ");
			QCol->Grey(" <entries>\n");
			QCol->LMagenta("\n\nAllowed switches\n");
			QCol->Yellow("-w\t"); QCol->Cyan("Enable Westwoord PAK files\n");
			QCol->Yellow("-e\t"); QCol->Cyan("Ignore 0x1a as end of file (DOS based systems such as Windows see this as a file end, Unix based systems do not)\n");
		}


		static int _Type_Action(int car, char** arg) {
			int ret{ 0 };
			FlagConfig f{};
			AddFlag_Bool(f, "w",false); 
			AddFlag_Bool(f, "e",false);
			auto PA{ ParseArg(car,arg,f) };
			auto fjcr{ ChReplace(PA.arguments[1],'\\','/') };
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
				QCol->Doing(TrSPrintF("%d/%d: Hexing", i - 1, PA.arguments.size() - 2), PA.arguments[i]);
				auto BT = rjcr->B(PA.arguments[i]);
				if (Last()->Error) {
					QCol->Error(Last()->ErrorMessage);
					QCol->Doing("- Main", Last()->MainFile);
					QCol->Doing("- Entry", Last()->Entry);
					ret = 3;
					break;
				}
				if (!BT) { QCol->Error("Input buffer null pointer for unknown reasons!"); ret = 6; break; }
				ChB R;
				cout << "\n\n";
				BT->Position(0);
				while (!BT->AtEnd()) {
					R.Bt = BT->ReadByte();
					if (R.Bt == 26 && (!PA.bool_flags["e"]))  break;
					if (R.Bt < 127) {
						QCol->Grey(TrSPrintF("%c", R.ch));
					} else {
						QCol->Red(TrSPrintF("\\x%02x", R.Bt));
					}
				}
				std::cout << "\n\n";
			}
		}

		void Jal_Jalondi_Type() {
			SJB("Jalondi_Type.cpp");
			J_Action J{ "Type",_Type_Action,_Type_Explain,"View Text File" };
		}
	}
}