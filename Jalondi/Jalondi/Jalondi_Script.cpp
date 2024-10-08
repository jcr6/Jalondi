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
// Version: 24.10.08
// EndLic

#include <Lunatic.hpp>

#include <SlyvBank.hpp>
#include <SlyvQCol.hpp>
#include <SlyvStream.hpp>

#include "Jalondi_Class.hpp"
#include "Jalondi_Create.hpp"
#include "Jalondi_Lua.hpp"
#include <SlyvDir.hpp>

using namespace Slyvina::NSLunatic;

using namespace Slyvina::Units;

namespace Slyvina {
	namespace Jalondi {

#pragma region APIs
		bool _StaakAlles{ false };
#define CSA if (_StaakAlles) return 12;
		static int PaniekLua(lua_State* L) {
			auto Uitleg{ NewVecString() };
			//cout << lua_gettop(L) << "\n";
			Uitleg->push_back(TrSPrintF("lua_gettop(State) = %d", lua_gettop(L)));
			for (int i = 1; i <= lua_gettop(L); i++) {
				auto line{ TrSPrintF("Arg #%d/%d:\t",i,lua_gettop(L)) };
				switch (lua_type(L, i)) {
				case LUA_TSTRING:
					line += TrSPrintF("String: \"%s\"", luaL_checkstring(L, i));
					break;
				case LUA_TNUMBER:
					line += TrSPrintF("Number: %f", luaL_checknumber(L, i));
					break;
				case LUA_TFUNCTION:
					line += "Function";
					break;
				default:
					line += TrSPrintF("Unknown: %d", lua_type(L, i));
					break;
				}
				Uitleg->push_back(line);
			}
			QCol->Error("Lua went in a state of panic");
			for (auto UL : *Uitleg) QCol->Grey("\t" + UL + "\n");
			_StaakAlles = true; 
			return 0;
		}
		static void LEr(lua_State *L,String Msg,bool staken=true) {
			QCol->Error("Error in Lua script: " + Msg); QCol->Reset();
			luaL_loadstring(L,"print(debug.traceback())");
			_StaakAlles = _StaakAlles || staken;
		}

		static int JA_Love(lua_State* L) {
			// Test message and also a nice easter egg.
			QCol->Yellow("Jalondi Smaragd loves Gyermo with all her heart\n");
			QCol->Yellow("Read \"The Wings Of War\" to see how she will manage with those who are against it!\n");
			QCol->Reset();
			return 0;
		}

		static int JA_Doing(lua_State* L) {
			QCol->Doing(luaL_checkstring(L, 1), luaL_checkstring(L,2),luaL_optstring(L,3,"\n"));
			QCol->Reset();
			return 0;
		}

		static int JA_FSplit(lua_State* L) {
			auto F{ Lunatic_CheckString(L,1) };
			Lunatic_PushString(L, ExtractDir(F));
			Lunatic_PushString(L, StripAll(F));
			Lunatic_PushString(L, ExtractExt(F));
			return 3;
		}

		static int JA_Clear(lua_State* L) {
			Create_Clear();
			return 0;
		}

		static int JA_Add(lua_State* L) {
			auto Source{ Lunatic_CheckString(L,1) };
			FileToAdd F2A{
				Source,
				Lunatic_OptString(L,2,StripDir(Source)),
				Lunatic_OptString(L,3,"Store"),
				Lunatic_OptString(L,4,""), // Author
				Lunatic_OptString(L,5,""), // Notes
				Lunatic_OptString(L,6,"") // Block
			};
			if (IsFile(Source))
				Create_AddFile(F2A);
			else if (IsDir(Source)) {
				if (!Suffixed(F2A.Target, "/")) F2A.Target += "/";
				auto Tree = GetTree(F2A.Source);
				for (auto f : *Tree) {
					Create_AddFile(Source + "/" + f, F2A.Target + f, F2A.Storage, F2A.Author, F2A.Notes, F2A.Block);
				}
			}
			return 0;
		}

