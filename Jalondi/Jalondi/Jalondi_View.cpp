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
// Version: 24.10.02
// EndLic

#include <SlyvQCol.hpp>
#include <SlyvString.hpp>

#include "Jalondi_Class.hpp"

using namespace Slyvina;
using namespace Units;

namespace Slyvina {
	namespace Jalondi {
		static void _View_Explain() {
			QCol->White("Will show all the contents of a JCR6 resource (including all external files it's been linked to)\n\n");
			QCol->Magenta("Usage: ");
			QCol->Yellow(StripAll(Jalondi_Exe));
			QCol->LGreen(" <command> ");
			QCol->LCyan(" [<switches>] ");
			QCol->Pink(" <file> \n\n");
			QCol->Magenta("switches\n");
			QCol->Yellow("-xd\t"); QCol->Cyan("Show all data fields that comes with each entry\n");
			QCol->Yellow("-x\t"); QCol->Cyan("Show author and notes (if set)\n");
			QCol->Yellow("-a\t"); QCol->Cyan("Show all aliases\n");
			QCol->Yellow("-w\t"); QCol->Cyan("Allow Westwood PAK files to be viewed\n");
		}

		static int _View_Action(int car, char** arg) {
			return 1;
		}

		void Jal_Jalondi_View() {
			SJB("Jalondi_View.cpp");
			J_Action J{ "View",_View_Action,_View_Explain,"Shows content of JCR6 resource" };
			J_Action::Register["list"] = J;
		}
	}
}