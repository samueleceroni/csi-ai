import random

def get_function_values(x_range, get_function_value):
	res = []
	for i in x_range:
		res.append((i, get_function_value(i)))
	return res

def main():
	print('started')

	f_x = lambda x : x + 1 + random.uniform(-0.1, 0.1)
	l = 0; r = 10; x_range = range(l, r + 1)
	
	res = get_function_values(x_range, f_x)
	
	for x in res:
		print(str(x[0]) + ': ' + str(x[1]))
	
	print('finish')

if __name__ == '__main__':
    main()