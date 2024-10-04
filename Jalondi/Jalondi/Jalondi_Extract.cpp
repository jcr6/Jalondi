// Lic:
// Jalondi
// Extract
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
// Version: 24.10.04
// EndLic

#include "Jalondi_Class.hpp"
#include <SlyvQCol.hpp>
#include <SlyvArgParse.hpp>
#include <SlyvStream.hpp>
#include <SlyvConInput.hpp>
#include <JCR6_Core.hpp>
using namespace Slyvina::JCR6;

using namespace Slyvina::Units;

namespace Slyvina {
	namespace Jalondi {

#pragma region "Yes or no?"
		static bool _Yes(String Question, bool alwaysyes) {
			QCol->Yellow(Question);
			QCol->LMagenta(" ? ");
			QCol->LCyan("<Y/N> ");
			QCol->Grey("");
			if (alwaysyes) {
				QCol->LGreen("Yes\n");
				return true;
			}
			String Answer{ "" };
			do {
				Answer = Trim(Upper(ReadLine("")));
			} while (!Answer.size());
			return Answer[0] == 'Y';
		}
#define GYes(Question) _Yes(Question,alwaysyes)
#pragma endregion

#pragma region Action!
		static void _xExplain() {
			QCol->White("Will show all the contents of a JCR6 resource (including all external files it's been linked to)\n\n");
			QCol->Magenta("Usage: ");
			QCol->Yellow(StripAll(Jalondi_Exe));
			QCol->LGreen(" extract ");
			QCol->LCyan(" [<switches>] ");
			QCol->Pink(" <JCR6 file> ");
			QCol->Grey(" [<entries>]\n");
			QCol->Red("NOTE! "); QCol->Yellow("When no entries are given, all entries will be extracted");
			QCol->LMagenta("\n\nAllowed switches\n");
			QCol->Yellow("-w       \t"); QCol->Cyan("Enable Westwoord PAK files\n");
			QCol->Yellow("-y       \t"); QCol->Cyan("Answer all yes/no questions with 'yes'\n");
			QCol->Yellow("-o <dir> \t"); QCol->Cyan("Define output dir (current dir is default)\n");
		}

		static int _xRun(int argc, char** args) {
			int errors{ 0 };
			int succeeded{ 0 };
			int ret{ 0 };
			FlagConfig C;
			AddFlag_Bool(C, "w", false);
			AddFlag_Bool(C, "y", false);
			AddFlag_String(C, "o", CurrentDir());
			auto PA{ ParseArg(argc,args,C) };
			auto outd{ PA.string_flags["o"] };
			auto alwaysyes{ PA.bool_flags["y"] };
			auto rjcr{ PA.arguments[1] };
			if (CurrentDir() == outd) {
				if (!GYes("Are you sure you want to output in the current directory")) return 14;
			}
			QCol->Doing("Reading", rjcr);
			auto djcr{ JCR6_Dir(rjcr) };
			auto ejcr{ djcr->Entries() };
			std::vector<String> WantEntries{};
			if (PA.arguments.size() == 2) {
				QCol->Doing("Extract", "* ALL");
				for (auto e : *ejcr) WantEntries.push_back(e->Name());
			} else {
				for (int i = 2; i < PA.arguments.size(); ++i) {
					QCol->Doing("Extract", PA.arguments[i]);
					WantEntries.push_back(PA.arguments[i]);					
				}
			}
			for (auto ew : WantEntries) {				
				if (!djcr->EntryExists(ew)) {
					QCol->Error(TrSPrintF("%d: ", ++errors) + "Entry \"" + ew + "\" doesn't exist");
					ret = 15;
					continue;
				}
				std::vector<String> Aliassed{};
				auto e{ djcr->Entry(ew) };
				for (auto ae : *ejcr) {
					if (e->Name() != ae->Name() && e->Storage() == ae->Storage() && e->CompressedSize() == ae->CompressedSize() && e->RealSize() == ae->RealSize() && e->Offset() == ae->Offset() && e->MainFile == ae->MainFile)
						Aliassed.push_back(ae->Name());
				}
				if (Aliassed.size()) {
					if (Aliassed.size() == 1) QCol->Warn(ew + " has an alias\n"); else QCol->Warn(ew + TrSPrintF(" has %d entries aliassed to it\n"));
					for (size_t i = 0; i < Aliassed.size(); i++) {
						QCol->Yellow(TrSPrintF("%9d -> ",i+1));
						QCol->Cyan(Aliassed[i] + "\n");
					}
					if (!GYes("Still extract it")) continue;
				}
				auto tarfile{ outd + "/" + ew };
				auto tardir{ ExtractDir(tarfile) };
				if (FileExists(tarfile) && (!GYes("Target file \"" + tarfile + "\" exists. Overwrite "))) continue;
				MakeDir(tardir);
				if (!DirectoryExists(tardir)) {
					QCol->Error(TrSPrintF("%d: ", ++errors) + "Couldn't create target directory for: "+tarfile);
					ret = 16;
					continue;
				}
				QCol->Doing("Defrosting", ew, " ");
				auto Bnk{ djcr->B(ew) };
				auto BTO{ WriteFile(tarfile) };
				if (!BTO) {
					QCol->Error(TrSPrintF("%d: ", ++errors) + "Couldn't create target file: " + tarfile);
					ret = 17;
					continue;
				}
				while (!Bnk->AtEnd()) BTO->Write(Bnk->ReadChar());
				BTO->Close();
				std::cout << "\r";				
				QCol->Doing(" Defrosted", ew);
				succeeded++;
			}
			QCol->Doing("Files wanted", (int)WantEntries.size());
			QCol->Doing("Extracted", succeeded);
			return ret;
		}
#pragma endregion

#pragma region Init
		void Jal_Jalondi_Extract() {
			SJB("Jalondi_Extract.cpp");
			J_Action A{ "extract",_xRun,_xExplain,"Extracts from JCR6 or packed files JCR6 can read" };
		}
#pragma endregion
	}
}