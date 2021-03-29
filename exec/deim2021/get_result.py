# encoding: UTF-8


import sys

with open(sys.argv[1], "r") as f:
    raw_data = [d.replace(" ", ",").replace(":", ",").replace("->",",") for d in filter(lambda x: len(x), [y.strip() for y in f.readlines()])]
    pass

i = 0
while not raw_data[i].startswith("JobID,"):
    i += 1
    continue
raw_data = raw_data[i:]

data = [[y.strip() for y in filter(lambda x: len(x), d.split(","))] for d in raw_data]

# for d in data:
#     print(d)
#     pass

data_1app = [x for x in filter(lambda y: y[1]=="1", data)]
data_2app = [x for x in filter(lambda y: y[1]=="2", data)]

# print(data_1app[512])
# print(data_1app[-512])

# print(data_2app[512])
# print(data_2app[-512])

s_jid = max(int(data_1app[511][0]), int(data_2app[511][0]))
e_jid = min(int(data_1app[-512][0]), int(data_2app[-512][0]))

# print(s_jid)
# print(e_jid)

def f(d):
    # result = []
    result = [float(y[4]) for y in filter(lambda x: int(x[0]) > s_jid, d)][:1000]
    # for d in data:
    #     if int(d[0]) <= s_jid:
    #         continue
    #     # if int(d[0]) >= e_jid:
    #     #     continue
    #     if len(result) >= 1000:
    #         break
    #     result.append(float(d[4]))
    #     pass
    result.sort()
    return result

def show(result, name):
    ave = int(sum(result)/len(result)*10000)/10.0
    per = int(result[int(len(result)*0.999)-1]*10000)/10.0
    maxi = int(result[-1]*10000)/10.0
    # print("{},{},{}".format(name, ave, per))
    # print(len(result), result[-2], result[-1], file=sys.stderr)
    # print(int(len(result)*0.999), result[998], result[-1])
    return [ave, per, maxi]

# print(sys.argv[1])
# show(f(data_1app), "app-1")
# show(f(data_2app), "app-2")

# print("{},{},{},{},{}".format(sys.argv[1], *show(f(data_1app), "app-1"), *show(f(data_2app), "app-2")))
print("{},{},{},{},{},{},{}".format(sys.argv[1], *show(f(data_1app), "app-1"), *show(f(data_2app), "app-2")))



