#include "20232907 ������ ���� 3 ���� ���.h"

typedef struct longint_with_size {
	unsigned int* num; //���� ���� ��ġ(���� �Ҵ� ���)
	size_t size : sizeof(size_t) - 1; //�����Ҵ� ���� ũ��
	char code : 1; //������ ��ȣ, 0�̸� ���
} lint;
//���� ���� ����� ���� digit�� �տ� ������

lint strtolint(const char* in);
char* linttostr(const lint in);
lint lintcpy(const lint num);
lint add(const lint num1, const lint num2);
lint sub(const lint num1, const lint num2);
lint mul(const lint num1, const lint num2);
lint push(const lint num, size_t len);
char MUX_uint(unsigned int to, unsigned int controller);
unsigned char last_digit_loc(const lint num);
char lintcmp(const lint num1, const lint num2);
void freeset_lint(lint* p);

void main() {
	char* strX = NULL, * strY = NULL, * strZ = NULL;
	lint numX = { 0,0,0 }, numY = { 0,0,0 }, sum = { 0,0,0 }, subt = { 0,0,0 }, mult = { 0,0,0 };

	while (1) {
		//���ڿ� �Է�
		printf("input X: "); //X �Է�
		strX = sgets(70, 10, stdin);
		printf("input Y: "); //Y �Է�
		strY = sgets(70, 10, stdin);
		if (!strX || !strY) break;

		//���ڷ� �ٲٱ�
		numX = strtolint(strX);
		numY = strtolint(strY);
		if (!numX.num || !numY.num) break;
		freeset(&strX);
		freeset(&strY);

		//���� ����
		sum = add(numX, numY);
		subt = sub(numX, numY);
		mult = mul(numX, numY);
		if (!sum.num || !subt.num || !mult.num) break;

		/*
		strX = linttostr(numX);
		strY = linttostr(numY);
		*/
		freeset_lint(&numX);
		freeset_lint(&numY);
		/*
		if (!strX || !strY) break;
		printf("X: ");
		puts(strX);
		printf("Y: ");
		puts(strY);
		freeset(&strX);
		freeset(&strY);*/

		//���ڿ��� �ٲٱ�
		strX = linttostr(sum);
		strY = linttostr(subt);
		strZ = linttostr(mult);
		if (!strX || !strY || !strZ) break;
		freeset_lint(&sum);
		freeset_lint(&subt);
		freeset_lint(&mult);
		//���
		putchar('\n');
		printf("X + Y :  ");
		puts(strX);
		printf("X - Y :  ");
		puts(strY);
		printf("X * Y :  ");
		puts(strZ);
		freeset(&strX);
		freeset(&strY);
		freeset(&strZ);
		putchar('\n');
		putchar('\n');
	}

	//�����Ҵ� ���� Ȯ��(�����Ҵ� ���� �� break;�� �ϱ⿡ ������ ���⼭ �� �־�� ��, freeset �Լ��� �̹� �����Ǿ������� ó�� ���� �ѱ�ٴ� ���� �̿�)
	freeset(&strX);
	freeset(&strY);
	freeset_lint(&numX);
	freeset_lint(&numY);
	freeset_lint(&sum);
	freeset_lint(&subt);
	freeset_lint(&mult);
}

