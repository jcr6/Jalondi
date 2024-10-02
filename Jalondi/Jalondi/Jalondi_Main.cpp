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