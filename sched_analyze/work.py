from datetime import datetime

def calc_date(t):
    boot_time = 1666330944.762007849
    """
        date | cat > /dev/null # Make sure programs are in cache
        set -- $(date +%s.%N) $(cat /proc/uptime)
        echo "$1-$2" | bc
    """
    return datetime.fromtimestamp(boot_time + t)

def main():
    input = 'wakeup_trace.log'
    index = 0
    s = 0
    interval = 5
    
    for line in open(input):
        try:
            arr = line.split(':')
            t = int(float(arr[0].split(' ')[-1])*100)
            cindex = t / interval
            t1 = cindex*interval/100.
            if cindex != index:
                print calc_date(t1), ' ', t1, 'per', interval*10, 'ms:', s
                s = 0
                index = cindex
            else:
                s += 1
        except Exception, e:
            # print e
            pass

if '__main__' == __name__:
    main()
