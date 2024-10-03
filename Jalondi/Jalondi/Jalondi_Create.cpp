// Lic:
// Jalondi
// Create JCR6 File
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

#include <SlyvRoman.hpp>
#include <SlyvString.hpp>

#include <JCR6_Write.hpp>

#include "Jalondi_Create.hpp"
#include "Jalondi_Class.hpp"
#include <SlyvTime.hpp>
#include <SlyvMD5.hpp>
#include <SlyvQCol.hpp>
#include <SlyvArgParse.hpp>
#include <SlyvDir.hpp>

using namespace Slyvina::Units;
using namespace Slyvina::JCR6;

#pragma region "Work it out!"
#define Err(abc) QCol->Error(TrSPrintF("%03d:",++errors)+abc)
#define ErrB(abc) {QCol->Error(TrSPrintF("%03d:",++errors)+abc); break; }
#define J6E \
	if (Last()->Error) {\
		QCol->Error(Last()->ErrorMessage);\
		QCol->Doing("Main",Last()->MainFile);\
		QCol->Doing("Entry",Last()->Entry);\
		QCol->Red("Error is fatal! Aborting operation! Please note created JCR6 files MAY be corrupted and unusable!\\n\n");\
		return;\
	}

namespace Slyvina {
	namespace Jalondi {
		static std::vector<FileToAdd> _FilesToAdd{};
		static std::vector<EntryToAlias> _EntriesToAlias{};
		static std::vector<PatchToAdd> _PatchesToAdd{};
		static std::map<String, JT_CreateBlock> _BlockNums{};

		void Slyvina::Jalondi::Create_Clear() {
			_FilesToAdd.clear();
			_BlockNums.clear();
		}

		String Create_AddFile(FileToAdd& F2A) {
			Trans2Upper(F2A.Block);
			String BlockTag{};
			if (F2A.Block == "*NEW") {
				auto i = 0;
				do {
					BlockTag = "BLOCK " + ToRoman(i++);
				} while (_BlockNums.count(BlockTag));
			} else { BlockTag = F2A.Block; }
			_BlockNums[BlockTag] = nullptr; // Actual numbers will be filled in once the packing is running
			return BlockTag;
		}

		String Create_AddFile(String Source, String Target, String Storage, String Author, String Notes, String Block) {
			FileToAdd F2A{ Source,Target,Storage,Author,Notes,Block };
			return Create_AddFile(F2A);
		}

		void Create_Alias(EntryToAlias E2A) { _EntriesToAlias.push_back(E2A); }

		void Create_Alias(String Source, String Alias) {
			EntryToAlias E2A{ Source,Alias };
			Create_Alias(E2A);
		}

		void Create_AddPatch(PatchToAdd P2A) { _PatchesToAdd.push_back(P2A); }

		void Create_AddPatch(String Source, PatchType PT, String Prefix) {
			PatchToAdd P2A{ Source,Prefix,PT };
			Create_AddPatch(P2A);
		}

