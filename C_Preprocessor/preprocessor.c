#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct macro { // #define���� ���ǵ� ������(m_name)�� �ش�Ǵ� ��(m_body) ����
	char m_name[30];
	char m_body[100];
};

void main(int argc, char** argv)
{
	int i = 0;
	if (argc != 2) {
		printf("Enter input correctly\n");
		return;
	}

	// �����ϰ��� �ϴ� ���� ����
	FILE* fp; fopen_s(&fp, argv[1], "r");

	if (!fp) {
		printf("File not fund\n");
		return;
	}

	///////////////////////////////////////////////////////////
	char ch, ch1, ch2;

	char s[100], s1[100], filename[100] = "";
	char* p;

	struct macro m[5] = {0, 0};
	int macro_no = 0;

	// #define�� ���� ������ �����ϰ� #include�� �ش� ���� ���� ������ ���� �� �ٿ��ֱ�
	FILE* fpnew; fopen_s(&fpnew, "file.i", "w");

	if (!fpnew) {
		printf("File not fund\n");
		return;
	}

	// ������ ��� ���� ������
	while ((ch = fgetc(fp)) != -1) {
		if (ch == '#') { // ��ũ�ζ��
			ch1 = fgetc(fp); // ���� �ϳ��� ���� ��
			i = 0;
			if (ch1 == ' ') {
				while ((ch2 = fgetc(fp)) == ' ') ; // Ȥ�� �� ���� ����
				s[i++] = ch2;
			}
			else { // ������ ���� ���
				s[i++] = ch1; // # ���� �ܾ�(ex. define, include)�� ù ���ڸ� s�� ����
			}

			// ������ ���� ������ �ش� �ܾ� �Ǵ� ����(ex. define, include "���ϸ�") ��θ� s�� ����
			if (ch1 == 'd') {
				while ((ch1 = fgetc(fp)) != ' ') {
					s[i++] = ch1;
				}
				s[i] = '\0'; // ���� �� �����ֱ�
			}
			else if (ch1 == 'i') {
				while ((ch1 = fgetc(fp)) != '\n') {
					s[i++] = ch1;
				}
				s[i] = '\0'; // ���� �� �����ֱ�
			}

			if (strcmp(s, "define") == 0) { // ���� ���� �ܾ define�̶��
				i = 0;

				// ���� �����
				while (ch1 == ' ') ch1 = fgetc(fp);

				// ���� ������ �ܾ s�� �Է�(ex. PI, MAX)
				s[i++] = ch1;
				while ((ch1 = fgetc(fp)) != ' ') s[i++] = ch1;
				s[i] = '\0'; // �����ֱ�
				
				printf("macro name = '%s'\n", s);
				
				// ������(ex. PI, MAX)�� ����ü ���ҿ� �Ҵ�
				strcpy(m[macro_no].m_name, s);

				// �ش�Ǵ� ���� ����ü ���ҿ� �Ҵ��ϱ� ���� s�� ���� ���ڿ��� ����
				i = 0;
				while ((ch1 = fgetc(fp)) != '\n') s[i++] = ch1;
				s[i] = '\0';
				printf("macro body = '%s'\n macro_no = %d\n", s, macro_no);

				// macro�� ��(ex. 3.14, 10)�� ����ü ���ҿ� �Ҵ�
				strcpy(m[macro_no++].m_body, s); 
			}
			else if (p = strstr(s, "include")) { // ���� ���忡 include�� �ִٸ�
				// include"���ϸ�"�� ����� ���ڿ� s���� ���ϸ� �и��ϱ�
				i = 7;
				while (!((s[i] > 64 && s[i] < 91) || (s[i] > 96 && s[i] < 123))) i++;

				int x;
				for (x = i; ((s[x] > 64 && s[x] < 91) || (s[x] > 96 && s[x] < 123) || s[x] == 46); x++) filename[x - i] = s[x];
				filename[x - i] = '\0';
				
				// ���ϸ��� ����� �����ִٸ�
				if (i) {
					// �ش� ��� ������ �б���� ����
					FILE* fhead = fopen(filename, "r");
					if (!fhead) {
						printf("Error: Header file not found\n");
						break;
					}

					// ��� ���� ������ ��� �о� �ٿ�����
					while (fgets(s1, sizeof(s1), fhead) != NULL) fputs(s1, fpnew);
				}
			}
		}
		else {
			fputc(ch, fpnew);
		}
	} // #define�� ���� ������ ����ü�� ���� �� ��� ������ ���� ġȯ ����
	fclose(fpnew);

	// #define���� ���ǵ� ������ ��� ����
	FILE* tmpfp;  fopen_s(&tmpfp, "file.i", "r");
	// ���� ������� ����� ���� fopen
	fopen_s(&fpnew, "result.i", "w"); 

	int n = macro_no - 1;
	if (macro_no > 0) {
		while ((fgets(s1, 100, tmpfp)) != 0) {
			macro_no = n;
			// �о�� ���忡 #define���� �����ߴ� ������ ã�� ����
			while (macro_no >= 0) {
				if (p = (strstr(s1, m[macro_no].m_name))) {
					// �������� ������ ������ ������ ������ �״�� result.i�� ���
					i = 0;
					while (s1 + i != p) fputc(s1[i++], fpnew);

					// ������ �ڸ��� �ش�Ǵ� ��(3.1415, 10)�� ���� �� ������(PI, MAX)�� ����
					fputs(m[macro_no].m_body, fpnew);
					i = i + strlen(m[macro_no].m_name);

					// ������ �о�� �κ� �״�� ����
					while (s1[i]) fputc(s1[i++], fpnew);

					printf("%s deleted. the sentence = %s\n", m[macro_no].m_name, s1);
					break;
				}
				else {
					printf("no this %s macro = %s\n", m[macro_no].m_name, s1);
					// ����� ��� macro�� ���� �˻��ϱ� ������
					// ���������� ��� macro�� ���ٰ� Ȯ������ ������ ���Ͽ� ���
					if (macro_no == 0) fputs(s1, fpnew);
				}
				macro_no--;
			}
		}
	}
}