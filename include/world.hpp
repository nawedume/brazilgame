#pragma once

#include <core.hpp>
#include <glm/glm.hpp>
#include <variant>

namespace game {
	struct Grid; // forward declearation
	struct Chu;
	struct Player;
	struct Scenery;
	struct Grass;
	typedef std::variant<Player, Chu, Scenery, Grass> CellRef;

	struct Player {
		ivec2 Dir;
	};

	typedef void (*ControllerFn)(ivec2 pos, Grid* grid, u32 step);

	extern ivec2 CommonAttack(ivec2 pos, Grid* grid, u32 step);

	extern void CommonRotate(ivec2 const axis[4], ivec2 pos, Grid* grid, u32 step);

	struct Chu {
		ivec2 Dir;
		ControllerFn Func = nullptr;
	};

	enum ScenearyType {
		TREE
	};

	struct Scenery {
		ScenearyType Type;
	};

	struct Grass {

	};



	struct Grid {
		u8 Height;
		u8 Width;
		std::vector<CellRef> Data;

		CellRef* get(u8 row, u8 col) {
			return &Data[(row * Width) + col];
		}

		void set(u8 row, u8 col, CellRef ref) {
			Data[(row * Width) + col] = ref;
		}

		void remove(u8 row, u8 col) {
			Data[(row * Width) + col] = (Grass) { };
		}

		Grid(u8 height, u8 width) {
			Height = height;
			Width = width;
			Data.resize(Height * Width);

			Grass scenery = { };
			CellRef ref = scenery;
			for (int i = 0; i < Width * Height; i++) {
				Data[i] = ref;
			}
		}
	};

	extern bool isInBounds(ivec2 position, game::Grid* grid);
}
