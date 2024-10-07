// Lic:
// Jalondi
// Convert
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
// Version: 24.10.07
// EndLic
#include <SlyvAsk.hpp>

#include "Jalondi_Class.hpp"
#include <SlyvDir.hpp>
#include <SlyvArgParse.hpp>
#include <SlyvDirry.hpp>
#include <SlyvMD5.hpp>
#include <SlyvTime.hpp>
#include <SlyvSilly.hpp>


#define _CRT_NONSTDC_NO_WARNINGS
using namespace Slyvina::Units;

namespace Slyvina {
	namespace Jalondi {
#pragma region "Creation Archiver Data"		
		static String ConvertFile{ Dirry("$AppSupport$/Jalondi_Convert.ini") };
		static GINIE CVD{ ParseGINIE("# Start\n#Nothing here yet!") };
		static void NewPacker(String Ext, String Packer, String UnPacker, String PackCommand, String UnPackCommand);
		static void NewPacker(String Ext, String Packer, String PackCommand, String UnPackCommand) { NewPacker(Ext, Packer, Packer, PackCommand, UnPackCommand); }
		static void NewPacker(String Ext, String PackCommand, String UnPackCommand) { NewPacker(Ext, Ext, PackCommand, UnPackCommand); }
		static void NewPacker(String Ext, String Packer, String UnPacker, String PackCommand, String UnPackCommand) {
			auto
				PackInternal{ Upper(Packer) == "JALONDI" },
				UnPackInternal{ Upper(UnPacker) == "JALONDI" };
#ifdef SlyvWindows
			Packer += ".exe";
			UnPacker += ".exe";
#endif  
			QCol->Doing("Format", Upper(Ext));
			String FPacker{ "" }; if (Packer[0]!='*') FPacker = FromPath(Packer);
			String FUnPacker{ "" }; if (FUnPacker[0]!='*') FUnPacker = FromPath(UnPacker);
			if (IndexOf(FPacker, ' ') >= 0) { QCol->Error("Can't add this format: Space in packer path " + FPacker); return; }
			if (IndexOf(FUnPacker, ' ') >= 0) { QCol->Error("Can't add this format: Space in packer path " + FUnPacker); return; }
			CVD->BoolValue(Ext, "Pack.Available", FPacker != "");
			CVD->BoolValue(Ext, "Unpack.Available", FUnPacker != "");
			if (FPacker != "" || PackInternal) {
				CVD->Value(Ext, "Pack.Exe", FPacker);
				CVD->Value(Ext, "Pack.Arg", StReplace(PackCommand, "\\", "$q$"));
				CVD->BoolValue(Ext, "Pack.Int", PackInternal);
				QCol->Doing("- Added", "Packer");
			}
			if (FUnPacker != "" || UnPackInternal) {
				CVD->Value(Ext, "UnPack.Exe", FUnPacker);
				CVD->Value(Ext, "UnPack.Arg", StReplace(UnPackCommand, "\\", "$q$"));
				CVD->BoolValue(Ext, "Unpack.Int", UnPackInternal);
				QCol->Doing("- Added", "Unpacker");
			}
		}

