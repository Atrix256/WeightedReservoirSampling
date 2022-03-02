import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

filenames=[
    "UniformReservoirSampling",
    "WeightedReservoirSampling",
    "SubsetUniformReservoirSampling",
    "SubsetWeightedReservoirSampling"]

for filename in filenames:
    fig, (ax1, ax2, ax3) = plt.subplots(1, 3)
    fig.suptitle(filename + " Histogram")
    df1 = pd.read_csv("./out/" + filename + "_1000.csv")
    df2 = pd.read_csv("./out/" + filename + "_10000.csv")
    df3 = pd.read_csv("./out/" + filename + "_100000.csv")
    df1 = df1.set_index('Value')
    df2 = df2.set_index('Value')
    df3 = df3.set_index('Value')
    ax1.plot(df1)
    ax2.plot(df2)
    ax3.plot(df3)
    ax1.set_ylim(bottom=0)
    ax2.set_ylim(bottom=0)
    ax3.set_ylim(bottom=0)
    ax1.title.set_text("1000 tests")
    ax2.title.set_text("10000 tests")
    ax3.title.set_text("100000 tests")
    fig.tight_layout()
    plt.savefig("./out/" + filename + ".png", bbox_inches='tight')