lint strtolint(const char* in) { //���ڸ� ���ڷ� �ٲٴ� �Լ�
	lint res = { 0,0,0 };
	char* stp = NULL; //���ڷ� ��ȯ�� �����ϴ� �κ��� ��Ÿ���� ������
	char nums[10] = { 0, }; //9�ڸ��� ��� ���ڷ� �ٲ� ��, �ٲ� ���ڸ� ������ �� ����
	lint tmp1 = { 0,0,0 }, tmp2 = { 0,0,0 }; //���갪�� �ӽ÷� ������ ����
	lint m10 = { 0,0,0 }; //������ ���� 9�� �ڸ����� ����� ���� ������ ����
	char flag = 0; //�����Ҵ� ���� ���θ� Ȯ���ϴ� ����
	//lint�� int�� ���� �����Ǿ� ���� �ʾ�, int�� lint�� ������־�� ��
	unsigned int n10 = 1000000000; //10�� 9����
	m10.num = &n10;
	m10.size = sizeof(n10);

	tmp1.num = calloc_in(sizeof(unsigned int)); //���ڸ� ���ڷ� �ٲ� �� ���⿡ ������ ����
	if (!tmp1.num) return tmp1;
	tmp1.size = sizeof(unsigned int); //�Ҵ���� ũ�� ����

	if (*in == '-') {
		tmp1.code = 1;
		in++;
	}
	else if (*in == '+') {
		tmp1.code = 0;
		in++;
	}

	//��ȯ
	stp = in;
	strncpy(nums, stp, 9); //���� ����
	*(tmp1.num) = atoi(nums);
	res = lintcpy(tmp1);
	if (!res.num) {
		freeset_lint(&tmp1);
		return res;
	}
	stp += 9; //���ڷ� �ٲ� ��ŭ �ڷ� �ű�

	//���� ���ں��� ����
	while (stp < in + strlen(in)) {
		if (stp + 9 > in + strlen(in)) { //���� ���� �������̰� �̹��� �ڸ����� ���ڶ� ���� ���� 1000000000�� ���ϸ� �� �Ǵ� ���
			for (flag = (stp + 9) - (in + strlen(in)); flag; flag--) {
				n10 /= 10; //�ڸ����� ���߱� ���� 10�� (���� �ڸ���) ������ n10(=m10)�� �ǵ��� ����� ��
			} //������� �ʴ� ���� Ȱ��(������ 0�̾�� ��)
		}

		strncpy(nums, stp, 9); //���� ����
		*(tmp1.num) = atoi(nums);
		tmp2 = mul(m10, res);
		if (!tmp2.num) {
			flag = 1;
			break;
		}
		freeset_lint(&res);
		res = add(tmp1, tmp2);
		if (!res.num) {
			flag = 1;
			break;
		}
		freeset_lint(&tmp2);
		stp += 9; //���ڷ� �ٲ� ��ŭ �ڷ� �ű�
	}
	freeset_lint(&tmp1);
	freeset_lint(&tmp2);
	return res;
}

char* linttostr(const lint in) { //���ڸ� ���ڷ� �ٲٴ� �Լ�
	//�̰��� ���ͳݿ��� ������ �Լ�, ��ó: https://stackoverflow.com/questions/8023414/how-to-convert-a-128-bit-integer-to-a-decimal-ascii-string-in-c
	char* res = NULL; //������� ������ ����
	unsigned long long left = 0;
	lint num = { 0,0,0 };
	size_t pl; //���� ��ġ
	size_t res_len = 0; //res�� �Ҵ�� ���ڿ� ũ��
	char* pos = NULL; //Ŀ��

	num = lintcpy(in); //���ڸ� ���� �ٲ�� �ϹǷ� ���� ����
	if (!num.num) return res;
	res_len = in.size / 2 * 5 + 1;
	res = calloc_in(res_len); //int �迭 ũ���� 2.5��(����Ʈ �������δ� 10��) + 1('\0')�� ���ڿ� �ϴ� ����
	if (!res) {
		freeset_lint(&num);
		return res;
	}
	pos = res + res_len - 2; //�Ҵ���� ���� ������ ���� ������ ����Ŵ(������ ������ '\0' �ʿ�)
	while (1) {
		pl = num.size / sizeof(unsigned int);
		if (!num.num[pl - 1]) { //���� ū digit�� ������ ��� �������
			num.size -= sizeof(unsigned int);
			pl--;
			if (!pl) break; //������ �������� ������� break;
		}
		if (pos < res) { //���Ҵ� �ʿ��(�־��� ���� ���� ���)
			pos = calloc_in(res_len + 10); //10�� ���� �� �ޱ�
			if (!pos) {
				freeset_lint(&num);
				freeset(res);
				return NULL;
			}
			res_len += 10;
			strcpy(pos + 10, res); //���� res���� ����
			freeset(&res); //res �Ҵ� ����
			res = pos; //���� �Ҵ�� ��ġ�� res�� ����
			pos = res + 10 - 1;
		}
		left = num.num[pl - 1];
		while (pl) {
			num.num[pl - 1] = left / 10;
			pl--;
			if (!pl) break;
			left = ((left % 10) << 8 * sizeof(unsigned int)) + num.num[pl - 1];
		}
		left = left % 10;
		*pos-- = '0' + left;
	}
	freeset_lint(&num); //0�� �� num �����Ҵ� ����
	if (in.code) { //num�� �����Ǿ����Ƿ� in���� �˻��ؾ� ��, ������� �����ΰ�� ��ȣ ���̱�
		if (pos < res) { //���Ҵ� �ʿ��(�־��� ���� ���� ���)
			pos = calloc_in(res_len + 1); //10�� ���� �� �ޱ�
			if (!pos) {
				freeset(res);
				return NULL;
			}
			res_len += 1;
			strcpy(pos + 1, res); //���� res���� ����
			freeset(&res); //res �Ҵ� ����
			res = pos; //���� �Ҵ�� ��ġ�� res�� ����
		}
		*pos = '-';
	}
	else pos++;
	//�̷��� pos�� ����Ű�� ��ġ�� ���ڿ� ���� ��ġ
	if (pos != res) strcpy(res, pos); //���ڿ� ���� ��ġ�� res�� ��ġ���� ������ ��ġ�ϵ��� ���ڿ��� �Ű���
	if (!*res || !strcmp(res, "-")) *res = '0'; //0�̸� � ���ڵ� ���� �ʴµ� �̸� ���� ����
	return res;
}

