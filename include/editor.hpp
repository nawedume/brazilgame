#pragma once

#include <core.hpp>
#include <level.hpp>

namespace game {
	enum EState {
		PLACING,
		ASSIGN_FUNC,
		WRITING,
		OPEN
	};

	struct EditorContext {
		Level* level;
		u8 entId = 2;
		u8 chuFunc;
		EState state;
	};

	void EditorController(EditorContext& context);

	Level* ReadLevel(string const& path); 
};
