#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct macro { // #define으로 정의된 변수명(m_name)과 해당되는 값(m_body) 저장
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

	// 실행하고자 하는 파일 열기
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

	// #define에 대한 정보를 저장하고 #include된 해당 파일 안의 내용을 복사 및 붙여넣기
	FILE* fpnew; fopen_s(&fpnew, "file.i", "w");

	if (!fpnew) {
		printf("File not fund\n");
		return;
	}

	// 파일을 모두 읽을 때까지
	while ((ch = fgetc(fp)) != -1) {
		if (ch == '#') { // 매크로라면
			ch1 = fgetc(fp); // 문자 하나만 읽은 후
			i = 0;
			if (ch1 == ' ') {
				while ((ch2 = fgetc(fp)) == ' ') ; // 혹시 모를 공백 삭제
				s[i++] = ch2;
			}
			else { // 공백이 없을 경우
				s[i++] = ch1; // # 뒤의 단어(ex. define, include)의 첫 글자를 s에 저장
			}

			// 공백이 나올 때까지 해당 단어 또는 문장(ex. define, include "파일명") 모두를 s에 저장
			if (ch1 == 'd') {
				while ((ch1 = fgetc(fp)) != ' ') {
					s[i++] = ch1;
				}
				s[i] = '\0'; // 저장 후 끊어주기
			}
			else if (ch1 == 'i') {
				while ((ch1 = fgetc(fp)) != '\n') {
					s[i++] = ch1;
				}
				s[i] = '\0'; // 저장 후 끊어주기
			}

			if (strcmp(s, "define") == 0) { // 현재 읽은 단어가 define이라면
				i = 0;

				// 공백 지우기
				while (ch1 == ' ') ch1 = fgetc(fp);

				// 공백 전까지 단어를 s에 입력(ex. PI, MAX)
				s[i++] = ch1;
				while ((ch1 = fgetc(fp)) != ' ') s[i++] = ch1;
				s[i] = '\0'; // 끊어주기
				
				printf("macro name = '%s'\n", s);
				
				// 변수명(ex. PI, MAX)을 구조체 원소에 할당
				strcpy(m[macro_no].m_name, s);

				// 해당되는 값을 구조체 원소에 할당하기 위해 s에 먼저 문자열로 저장
				i = 0;
				while ((ch1 = fgetc(fp)) != '\n') s[i++] = ch1;
				s[i] = '\0';
				printf("macro body = '%s'\n macro_no = %d\n", s, macro_no);

				// macro의 값(ex. 3.14, 10)을 구조체 원소에 할당
				strcpy(m[macro_no++].m_body, s); 
			}
			else if (p = strstr(s, "include")) { // 현재 문장에 include가 있다면
				// include"파일명"이 저장된 문자열 s에서 파일명만 분리하기
				i = 7;
				while (!((s[i] > 64 && s[i] < 91) || (s[i] > 96 && s[i] < 123))) i++;

				int x;
				for (x = i; ((s[x] > 64 && s[x] < 91) || (s[x] > 96 && s[x] < 123) || s[x] == 46); x++) filename[x - i] = s[x];
				filename[x - i] = '\0';
				
				// 파일명이 제대로 적혀있다면
				if (i) {
					// 해당 헤더 파일을 읽기모드로 열기
					FILE* fhead = fopen(filename, "r");
					if (!fhead) {
						printf("Error: Header file not found\n");
						break;
					}

					// 헤더 파일 내용을 모두 읽어 붙여넣음
					while (fgets(s1, sizeof(s1), fhead) != NULL) fputs(s1, fpnew);
				}
			}
		}
		else {
			fputc(ch, fpnew);
		}
	} // #define에 대한 정보를 구조체로 저장 및 헤더 파일의 내용 치환 종료
	fclose(fpnew);

	// #define으로 정의된 변수를 모두 변경
	FILE* tmpfp;  fopen_s(&tmpfp, "file.i", "r");
	// 최종 결과물을 출력할 파일 fopen
	fopen_s(&fpnew, "result.i", "w"); 

	int n = macro_no - 1;
	if (macro_no > 0) {
		while ((fgets(s1, 100, tmpfp)) != 0) {
			macro_no = n;
			// 읽어온 문장에 #define으로 정의했던 변수를 찾아 수정
			while (macro_no >= 0) {
				if (p = (strstr(s1, m[macro_no].m_name))) {
					// 변수명이 나오기 전까지 파일의 내용을 그대로 result.i에 출력
					i = 0;
					while (s1 + i != p) fputc(s1[i++], fpnew);

					// 변수명 자리에 해당되는 값(3.1415, 10)을 삽입 후 변수명(PI, MAX)은 삭제
					fputs(m[macro_no].m_body, fpnew);
					i = i + strlen(m[macro_no].m_name);

					// 나머지 읽어온 부분 그대로 삽입
					while (s1[i]) fputc(s1[i++], fpnew);

					printf("%s deleted. the sentence = %s\n", m[macro_no].m_name, s1);
					break;
				}
				else {
					printf("no this %s macro = %s\n", m[macro_no].m_name, s1);
					// 저장된 모든 macro에 대해 검사하기 때문에
					// 최종적으로 모든 macro가 없다고 확정됐을 때에만 파일에 출력
					if (macro_no == 0) fputs(s1, fpnew);
				}
				macro_no--;
			}
		}
	}
}