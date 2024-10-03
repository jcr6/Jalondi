// Lic:
// Jalondi
// HEX viewer
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
#include <SlyvString.hpp>
#include <SlyvQCol.hpp>
#include <JCR6_Core.hpp>

using namespace Slyvina;
using namespace Units;
using namespace JCR6;

namespace Slyvina {
	namespace Jalondi {

		static void _HEX_Explain() {
			QCol->White("Will show all the contents of a JCR6 resource (including all external files it's been linked to)\n\n");
			QCol->Magenta("Usage: ");
			QCol->Yellow(StripAll(Jalondi_Exe));
			QCol->LGreen(" view ");
			QCol->LCyan(" [<switches>] ");
			QCol->Pink(" <JCR6 file> ");
			QCol->Grey(" <entries>\n");
		}

		static int _HEX_Action(int car, char** arg){
			int ret{ 0 };
			return ret;
		}


		void Jal_Jalondi_HEX() {
			SJB("Jalondi_HEX.cpp");
			J_Action J{ "Hex",_HEX_Action,_HEX_Explain,"HEX dump of one of more entries inside a JCR6 file" };
		}
	}
}