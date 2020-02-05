import random
import numpy

def get_random_numbers(l, r, n_samples):
	randoms = []
	for i in range(0, n_samples):
		randoms.append(random.uniform(l, r + 1))
	return randoms

def main():
	print('started')

	n_samples = 100000
	l = -0.1; r = 0.1;
	n_intervals = 100;
	intervals, step = numpy.linspace(l, r, num=n_intervals + 1, endpoint=True, retstep=True)
	samples = get_random_numbers(l, r, n_samples);

	samples.sort()
	intervals_count = []

	i_intervals = 1
	curr_count = 0
	for sample in samples:
		if i_intervals >= len(intervals):
			break
		if sample > intervals[i_intervals]:
			intervals_count.append((i_intervals, curr_count))
			curr_count = 0
			i_intervals += 1
		else:
			curr_count += 1

	print('interval number: count')
	for i_count in intervals_count:
		print(str(i_count[0]) + ': ' + str(i_count[1]))

	print('finish')

if __name__ == '__main__':
    main()
