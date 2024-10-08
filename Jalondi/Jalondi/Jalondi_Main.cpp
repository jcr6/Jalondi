// Lic:
// Jalondi
// Main Program
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

// Config MUST be on top!
#include "Jalondi_Config.hpp"

#include <cstring>

#include <SlyvQCol.hpp>
#include <SlyvString.hpp>

/*
#include <JCR6_Core.hpp>
#include <JCR6_JQL.hpp>
#include <JCR6_QuakePAK.hpp>
#include <JCR6_RealDir.hpp>
#include <JCR6_WAD.hpp>
#include <JCR6_Westwood.hpp>
#ifdef Use_zlib_in_JCR6
#include <JCR6_zlib.hpp>
#endif
//*/

#include "Jalondi_Class.hpp"
#include "Jalondi_Init.hpp"


using namespace Slyvina;
using namespace Units;
using namespace Jalondi;
//using namespace JCR6;

namespace Slyvina { namespace Jalondi { void Jal_Jalondi_Main() {} } } // must only exist, but has no value at all.

int main(int argc, char** args) {
	
	int ret{ 0 };
	QCol->LMagenta("Jalondi\t");
	QCol->Yellow("JCR6 quick CLI management tool\n\n");
	QCol->Doing("Coded by", "Jeroen P. Broks");
	QCol->Doing("License", "General Public License v3");
	QCol->LMagenta("(c) Jeroen P. Broks 2024!\n\n");
	//Jalondi_Exe = args[0];
/*
	SJB("Jalondi_Main.cpp");
	InitAll();
	InitJQL();
	InitQuake();
	InitWAD();
	for (int i = 2; i < argc; ++i) if (strcmp(args[i], "-w")==0) { Westwood_Init(); QCol->Warn("Westwood PAK files available"); }
#ifdef Use_zlib_in_JCR6
	init_zlib();
#endif
//*/
	switch(argc)
	{
	case 0:
		QCol->Error("Start up trouble! (Must be a bug in the C++ compiler, as this error can normally never happen).");
		return 999;
	case 1:
		QCol->Magenta("Usage: ");
		QCol->Yellow(StripAll(args[0]));
		QCol->LGreen(" <command> ");
		QCol->LCyan(" [<switches>] ");
		QCol->Pink(" <file> \n\n");
		QCol->Magenta("Known commands (two commands with the same function are merely aliases)\n");
		for (auto f : J_Action::Register) {
			QCol->Yellow(Left(f.first + "          ", 10) + " ");
			QCol->Cyan(f.second.QuickExplanation);
			std::cout << "\n";
		}
		break;
		/*
	case 2:
		if (!J_Action::Register.count(Lower(args[1]))) { QCol->Error("Command " + String(args[1]) + " not recognized!"); return 1; }
		J_Action::Register[Lower(args[1])].Explain();
		break;
	default:
		if (!J_Action::Register.count(Lower(args[1]))) { QCol->Error("Command " + String(args[1]) + " not recognized!"); return 1; }
		ret = J_Action::Register[Lower(args[1])].Func(argc, args);
	//*/
	}
	QCol->Reset();
	return ret;
}