		static bool CreatePackerFile(bool forcerenew) {
			if (forcerenew) {
				QCol->Doing("Renewing","packer data");
			} else if (!FileExists(ConvertFile)) {
				if (!QuickYes("Jalondi uses a file named " + ConvertFile + " for all data about packers.\nIt does not yet exist, may I created it")) return false;
				QCol->Doing("Creating", "packer data");
			} else {
				CVD = LoadGINIE(ConvertFile);
				return true; // No need to redo the data, when everything has been set, eh?
			}
			CVD->Clear();
			// Jalondi internal
			NewPacker("JCR", "Jalondi", "Jalondi", "create -cm zlib $Package$", "extract $Package$");
			NewPacker("WAD", "*", "Jalondi", "", "extract -y $Package$");
			NewPacker("PAK", "*", "Jalondi", "", "extract -y $Package$"); // I'll assume this will be Quake PAK files and not Westwood (former is more common these days)
			NewPacker("JQL", "*", "Jalondi", "", "extract -y $Package$");

			// External
			NewPacker("ZIP", "zip", "unzip", "$Package$ * -r", "$Package$");
			NewPacker("rar", "a -m5 -r  $Package$ *", "x $Package$");
			NewPacker("7z", "a $Package$", "x $Package$");
			NewPacker("lzh", "lha", "lha", "a $Package$", "x $Package$");
			NewPacker("lha", "lha", "lha", "a $Package$", "x $Package$");

			// ARJ files are being unpacked by 7z. This because ARJ hasn't seen updates in years (last one in 2012), 
			// yet it will pester the user to update it to a new version (which apparently doesn't exist).
			// Since 7z is able to unpack ARJ files, no need to have needless pestering.
			NewPacker("ARJ", "arj32", "7z", "a -m4 -r -jt $Package$", "x $Package$");

			NewPacker("tar", "-cf $Package$", "-x $Package$");

			CVD->AutoSaveHeader = "Jalondi convert data\nDon't modify unless you know what you are doing!";
			QCol->Doing("Saving", ConvertFile);
			CVD->AutoSave = ConvertFile;
			CVD->Value("*SYS", "Created", CurrentDate());
			return true;
		}
#pragma endregion

#pragma region Explain
		static void conv_Explain() {
			QCol->White("Converts archives and packages from one format to another\n\n");
			QCol->Magenta("Usage: ");
			QCol->Yellow(StripAll(Jalondi_Exe));
			QCol->LGreen(" convert ");
			QCol->LCyan(" [<switches>] ");
			QCol->Pink(" <original> ");
			QCol->Grey(" <target>\n");
			QCol->LMagenta("\n\nAllowed switches\n");
			//QCol->Yellow("-w         \t"); QCol->Cyan("Enable Westwoord PAK files\n");
			QCol->Yellow("-renew "); QCol->Cyan("Renew data file about packers and unpackers");
			QCol->Yellow("-ovr   "); QCol->Cyan("Delete target file before converting if it exists");
			
			QCol->Grey("\n\nSince Jalondi is not compatible with the formats you can access this way, Jalondi will merely call the programs that can and only do those actions by itself that it can due to JCR6 compatibility.\n\nThe first time you try to make Jalondi convert resources/archives, it will try to scan for the programs it knows to be able to handle them.\nThe data will be stored in "+ConvertFile+" for future referrence. You can alter that file if you know/have resource/archive formats that Jalondi doesn't know of, and that will cause Jalondi to be able to work with those\n\n");
			QCol->Red("IMPORTANT NOTE!\n"); QCol->Yellow("Please note that JCR6 supports stuff that archives do not and vice versa.\nConverted files can basically never be 'perfect'.\n\n");
		}
#pragma endregion


#pragma region Action
#ifdef SlyvWindows		
#define DSRE(EM,EX) {QCol->Error(EM); ChangeDir(pwd); system((String("cmd /c rd /s /q ")+ChReplace(sessiondir,'/','\\')).c_str()); return EX;}
		
#else
#define DSRE(EM,EX) {QCol->Error(EM); ChangeDir(pwd); system(String("rm -R "+sessiondir).c_str()); return EX;}
#endif
		static int conv_Action(int argc, char** chars) {
			FlagConfig PC;
			AddFlag(PC, "renew", false);
			AddFlag(PC, "ovr", false);
			auto PA{ ParseArg(argc,chars,PC) };
			auto alwaysrenew{ PA.bool_flags["renew"] };
			if (PA.arguments.size() != 3) { QCol->Error("Invalid number of parameters"); return 30; }
			auto CMD{ PA.arguments[0] }, SRC{ PA.arguments[1] }, TAR{ PA.arguments[2] }, SEX{ ExtractExt(SRC) }, TEX{ ExtractExt(TAR) };
			CreatePackerFile(alwaysrenew);
			if (FileExists(TAR)) {
				if (!PA.bool_flags["ovr"]) {
					QCol->Error("Target already exists! (Use switch -ovr to overwrite it"); return 31;
				}
				if (!FileDelete(TAR)) { QCol->Error("Deletion of target " + TAR + " failed"); return 32; }
			}
			if (ExtractDir(SRC) == "" || ExtractDir(SRC) == ".") SRC = CurrentDir() + "/" + SRC;
			if (ExtractDir(TAR) == "" || ExtractDir(TAR) == ".") TAR = CurrentDir() + "/" + TAR;
			QCol->Doing("Source", SRC);
			QCol->Doing("Target", TAR);
			QCol->Yellow("Converting from "); QCol->Cyan(SEX); QCol->Yellow(" to "); QCol->Cyan(TEX); QCol->Yellow("!\n\n\n");
			auto sessionroot{ Ask(CVD,"*SYS","sessionroot","I need a directory to store temporary files. Where can I do that? ",Dirry("$AppSupport$/Jalondi_Sessions")) };
			if (!DirectoryExists(sessionroot)) { QCol->Doing("Creating", sessionroot); MakeDir(sessionroot); }
			auto session{ md5(__DATE__ __TIME__ " Jalondi " + SRC + " " + TAR + " " + CurrentDate() + " " + CurrentTime()) };
			auto sessiondir{ ChReplace(sessionroot, '\\', '/') + "/" + session };
			auto pwd{ CurrentDir() };
			// Create session
			QCol->Doing("Start", "Session " + session);
			if (DirectoryExists(sessiondir)) { QCol->Error("Session conflict!"); return 33; }
			MakeDir(sessiondir);
			ChangeDir(sessiondir);

			// Unpack source
			QCol->Doing("Unpacking", SRC);
			if (CVD->BoolValue(SEX, "UNPACK.INT")) {
				auto vsarg{ Split("INTERNAL::JALONDI "+CVD->Value(SEX,"UNPACK.ARG"),' ')};
				auto cmd{ Lower((*vsarg)[1]) };
				if (cmd == "convert") DSRE("Playing games with me, huh!",500); 
				for (size_t i = 2; i < vsarg->size(); ++i) {
					if ((*vsarg)[i] == "$Package$") (*vsarg)[i] = SRC;
				}
				auto carg{ UCSA(vsarg) };
				if (!J_Action::Register.count(cmd)) DSRE("Internal command " + cmd + " not found!", 35);
				auto ret{ J_Action::Register[cmd].Func(carg->size(), carg->str) };
				if (ret) DSRE(TrSPrintF("Error in unpacking (%d)", ret), ret);
			} else if (CVD->BoolValue(SEX, "UNPACK.AVAILABLE")) {
				auto cmd{ CVD->Value(SEX,"UNPACK.EXE") + " " };				
#ifdef SlyvWindows
				auto src{ ChReplace(SRC, '/', '\\') };
#else
				auto src = ChReplace(SRC, '\\', '/');
#endif
				cmd += StReplace(StReplace(CVD->Value(SEX, "UNPACK.ARG"), "$q$", "\""), "$Package$", "\"" + src + "\"");
				QCol->Doing("Executing", cmd);
				QCol->Reset();

				auto ret{ system(cmd.c_str()) };
				if (ret) DSRE(TrSPrintF("Error in unpacking (%d)", ret), ret);
			} else {
				DSRE("No unpacker available for " + SEX + " format!", 36);
			}
				//pause(); //debug

			// Pack target
			QCol->Doing("Packing", TAR);
			if (CVD->BoolValue(TEX, "PACK.INT")) {
				auto vsarg{ Split("INTERNAL::JALONDI " + CVD->Value(TEX,"PACK.ARG"),' ') };
				auto cmd{ Lower((*vsarg)[1]) };
				if (cmd == "convert") DSRE("Playing games with me, huh!", 500);
				for (size_t i = 2; i < vsarg->size(); ++i) {
					if ((*vsarg)[i] == "$Package$") (*vsarg)[i] = TAR;
				}
				auto carg{ UCSA(vsarg) };
				if (!J_Action::Register.count(cmd)) DSRE("Internal command " + cmd + " not found!", 35);
				auto ret{ J_Action::Register[cmd].Func(carg->size(), carg->str) };
				if (ret) DSRE(TrSPrintF("Error in packing (%d)", ret), ret);
			} else if (CVD->BoolValue(TEX, "PACK.AVAILABLE")) {
				auto cmd{ CVD->Value(TEX,"PACK.EXE") + " " };
#ifdef SlyvWindows
				auto tar{ ChReplace(TAR, '/', '\\') };
#else
				auto tar = ChReplace(TAR, '\\', '/');
#endif
				cmd += StReplace(StReplace(CVD->Value(TEX, "PACK.ARG"), "$q$", "\""), "$Package$", "\"" + tar + "\"");
				QCol->Doing("Executing", cmd);
				QCol->Reset();

				auto ret{ system(cmd.c_str()) };
				if (ret) DSRE(TrSPrintF("Error in unpacking (%d)", ret), ret);
			} else {
				DSRE("No packer available for " + TEX + " format!", 36);
			}
			// pause();
			
			// End session
			QCol->Doing("Kill", "Session " + session); 
			ChangeDir(pwd); 
#ifdef SlyvWindows
			auto e{ system(String("cmd /c rd /s /q " + ChReplace(sessiondir,'/','\\')).c_str()) };
#else
			auto e{ system(String("rm -R " + sessiondir).c_str()) };
#endif
			if (e) { QCol->Error(TrSPrintF("Error in session kill (%d)", e)); return 34; }
		}
#pragma endregion

#pragma region "Jalondi_Convert.cpp initiation"
		void Jal_Jalondi_Convert() {
			SJB("Jalondi_convert.cpp");
			J_Action J{ "convert",conv_Action,conv_Explain,"Converts an archive/resource file from one format to another" };
		}
#pragma endregion
	}
}