		void Create_Run(String JCR6File, String Storage, String Signature) {
			QCol->Doing("Creating", JCR6File);
			uint32 errors{ 0 };
			auto Sig{ Signature }; if (Upper(Sig) == "*AUTO") Sig = md5(JCR6File + "." + Storage + "." + CurrentTime() + "." + CurrentDate());
			auto JO{ CreateJCR6(JCR6File,Storage,Sig) }; J6E;
			QCol->Doing("FT Storage", Storage);
			if (Sig.size()) QCol->Doing("Signature", Sig);
			for (auto F2A : _FilesToAdd) {
				QCol->Doing("Freezing", F2A.Source, " ");
				if (!FileExists(F2A.Source)) ErrB("Source file not found!");
				if (!GetCompDrivers()->count(F2A.Storage)) {
					Err("Storage method '" + F2A.Storage + "' not found -- Storing!");
					F2A.Storage = "Store";
					QCol->Doing("Freezing", F2A.Source, " ");
				}
				if (JO->Entries.count(Upper(F2A.Target))) {
					ErrB("Duplicate target name: " + F2A.Target);
				}
				if (F2A.Block.size()) {
					if (!_BlockNums[F2A.Block]) {
						_BlockNums[F2A.Block] = JO->AddBlock(F2A.Storage); J6E;
						QCol->LBlue(TrSPrintF("New Block #%d", _BlockNums[F2A.Block]->ID()));
					}
					_BlockNums[F2A.Block]->AddFile(F2A.Source, F2A.Target, F2A.Author, F2A.Notes);
					QCol->Green(TrSPrintF("\rBlock #%03d", _BlockNums[F2A.Block]->ID()));
				} else {
					JO->AddFile(F2A.Source, F2A.Target, F2A.Storage, F2A.Author, F2A.Notes); J6E;
					auto e{ JO->Entries[Upper(F2A.Target)] };
					if (e->Storage() == "Store\n")
						QCol->White("\rStored:   ");
					else {
						QCol->LGreen(e->Storage() + "\r");
						QCol->LMagenta(TrSPrintF("%9.2%% \n", ((double)e->CompressedSize() / (double)e->RealSize()) * 100.0));
					}
				}
			}
			for (auto B2C : _BlockNums) {
				QCol->Doing("Closing", TrSPrintF("Block #%03d", B2C.second->ID()), " ");
				B2C.second->Close();  J6E;
				if (B2C.second->Storage() == "Store\n")
					QCol->White("\rStored:   ");
				else {
					QCol->LGreen(B2C.second->Storage() + "\r");
					QCol->LMagenta(TrSPrintF("%9.2%% \n", ((double)B2C.second->CompressedSize() / (double)B2C.second->Size()) * 100.0));
				}
			}
			for (auto AE : _EntriesToAlias) {
				QCol->Doing("Alias", AE.Source, ""); QCol->LMagenta(" -> "); QCol->LGreen(AE.Alias);
				if (!JO->Entries.count(Upper(AE.Source))) ErrB("Source not present!");
				if (JO->Entries.count(Upper(AE.Alias))) ErrB("Dupe target");
				JO->Alias(AE.Source, AE.Alias);
				QCol->LCyan(" Ok\n");
			}
			for (auto Dep : _PatchesToAdd) {
				switch (Dep.Type) {
				case PatchType::Import:
					QCol->Doing("Import", Dep.Source);
					JO->Import(Dep.Source); J6E;
					break;
				case PatchType::Require:
					QCol->Doing("Require", Dep.Source);
					JO->Require(Dep.Source); J6E;
					break;
				default:
					Err(TrSPrintF("Unknown patch type (%d)", (int)Dep.Type));;
					break;
				}
			}
			QCol->Doing("Finalizing", JCR6File);
			JO->Close(); J6E;
			QCol->Cyan("Ok\n\n");
		}
#pragma endregion

#pragma region Module
		void _Create_Explain() {
			QCol->White("Will show all the contents of a JCR6 resource (including all external files it's been linked to)\n\n");
			QCol->Magenta("Usage: ");
			QCol->Yellow(StripAll(Jalondi_Exe));
			QCol->LGreen(" create ");
			QCol->LCyan(" [<switches>] ");
			QCol->Pink(" <JCR6 file> ");
			QCol->Grey(" <files to pack>\n");
			QCol->LMagenta("\n\nAllowed switches\n");
			QCol->Yellow("-cm <method>\t"); QCol->Cyan("Compression method (default is Store)\n");
			QCol->Yellow("-fc <method>\t"); QCol->Cyan("Compression method for File Table (default is Store)\n");
			QCol->Yellow("-s <signature>\t"); QCol->Cyan("Set signature manually\n");
			QCol->Yellow("-a <author>\t"); QCol->Cyan("Set Author\n");
			QCol->Yellow("-n <notes>\t"); QCol->Cyan("Set Notes\n");
			QCol->Yellow("-imp <imports>\t"); QCol->Cyan("Import external JCR6 files. All must be in 1 argument divided by semi-colons\n");
			QCol->Yellow("-req <imports>\t"); QCol->Cyan("Require external JCR6 files. All must be in 1 argument divided by semi-colons\n");
			QCol->Yellow("-y\t"); QCol->Cyan("Answer all yes/no questions with 'yes'\n");
		}

		static int _Create_Action(int car, char** arg) {
			int ret{ 0 };
			FlagConfig f{};
			//AddFlag_Bool(f, "w", false);
			AddFlag_Bool(f, "y", false);
			AddFlag_String(f, "cm", "Store");
			AddFlag_String(f, "fc", "Store");
			AddFlag_String(f, "s", "*AUTO");
			AddFlag_String(f, "a", "");
			AddFlag_String(f, "n", "");
			AddFlag_String(f, "imp", "");
			AddFlag_String(f, "req", "");
			auto PA{ ParseArg(car,arg,f) };
			auto fjcr{ ChReplace(PA.arguments[1],'\\','/') };
			auto cm{ PA.string_flags["cm"] };
			auto author{ PA.string_flags["author"] };
			auto notes{ PA.string_flags["notes"] };
			auto alwaysyes{ PA.bool_flags["y"] };
			std::vector<String> AddStuff{};
			if (car <= 3) AddStuff.push_back(ChReplace(CurrentDir(), '\\', '/')); else for (int i = 3; i < car; i++) AddStuff.push_back(ChReplace(arg[i], '\\', '/'));
			Create_Clear();
			for (auto A : AddStuff) {
				if (IsFile(A)) {
					Create_AddFile(A, StripDir(A), cm, author, notes);
				} else if (IsDir(A)) {
					QCol->Doing("Analysing", A);
					auto D{ GetTree(A) };
					for (auto F : *D) {
						QCol->Dark(ST(F, 75) + "\r");
						Create_AddFile(A + "/" + F, F, cm, author, notes);
					}
					QCol->Dark(ST("", 75) + "\r");
				} else {
					QCol->Error("File or directory \"" + A + "\" not found!"); 
					ret = 404;
				}
			}
			return ret;
		}

#pragma endregion

		void Jal_Jalondi_Create() {
			SJB("Jalondi_Create");
			J_Action JC{ "create",_Create_Action,_Create_Explain,"Create new JCR file" };
		}


	}
}