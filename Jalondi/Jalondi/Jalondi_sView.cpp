// Lic:
// Jalondi
// Syntax Highlight View
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
// Version: 24.10.06
// EndLic
#include <Kitty_High.hpp>
#include <High_Headers.hpp>
#include <SlyvString.hpp>
#include <SlyvArgParse.hpp>
#include "Jalondi_Class.hpp"
#include <SlyvStream.hpp>
#include <JCR6_Core.hpp>

using namespace Slyvina::JCR6;
using namespace Slyvina::Units;
using namespace Slyvina::Kitty;

#define JCheck(exitcode) \
	if (Last()->Error) { \
		QCol->Error(Last()->ErrorMessage); \
		QCol->Doing("Main",Last()->MainFile); \
		QCol->Doing("Entry",Last()->Entry); \
		if (exitcode > 0) return exitcode; \
	}
#define JCheckC(exitcode) \
	if (Last()->Error) { \
		QCol->Error(Last()->ErrorMessage); \
		QCol->Doing("Main", Last()->MainFile); \
		QCol->Doing("Entry", Last()->Entry); \
		ret=exitcode; continue;  \
}

namespace Slyvina {
	namespace Jalondi {
		static void _sType_Explain() {
			QCol->White("Will display the content of a file and perform syntax highlight if the file is recognized as code, script, or markup\n\n");
			QCol->Magenta("Usage: ");
			QCol->Yellow(StripAll(Jalondi_Exe));
			QCol->LGreen(" type ");
			QCol->LCyan(" [<switches>] ");
			QCol->Pink(" <JCR6 file> ");
			QCol->Grey(" <entries>\n");
			QCol->LMagenta("\n\nAllowed switches\n");
			QCol->Yellow("-w         \t"); QCol->Cyan("Enable Westwoord PAK files\n");
			QCol->Yellow("-ln        \t"); QCol->Cyan("Don't show line numbers\n");
			QCol->Yellow("-lng <Lang>\t"); QCol->Cyan("Enforce a certain language (providing it's supported)");
			QCol->Grey("\n\nIn order to make this possible Jalondi uses the Kitty module.\mWhatever the (C++) version of Kitty supports should also be supported by Jalondi\n(if Jalondi was rebuild since of course)\n\n");
		}

		static int _sType_Action(int argc, char** args) {
			int ret{ 0 };
			FlagConfig FC{};
			AddFlag_Bool(FC, "w",false);
			AddFlag_Bool(FC, "ln", false);
			AddFlag_String(FC, "lng", "AUTO");
			auto PA{ ParseArg(argc,args,FC) };
			auto fjcr{ ChReplace(PA.arguments[1],'\\','/') }; 
			if (PA.arguments.size() < 3) { QCol->Error("No entries to be viewed!"); return 4; }
			auto ShowLineNumbers{ PA.bool_flags["ln"] };
			auto Language{ Lower(PA.string_flags["lng"]) };
			auto AutoLang{ PA.string_flags["lng"] == "AUTO" };
			QCol->Doing("Reading", fjcr);
			if (!FileExists(fjcr)) { QCol->Error("File not found"); return 404; }
			auto rjcr{ JCR6_Dir(fjcr) }; JCheck(20);
			_KittyHigh::Init();
			KittyHigh_ALL();
			if (AutoLang) {
				QCol->Doing("ReqLang", "Auto Detect");
			} else {
				if (!_KittyHigh::Langs.count(Language)) {
					QCol->Error("Language \"" + Language + "\" is unknown");
					return 20;
				}
				QCol->Doing("ReqLang", _KittyHigh::Langs[Language]->Language);
			}
			for (int i = 2; i < PA.arguments.size(); i++) {
				auto nent{ PA.arguments[i] };
				QCol->Doing(TrSPrintF("%d/%d Entry", i - 1, PA.arguments.size() - 2), nent);
				auto src{ rjcr->GetString(nent) }; JCheckC(21);				 
				auto lng{ Language };
				auto ext{ Lower(ExtractExt(nent)) };
				KittyHigh Viewer = _KittyHigh::Langs["OTHER"];
				if (AutoLang) {
					if (_KittyHigh::Langs.count(ext)) Viewer = _KittyHigh::Langs[ext];
				} else {
					Viewer = _KittyHigh::Langs[lng];
				}
				if (!Viewer) {
					QCol->Error("Internal fatal error: Nullviewer! Please report!");
					QCol->Doing("lng", lng);	
					QCol->Doing("ext", ext);
					QCol->Doing("AutoLang", boolstring(AutoLang));
					QCol->Doing("extCNT", boolstring(_KittyHigh::Langs.count(ext)));
					return 500;
				}
				QCol->Doing("Language", Viewer->Language);
				Viewer->Show(src, !ShowLineNumbers);
				QCol->Grey("\n\n");
			}


		}

		void Jal_Jalondi_sView() {
			SJB("Jalondi_sView.cpp");
			J_Action J{ "sType",_sType_Action,_sType_Explain,"View Text File with syntax highlight" };
		}
	}
}