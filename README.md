# DJ Session Management System

A robust, high-performance C++ backend system designed to simulate a professional DJ session environment. This project demonstrates clean architectural patterns, advanced memory management, and efficient data structures for handling real-time audio track processing and playback orchestration.

## 🚀 Key Features

**Service-Oriented Architecture**: Decoupled service layers for Controller, Library, and Mixing Engine operations, ensuring high maintainability and testability.
**LRU Cache Implementation**: Custom Least Recently Used (LRU) caching mechanism to optimize audio track access and minimize disk I/O latency.
**Polymorphic Audio Management**: Extensible track management system supporting multiple formats (MP3, WAV) through a robust inheritance hierarchy.
**Smart Memory Management**: Utilization of custom smart pointer wrappers to ensure zero memory leaks and safe resource handling across complex data structures.
**Automated Build System**: Comprehensive `Makefile` for streamlined compilation, testing, and memory analysis.

## 🏗️ Architecture & Design Patterns

### Service Layer Pattern
The system is divided into specialized services that communicate through well-defined interfaces:
**DJLibraryService**: Manages the persistent music database.
**DJControllerService**: Orthogonal control logic for session manipulation.
**MixingEngineService**: Handles the computational logic of audio mixing.

### Caching Strategy
A custom `LRUCache` resides at the heart of the system, implementing an efficient lookup and replacement strategy. This ensures that frequently accessed tracks remain in memory, providing a significant performance boost during intensive mixing sessions.

### Polymorphism & Object-Oriented Design
Leveraging standard C++ polymorphism, the system treats different audio formats generically at the library level while allowing format-specific logic (e.g., bit-rate specific processing) to reside in derived classes.

## 🛠️ Technologies Used

**Language**: C++11
**Toolchain**: GCC (g++), GNU Make
**Analysis**: Valgrind (Memory Profiling)
**Design Patterns**: Service Layer, Singleton (Configuration Management), Strategy, Polymorphism.

## 🏁 Getting Started

### Prerequisites
- Linux/UNIX environment or WSL.
- `g++` compiler (supporting C++11).
- `make` build tool.

### Installation & Execution
1. Clone the repository.
2. Build the project using the default target:
   ```bash
   make
   ```
3. Run the interactive session manager:
   ```bash
   ./bin/dj_manager -I -A
   ```

### Development & Testing
**Debug Build**: `make debug`
**Memory Analysis**: `make test-leaks` (Run with Valgrind)
**Clean Build Files**: `make clean`

## 👥 Authors

**Hila Zuckerman** - 212735450
**Dana Bernstein** - 325107266

---


*This project was developed as part of the Systems Programming Lab (SPL) course, focusing on advanced C++ concepts and efficient resource management.*
