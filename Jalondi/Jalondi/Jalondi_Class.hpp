// Lic:
// Jalondi
// Class (header)
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
#pragma once
#include <Slyvina.hpp>
#include <SlyvString.hpp>


namespace Slyvina {
	namespace Jalondi {

		extern std::string Jalondi_Exe;

		typedef int(*J_Action_Function)(int argc, char** args);
		typedef void(*J_Action_Explanation)();
		
		class J_Action;
		class J_Build;


		class J_Build {
		private:
			static std::map<std::string, J_Build> _Register;
			std::string _Date{ "" }, _Time{ "" };
		public:
			J_Build();
			J_Build(std::string Source, std::string D, std::string T);
			inline std::string Date() { return _Date; }
			inline std::string Time() { return _Time; }
			static void Show();
		};
#define SJB(Source) static J_Build _JB{Source,__DATE__,__TIME__}


		class J_Action {
		private:
		public:
			static std::map<std::string, J_Action> Register; 
			J_Action_Function Func{ nullptr };
			J_Action_Explanation Explain{ nullptr };
			std::string QuickExplanation;
			J_Action();
			J_Action(std::string cmd, J_Action_Function F, J_Action_Explanation E,std::string QE);
		};

		inline String ST(String A, size_t T) {
			auto ret = A; for (auto i = A.size(); i < T; i++) ret += " ";
			return Units::Left(ret, T);
		}
	}
}