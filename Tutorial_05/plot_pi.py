import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("pi_results.csv")

# Graph 1: Number of points vs estimated Pi
plt.figure(figsize=(8, 5))
plt.semilogx(df["Points"], df["EstimatedPi"], marker="o")
plt.axhline(3.141592653589793, linestyle="--", label="Actual pi")
plt.xlabel("Number of points")
plt.ylabel("Estimated pi")
plt.title("Monte Carlo: Number of Points vs Estimated Pi")
plt.legend()
plt.grid(True)
plt.savefig("graph1_pi_estimate.png", dpi=150, bbox_inches="tight")
plt.show()

# Graph 2 is Number of points vs absolute error
plt.figure(figsize=(8, 5))
plt.loglog(df["Points"], df["AbsoluteError"], marker="o")
plt.xlabel("Number of points")
plt.ylabel("Absolute error")
plt.title("Monte Carlo: Sample Size vs Error")
plt.grid(True)
plt.savefig("graph2_pi_error.png", dpi=150, bbox_inches="tight")
plt.show()

print("Both graphs saved successfully.")
