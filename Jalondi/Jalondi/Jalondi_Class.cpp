#include <SlyvQCol.hpp>
#include <SlyvString.hpp>

#include "Jalondi_Class.hpp"

using namespace Slyvina::Units;

namespace Slyvina {
	namespace Jalondi {


		std::string Jalondi_Exe{};


#pragma region J_Build
		const int _JBSTab = 25;

		std::map<std::string, J_Build> J_Build::_Register{};

		J_Build::J_Build(std::string Source, std::string D, std::string T) {
			_Date = D;
			_Time = T;
			_Register[Source] = *this;
		}

		void J_Build::Show() {
			for (auto b : _Register) {
				QCol->Yellow(b.first+" ");
				auto p{ b.first.size() + 1 };
				if (p >= _JBSTab) { p = 0; std::cout << "\n"; }
				for (size_t i = p; i < _JBSTab; ++i) QCol->Magenta(".");
				QCol->Green(" " + b.second.Date() + " ");
				QCol->Cyan(b.second.Time() + "\n");
			}
			QCol->Reset();
		}
#pragma endregion


#pragma region J_Action
		std::map<std::string, J_Action> J_Action::Register{};

		J_Action::J_Action(std::string cmd, J_Action_Function F, J_Action_Explanation E,std::string QE) {
			Func = F;
			Explain = E;
			QuickExplanation = QE;
			Register[Lower(cmd)] = *this;
		}
#pragma endregion


#pragma region version
		static void __Version() {
			J_Build::Show();
		}
		static int __VersionA(int, char**) { __Version(); return 0; }
		static void __VersionA() { __Version(); }
		void Jal_Jalondi_Class() { 
			J_Action __MyVersion{ "version",__VersionA,__VersionA,"Build times of Jalondi" };
		}
#pragma endregion
	}
}

