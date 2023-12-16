#pragma once

#include <core.hpp>
#include <glm/glm.hpp>

namespace game {
	struct Grid; // forward declearation
	struct Chu;
	struct Player;
	struct Scenery;
	struct Grass;
	struct NextLevelPortal;
	struct Goat;

	//typedef std::variant<Player, Chu, Scenery, NextLevelPortal, Goat, Grass> CellRef;

	enum Type {
		PLAYER,
		CHU,
		SCENERY,
		NEXTLEVELPORTAL,
		GOAT,
		GRASS
	};

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

	struct Goat {
	};

	struct CellRef {
		Type type;

		union {
			Player player;
			Chu chu;
			Scenery scenery;
			NextLevelPortal nlPortal;
			Goat goat;
			Grass grass;
		};

		CellRef() {
			this->type = Type::GRASS;
			grass = {};
		}

		CellRef(Type type, Player player) {
			this->type = Type::PLAYER;
			this->player = player;
		}

		CellRef(Type type, Chu chu) {
			this->type = Type::CHU;
			this->chu = chu;
		}

		CellRef(Type type, Scenery scenery) {
			this->type = Type::SCENERY;
			this->scenery = scenery;
		}

		CellRef(Type type, NextLevelPortal nlPortal) {
			this->type = Type::NEXTLEVELPORTAL;
			this->nlPortal = nlPortal;
		}

		CellRef(Type type, Goat goat) {
			this->type = Type::GOAT;
			this->goat = goat;
		}

		CellRef(Type type, Grass grass) {
			this->type = Type::GRASS;
			this->grass = grass;
		}

		CellRef(const CellRef& other) {
			type = other.type;
			switch(other.type) {
				case PLAYER: 
					player = other.player;
					break;
				case CHU:
					chu = other.chu;
					break;
				case SCENERY:
					scenery = other.scenery;
					break;
				case NEXTLEVELPORTAL:
					nlPortal = other.nlPortal;
					break;
				case GOAT:
					goat = other.goat;
					break;
				case GRASS:
					grass = other.grass;
					break;
			}
		}
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

		CellRef* get(ivec2 pos) {
			return get(pos.y, pos.x);
		}

		void set(u8 row, u8 col, CellRef ref) {
			Data[(row * Width) + col] = ref;
		}

		void remove(u8 row, u8 col) {
			CellRef ref = CellRef();
			Data[(row * Width) + col] = ref;
		}

		void remove(ivec2 pos) {
			return remove(pos.y, pos.x);
		}

		void move(ivec2 src, ivec2 dst) {
			CellRef ref = *get(src.y, src.x);
			remove(src.y, src.x);
			set(dst.y, dst.x, ref);
		}

		Grid(u8 height, u8 width) {
			Height = height;
			Width = width;
			Data.resize(Height * Width);

			CellRef ref = CellRef();
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
