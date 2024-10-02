#pragma once
#include <Slyvina.hpp>


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
			inline J_Build() {};
			J_Build(std::string Source, std::string D, std::string T);
			inline std::string Date() { return _Date; }
			inline std::string Time() { return _Time; }
			static void Show();
		};
#define SJB(Source) static J_Build _JB{Source,__DATE__,__TIME__};


		class J_Action {
		private:
		public:
			static std::map<std::string, J_Action> Register; 
			J_Action_Function Func{ nullptr };
			J_Action_Explanation Explain{ nullptr };
			std::string QuickExplanation;
			inline J_Action() {} // just has to exist for some idiotic reason.			
			J_Action(std::string cmd, J_Action_Function F, J_Action_Explanation E,std::string QE);
		};
	}
}