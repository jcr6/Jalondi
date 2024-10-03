// Lic:
// Jalondi
// Script Manager
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

#include <Lunatic.hpp>

#include <SlyvQCol.hpp>
#include "Jalondi_Class.hpp"

#include "Jalondi_Create.hpp"
#include "Jalondi_Lua.hpp"

using namespace Slyvina::Units;

namespace Slyvina {
	namespace Jalondi {

#pragma region APIs


		static int JA_Love(lua_State* L) {
			// Test message and also a nice easter egg.
			QCol->Yellow("Jalondi Smaragd loves Gyermo with all her heart\n");
			QCol->Yellow("Read \"The Wings Of War\" to see how she will manage with those who are against it!\n");
			return 0;
		}
		
		static std::map<String, lua_CFunction> Jalondi_API{
			{ "LOVE", JA_Love }
		};
#pragma endregion

#pragma region Action!
		static void Script_Explain() {
			QCol->White("Will show all the contents of a JCR6 resource (including all external files it's been linked to)\n\n");
			QCol->Magenta("Usage: ");
			QCol->Yellow(StripAll(Jalondi_Exe));
			QCol->LGreen(" script ");			
			QCol->Pink(" <Lua script file> ");
			QCol->Grey("[arguments]");
		}
#pragma endregion


#pragma region Init
		void Jal_Jalondi_Script() {
			SJB("Jalondi_Script.cpp");
			J_Action J{ "script",nullptr,Script_Explain,"Run a lua script in order to configure how to create a JCR6 file" };
		}
#pragma endregion
	}
}