lint lintcpy(const lint num) { //���� ����
	lint res = { 0,0,0 };
	size_t pl = 0;
	res.num = calloc_in(num.size);
	if (!res.num) return res;
	for (pl = 0; pl < num.size / sizeof(unsigned int); pl++)
		res.num[pl] = num.num[pl];
	res.size = num.size;
	res.code = num.code;
	return res;
}

lint add(const lint num1, const lint num2) {
	lint res = { 0,0,0 }; //return���� ������ ����
	unsigned long long tmp = 0; //���갪�� �ӽ� ������ �Լ�
	size_t pl = 0; //���� ��ġ
	//unsigned int* ptmp = NULL; //realloc�� realloc_in�� return���� �ӽ� ������ ����

	if (num1.code ^ num2.code) { //���� ��ȣ�� ���� �ٸ���(���� ���꿡 �ش�)
		if (num2.code) {
			res = num2; //���� ����
			res.code = 0;
			return sub(num1, res);
		}
		else {
			res = num1; //���� ����
			res.code = 0;
			return sub(num2, res);
		}
	}

	tmp = num1.size > num2.size ? num1.size : num2.size; //�� �� ū ũ�� ����
	tmp += sizeof(unsigned int); //1�� int �ڸ���ŭ ���ϱ�
	res.num = calloc_in(tmp);
	if (!res.num) return res;
	res.size = tmp; //�Ҵ���� ũ�� ����

	//���
	tmp = 0;
	while (pl * sizeof(unsigned int) < num1.size || pl * sizeof(unsigned int) < num2.size || tmp) { //�� ���� �� ����� ũ�⸦ ����� tmp�� ����� ���� 0�� ������ �ݺ�
		if (pl * sizeof(unsigned int) < num1.size) tmp += (num1.num)[pl]; //num1 ���ϱ�
		if (pl * sizeof(unsigned int) < num2.size) tmp += (num2.num)[pl]; //num2 ���ϱ�
		res.num[pl] = (unsigned int)tmp; //���� ���� unsigned int ������ ������
		tmp >>= 8 * sizeof(unsigned int); //����� �κ��� ����� ���� �� �� �� �κ��� ���ڸ��� ��
		pl++;
	}

	//���ʿ��� ������� ����ó��, ���� ���ʿ��� ��������� �ִ����� Ȯ������ �ʱ� ������ �ʿ�(�� ���α׷����� return������ string ��ȯ �� �߰� �۾��� ������ �����Ƿ� ������ �ʿ�� ����)
	if (pl * sizeof(unsigned int) != res.size) { //�ʿ��� �������� �� ū ũ�Ⱑ �Ҵ�Ǿ�����
		//ptmp = realloc_in(res.num, pl * sizeof(unsigned int));
		//if (ptmp) res.num = ptmp;
		res.size = pl * sizeof(unsigned int);
	}
	res.code = num1.code;
	return res;
}

