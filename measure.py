#!/usr/bin/python
import subprocess
from pylab import *
from string import *
from statistics import median

algorithms = (1, 2, 3, 4)
start_number = 100
end_number = 10000
steps = 10
path = '.'
command = path + '/subsequence'

numbers = linspace(start_number, end_number, steps)

times = {}
for algorithm in algorithms:
    elapsed = []
    for n in numbers:
        args = ['-n', '{:d}'.format(int(n)),
                '-a', '{:d}'.format(algorithm)]
        print(' '.join([command] + args))
        output = subprocess.run([command] + args, check=True, shell=False,
                                stdout=subprocess.PIPE)
        output = output.stdout.decode()
        print(output)
        lines = output.split('\n')
        line = lines[-2]
        line = line.split(': ')
        elapsed.append(float(line[1]))
    times[algorithm] = elapsed
figure()
maxima = []
for algorithm in algorithms:
    plot(numbers, times[algorithm])
    maxima.append(times[algorithm][-1])
axis(xmin=start_number, xmax=end_number, ymin=0, ymax=median(maxima))
xlabel('number of items')
ylabel('elapsed time (microseconds)')
legend(times.keys())
show()

