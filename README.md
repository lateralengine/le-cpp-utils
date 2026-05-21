# Lateral Engine commonly used C++ utilities

## Dependencies

If imported as a library using CMake, this library depends on `spdlog` and `nlohmann-json`.

Many of the individual headers, however, can be used as is.

## Modules

| Header                    | Description                                                   |
|---------------------------|---------------------------------------------------------------|
| `consumer.h`              | Thread-safe producer-consumer queue with worker pattern       |
| `debugmutex.h`            | Debug mutex that tracks owning thread in debug builds         |
| `endianness.h`            | Cross-platform endianness detection and byte-swapping         |
| `fileread.h`              | Utility function for reading entire file contents             |
| `finally.h`               | RAII class executing cleanup function on scope exit           |
| `jsonutility.h`           | JSON serialization helpers with nlohmann/json integration     |
| `logging.h`               | Logging macros and spdlog setup functions                     |
| `median.h`                | STL-based median calculation with exception handling          |
| `mtautoconfiguration.h`   | Automatic threaded resource configuration and heartbeat       |
| `pairhash.h`              | Hash function for `std::pair` types in hash containers        |
| `persistentvalue.h`       | Template class for reading/writing values to files            |
| `pimpl.h`                 | Pointer-to-implementation idiom helper macros                 |
| `signal.h`                | Signal handler registration for SIGINT and SIGTERM            |
| `speedmonitor.h`          | Monitors value changes and detects speed threshold crossings  |
| `subcommand.h`            | CLI11-based subcommand framework base class                   |
| `threadinterval.h`        | Periodic callback execution in background thread              |
| `threadsafedata.h`        | Mutex-protected wrapper for thread-safe data access           |
| `threadsafeevent.h`       | One-shot synchronization event for thread coordination        |
| `threadsafequeue.h`       | Thread-safe queue with optional blocking wait operations      |
| `time.h`                  | Time utilities for milliseconds and ISO 8601 timestamps       |
| `uuid_v4.h`               | SIMD-accelerated RFC-4122 UUIDv4 generation and conversion    |
| `valueinfile.h`           | Template class for typed file-based value persistence         |
| `versioncompare.h`        | Version string parsing and lexicographical comparison         |
| `winutils.h`              | Windows version string retrieval (MSVC only)                  |

## License

MIT