*This project has been created as part of the 42 curriculum by faeljedd.*

# Codexion

## Description

Codexion simulates coders sitting in a circular co-working hub, sharing a limited
pool of USB dongles to compile "quantum code." Each coder repeatedly cycles through
four phases: acquiring two dongles (their left and right neighbor's), compiling,
debugging, and refactoring. There are exactly as many dongles as coders, arranged
so that each dongle sits between two adjacent coders and is shared by both of them.

A coder needs **two** dongles at once to compile, which means every compile attempt
competes directly with both neighbors. If a coder fails to start compiling within
`time_to_burnout` milliseconds of their last compile (or of the simulation start),
they burn out and the simulation stops.

The goal of the project is to implement this resource-sharing scenario correctly
using POSIX threads and mutexes: every coder is a real thread, every dongle is a
shared resource protected against concurrent access, and the whole simulation must
run without deadlocks, starvation, data races, or memory leaks — while
logging every state transition with millisecond precision.

## Instructions

### Compilation

```bash
make
```

This builds the `codexion` binary using `cc` with `-Wall -Wextra -Werror -pthread`.
Other available rules: `clean`, `fclean`, `re`.

### Execution

```bash
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug \
           time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

| Argument                      | Description                                                              |
|--------------------------------|----------------------------------------------------------------------|
| `number_of_coders`             | Number of coders (and number of dongles)                             |
| `time_to_burnout`               | Max time (ms) a coder can go without starting a compile             |
| `time_to_compile`               | Time (ms) spent compiling                                            |
| `time_to_debug`                 | Time (ms) spent debugging                                            |
| `time_to_refactor`              | Time (ms) spent refactoring                                          |
| `number_of_compiles_required`   | Simulation stops once every coder has compiled this many times       |
| `dongle_cooldown`               | Time (ms) a dongle stays unavailable after being released            |
| `scheduler`                     | `fifo` or `edf` — arbitration policy when several coders want a dongle|

Example:
```bash
./codexion 5 800 200 200 200 3 100 edf
```

All arguments are mandatory and must be positive integers (`scheduler` excluded).
Invalid input is rejected with an explanatory error message.

## Resources

- POSIX Threads Programming (LLNL tutorial) — background on `pthread_create`,
  `pthread_mutex_t`, `pthread_cond_t`, and `pthread_cond_timedwait`.
- `man` pages for `pthread_mutex_lock`, `pthread_cond_wait`, `pthread_cond_timedwait`,
  `gettimeofday`, `clock_gettime`.
- General references on the Dining Philosophers problem and Coffman's deadlock
  conditions, used as the conceptual basis for the dongle-acquisition strategy.
- Binary heap / priority queue references, used when implementing the custom
  min-heap (no standard library priority queue is allowed by the subject).

### AI usage

Artificial intelligence was used as a learning and review tool throughout the project.

AI assisted with:

- understanding POSIX thread synchronization primitives
- explaining mutexes, condition variables, and race conditions
- reviewing synchronization architecture
- discussing deadlock prevention strategies

## Blocking cases handled

- **Deadlock prevention (Coffman's conditions)**: a coder only takes a dongle if
  it can take *both* required dongles atomically. If only one of the two is
  available, neither is taken — this removes the classic "hold one, wait for the
  other" hold-and-wait pattern that causes circular-wait deadlocks (the
  Dining Philosophers failure mode), since no coder ever holds a partial set of
  resources while blocked.
- **Starvation prevention**: dongle access is arbitrated through a custom binary
  min-heap acting as a priority queue. Under `fifo`, requests are served in
  arrival order. Under `edf`, the coder with the earliest burnout deadline
  (`last_compile_start + time_to_burnout`) is served first, with `compile_count`
  and then coder `id` used as deterministic tie-breakers so equal deadlines never
  produce ambiguous ordering.
- **Cooldown handling**: each dongle tracks its own `last_released` timestamp.
  A dongle is only considered available once `dongle_cooldown` milliseconds have
  passed since it was last released, checked before every acquisition attempt.
- **Precise burnout detection**: a dedicated monitor thread polls every coder's
  `last_comp_start` at short intervals and stops the simulation as soon as any
  coder exceeds `time_to_burnout`, keeping the reported burnout timestamp within
  the required tolerance of the actual event.
- **Log serialization**: all state-change logs go through `log_state`, which
  takes a single shared `print_mtx` around the timestamp read and the `printf`
  call, guaranteeing that two threads can never interleave partial lines.

## Thread synchronization mechanisms

- **`sim_mtx`**: the global lock guarding all dongle state (`is_available`,
  `last_released`, each dongle's waiter heap) and the shared `stop` flag. Every
  read or write to this shared state — from coder threads requesting/releasing
  dongles and from the monitor checking for a stop condition — happens under
  this single mutex, which is what makes the acquire/release logic and the
  shutdown check safe to reason about as one consistent view of the world.
- **`coder_mtx`** (one per coder): protects a coder's own `last_comp_start` and
  `priority` fields, which are written by the coder's own thread and read by the
  monitor thread for burnout detection — preventing a torn/stale read across
  threads.
- **`print_mtx`**: serializes all `printf` calls in `log_state` so log lines
  from different coder threads and the monitor thread are never interleaved.
- **`sim->cond` (condition variable)**: used with `pthread_cond_timedwait` so a
  coder waiting for a dongle sleeps until either it's woken by a
  `pthread_cond_broadcast` (fired whenever any dongle is released) or its
  cooldown-based deadline passes, instead of busy-polling.
- **Per-dongle state, not per-dongle locking, as the source of truth**: dongle
  possession is represented explicitly by the `is_available` flag under
  `sim_mtx`, rather than by holding a dongle's own mutex for the duration of a
  compile — this avoids the lock-order inversion that arises when a thread
  holding a resource-specific lock also needs to re-acquire the global lock
  (e.g. to check the stop flag) while waiting.
- **Race-condition example prevented**: without `coder_mtx`, the monitor thread
  reading `last_comp_start` could observe a half-written value while a coder
  thread was updating it at the start of a new compile; with `coder_mtx` held
  by both the writer (coder) and the reader (monitor) around that field, the
  read is guaranteed to see a fully-written, consistent value.