import csv
import matplotlib.pyplot as plt

sizes = []
blocking = []
nonblocking = []

with open("results/timing_results.csv", newline="") as file:
    reader = csv.DictReader(file)
    for row in reader:
        sizes.append(float(row["Message Size (MB)"]))
        blocking.append(float(row["Blocking Average Time (seconds)"]))
        nonblocking.append(float(row["Non-blocking Average Time (seconds)"]))

plt.figure(figsize=(8, 5))
plt.plot(sizes, blocking, marker="o", label="Blocking")
plt.plot(sizes, nonblocking, marker="o", label="Non-blocking")
plt.xlabel("Message Size (MB)")
plt.ylabel("Average Time (seconds)")
plt.title("MPI Blocking vs Non-blocking Communication")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig("graphs/communication_time.png", dpi=180)
print("Graph saved to graphs/communication_time.png")
