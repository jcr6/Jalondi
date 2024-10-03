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

using namespace Slyvina::Units;
using namespace Slyvina::JCR6;

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
				if (!GetCompDrivers()->count(F2A.Storage)){
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

				}
			}
		}

		void Jal_Jalondi_Create() {
			SJB("Jalondi_Create");
			J_Action JC{ "create",nullptr,nullptr,"Create new JCR file" };
		}

		
	}
}