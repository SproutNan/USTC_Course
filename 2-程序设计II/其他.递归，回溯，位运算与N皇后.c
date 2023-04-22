#include <stdio.h>
unsigned int n, full, count = 0;

void Queens(unsigned int row,unsigned int y,unsigned int x1,unsigned int x2) {
	unsigned int now, layout;
	if (row == n + 1) {
		count += 1;
		return ;
	}
	x1 <<= 1;
	x2 >>= 1;
	layout = (~(y | x1 | x2)) & full;
	//这里layout二进制表示如果对应的位为1就代表这一位可以放 
	while (layout) {
		now = layout & (~layout+1);
		layout &= (~now);
		//找到1位能放之后就把这一位关掉
		//while循环枚举了能放的每一位 
		Queens(row + 1, y | now, x1 | now, x2 | now);
	}
}

int main() {
	scanf_s("%d", &n);
	full = (1 << n) - 1;
	Queens(1,0,0,0);
	printf("%d", count);
}
