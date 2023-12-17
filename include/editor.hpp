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
		Type entId = Type::SCENERY;
		u8 chuFunc;
		EState state;
		vector<ivec2> recordedInstructions;
		u32 instructionIndex;
	};

	void EditorController(EditorContext& context);

	Level* ReadLevel(string const& path); 
};
