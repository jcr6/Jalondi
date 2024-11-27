// License:
// 
// Jalondi
// Create (header)
// 
// 
// 
// 	(c) Jeroen P. Broks, 2024
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
// Version: 24.10.30
// End License
// Lic:
// Jalondi
// Create JCR6 File (header)
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

#pragma once
#include <Slyvina.hpp>


namespace Slyvina {
	namespace Jalondi {
		enum class PatchType { Unknown, Import, Require };
		enum class CommentFrom { Unknown, String, File };
		struct FileToAdd {
			String
				Source{ "" },
				Target{ "" },
				Storage{ "" },
				Author{ "" },
				Notes{ "" },
				Block{ "" };
			CommentFrom 
				Type{ CommentFrom::File };
		};
		struct EntryToAlias {
			String
				Source{ "" },
				Alias{ "" };
		};
		struct PatchToAdd {
			String
				Source{ "" },
				Prefix{ "" };
			PatchType
				Type{ PatchType::Unknown };
		};
		extern bool ScriptMerge;
		void Create_Clear();
		String Create_AddFile(FileToAdd& F2A);
		String Create_AddFile(String Source, String Target, String Storage = "Store", String Author="",String Notes="", String Block = "");
		String Create_AddString(FileToAdd& F2A);
		String Create_AddString(String Source, String Target, String Storage = "Store", String Author = "", String Notes = "", String Block = "");
		void Create_Alias(EntryToAlias E2A);
		void Create_Alias(String Source, String Alias);
		void Create_AddPatch(PatchToAdd P2A);
		void Create_AddPatch(String Source, PatchType PT = PatchType::Import, String Prefix = "");
		void Create_AddComment(String Source, String Tag, CommentFrom From = CommentFrom::String);
		bool Create_HaveComment(String Tag);

		void Create_Run(String JCR6File, String Storage = "Store", String Signature = "*AUTO");
	}
}
