*This project has been created as part of the 42 curriculum by amtan.*

# Philosophers

A concurrency project based on the classic **Dining Philosophers** problem. I implemented a simulation where philosophers alternate between thinking, eating, and sleeping while sharing limited forks, with strict timing rules and synchronized output.

## Highlights

* **C concurrency:** pthreads/mutexes  and processes/semaphores 
* **Deadlock avoidance + starvation mitigation** under strict time constraints
* **Consistent timing:** start gate + jitter/stagger to reduce contention spikes
* **Quality checks:** Valgrind Memcheck, Helgrind/DRD 

## Description

This repository contains two implementations of the Dining Philosophers simulation:

* `philo/` — **pthreads + mutexes**
* `philo_bonus/` — **processes + POSIX semaphores**

The goal is to satisfy strict timing rules while avoiding deadlocks and preventing starvation **whenever it’s practically possible** for the chosen timings.

* There are **N philosophers** sitting in a **circle** (philosopher 1 sits next to philosopher N).
* There are **N forks** total, but each philosopher must hold **two forks** to start eating.
* Philosophers **cannot communicate** and **don’t know** when another philosopher is about to die.
* The simulation stops when **someone dies**, or (if provided) when **everyone** has eaten at least `number_of_times_each_philosopher_must_eat` times.
* Mandatory Scenario (`philo/`): forks are **between neighbors** → each philosopher can only take the fork on their **left** and **right**.
* Bonus Scenario (`philo_bonus/`): forks are modeled as a shared pool in the **center of the table** (a counting semaphore).

## Instructions

### Build

Mandatory:

```bash
cd philo
make
```

Bonus:

```bash
cd philo_bonus
make
```

Common targets (both folders):

```bash
make clean
make fclean
make re
```

### Run

Mandatory:

```bash
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
```

Bonus:

```bash
./philo_bonus number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
```

## Usage

### Arguments

All times are in **milliseconds**.

1. `number_of_philosophers` — number of philosophers (and forks)
2. `time_to_die` — max time without starting a meal before death
3. `time_to_eat` — eating duration
4. `time_to_sleep` — sleeping duration
5. *(optional)* `number_of_times_each_philosopher_must_eat` — when provided, the simulation stops once **every** philosopher has eaten at least this many times

### Stop conditions

* **Death:** the moment a philosopher exceeds `time_to_die` without starting to eat.
* **Meal limit (optional 5th arg):** when all philosophers reach the required meal count.

### Output format

Each line is printed as:

`timestamp_ms philosopher_id message`

* `timestamp_ms` is **milliseconds since the simulation start**
* `philosopher_id` is in **[1..N]**

Example (last few lines of a run — your output will vary):

```text
480 3 is eating
680 3 is sleeping
681 2 has taken a fork
682 2 has taken a fork
682 2 is eating
831 1 is thinking
840 4 died
```

### Exit status

Returns **non-zero** on invalid arguments and on unrecoverable failures (e.g., allocation errors).

## Examples

Use the same arguments for either target—just swap `./philo` ↔ `./philo_bonus`.

### 1) Ends by death (4 args)

```bash
./philo 2 150 200 50
```

```bash
./philo_bonus 2 150 200 50
```

### 2) Stops after everyone eats enough times (5 args)

```bash
./philo 4 700 150 150 5
```

```bash
./philo_bonus 4 700 150 150 5
```

### 3) Intended to run indefinitely

```bash
./philo 5 800 200 200
```

```bash
./philo_bonus 5 800 200 200
```

> Note: “Runs indefinitely” depends on real-world factors like CPU load, OS scheduling, timer precision, and how strict the timings are. On a busy machine or with tighter timings, a death may still occur.

## Design choices

### Mandatory (`philo/`): threads + mutexes

* **Deadlock avoidance (odd/even fork order):** philosophers lock forks in a different order depending on their id (e.g., odd picks left then right; even picks right then left) to break circular wait.
* **Start gate (synchronized start):** philosopher threads block until all threads are created and a single `start_ms` is set, so timing and logs share the same “time zero.”
* **Monitor loop:** a dedicated monitor checks each philosopher’s `last_meal` time to detect death and stop the simulation cleanly.
* **Serialized printing:** a print mutex prevents interleaved logs so each event appears as one coherent line.
* **Jitter / staggered timing:** a tiny start delay (and optional think jitter) desynchronizes fork acquisition to reduce contention spikes and stabilize tight timing edge-cases.

- **Precision sleep with early-exit:** custom `ms_sleep()` sleeps in small chunks for accuracy and checks the stop flag so threads terminate quickly.
- **Monitor idle sleep (deadline-aware):** the monitor sleeps briefly between checks instead of busy-spinning to keep CPU usage low.
- **Edge-case handling (N = 1):** takes one fork, prints once, and waits until death (the only possible outcome with a single fork).
- **Robust logging:** builds log lines manually and retries `write()` if interrupted; suppresses prints after the simulation stops.

### Bonus (`philo_bonus/`): processes + semaphores

* **One process per philosopher:** each philosopher runs in its own process.
* **Start gate (synchronized start):** children block on a start semaphore; the parent sets `start_ms` and releases everyone together for consistent timing.
* **Forks as a counting semaphore:** a semaphore represents the total number of forks; eating requires successfully decrementing it twice.
* **Room semaphore (N-1 gate):** limits how many philosophers may try to acquire forks at once (typically `N-1`), preventing the “everyone holds one fork” deadlock scenario.
* **Monitor thread per child:** inside each philosopher process, a lightweight thread monitors timing to detect death while the process is blocked/sleeping.
* **End gate for logging:** once a death is printed, an end semaphore prevents any further processes from printing additional lines.

- **Jitter / staggered timing:** small per-philosopher offsets reduce thundering-herd contention on semaphores.
- **Per-philosopher meal semaphore:** protects `last_meal_ms` / `meals_eaten` between the main loop and the monitor thread; children close other philosophers’ meal semaphores after `fork()` to reduce inherited handles.
- **Clean semaphore lifecycle:** named semaphores are unlinked before creation to avoid stale `/dev/shm` state; semaphore waits retry on `EINTR`.
- **Supervisor strategy:** the parent waits on child exit codes and terminates remaining children on death/error (SIGTERM, then SIGKILL) to guarantee cleanup.

## Verification

After building, I verified **memory safety** with Valgrind **Memcheck** (both mandatory and bonus) and checked for **thread races** with **Helgrind/DRD** (mandatory only). The commands below use a small philosopher count to keep reports readable.

Memory checks (both mandatory and bonus):

```bash
valgrind --leak-check=full --show-leak-kinds=all ./philo 3 600 120 120 3
valgrind --leak-check=full --show-leak-kinds=all ./philo_bonus 3 600 120 120 3
```

Thread-race checks (mandatory only):

```bash
valgrind --tool=helgrind ./philo 3 600 120 120 3
valgrind --tool=drd ./philo 3 600 120 120 3
```

## Resources

* The Open Group Base Specifications (POSIX threads): `pthread_create`, `pthread_join`, `pthread_mutex_*` (mutex locking/unlocking semantics)
* Linux manual pages for POSIX semaphores: `sem_overview`, `sem_open`, `sem_wait`, `sem_post`, `sem_unlink`
* Valgrind documentation: Memcheck, Helgrind, and DRD manuals
* Dining Philosophers problem (classic CS concurrency reference)
* AI usage: ChatGPT was used only for learning purposes to help explain concepts, give debugging ideas, plan tests, etc.
