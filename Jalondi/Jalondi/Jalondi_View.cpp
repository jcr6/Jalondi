// Lic:
// Jalondi
// View
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

#include <SlyvArgParse.hpp>
#include <SlyvQCol.hpp>
#include <SlyvString.hpp>
#include <SlyvMD5.hpp>

#include <JCR6_Core.hpp>

#include "Jalondi_Class.hpp"
#include <SlyvStream.hpp>

using namespace Slyvina;
using namespace Units;
using namespace JCR6;

namespace Slyvina {
	namespace Jalondi {

		static std::map<String, String> ExtReg{
			{"PNG","Image"},{"BMP","Image"},{"JPG","Image"},{"JPEG","Image"},{"GIF","GIF"},
			{"PDN","ImgSource"},{"XCF","ImgSource"},
			{"WAV","Audio"},{"OGG","Audio"},{"MP3","Audio"},
			{"LUA","Script"},{"LBC","Lua Byte Code"},
			{"INI","Config"},{"XML","Data"},
			{"MYDATA","DATA"},
			{"MD","MarkDown"}
		};

		static void _View_Explain() {
			QCol->White("Will show all the contents of a JCR6 resource (including all external files it's been linked to)\n\n");
			QCol->Magenta("Usage: ");
			QCol->Yellow(StripAll(Jalondi_Exe));
			QCol->LGreen(" view ");
			QCol->LCyan(" [<switches>] ");
			QCol->Pink(" <file> \n\n");
			QCol->Magenta("switches\n");
			QCol->Yellow("-xd\t"); QCol->Cyan("Show all data fields that comes with each entry\n");
			QCol->Yellow("-x\t"); QCol->Cyan("Show author and notes (if set)\n");
			QCol->Yellow("-a\t"); QCol->Cyan("Show all aliases\n");
			QCol->Yellow("-w\t"); QCol->Cyan("Allow Westwood PAK files to be viewed\n");
		}

