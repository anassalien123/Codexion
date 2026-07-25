<img width="1408" height="768" alt="Gemini_Generated_Image_ib2fb8ib2fb8ib2f" src="https://github.com/user-attachments/assets/bbbe9461-6455-402c-8281-92ec3c80e8c1" />


*This project has been created as part of the 42 curriculum by ancheab.*

# Codexion

## Description

Codexion is a C implementation of the classic **Dining Philosophers** problem, reframed around a team of "coders" who need to share a limited pool of "dongles" (hardware license keys) to compile their code. It is a study in multithreaded synchronization: N coders run concurrently as POSIX threads, each repeatedly acquiring two shared dongles, compiling, debugging, and refactoring, while a dedicated monitor thread watches for coders who "burn out" (fail to start a compile in time).

The goal of the project is to implement a correct, deadlock-free, starvation-free concurrent simulation using only low-level threading primitives (`pthread_mutex_t`), without relying on higher-level concurrency libraries, while also supporting two different request-scheduling strategies (FIFO and EDF).

Each coder repeats the following cycle until it has completed the required number of compiles or the simulation is stopped:

1. Request its left and right dongle.
2. Compile (holds both dongles).
3. Release both dongles.
4. Debug.
5. Refactor.

A background monitor thread continuously checks whether any coder has gone too long without starting a compile (burnout) and, if so, stops the simulation cleanly.

## Instructions

### Compilation

```bash
make        # builds the "codexion" binary
make clean  # removes object files
make fclean # removes object files and the binary
make re     # fclean + all
```

The project is built with `cc -Wall -Wextra -Werror -pthread`, so it compiles warning-free and links against pthreads.

### Execution

