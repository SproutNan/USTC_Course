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
	//����layout�����Ʊ�ʾ�����Ӧ��λΪ1�ʹ�����һλ���Է� 
	while (layout) {
		now = layout & (~layout+1);
		layout &= (~now);
		//�ҵ�1λ�ܷ�֮��Ͱ���һλ�ص�
		//whileѭ��ö�����ܷŵ�ÿһλ 
		Queens(row + 1, y | now, x1 | now, x2 | now);
	}
}

int main() {
	scanf_s("%d", &n);
	full = (1 << n) - 1;
	Queens(1,0,0,0);
	printf("%d", count);
}
