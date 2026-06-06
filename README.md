# QueenBumblebee

**QueenBumblebee** is a modern **UCI chess engine** written in **C++23**.
It focuses on **correctness**, **clarity**, and a solid foundation for future strength improvements.

> ⚠️ **Platform Notice:**
> QueenBumblebee is currently **developed and tested only on Linux**.

---

## Features

- **UCI-compatible** (works with GUIs like CuteChess)
- **Bitboard-based** board representation with Zobrist hashing
- Full **legal move generation** and **perft testing**
- **Polyglot** opening book support
- Simple **fixed-depth search** (initial implementation)
- Clear **logging** and colored board printing for debugging

---

## Quick Start

### Requirements

- **Linux** system
- C++26-capable compiler (GCC 16)
- CMake ≥ 4.3

---

### Build

```bash
# Automatically create build directory and compile
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
```

- The first command automatically creates the `build/` directory if it doesn’t exist.
- The resulting binary will be located in `build/` (e.g., `build/QueenBumblebee`).

---

### Running in a GUI

**CuteChess**:
- Engines → Manage → Add...
- Select the compiled executable
- Start a match or tournament

---

## UCI Overview

QueenBumblebee speaks the **Universal Chess Interface (UCI)** protocol.

**Common commands** (manual terminal testing):
```
uci
isready
ucinewgame
position startpos
go
stop
quit
```

**Additional commands:**
- `position fen <FEN> [moves <...>]` — set an arbitrary position
- `perft <depth>` — run a perft test from the current position
- `display` — print the board with state info

**Engine-specific options:**
- `option name UseBook type check default true`  
  When enabled, the engine selects the most weighted move from the Polyglot opening book.

---

## Details

### Board
- Bitboards for fast move and attack computations
- Zobrist hashing for transposition/repetition detection
- Full move history for correct make/unmake
- Colored board print for debugging

### Move Generation
- Legal move generation for all pieces
- Check detection and attacked-square queries
- Castling, en passant, promotions

### Search
- Fixed-depth search (initial version)
- Pluggable move generator

### Opening Book
- Polyglot book probing
- Picks the most weighted entry

### Perft
- Fast perft driver for validation and performance measurements

---

## Examples

**Search a move from the starting position:**
```
position startpos
go
```

**Set a specific FEN and apply a few moves:**
```
position fen rnbqkbnr/pp1ppppp/8/2p5/8/4P3/PPPP1PPP/RNBQKBNR w KQkq c6 0 2 moves g1f3 d7d6
go
```

**Run a perft test to depth 4:**
```
position startpos
perft 4
```