		struct cblck { int count{ 0 }, ID{ 0 }; JT_Block Blck{ nullptr }; String Main{ "" }; };
		static int _View_Action(int car, char** arg) {
			int ret{ 0 };
			FlagConfig VA{};
			AddFlag_Bool(VA, "w", false); // ignored, as this is handled by the main program, but this will at least prevent issues!
			AddFlag_Bool(VA, "x", false);
			AddFlag_Bool(VA, "a", false);
			AddFlag_Bool(VA, "xd", false);
			auto PA{ ParseArg(car,arg,VA) };
			for (size_t i = 1; i < PA.arguments.size(); ++i) {
				auto fjcr{ ChReplace(PA.arguments[i],'\\','/') };
				QCol->Doing("Request", TrSPrintF("%d/%d", i, PA.arguments.size() - 1));
				QCol->Doing("Reading", fjcr);
				if (!FileExists(fjcr)) {
					if (DirectoryExists(fjcr)) {
						QCol->Error(fjcr + " is a directory!");
						ret = 2;
						break;
					} else {
						QCol->Error(fjcr + "has not been found");
						ret = 404; // Hey, let's remain in 'style' here.
						break;
					}
				}
				auto rjcr{ JCR6_Dir(fjcr) };
				if (Last()->Error) {
					QCol->Error(Last()->ErrorMessage);
					QCol->Doing("- Main", Last()->MainFile);
					QCol->Doing("- Entry", Last()->Entry);
					ret = 3;
					break;
				}
				if (!rjcr) { QCol->Error("JCR failed to load for unknown reasons!"); ret = 4; break; }
				QCol->Doing("Analyzing", fjcr);
				auto ejcr{ rjcr->Entries() };
				std::map<String, String> MainCode{};
				std::map<String, String> MainType{};
				std::map<String, int> MainCount{};
				std::map<String, int> StorageCount{};
				std::map<String, cblck> BlockCount{};
				for (auto ent : *ejcr) {
					MainCode[ent->MainFile] = Right(md5(ent->MainFile), 8);
					if (!MainCount.count(ent->MainFile)) MainCount[ent->MainFile] = 0;
					if (!StorageCount.count(ent->Storage())) StorageCount[ent->Storage()] = 0;
					if (!MainType.count(ent->MainFile)) {
						if (ent->Offset() == 0) MainType[ent->MainFile] = "RAW file";
						else MainType[ent->MainFile] = _JT_Dir::Recognize(ent->MainFile);
					}
					MainCount[ent->MainFile]++;
					StorageCount[ent->Storage()]++;
					if (ent->Block()) {
						auto btag{ TrSPrintF((Right(md5(ent->MainFile) ,8) + ".%09d").c_str(),ent->Block()) };
						if (!BlockCount.count(btag)) {
							BlockCount[btag].count = 0;
							BlockCount[btag].ID = ent->Block();
							BlockCount[btag].Blck = rjcr->Blocks[TrSPrintF("%d:%s", ent->Block(), ent->MainFile.c_str())];
							BlockCount[btag].Main = Right(md5(ent->MainFile), 8);
						}
						BlockCount[btag].count++;
					}
				}
				std::cout << "\n\n";
				QCol->White(ST("MainType", 10) + " " + ST("MainCode", 10) + " " + ST("Entries", 8) + " Main File\n");
				QCol->White(ST("========", 10) + " " + ST("========", 10) + " " + ST("=======", 8) + " =========\n");
				for (auto m : MainCount) {
					QCol->Magenta(ST(MainType[m.first], 10) + " ");
					QCol->LBlue(ST(MainCode[m.first], 10) + " ");
					QCol->Cyan(ST(TrSPrintF("%7d", m.second), 8) + " ");
					QCol->White(m.first + "\n");
				}
				std::cout << "\n\n";
				QCol->White(ST("Storage", 10) + " Used\n");
				QCol->White(ST("=======", 10) + " ====\n");
				for (auto s : StorageCount) {
					QCol->LMagenta(ST(s.first, 10) + " ");
					QCol->LCyan(TrSPrintF("%4d\n", s.second));
				}
				std::cout << "\n\n";
				QCol->White("MainCode Block Entries " + ST("Compressed", 12) + " " + ST("Real Size", 10) + " Ratio  Storage\n");
				QCol->White("======== ===== ======= " + ST("==========", 12) + " " + ST("=========", 10) + " =====  =======\n");
				for (auto b : BlockCount) {
					QCol->LBlue(b.second.Main + " ");
					QCol->LCyan(TrSPrintF("%5d ", b.second.ID));
					QCol->Cyan(TrSPrintF("%7d ", b.second.count));
					QCol->LGreen(TrSPrintF(" %9d   ", b.second.Blck->CompressedSize()));
					QCol->Pink(TrSPrintF("%9d  ", b.second.Blck->RealSize()));
					if (b.second.Blck->RealSize() == 0)
						QCol->Magenta("----- ");
					else
						QCol->LMagenta(TrSPrintF("%5.1f%% ", ((double)b.second.Blck->CompressedSize() / (double)b.second.Blck->RealSize()) * 100));
					QCol->Grey(b.second.Blck->dataString["__Storage"] + "\n");

				}
				std::cout << "\n\n";
				//                    1
				//           12345678901234
				QCol->White("     File Type " + ST("Compressed", 12) + " " + ST("Real Size", 10) + " Ratio  " + ST("MainCode", 10) + " " + ST("Storage", 10) + " Entry\n");
				QCol->White("     ========= " + ST("==========", 12) + " " + ST("=========", 10) + " =====  " + ST("========", 10) + " " + ST("=======", 10) + " =====\n");
				int ecnt{ 0 };
				for (auto e : *ejcr) {
					auto XT{ Upper(ExtractExt(e->Name())) };
					String ET{ "" }; if (ExtReg.count(XT)) ET = ExtReg[XT];
					QCol->Blue(Right("                    " + ET, 14) + " ");
					if (e->Block())
						QCol->Green(ST(TrSPrintF("Block #%03d", e->Block()), 12) + " ");
					else
						QCol->LGreen(TrSPrintF("   %9d ", e->CompressedSize()));
					QCol->Pink(TrSPrintF(" %9d ", e->RealSize()));
					if (e->Storage() == "Store" || e->RealSize() == 0 || e->Block())
						QCol->Magenta("------ ");
					else
						QCol->LMagenta(TrSPrintF("%5.1f%% ", ((double)e->CompressedSize() / (double)e->RealSize()) * (double)100));
					QCol->LBlue(ST(MainCode[e->MainFile], 10) + " ");
					QCol->Grey(ST(e->Storage(), 10) + " ");
					QCol->Yellow(e->Name() + "\n");
					if (PA.bool_flags["x"]) {
						QCol->Doing("\tAuthor", e->Author());
						QCol->Doing("\tNotes", e->Notes());
					}
					if (PA.bool_flags["xd"]) {
						for (auto v : e->_ConfigInt) { QCol->White("int    "); QCol->Yellow(v.first); QCol->White(" = "); QCol->Cyan(TrSPrintF("%d\n", v.second)); }
						for (auto v : e->_ConfigBool) { QCol->White("bool   "); QCol->Yellow(v.first); QCol->White(" = "); if (v.second) QCol->Green("True\n"); else QCol->Red("False\n"); }
						for (auto v : e->_ConfigString) { QCol->White("string "); QCol->Yellow(v.first); QCol->White(" = "); QCol->LMagenta("\"" + v.second + (String)"\"\n"); }
					}

					if (PA.bool_flags["a"]) {
						for (auto a : *ejcr) {
							if (a->Name() != e->Name() && e->MainFile==a->MainFile && e->Block()==a->Block() && e->Offset()==a->Offset() && e->RealSize()==a->RealSize() && e->CompressedSize()==a->CompressedSize() ) {
								QCol->Doing("Alias", a->Name());
							}
						}
					}
				}
			}
			return ret;

		}


		void Jal_Jalondi_View() {
			SJB("Jalondi_View.cpp");
			J_Action J{ "View",_View_Action,_View_Explain,"Shows content of JCR6 resource" };
			J_Action::Register["list"] = J;
		}
	}
}