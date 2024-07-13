import subprocess
import time


def measure_execution_time(executable_path, argument, runs=10, warmups=3):
    # Warm-up executions
    for _ in range(warmups):
        subprocess.run([executable_path, argument], check=True)

    # Measure execution times
    execution_times = []
    for _ in range(runs):
        start_time = time.time()
        subprocess.run([executable_path, argument], check=True)
        end_time = time.time()
        execution_times.append(end_time - start_time)

    # Calculate average execution time
    average_time = sum(execution_times) / runs
    return average_time, runs


if __name__ == "__main__":
    executable_path = "./heat"
    argument = "test.dat"
    average_time, runs = measure_execution_time(executable_path, argument)
    print(f"Average execution time over {runs} runs: {average_time:.4f} seconds")
