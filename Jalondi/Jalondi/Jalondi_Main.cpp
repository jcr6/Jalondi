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
// Version: 24.10.02
// EndLic
#include <SlyvQCol.hpp>
#include <SlyvString.hpp>

#include "Jalondi_Class.hpp"
#include "Jalondi_Init.hpp"

using namespace Slyvina;
using namespace Units;
using namespace Jalondi;

void Slyvina::Jalondi::Jal_Jalondi_Main() {}; // must only exist, but has no value at all.

int main(int argc, char** args) {
	SJB("Jalondi_Main.cpp");
	Jalondi_Exe = args[0];
	QCol->LMagenta("Jalondi\t");
	QCol->Yellow("JCR6 quick CLI management tool\n\n");
	QCol->Doing("Coded by", "Jeroen P. Broks");
	QCol->Doing("License", "General Public License v3");
	QCol->LMagenta("(c) Jeroen P. Broks 2024!\n\n");
	InitAll();
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
	}
	QCol->Reset();
	return 0;
}