lint sub(const lint num1, const lint num2) {
	lint res = { 0,0,0 }, ltmp = { 0,0,0 }; //return���� ������ ����
	signed long long tmp = 0; //���갪�� �ӽ� ������ �Լ�
	size_t pl = 0; //���� ��ġ
	char left = 0;
	const signed long long debt = (long long)1 << (8 * sizeof(unsigned int)); //unsigned int���� ������ �� �ִ� �ִ밪 + 1
	//unsigned int* ptmp = NULL; //realloc�� realloc_in�� return���� �ӽ� ������ ����

	if (num1.code ^ num2.code) {
		res = num2; //���� ����
		if (num2.code) res.code = 0; //+-, ������ ���� ���(== ����� ���ϴ� ���)
		else res.code = 1; //-+, ������ ����� ���� ���(== ������ ���� ���)
		// if���� code�� ���� ������ ��(code�� �����Ҵ� ���� ���� �ƴ϶� �ٲ㵵 ������ ���� ����)
		return add(num1, res); //�̵��� ���ϱ� �������� ó����
	}
	else if (num1.code && num2.code) { //�������� ������ ���� ���
		ltmp = num1;
		res = num2;
		ltmp.code = 0;
		res.code = 0;
		return sub(res, ltmp); //������� ����� ���� ������ �ٲ��ֱ�
	}
	else if (lintcmp(num1, num2) < 0) { //����� �������̸�(���� �ٲ㼭 ���� ��ȣ ���̱�)
		res = sub(num2, num1);
		res.code = 1;
		return res;
	}
	//���� ����� ���� ū ������� ���� ����� ���� �� ��

	tmp = num1.size; //�� �� ū ũ�� ����
	res.num = calloc_in(tmp);
	if (!res.num) return res;
	res.size = tmp; //�Ҵ���� ũ�� ����

	//���
	tmp = 0;
	while (pl * sizeof(unsigned int) < num1.size || pl * sizeof(unsigned int) < num2.size || tmp) { //�� ���� �� ����� ũ�⸦ ����� tmp�� ����� ���� 0�� ������ �ݺ�
		if (pl * sizeof(unsigned int) < num1.size) tmp += (num1.num)[pl]; //num1 ���ϱ�
		if (pl * sizeof(unsigned int) < num2.size) tmp -= (num2.num)[pl]; //num2 ����
		if (tmp < 0) { //���� ������� 0���� ���� ���
			tmp += debt; //unsigned int�� �ִ� ���� + 1 �����ֱ�(�̷��� ����� ��)
			left = 1; //flag �ѱ�(�� ���� �� ���߿� �� �־�� ��)
		}
		res.num[pl] = (unsigned int)tmp; //���� ���� unsigned int ������ ������
		tmp >>= 8 * sizeof(unsigned int); //����� �κ��� ����� ���� �� �� �� �κ��� ���ڸ��� ��
		pl++;
		if (left) {
			tmp--; //���� ��ŭ ����(���� ���� shift �������� 1�� �Ǿ���)
			left = 0;
		}
	}

	//���ʿ��� ������� ����ó��, ���� ���ʿ��� ��������� �ִ����� Ȯ������ �ʱ� ������ �ʿ�(�� ���α׷����� return������ string ��ȯ �� �߰� �۾��� ������ �����Ƿ� ������ �ʿ�� ����)
	while (!last_digit_loc(res) && res.size) res.size -= sizeof(unsigned int);
	if (!res.size) res.size = sizeof(unsigned int); //size�� 0�� �Ǵ� �� ����
	/*if (pl * sizeof(unsigned int) != res.size) { //�ʿ��� �������� �� ū ũ�Ⱑ �Ҵ�Ǿ�����
		ptmp = realloc_in(res.num, pl * sizeof(unsigned int));
		if (ptmp) res.num = ptmp;
		res.size = pl * sizeof(unsigned int);
		//���Ҵ翡 �����ϱ� ��ư����� ���� �׷��ٸ� �Ҵ�� ũ�⸸ �ٲ� ������, ���̴� �������� �޸� ���� �������� ���� ����
	}*/
	return res;
}

lint mul(const lint num1, const lint num2) {
	lint res = { 0,0,0 }; //return���� ������ ����
	unsigned long long tmp = 0; //���갪�� �ӽ� ������ ����
	lint add_res = { 0,0,0 }; //���� ����� ������ ����
	lint push_res = { 0,0,0 }; //push�� ����� ������ ���� + �ӽ� ������ �������
	size_t pl = 0; //���� ��ġ
	char flag = 0; //���� �Ҵ� ���� Ȯ��
	unsigned int cont = 1; //MUX�� controller�� ���� ����
	size_t pu_pl = 0; //push�ؾ� �ϴ� �ڸ���
	unsigned int base = 0; //0�� ǥ���ϱ� ���� ���(0 ��ġ ��� ����)

	res.num = &base; //�̰� �ϳ������� base ������ ������
	res.size = sizeof(unsigned int); //ũ�� ����

	//���
	tmp = 0;
	while (pl * sizeof(unsigned int) < num2.size) {
		if (MUX_uint(num2.num[pl], cont)) { //��Ʈ �ϳ��� �˻��ϰ� �̹� ��Ʈ�� 1�̸�(2������ ����)
			push_res = push(num1, pu_pl);
			if (!push_res.num) {
				flag = 1;
				break;
			}
			add_res = add(res, push_res); //push�� ���� add��
			if (!add_res.num) {
				flag = 1;
				break;
			}
			freeset_lint(&push_res); //free�� �� �־�� �ϱ� ������ add�Լ��� �Ű������� �ٷ� push�� ����� �� ������
			if (res.num != &base) freeset_lint(&res);
			res = add_res;
		}
		pu_pl++;
		cont <<= 1;
		if (!cont) { //�ٸ� �ڸ����� �Ѿ �ʿ� ����
			pl++;
			cont = 1;
		}
	}
	if (flag) { //�����Ҵ� ���д� �ѹ��� ó��
		freeset_lint(&res);
		freeset_lint(&add_res);
		freeset_lint(&push_res);
	}
	res.code = num1.code ^ num2.code;
	return res;
}

