
#include <Lunatic.hpp>

#include "Jalondi_Create.hpp"
#include <SlyvQCol.hpp>
#include "Jalondi_Class.hpp"

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
