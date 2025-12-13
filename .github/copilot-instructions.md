# CppTD - Weather Station Project Guide for AI Agents

## Architecture Overview

**Three-layer architecture:**
- **Data Layer** (`Measurement` class): Encapsulates a single weather reading with 5 properties (temperature, humidity, windSpeed, windDirection, dateTime). Implements serialization via pipe-delimited format for file persistence.
- **Storage Layer** (`WeatherStation` class): Manages a vector of Measurements, handles CRUD operations, and file I/O (`loadFromFile`, `saveToFile`).
- **Analytics Layer** (`Analyzer` class): Static utility methods for statistical analysis over measurement collections (averages, min/max, trend detection).

Data flow: User input → main.cpp menu → WeatherStation operations → Analyzer for statistics → File persistence.

## Key Design Patterns

**Serialization Pattern:** Measurement uses pipe-delimited format (`temp|humidity|windSpeed|direction|dateTime`) for file storage. The `serialize()`/`deserialize()` static pair enables roundtrip persistence. File format lines are case-sensitive; empty lines are skipped on load.

**Static Analysis Pattern:** `Analyzer` uses only static methods—intentionally stateless for functional analysis. All methods require the measurement vector passed as parameter.

**Safe Indexing:** `removeMeasurement()` validates bounds before erasing; always returns bool success indicator.

## Development Workflow

**Build & Compilation:**
- No CMake/Makefile exists; use compiler directly with relative include paths: `g++ -I./include src/*.cpp -o weather`
- Build artifacts go to `build/` directory (not tracked in git)
- Headers in `include/`, implementations in `src/`

**Data Persistence:**
- Measurements load from `data/measurements.txt` at startup
- File format: one measurement per line, pipe-delimited, comments start with `#`
- Auto-save on user request; graceful failure if file inaccessible

**Code Comments:** Predominantly French (class docs, variable names, ui strings). Maintain language consistency when adding features.

## File-Specific Patterns

**[main.cpp](src/main.cpp):** Interactive menu-driven CLI. Helper functions handle repeated I/O patterns (e.g., `addMeasurementInteractive`, `displayMenu`). Gets current time via `getCurrentDateTime()`.

**[Measurement.cpp](src/Measurement.cpp):** Deserializer handles malformed lines gracefully (returns default Measurement). Precision: 2 decimals for serialization, 1 for display.

**[WeatherStation.cpp](src/WeatherStation.cpp):** Empty constructor; vector grows with `push_back`. Load reports count of successful deserialized lines.

**[Analyzer.cpp](src/Analyzer.cpp):** Temperature trend detection needs ≥3 measurements (last 3). Empty vector checks return 0.0f.

## Common Extensions

When adding features, follow these patterns:
- Add getter/setter pairs to `Measurement` if introducing new data fields
- Extend `Analyzer` static methods for new statistics
- Use `WeatherStation::getMeasurements()` const reference for iteration
- Maintain file format compatibility; increment version comment if format changes
- Validate user input range before passing to setters (e.g., humidity 0-100%)

