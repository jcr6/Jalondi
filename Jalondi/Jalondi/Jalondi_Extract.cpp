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

using namespace Slyvina::Units;

namespace Slyvina {
	namespace Jalondi {

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
			QCol->Yellow("-o <dir> \t"); QCol->Cyan("Define output dir (current dir is default)");
		}
#pragma endregion

#pragma region Init
		void Jal_Jalondi_Extract() {
			SJB("Jalondi_Extract.cpp");
			J_Action A{ "extract",nullptr,_xExplain,"Extracts from JCR6 or packed files JCR6 can read" };
		}
#pragma endregion
	}
}