lint push(const lint num, size_t len) { //<<����, ������ Ȱ��
	lint res = { 0,0,0 }; //return���� ������ ����
	unsigned long long tmp = 0; //���갪�� �ӽ� ������ �Լ�
	size_t pl = 0; //���� ��ġ
	size_t move = 0; //int ������ �� �� �̵��ؾ� �ϴ����� �����ϴ� �Լ�
	char flag = (len + last_digit_loc(num)) > (8 * sizeof(unsigned int)); //��Ʈ ������ �з��� int�� �ϳ��� �� �ʿ��� ������� Ȯ��

	move = len / (8 * sizeof(unsigned int));
	len %= (8 * sizeof(unsigned int));
	tmp = num.size + (move + flag) * sizeof(unsigned int); //res.num�� ����(���� ����+int ������ �и��� ����, ���⿡ ��Ʈ ������ �з��� int�� �ϳ��� �� �ʿ��� ��� �̰͵� ��)

	res.num = calloc_in(tmp);
	if (!res.num) return res;
	res.size = tmp; //�Ҵ���� ũ�� ����


	//���
	pl = num.size / sizeof(unsigned int); //�ݺ��� ���ۺ��� ���Ƿ� 1 ������
	tmp = 0;
	if (!flag) {
		pl--;
		tmp += num.num[pl];
		tmp <<= (8 * sizeof(unsigned int));
	}
	while (pl) {
		pl--;
		tmp += num.num[pl];
		tmp <<= len;
		res.num[pl + move + 1] = tmp >> (8 * sizeof(unsigned int)); //�� 32��Ʈ ����
		tmp <<= (8 * sizeof(unsigned int)) - len;
	}
	tmp <<= len;
	res.num[move] = tmp >> (8 * sizeof(unsigned int)); //�� 32��Ʈ ����
	res.code = num.code;
	return res;
}

char MUX_uint(unsigned int to, unsigned int controller) { //Ư�� �ڸ����� ��Ʈ���� 1���� 0���� Ȯ���ϴ� �Լ�
	if (to & controller) return 1;
	else return 0;
}

unsigned char last_digit_loc(const lint num) { //���� ū �ڸ���(int ������ ����)�� �ڸ����� ����� ����Ǿ� �ִ°� Ȯ��
	unsigned char res = 0; //return���� ������ ����
	unsigned int fin = num.num[num.size / sizeof(unsigned int) - 1]; //���� ū �ڸ���(int ������ ����) ����
	for (res = 0; fin; res++) fin >>= 1; //�ϳ��� �����̸鼭 fin�� 0�� �� ���� �������� Ȯ��
	return res;
}

char lintcmp(const lint num1, const lint num2) { // ũ�� ��
	size_t pl = num1.size / sizeof(unsigned int);
	//��ȣ ��ü�� �ٸ���
	if (num1.code != num2.code) {
		if (num1.code) return 4;
		return (-4);
	}
	//�Ҵ�� ũ�� ��ü�� �ٸ���
	else if (num1.size > num2.size ^ num1.code) return 3;
	else if (num1.size < num2.size ^ num1.code) return (-3);
	//�Ҵ�� ũ��� ������ ���� ũ�� �ȿ��� ù ��°�� �����ϴ� ������ ��ġ�� �ٸ� ���
	else if (last_digit_loc(num1) > last_digit_loc(num2) ^ num1.code) return 2;
	else if (last_digit_loc(num1) < last_digit_loc(num2) ^ num1.code) return (-2);
	//������ ���� �ڸ����� ������ ������ ���� ũ�Ⱑ �ٸ� ���
	while (pl) {
		pl--;
		if (num1.num[pl] > num2.num[pl] ^ num1.code) return 1;
		else if (num1.num[pl] < num2.num[pl] ^ num1.code) return (-1);
	}
	//���� ���
	return 0;
}

void freeset_lint(lint* p) { //lint�� freeset
	if (p->num) freeset(&(p->num));
	p->size = 0;
	p->code = 0;
}