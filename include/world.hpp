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
	struct NextLevelPortal;
	typedef std::variant<Player, Chu, Scenery, Grass, NextLevelPortal> CellRef;

	struct Player {
		ivec2 Dir;
	};

	typedef void (*ControllerFn)(ivec2 pos, Grid* grid, u32 step);

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

	struct NextLevelPortal {
	};

	struct Cell {
		ivec2 Pos;
		CellRef ref;
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

		void fillRow(u8 row, CellRef ref) {
			u8 rowIdx = row * Width;
			for (u8 col = 0; col < Height; col++) {
				Data[rowIdx + col] = ref;
			}
		}

		void fillCol(u8 col, CellRef ref) {
			for (u8 row = 0; row < Height; row++) {
				Data[index(row, col)] = ref;
			}
		}

		inline u16 index(u8 row, u8 col) {
			return (row * Width) + col;
		}
	};

	extern bool isInBounds(ivec2 position, game::Grid* grid);
}
