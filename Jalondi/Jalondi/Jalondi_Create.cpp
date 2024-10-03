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

#include "Jalondi_Create.hpp"
#include "Jalondi_Class.hpp"

using namespace Slyvina::Units;

namespace Slyvina {
	namespace Jalondi {
		static std::vector<FileToAdd> _FilesToAdd{};
		static std::vector<EntryToAlias> _EntriesToAlias{};
		static std::map<String, Int32> _BlockNums{};

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
			_BlockNums[BlockTag] = 0; // Actual numbers will be filled in once the packing is running
			return BlockTag;
		}

		String Create_AddFile(String Source, String Target, String Storage, String Block) {
			FileToAdd F2A{ Source,Target,Storage,Block };
			return Create_AddFile(F2A);
		}

		String Create_Alias(EntryToAlias E2A) { _EntriesToAlias.push_back(E2A); }

		String Create_Alias(String Source, String Alias) {
			EntryToAlias E2A{ Source,Alias };
			Create_Alias(E2A);
		}



		void Jal_Jalondi_Create() {
			SJB("Jalondi_Create");
			J_Action JC{ "create",nullptr,nullptr,"Create new JCR file" };
		}
	}
}