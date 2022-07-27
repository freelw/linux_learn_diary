import sys
intervals = [

    [1,2],
    [4,4],
    [6,10],
    [25,26],
    [28,28],
    [30,34],
    [12,12],
    [14,16],
    [18,21],
    [36,36],
    [38,40],
    [42,45],
]
def hit_cpu(cpuid):
    for item in intervals:
        if cpuid >= item[0] and cpuid <= item[1]:
            return True
    return False
if '__main__' == __name__:

    for line in sys.stdin:
        try:
            arr = line.strip().split()
            cpuid = int(arr[1][2:4])
            if hit_cpu(cpuid):
                print arr[10], arr[11]
        except:
            pass
    