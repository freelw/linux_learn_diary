import linecache,sys

start, end = int(sys.argv[1]), int(sys.argv[2])
index = start
total = {}
total["bandwidth"], total["jeffier"], total["lost"], total["packet"] = 0, 0, 0, 0
while index <= end:
    filename = "server_%d" % index
    line = linecache.getline(filename, 7).strip()
    arrays = line.split()
    res = {}
    # ['[', '5]', '0.00-5.00', 'sec', '627', 'KBytes', '1.03', 'Mbits/sec', '0.012', 'ms', '0/10039', '(0%)']
    print (arrays)
    res["bandwidth"] = float(arrays[-6])
    res["jeffier"] = float(arrays[-4])
    res["lost"] = float(arrays[-1].split("%")[0].split("(")[1])
    res["packet"] = float(arrays[-2].split("/")[1])
    index = index + 1
    total["bandwidth"] = res["bandwidth"] + total["bandwidth"]
    total["jeffier"] = res["jeffier"] + total["jeffier"]
    total["lost"] = res["lost"] + total["lost"]
    total["packet"] = res["packet"] + total["packet"]
    print(res)

print("---------------------------")
#total["bandwidth"] = total["bandwidth"] / (end - start + 1) * 1.0
total["jeffier"] = total["jeffier"] / (end - start + 1) * 1.0
total["lost"] = total["lost"] / (end - start + 1) * 1.0
total["packet"] = total["packet"] / 60
print(total)