		static int JA_Import(lua_State* L) {
			Create_AddPatch(luaL_checkstring(L, 1), PatchType::Import);
			return 0;
		}
		static int JA_Require(lua_State* L) {
			Create_AddPatch(luaL_checkstring(L, 1), PatchType::Require);
			return 0;
		}
		static int JA_Alias(lua_State* L) {
			Create_Alias(luaL_checkstring(L, 1), luaL_checkstring(L, 2));
			return 0;
		}
		static int JA_PWD(lua_State* L) {
			Lunatic_PushString(L, CurrentDir());
			return 1;
		}
		static int JA_Dir(lua_State* L) {
			static VecString _Dir{ NewVecString() };
			switch(luaL_checkinteger(L,1)){
			case 1: // get
				_Dir = GetTree(Lunatic_OptString(L, 2, CurrentDir()));
				lua_pushinteger(L, _Dir->size());
				return 1;
			case 2: // Check File
			{
				auto idx{ luaL_checkinteger(L,2) };
				if (idx >= _Dir->size()) { LEr(L, TrSPrintF("Too high dir index (%d>=%d)", idx, _Dir->size())); return 0; }
				Lunatic_PushString(L, (*_Dir)[idx]);
				return 1;
			}
			default:
				QCol->Error("Invalid input in JA_Dir API");
				return 0;
			}
		}
		static int JA_Platform(lua_State* L) {
			Lunatic_PushString(L, Platform(luaL_optinteger(L, 1, 1)));
			return 1;
		}

		static int JA_Start(lua_State* L) {
			Create_Run(
				Lunatic_CheckString(L, 1),
				Lunatic_OptString(L, 2, "Store"),
				Lunatic_OptString(L, 3, "*AUTO")
			);
			return 0;
		}

		static std::map<String, lua_CFunction> Jalondi_API{
			{ "LOVE", JA_Love },
			{ "DOING", JA_Doing },
			{ "FSPLIT", JA_FSplit },
			{ "CLEAR", JA_Clear },
			{ "ADD", JA_Add },
			{ "IMPORT", JA_Import },
			{ "REQUIRE", JA_Require },
			{ "ALIAS", JA_Alias },
			{ "PWD", JA_PWD },
			{ "DIR_TRUE_IGNORE",JA_Dir },
			{ "PLATFORM",JA_Platform },
			{ "START", JA_Start },
			{ "RUN",JA_Start }
		};
#pragma endregion

#pragma region Action!
		static String LSafe(String S) {
			String ret{ "" };
			for (size_t i = 0; i < S.size(); ++i) {
				switch (S[i]) {
				case '"': ret += "\\\"";
				case '\t': ret += "\\t";
				case '\n': ret += "\\n";
				case '\r': ret += "\\r";
				default:
					if (S[i] >= 32 && S[i] < 127)
						ret += S[i];
					else
						ret += TrSPrintF("\\x%02x", S[i]);
				}
			}
			return ret;
		}

		static void Script_Explain() {
			QCol->White("Will show all the contents of a JCR6 resource (including all external files it's been linked to)\n\n");
			QCol->Magenta("Usage: ");
			QCol->Yellow(StripAll(Jalondi_Exe));
			QCol->LGreen(" script ");			
			QCol->Pink(" <Lua script file> ");
			QCol->Grey("[arguments]");
		}

		static int Script_Go(int c, char** a) {
			_StaakAlles = false;
			Create_Clear();
			QCol->Doing("Preparing", "Lua State");
			String
				Script{ a[2] },
				ArgArray{ "JALONDI_ARGS = { script=\"" + Script + "\"}\n" };
			for (int i = 3; i < c; ++i) ArgArray += " JALONDI_ARGS[#JALONDI_ARGS+1] = \"" + LSafe(a[i]) + "\"\n";
			_Lunatic::Panick = PaniekLua;
			auto State{ LunaticBySource(ArgArray) }; CSA;
			for (auto JLM : Jalondi_API) {
				State->Register("JALONDI_" + Upper(JLM.first), JLM.second);
			}
			State->QDoString(Lua_Jalondi); CSA;
			auto Ext{ Upper(ExtractExt(Script)) };
			if (Ext == "LUA") {
				QCol->Doing("Compiling", Script);
				if (!FileExists(Script)) { QCol->Error("Script not found!"); return 404; }
				State->QDoString("--[["+Script+"]]\t"+FLoadString(Script)); CSA;
			} else if (Ext == "LUAC" || Ext == "LBC") {
				QCol->Doing("Loading", Script);
				if (!FileExists(Script)) { QCol->Error("Compiled script not found!"); return 404; }
				auto Buf{ LoadCharBuf(Script) };
				auto B{ CreateBank(Buf,FileSize(Script)) };
				State->QDoByteCode(B); 
				delete Buf;
				CSA;
			}
			return 0;
		}
#pragma endregion


#pragma region Init
		void Jal_Jalondi_Script() {
			SJB("Jalondi_Script.cpp");
			J_Action J{ "script",Script_Go,Script_Explain,"Run a lua script in order to configure how to create a JCR6 file" };
		}
#pragma endregion
	}
}