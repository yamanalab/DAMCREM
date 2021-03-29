# encoding: UTF-8

import pylab
import sys
import numpy as np

data = {}
with open(sys.argv[1], "r") as f:
    xx = [d.strip().split(",") for d in f.readlines()]
    for x in xx:
        data[x[0]] = [float(y) for y in x[1:]]
        pass
    pass

# alloc_method_list = ["bits2020"]
sched_method_list = [
    # "fifo_all_path", "deadline_critical_path", "deadline_all_path",
    # "fifo_fifo_all_path", "deadline_deadline_critical_path", "deadline_deadline_all_path"

    "fifo_all_path", "deadline_critical_path", "deadline_all_path",
    "fifo_fifo_all_path", "fifo_deadline_critical_path", "fifo_deadline_all_path",
    "deadline_fifo_critical_path", "deadline_fifo_all_path",
    "deadline_deadline_critical_path", "deadline_deadline_all_path"
]
sched_method_name_list =[
    # "1-1", "1-2", "1-3",
    # "2-1", "2-2", "2-3",

    "1A", "1B", "1C",
    "2AA", "2AB", "2AC",
    "2BA", "2CA",
    "2BB", "2CC",
]

# dt1_list = [200000, 240000, 280000, 320000, 360000]
# dt2_list = [320000, 360000, 400000, 440000, 480000]
# dt1_list = [200000, 280000, 360000]
dt1_list = [360000, 280000, 200000]
dt2_list = [320000, 400000, 480000]

trial_list = ["{}_{}".format(i, i+1) for i in range(0, 10, 2)]

y_limits = [
    2000, 5000, 5000, 4000, 10000, 10000
]

def draw(ax, i_dt1, i_dt2, i):
    key = "result/deim2021/bits2020_{}_{}_{}_0.7_{}.txt"
    values = [
        [data[key.format(sched_method, dt1_list[i_dt1], dt2_list[i_dt2], t)][i] for t in trial_list]
        for sched_method in sched_method_list
    ]
    print(i_dt1, i_dt2, i)
    print(values)
    print(np.mean(values, axis=1))
    print()
    ax[i_dt1][i_dt2].boxplot(values)
    # ax[i_dt1][i_dt2].set_xticklabels(sched_method_name_list)
    ax[i_dt1][i_dt2].set_xticklabels(sched_method_name_list, rotation=90)
    ax[i_dt1][i_dt2].tick_params(labelsize=12)
    ax[i_dt1][i_dt2].set_ylim(0, y_limits[i])
    ax[i_dt1][i_dt2].yaxis.set_ticks(np.linspace(0, y_limits[i], 6))
    if i_dt1 == len(dt1_list)-1:
        # ax[i_dt1][i_dt2].set_xlabel("{}[ms]".format(dt2_list[i_dt2]//1000), fontsize=18)
        ax[i_dt1][i_dt2].set_xlabel("${dt}_{nn}="+"{}".format(dt2_list[i_dt2]//1000)+"\mathrm{[ms]}$", fontsize=18)
    if i_dt2 == 0:
        # ax[i_dt1][i_dt2].set_ylabel("{}[ms]".format(dt1_list[i_dt1]//1000), fontsize=18)
        ax[i_dt1][i_dt2].set_ylabel("${dt}_{pa}="+"{}".format(dt1_list[i_dt1]//1000)+"\mathrm{[ms]}$", fontsize=18)
    return


for i_metric in range(6):
    fig, ax = pylab.subplots(len(dt1_list), len(dt2_list), figsize=(3*len(dt1_list), 3*len(dt2_list)))
    # pylab.subplots_adjust(left=0.1, right=0.995, bottom=0.08, top=0.99, wspace=0.4, hspace=0.2)
    pylab.subplots_adjust(left=0.11, right=0.995, bottom=0.08, top=0.99, wspace=0.35, hspace=0.2)
    for i_dt1 in range(len(dt1_list)):
        for i_dt2 in range(len(dt2_list)):
            draw(ax, i_dt1, i_dt2, i_metric)
            pass
        pass
    # pylab.savefig("result/deim2021/graph_{}.svg".format(i_metric))
    pylab.savefig("result/deim2021/graph2_{}.svg".format(i_metric))
    pylab.clf()
    pass



