import os
import click
import subprocess

import numpy as np
import matplotlib.pyplot as plt

from tqdm import tqdm
from utils import loguniform

os.chdir(os.path.dirname(os.path.abspath(__file__)))


@click.group()
def cli():
    """A small command-line interface for the LMECA2170 Project."""


@cli.group()
def benchmark():
    """Benchmark-related commands"""


@benchmark.command()
@click.option("-v", "--version", default=1, help="Version", type=int)
@click.option("-b", "--boundary", nargs=2, default=(2, 6), type=int, help="Benchmark lower & upper boundary")
@click.option("-n", "--number", default=200, type=int, help="Number of points")
def run(version, boundary, number):
    """Run a benchmark"""

    # Detect which version
    minor = 0
    dirs = os.listdir('./Data')
    for f in dirs:
        if f"V{version}" in f:
            tmp = int(f.split("-")[1])
            if tmp > minor:
                minor = tmp

    # Run the benchmark
    with open(f"Data/V{version}-{minor+1}", "w+") as f:
        for i in tqdm(range(number)):
            n = int(loguniform(boundary[0], boundary[1]))
            cmd = "./build/bin/myexec -b " + str(n)
            process = subprocess.Popen(cmd.split(), stdout=subprocess.PIPE)
            output, error = process.communicate()
            if error is not None:
                print(error)
            else:
                exec_time = int(output.decode('UTF-8').split('\n')[0])
                f.write(f"{n} {exec_time}\n")

    click.secho("Benchmark done !", fg="green")


@benchmark.command()
@click.option("-v", "--versions", default=[6], multiple=True, help="Versions to plot")
def plot(versions):
    """Plot the benchmark results"""

    filenames = ["Data/V0-1"]
    filenames.extend([f"Data/V1-{v}" for v in versions])

    colors = [".r", ".g", ".b"]
    fig = plt.figure()
    ax = fig.add_subplot(1, 1, 1)
    for index, filename in enumerate(filenames):
        x = list()
        y = list()
        with open(filename) as f:
            for line in f:
                x.append(int(line.split(" ")[0]))
                y.append(int(line.split(" ")[1])/1000000)
                ax.loglog(x, y, colors[index])
    plt.plot([1e2, 1e6], np.power([1e-4, 1e0], 1)*.9, ls='--', color="#5e81ac")
    plt.plot([1e2, 1e6], np.power([1e-2, 1e2], 2)*3., ls='--', color="#5e81ac")
    plt.grid(True, which="both", ls="dotted")
    plt.show()


if __name__ == "__main__":
    cli()
