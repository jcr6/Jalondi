
#include "Jalondi_Create.hpp"
#include "Jalondi_Class.hpp"

namespace Slyvina {
	namespace Jalondi {
		static std::vector<FileToAdd> _FilesToAdd{};

		void Slyvina::Jalondi::Create_Clear() { _FilesToAdd.clear(); }



		void Jal_Jalondi_Create() {
			J_Action JC{ "create",nullptr,nullptr,"Create new JCR file" };
		}
	}
}