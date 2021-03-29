# coding: UTF-8

import sys
import os
import numpy as np

# unit is [us].

def generate_dt(mu, M):
    return -mu * np.log(1-np.random.random(M))

def generate(mu, M):
    dt = generate_dt(mu, M)
    result = np.zeros([M], dtype=np.float)
    result[0] = dt[0] + 1000000
    for i in range(1, M):
        result[i] = result[i-1] + dt[i]
    return result

def run(dirname, mu, M, trial):
    filename = os.path.join(dirname, "received_time_M{}_mu{}_{}.txt".format(M, mu, trial))
    result = generate(mu, M)

    if os.path.exists(filename):
        print("{} already exists.".format(filename), file=sys.stderr)
        exit(1)
    with open(filename, "w") as f:
        print("# {}[us], {}, {}".format(mu, M, trial), file=f)
        for t in result:
            print(t, file=f)
    return filename

if __name__ == "__main__":
    argc = len(sys.argv)
    # 単位はus
    dirname = sys.argv[1]
    mu = int(sys.argv[2])
    M = int(sys.argv[3])
    N = int(sys.argv[4])

    for trial in range(N):
        print(run(dirname, mu, M, trial))
        pass
    
