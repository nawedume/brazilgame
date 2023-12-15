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

	enum EntId {
		PLAYER,
		CHU,
		SCENERY,
		NEXTLEVELPORTAL,
		GRASS
	};

	struct EditorContext {
		Level* level;
		EntId entId = SCENERY;
		u8 chuFunc;
		EState state;
	};

	void EditorController(EditorContext& context);

	Level* ReadLevel(string const& path); 
};