```bash
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

| Argument | Description |
|---|---|
| `number_of_coders` | Number of concurrent coder threads (and dongles) |
| `time_to_burnout` | Max time (ms) a coder can go without starting a compile before burning out |
| `time_to_compile` | Time (ms) spent compiling (dongles held) |
| `time_to_debug` | Time (ms) spent debugging (no dongles held) |
| `time_to_refactor` | Time (ms) spent refactoring (no dongles held) |
| `number_of_compiles_required` | Number of successful compiles before a coder is done |
| `dongle_cooldown` | Minimum time (ms) a dongle must sit idle before it can be reused |
| `scheduler` | Request ordering strategy: `fifo` or `edf` |

Example:

```bash
./codexion 5 800 200 100 100 7 50 fifo
```

Each output line has the format:

```
<timestamp_ms> <coder_id> <status message>
```

where `<timestamp_ms>` is the time elapsed since the simulation started.

### Tests

A test script is provided:

```bash
./tests/test_codexion.sh
```

## Blocking cases handled

- **Deadlock prevention (Coffman's conditions):** the classic deadlock scenario in Dining Philosophers arises when every thread grabs its left resource first and then waits forever for its right one, forming a circular wait. Codexion breaks this by imposing a **total, consistent ordering on dongle acquisition**: each coder compares the memory addresses of its two dongles and always requests the lower-addressed one first, then the higher-addressed one. Since all coders acquire resources in the same global order, a circular wait chain can never form, which eliminates one of Coffman's four necessary conditions for deadlock (circular wait) by construction.
- **Starvation prevention:** access to each dongle is arbitrated through a per-dongle **priority queue (min-heap)** of pending requests rather than a simple lock or "first thread to notice" race. Under FIFO scheduling, requests are served strictly in arrival order (`request_order`), so no coder can be perpetually skipped by later arrivals. Under EDF scheduling, requests are served by nearest deadline (a coder's burnout key), with `order` used only as a tie-breaker, so a coder close to burning out is always prioritized without ties producing indefinite postponement.
- **Cooldown handling:** after a dongle is released, it cannot be re-acquired by anyone (including the same coder) until `dongle_cooldown` ms have passed (`dongle->last_release`). A waiting coder polls this condition and yields the dongle's lock between checks (`usleep(DONGLE_POLL_US)`), so cooldown enforcement never blocks other threads from checking or updating dongle state in the meantime.
- **Precise burnout detection:** a dedicated monitor thread periodically scans every coder's `last_compile_start` timestamp under the shared state lock and compares it against the current time. If a coder that hasn't finished all required compiles exceeds `time_to_burnout` without starting a new compile, the monitor flags it as burned out and raises a global `stop` flag, which every coder and dongle-wait loop checks cooperatively so the whole simulation winds down promptly instead of leaving threads blocked indefinitely.
- **Log serialization:** all status output goes through `print_status`, which holds a single dedicated `print_mutex` for the duration of each `printf`, so lines from different coder threads and the monitor thread are never interleaved or torn. It also checks the `stop` flag before printing, so no output is produced after the simulation has been asked to shut down.

## Thread synchronization mechanisms

Codexion relies exclusively on `pthread_mutex_t` (no condition variables), using several per-purpose mutexes to keep contention low and critical sections small:

- **`t_dongle.mutex` (one per dongle):** guards that dongle's private request heap, its `in_use` flag, and its `last_release` timestamp. All access to a dongle's state — pushing a request, checking readiness, popping the winning request, marking it in use, and releasing it — happens only while holding this lock, which makes the dongle a self-contained, thread-safe resource. Because there is one mutex per dongle rather than one global lock, coders contending for different dongles never block each other.
- **`prog.state_mutex`:** protects the coders' shared mutable state (`compile_count`, `last_compile_start`), which is written by a coder's own thread and read by the monitor thread. Without this lock, the monitor could read a half-updated timestamp or the coder could race with itself across two field writes; the mutex makes each read/update atomic from the monitor's point of view.
- **`prog.stop_mutex`:** protects the single global `stop` flag. It is written once (by the monitor, on burnout) and read constantly by every coder thread and every dongle-wait loop (`simulation_stopped`). A dedicated, cheap-to-acquire lock lets every thread check it frequently without contending with the heavier dongle or state locks.
- **`prog.order_mutex`:** protects the monotonically increasing `request_order` counter, guaranteeing that two coders calling `next_request_order` concurrently never receive the same sequence number — this is what makes FIFO ordering (and EDF tie-breaking) well defined.
- **`prog.print_mutex`:** serializes all `printf` calls, as described above.

**Race condition prevention example:** without `order_mutex`, two coders' calls to `prog->request_order++` could interleave at the machine-instruction level and hand out the same order value to both, breaking FIFO fairness and EDF tie-breaking. Wrapping the read-increment-write in the mutex makes the whole operation atomic.

**Thread-safe coder ↔ monitor communication:** the monitor thread never touches a coder's dongles or drives its state machine directly — it only reads `last_compile_start` and `compile_count` through `state_mutex`, and communicates back to every coder purely through the `stop` flag behind `stop_mutex`. This one-way, lock-mediated signal keeps the monitor fully decoupled from coder logic: a coder never has to know it's being watched, and the monitor never has to synchronize with a coder beyond these two flags, avoiding any risk of the two threads deadlocking against each other.

## Resources

- POSIX Threads Programming — Lawrence Livermore National Laboratory tutorial on `pthread_create`, `pthread_mutex_t`, and thread-safe design.
- `man pthread_mutex_lock`, `man pthread_create`, `man pthread_join` — POSIX manual pages.
- E.G. Coffman, M.J. Elphick, A. Shoshani, *"System Deadlocks"*, ACM Computing Surveys, 1971 — the original formulation of the four necessary conditions for deadlock.
- Edsger W. Dijkstra, *"Hierarchical Ordering of Sequential Processes"* — the original Dining Philosophers problem and the resource-ordering solution to deadlock.
- C.L. Liu and James W. Layland, *"Scheduling Algorithms for Multiprogramming in a Hard-Real-Time Environment"*, JACM, 1973 — background on Earliest Deadline First (EDF) scheduling.
- 42 School subject documentation for the Dining Philosophers / concurrency project family.

### AI usage

Claude (Anthropic) was used throughout this project's development:
- Generating the initial project structure and full C implementation (headers, `Makefile`, and all source files) from the assignment specification.
- Debugging several `Makefile` issues (object directory handling, rebuild rules) and a runtime argument-parsing/invocation error.
- Drafting and structuring this README, including summarizing the concurrency mechanisms already implemented in the codebase.

<img width="1408" height="768" alt="Gemini_Generated_Image_bsvqp5bsvqp5bsvq" src="https://github.com/user-attachments/assets/3e43a24c-bc41-4298-a22d-e0d0183cce80" />

All generated code was reviewed and is understood by the author; no part of the assignment's logic was produced without review.
