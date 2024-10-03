
#pragma once
#include <Slyvina.hpp>


namespace Slyvina {
	namespace Jalondi {
		struct FileToAdd {
			String
				Source{ "" },
				Target{ "" },
				Storage{ "" },
				Block{ "" };
		};
		struct EntryToAlias {
			String
				Source{ "" },
				Alias{ "" };
		};

		void Create_Clear();
	}
}