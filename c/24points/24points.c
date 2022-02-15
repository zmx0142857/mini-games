#include <stdio.h>
#include <stdbool.h>

const float digits[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
float d[4] = {1, 4, 5, 6};
// 4 / (1 - (5 / 6)) 或 6 / ((5 / 4) - 1)

float add(float x, float y) { return x + y; }
float sub(float x, float y) { return x - y; }
float mul(float x, float y) { return x * y; }
float div(float x, float y) { return x / y; }

typedef float func(float, float);
func *operators[] = {add, sub, mul, div};
func *op[3];

const int size = sizeof(digits) / sizeof(float);
const int count = sizeof(d) / sizeof(float);
const int node_count = sizeof(op) / sizeof(func*);

bool has_solution = false;

char name(int i)
{
	if (op[i] == add)
		return '+';
	if (op[i] == sub)
		return '-';
	if (op[i] == mul)
		return '*';
	if (op[i] == div)
		return '/';
	return '?';
}

bool test(float f)
{
	f -= 24;
	return -1e-5 < f && f < 1e-5;
}

/* 尝试 Catalan(3) = 5 种树形
 *   0         0  0          0   0
 *  / \       /    \        /     \
 * 1   2     1      1      1       1
 *          /        \      \     /
 *         2          2      2   2
 */
bool try_trees()
{
	if (test(op[0](op[1](d[0], d[1]), op[2](d[2], d[3])))) {
		printf("(%.0f %c %.0f) %c (%.0f %c %.0f)\n",
				d[0], name(1), d[1], name(0), d[2], name(2), d[3]);
	} else if (test(op[0](op[1](op[2](d[0], d[1]), d[2]), d[3]))) {
		printf("((%.0f %c %.0f) %c %.0f) %c %.0f\n",
				d[0], name(2), d[1], name(1), d[2], name(0), d[3]);
	} else if (test(op[0](d[0], op[1](d[1], op[2](d[2], d[3]))))) {
		printf("%.0f %c (%.0f %c (%.0f %c %.0f))\n",
				d[0], name(0), d[1], name(1), d[2], name(2), d[3]);
	} else if (test(op[0](op[1](d[0], op[2](d[1], d[2])), d[3]))) {
		printf("(%.0f %c (%.0f %c %.0f)) %c %.0f\n",
				d[0], name(1), d[1], name(2), d[2], name(0), d[3]);
	} else if (test(op[0](d[0], op[1](op[2](d[1], d[2]), d[3])))) {
		printf("%.0f %c ((%.0f %c %.0f) %c %.0f)\n",
				d[0], name(0), d[1], name(2), d[2], name(1), d[3]);
	} else {
		return false;
	}
	has_solution = true;
	return true;
}

// 从数组 operators 取出 node_count-pos 个元素, 计次序, 可重复
// 存入数组 op 中
void calc(int pos)
{
	if (pos == node_count) {
		try_trees();
	} else {
		for (int i = 0; i < count; ++i) {
			op[pos] = operators[i];
			calc(pos+1);
		}
	}
}

// 打印 d 数组
void print()
{
	for (int i = 0; i < count; ++i)
		printf("%.0f ", d[i]);
	putchar('\n');
}

void swap(float *i, float *j)
{
	float tmp = *i;
	*i = *j;
	*j = tmp;
}

// 产生数组 d 的全排列
// 解决 d 中保存的 24 点问题
void permutate(int pos)
{
	if (pos == count) {
		calc(0);
	} else {
		for (int i = pos; i < count; ++i) {
			swap(d+pos, d+i);
			permutate(pos+1);
			swap(d+pos, d+i);
		}
	}
}

// 从数组 digits+lo 中选取 count-pos 个元素, 不计顺序, 可以重复选取
// 按从小到大的次序输出到 d 数组中
void choose(int lo, int pos)
{
	if (pos == count) {
		has_solution = false;
		permutate(0);
		if (!has_solution) {
			printf("[failed] ");
			print();
		}
	} else {
		for (int i = lo; i < size; ++i) {
			d[pos] = digits[i];
			choose(i, pos+1);
		}
	}
}

int main()
{
	//permutate(0);
	choose(0, 0);
	return 0;
}
