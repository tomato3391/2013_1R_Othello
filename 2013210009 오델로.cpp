#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <Windows.h>
#define N 8
typedef struct person
{
	char name[100];
	int age;
	int win;
	int draw;
	int lose;
} PERSON; // ������ ����ü ����
void SinglePlayer();
void MultiPlayer();
void gotoxy(int x, int y);
int board[8][8]; // 0 = �� ����, 1 = ��, 2 = ��, 3 = ���� �� �ִ� ��
int direction; // 8������ ����
int changeable; // single/multiplayer �Լ����� �� ���� ���� ��� �ϳѱ�� ���� & multiplayer �Լ����� ��ȿ�� ��ġ������ �����ϴ� ���� ���
int x_axis[N] = {0, 1, 1, 1, 0, -1, -1, -1};
int y_axis[N] = {1, 1, 0, -1, -1, -1, 0, 1}; // [0]�迭 ���к��� ����, �ϵ���, ����, ������, ����, ������, ����, �ϼ����� ����Ŵ
PERSON user1, user2, user3;

int main()
{
	FILE *fp;
	int n, size, position, i;
	char test; // ����� �������� �����ϴ��� �˾ƺ��� ���� ����
	fp = fopen("Profiles.txt", "a+t");
	if (fp == NULL)
	{
		printf("File opening error! \n");
		exit(1);
	}
	fscanf(fp, "%c", &test);
	if (feof(fp))
	{
		user1.win = 0;
		user1.draw = 0;
		user1.lose = 0; // ���ο� �������� �¹��� �ʱ�ȭ
		printf("Register new profile.\n");
		printf("This profile will be on your main screen automatically every time you start this program.\n");
		printf("So, decide with caution.\n");
		printf("Write name : ");
		scanf("%s", &user1.name);
		printf("Write age : ");
		scanf("%d", &user1.age);
		fwrite(&user1, sizeof(PERSON), 1, fp);
		printf("Registration complete.\n");
		getch();
	}
	fclose(fp);
	fp = fopen("Profiles.txt", "a+t");
	if (fp == NULL)
	{
		printf("File opening error! \n");
		exit(1);
	}
	fread(&user1, sizeof(PERSON), 1, fp);
	while (1)
	{
		system("cls");
		printf("Welcome to Othello World!!			%s (%d)\n", user1.name, user1.age);
		printf("Choose one of these menus!			Win : %d Lose : %d Draw : %d\n", user1.win, user1.lose, user1.draw);
		printf("1: Single Game (Player VS Computer)\n");
		printf("2: Multi Game (Player VS Player)\n");
		printf("3: Register New Profile\n");
		printf("4: Load existing profiles\n");
		printf("5: Exit\n");
		fflush(stdin);
		scanf("%d", &n);
		if (n == 1)
		{
			system("cls");
			printf("Do you want to play Othello game with current profile?\n"); // ������ ��ȯ ����
			printf("1. Yes, I will play with this profile.\n");
			printf("2. No, I want to choose another profile from the list.\n");
			fflush(stdin);
			scanf("%d", &n);
			if (n != 1 && n != 2)
			{
				printf("Invalid input. Try again!\n");
				getch();
				continue;
			}
			if (n == 2) // ��ȯ�� �������� ���
			{
				while(1)
				{
					system("cls");
					fseek(fp, 0, SEEK_END);
					position = ftell(fp);
					size = position / sizeof(PERSON);
					rewind(fp);
					printf("===============================\n");
					printf("NO  name  age  W  D  L\n");
					printf("===============================\n"); // ��� ���
					for (i = 0; i < size; i++)
					{
						fread(&user1, sizeof(PERSON), 1, fp);
						printf("%d %s %d %d %d %d\n", i + 1, user1.name, user1.age, user1.win, user1.draw, user1.lose);
					}
					printf("Choose your profile. "); // ������ ����
					fflush(stdin);
					scanf("%d", &n);
					if (n < 1 || n > size)
					{
						printf("Invalid input. Try again!");
						getch();
						continue;
					}
					fseek(fp, sizeof(PERSON) * (n - 1), SEEK_SET);
					fread(&user1, sizeof(PERSON), 1, fp);
					printf("Successfully changed!\n");
					getch();
					break;
				}
			}
			system("cls");
			SinglePlayer();
			fseek(fp, 0, SEEK_END);
			position = ftell(fp);
			size = position / sizeof(PERSON);
			rewind(fp);
			for (i = 0; i < size; i++)
			{
				fread(&user3, sizeof(PERSON), 1, fp);
				if (strcmp(user1.name, user3.name) == 0)
				{
					fseek(fp, sizeof(PERSON) * i, SEEK_SET);
					fwrite(&user1, sizeof(PERSON), 1, fp);
				}
			}
			fclose(fp);
			fp = fopen("Profiles.txt", "a+t");
			continue;
		}
		else if (n == 2)
		{
			while(1)
			{
				system("cls");
				fseek(fp, 0, SEEK_END);
				position = ftell(fp);
				size = position / sizeof(PERSON);
				rewind(fp);
				printf("Choose 1 players you want to play with.\n"); // ���� �����ʰ� PVP�� ������ ����
				printf("=====================================\n");
				printf("NO  name  age  W  D  L\n");
				printf("=====================================\n");
				for (i = 0; i < size; i++)
				{
					fread(&user3, sizeof(PERSON), 1, fp);
					printf("%d %s %d %d %d %d\n", i + 1, user3.name, user3.age, user3.win, user3.draw, user3.lose);
				}
				fflush(stdin);
				scanf("%d", &n);
				if (n < 1 || n > size)
				{
					printf("Invalid input. Try again!\n");
					getch();
					continue;
				}
				fseek(fp, sizeof(PERSON) * (n - 1), SEEK_SET);
				fread(&user2, sizeof(PERSON), 1, fp);
				if (strcmp(user1.name, user2.name) == 0)
				{
					printf("Invalid input. You can't fight with yourself! Try again!\n");
					getch();
					continue;
				}
				break;
			}
			system("cls");
			MultiPlayer();
			fseek(fp, 0, SEEK_END);
			position = ftell(fp);
			size = position / sizeof(PERSON);
			rewind(fp);
			for (i = 0; i < size; i++)
			{
				fread(&user3, sizeof(PERSON), 1, fp);
				if (strcmp(user1.name, user3.name) == 0)
				{
					fseek(fp, sizeof(PERSON) * i, SEEK_SET);
					fwrite(&user1, sizeof(PERSON), 1, fp);
				}
			}
			for (i = 0; i < size; i++)
			{
				fread(&user3, sizeof(PERSON), 1, fp);
				if (strcmp(user2.name, user3.name) == 0)
				{
					fseek(fp, sizeof(PERSON) * i, SEEK_SET);
					fwrite(&user2, sizeof(PERSON), 1, fp);
				}
			}
			fclose(fp);
			fp = fopen("Profiles.txt", "a+t");
			continue;
		}
		else if (n == 3)
		{
			while(1)
			{
				user1.win = 0;
				user1.draw = 0;
				user1.lose = 0;
				system("cls");
				fseek(fp, 0, SEEK_END);
				position = ftell(fp);
				size = position / sizeof(PERSON);
				rewind(fp);
				printf("Register new profile.\n");
				printf("Write name : ");
				fflush(stdin);
				scanf("%s", &user1.name);
				printf("Write age : ");
				fflush(stdin);
				scanf("%d", &user1.age);
				for (i = 0; i < size; i++) // �Է��� �̸��� ���� �������� �̹� �����ϴ��� ����
				{
					fseek(fp, sizeof(PERSON) * (i - 1), SEEK_SET);
					fread(&user3, sizeof(PERSON), 1, fp);
					if (strcmp(user1.name, user3.name) == 0)
					{
						printf("Invalid user name. Identical name exists. Try again!\n");
						i = -1;
						getch();
						break;
					}
				}
				if (i == -1) // (�̸���) ������
					continue;
				else
					break;
			}
			fseek(fp, 0, SEEK_END);
			fwrite(&user1, sizeof(PERSON), 1, fp);
			fclose(fp);
			fp = fopen("Profiles.txt", "a+t");
			if (fp == NULL)
			{
				printf("File opening error! \n");
				exit(1);
			}
			fread(&user1, sizeof(PERSON), 1, fp);
			printf("Registration complete.\n");
			getch();
			continue;
		}
		else if (n == 4)
		{
			while(1)
			{
				system("cls");
				fseek(fp, 0, SEEK_END);
				position = ftell(fp);
				size = position / sizeof(PERSON);
				rewind(fp);
				printf("=====================================\n");
				printf("NO  name  age  W  D  L\n");
				printf("=====================================\n");
				for (i = 0; i < size; i++)
				{
					fread(&user1, sizeof(PERSON), 1, fp);
					printf("%d %s %d %d %d %d\n", i + 1, user1.name, user1.age, user1.win, user1.draw, user1.lose);
				}
				printf("Choose profile you want to load : ");
				fflush(stdin);
				scanf("%d", &n);
				if (n < 1 || n > size)
				{
					printf("Invalid input. Try again!\n");
					getch();
					continue;
				}
				fseek(fp, sizeof(PERSON) * (n - 1), SEEK_SET);
				fread(&user1, sizeof(PERSON), 1, fp);
				break;
			}
			printf("Successfully loaded!\n");
			getch();
			continue;
		}
		else if (n == 5)
		{
			fclose(fp);
			printf("Quit the game...\n");
			break;
		}
		else
		{
			printf("Invalid input. Try again.\n");
			system("pause");
			system("cls");
			continue;
		}
	}
	return 0;
}

void SinglePlayer()
{
	int i, j, k; // for�� �������
	int x, y, turn = 1; // ���� x, y��ǥ, turn
	int temp_x, temp_y; // ���� �ִ��� �˻��ϴ� ��ǥ
	int black = 0, white = 0; // ����, �� �� ����
	char char_x; // x�� ������ ���ĺ����� �����Ƿ� char�� ���� �� ASCII �ڵ� �� �̿�
	int willbechanged; // (AI����) �� �ڸ��� �������ν� ��밡���� �� ȹ�淮
	int max; // willbechanged ���� �� ���� ū ��
	for (i = 0; i < N; i++)
		for (j = 0; j < N; j++)
			board[i][j] = 0; // �迭 �� �ʱ�ȭ
	board[3][3] = 1;
	board[3][4] = 2;
	board[4][3] = 2;
	board[4][4] = 1;
	printf("        A     B     C     D     E     F     G     H	Black\n");
	printf("    ��������������������������������������������������	%s (%d)\n", user1.name, user1.age);
	printf(" 1  ��    ��    ��    ��    ��    ��    ��    ��    ��	%dW %dD %dL\n", user1.win, user1.draw, user1.lose);
	printf("    ��������������������������������������������������\n");
	printf(" 2  ��    ��    ��    ��    ��    ��    ��    ��    ��	White\n");
	printf("    ��������������������������������������������������	Computer\n");
	printf(" 3  ��    ��    ��    ��    ��    ��    ��    ��    ��\n");
	printf("    ��������������������������������������������������\n");
	printf(" 4  ��    ��    ��    �� �� �� �� ��    ��    ��    ��\n");
	printf("    ��������������������������������������������������\n");
	printf(" 5  ��    ��    ��    �� �� �� �� ��    ��    ��    ��\n");
	printf("    ��������������������������������������������������\n");
	printf(" 6  ��    ��    ��    ��    ��    ��    ��    ��    ��\n");
	printf("    ��������������������������������������������������\n");
	printf(" 7  ��    ��    ��    ��    ��    ��    ��    ��    ��\n");
	printf("    ��������������������������������������������������\n");
	printf(" 8  ��    ��    ��    ��    ��    ��    ��    ��    ��\n");
	printf("    ��������������������������������������������������\n");
	while (1)
	{
		gotoxy(0, 19);
		printf("                                                                              ");
		gotoxy(0, 20);
		printf("                                                                              ");
		gotoxy(0, 21);
		printf("                                                                              ");
		gotoxy(0, 22);
		printf("                                                                              ");
		gotoxy(0, 23);
		printf("                                                                              "); // ���찳 : �������̽��� �����(?)�� ����
		gotoxy(0, 19);
		if (turn == 1) // �÷��̾�
		{
			for (i = 0; i < N; i++) // A1~H8���� 8�������� �� �� �ִ� �� ���� �˻�
				for (j = 0; j < N; j++)
				{
					if (board[i][j] != 0)
						continue;
					for (direction = 0; direction < N; direction++)
					{
						temp_x = i;
						temp_y = j;
						for (k = 0; k < N - 1; k++) // �ִ� �̵� ������ Ƚ���� 7ȸ�̹Ƿ� i < N - 1
						{
							temp_x += x_axis[direction];
							temp_y += y_axis[direction];
							if ((temp_x < 0) || (temp_x > 7) || (temp_y < 0) || (temp_y > 7) || (board[temp_x][temp_y] == 0))
								break;
							if (board[temp_x][temp_y] == turn)
							{
								for (; k > 0; k--)
									changeable++;
								break;
							}
						}
					}
				}
			if (changeable == 0) // �� �� �ִ� ���� ������
			{
				gotoxy(0, 19);
				printf("User cannot overflate any disc.\n");
				Sleep(1500);
				turn = 3 - turn;
				continue;
			}
			printf("User(Black) turn...\n");
			printf("Select horizontal coordinate component (A ~ H): ");
			fflush(stdin);
			scanf("%c", &char_x);
			if ((char_x >= 'A') && (char_x <= 'Z'))
				x = char_x - 'A' + 1;
			else if ((char_x >= 'a') && (char_x <= 'z'))
				x = char_x - 'a' + 1;
			else
			{
				printf("Invalid input. Try again!\n");
				system("pause");
				continue;
			}
			printf("Select vertical coordinate component (1 ~ 8): ");
			fflush(stdin);
			scanf("%d", &y);
			if (y < 1 || y > 8)
			{
				printf("Invalid input. Try again!\n");
				system("pause");
				continue;
			}
			if (board[x - 1][y - 1] != 0) // �Է��� ���� �� ���� �ƴϸ�
			{
				printf("Invalid input. Try again!\n");
				system("pause");
				continue;
			}
			changeable = 0;
			for (direction = 0; direction < N; direction++) // �Է��� ���� �� �� �ִ� ������ 8�������� ����
			{
				temp_x = x - 1;
				temp_y = y - 1;
				for (i = 0; i < N - 1; i++)
				{
					temp_x += x_axis[direction];
					temp_y += y_axis[direction];
					if ((temp_x < 0) || (temp_x > 7) || (temp_y < 0) || (temp_y > 7) || (board[temp_x][temp_y] == 0))
						break;
					if (board[temp_x][temp_y] == turn)
					{
						for (; i > 0; i--)
						{
							temp_x -= x_axis[direction];
							temp_y -= y_axis[direction];
							board[temp_x][temp_y] = turn;
							changeable++;
						}
						break;
					}
				}
			}
			if (changeable != 0) // �Է��� ���� �� �� �ִ� ���̸�
			{
				board[x - 1][y - 1] = turn;
				black = 0;
				white = 0;
				for (i = 0; i < N; i++)
				{
					for (j = 0; j < N; j++)
					{
						if (board[i][j] == 1)
						{
							gotoxy(i * 6 + 7, j * 2 + 2);
							printf("��");
							black++; // �ش��ϴ� ��ǥ�� ���� �̵��� �� Ư������ ���
						}
						else if (board[i][j] == 2)
						{
							gotoxy(i * 6 + 7, j * 2 + 2);
							printf("��");
							white++;
						}
					}
				}
				turn = 3 - turn;
			}
			else // �Է��� ���� �� �� �ִ� ���� �ƴϸ�
			{
				printf("Invalid input. Try again!!\n");
				system("pause");
				continue;
			}
		}
		else // AI
		{
			max = 0; // �ִ� �ʱ�ȭ
			for (i = 0; i < N; i++) // ��� ��ǥ�� ���� willbechanged(������ �� �ִ� ���� ����) ����
				for (j = 0; j < N; j++)
				{
					if (board[i][j] != 0) // �ش� ��ǥ�� �� ���� �ƴ� ���
						continue;
					willbechanged = 0; // �ʱ�ȭ
					for (direction = 0; direction < N; direction++)
					{
						temp_x = i;
						temp_y = j;
						for (k = 0; k < N - 1; k++)
						{
							temp_x += x_axis[direction];
							temp_y += y_axis[direction];
							if ((temp_x < 0) || (temp_x > 7) || (temp_y < 0) || (temp_y > 7) || (board[temp_x][temp_y] == 0))
								break;
							if (board[temp_x][temp_y] == turn)
							{
								for (; k > 0; k--)
								{
									willbechanged++;
									changeable++;
								}
								break;
							}
						}
					}
					if (max < willbechanged) // �ش� ��ǥ�� willbechanged�� ��������� �ִ밪���� Ŭ ���
					{
						max = willbechanged;
						x = i;
						y = j; // �ش� ��ǥ�� ���� ����(�� ������)�� ������ ��ǥ�� ����
					}
				}
			if (changeable == 0) // ������ ���� �ϳ��� ������
			{
				gotoxy(0, 19);
				printf("Computer cannot overflate any disc.\n");
				Sleep(1500);
				turn = 3 - turn;
				continue;
			}
			printf("Computer(White) turn... \n");
			Sleep(1500);
			changeable = 0;
			for (direction = 0; direction < N; direction++) // ������ ������ ��ǥ�� ������ ����
			{
				temp_x = x;
				temp_y = y;
				for (i = 0; i < N - 1; i++)
				{
					temp_x += x_axis[direction];
					temp_y += y_axis[direction];
					if ((temp_x < 0) || (temp_x > 7) || (temp_y < 0) || (temp_y > 7) || (board[temp_x][temp_y] == 0))
						break;
					if (board[temp_x][temp_y] == turn)
					{
						for (; i > 0; i--)
						{
							temp_x -= x_axis[direction];
							temp_y -= y_axis[direction];
							board[temp_x][temp_y] = turn;
							changeable++;
						}
						break;
					}
				}
			}
			if (changeable != 0) // ������ ���� ������(��ǻ� ��� ������ ���� - ������ �̹� ��������Ƿ�)
			{
				board[x][y] = turn;
				black = 0;
				white = 0;
				for (i = 0; i < N; i++)
				{
					for (j = 0; j < N; j++)
					{
						if (board[i][j] == 1)
						{
							gotoxy(i * 6 + 7, j * 2 + 2);
							printf("��");
							black++;
						}
						else if (board[i][j] == 2)
						{
							gotoxy(i * 6 + 7, j * 2 + 2);
							printf("��");
							white++;
						}
					}
				}
				turn = 3 - turn;
			}
		}
		if ((black == 0) || (white == 0) || (black + white == 64)) // black + white�� 64�̰ų� �� �� �ϳ��� ����ϸ� ���� ����
				break;
	}
	gotoxy(0, 22);
	printf("Black : %d\n", black);
	printf("White : %d\n", white);
	if (black > white)
	{
		printf("User(Black) wins!!\n");
		user1.win++;
	}
	else if (white > black)
	{
		printf("Computer(White) wins!!\n");
		user1.lose++;
	}
	else
	{
		printf("Draw!!\n");
		user1.draw++;
	}
	system("pause");
}

void MultiPlayer()
{
	int i, j, k;
	int x, y, turn = 1;
	int temp_x, temp_y;
	int black = 0, white = 0;
	char char_x;
	for (i = 0; i < N; i++)
		for (j = 0; j < N; j++)
			board[i][j] = 0;
	board[3][3] = 1;
	board[3][4] = 2;
	board[4][3] = 2;
	board[4][4] = 1;
	printf("        A     B     C     D     E     F     G     H	Black\n");
	printf("    ��������������������������������������������������	%s (%d)\n", user1.name, user1.age);
	printf(" 1  ��    ��    ��    ��    ��    ��    ��    ��    ��	%dW %dD %dL\n", user1.win, user1.draw, user1.lose);
	printf("    ��������������������������������������������������\n");
	printf(" 2  ��    ��    ��    ��    ��    ��    ��    ��    ��	White\n");
	printf("    ��������������������������������������������������	%s (%d)\n", user2.name, user2.age);
	printf(" 3  ��    ��    ��    ��    ��    ��    ��    ��    ��	%dW %dD %dL\n", user2.win, user2.draw, user2.lose);
	printf("    ��������������������������������������������������\n");
	printf(" 4  ��    ��    ��    �� �� �� �� ��    ��    ��    ��\n");
	printf("    ��������������������������������������������������\n");
	printf(" 5  ��    ��    ��    �� �� �� �� ��    ��    ��    ��\n");
	printf("    ��������������������������������������������������\n");
	printf(" 6  ��    ��    ��    ��    ��    ��    ��    ��    ��\n");
	printf("    ��������������������������������������������������\n");
	printf(" 7  ��    ��    ��    ��    ��    ��    ��    ��    ��\n");
	printf("    ��������������������������������������������������\n");
	printf(" 8  ��    ��    ��    ��    ��    ��    ��    ��    ��\n");
	printf("    ��������������������������������������������������\n");
	while (1)
	{
		gotoxy(0, 19);
		printf("                                                                              ");
		gotoxy(0, 20);
		printf("                                                                              ");
		gotoxy(0, 21);
		printf("                                                                              ");
		gotoxy(0, 22);
		printf("                                                                              ");
		gotoxy(0, 23);
		printf("                                                                              ");
		for (i = 0; i < N; i++)
			for (j = 0; j < N; j++)
			{
				if (board[i][j] != 0)
					continue;
				for (direction = 0; direction < N; direction++)
				{
					temp_x = i;
					temp_y = j;
					for (k = 0; k < N - 1; k++)
					{
						temp_x += x_axis[direction];
						temp_y += y_axis[direction];
						if ((temp_x < 0) || (temp_x > 7) || (temp_y < 0) || (temp_y > 7) || (board[temp_x][temp_y] == 0))
							break;
						if (board[temp_x][temp_y] == turn)
						{
							for (; k > 0; k--)
								changeable++;
							break;
						}
					}
				}
			}
		if (changeable == 0)
		{
			gotoxy(0, 19);
			printf("Player %d cannot overflate any disc.\n", turn);
			Sleep(1500);
			turn = 3 - turn;
			continue;
		}
		gotoxy(0, 19);
		if (turn == 1)
			printf("Black turn... \n");
		else
			printf("White turn... \n");
		printf("Select horizontal coordinate component (A ~ H): ");
		fflush(stdin);
		scanf("%c", &char_x);
		if ((char_x >= 'A') && (char_x <= 'H'))
			x = char_x - 'A' + 1;
		else if ((char_x >= 'a') && (char_x <= 'h'))
			x = char_x - 'a' + 1;
		else
		{
			printf("Invalid input. Try again!\n");
			system("pause");
			continue;
		}
		printf("Select vertical coordinate component (1 ~ 8): ");
		fflush(stdin);
		scanf("%d", &y);
		if (y < 1 || y > 8)
		{
			printf("Invalid input. Try again!\n");
			system("pause");
			continue;
		}
		if (board[x - 1][y - 1] != 0)
		{
			printf("Invalid input. Try again!\n");
			system("pause");
			continue;
		}
		changeable = 0;
		for (direction = 0; direction < N; direction++)
		{
			temp_x = x - 1;
			temp_y = y - 1;
			for (i = 0; i < N - 1; i++)
			{
				temp_x += x_axis[direction];
				temp_y += y_axis[direction];
				if ((temp_x < 0) || (temp_x > 7) || (temp_y < 0) || (temp_y > 7) || (board[temp_x][temp_y] == 0))
					break;
				if (board[temp_x][temp_y] == turn)
				{
					for (; i > 0; i--)
					{
						temp_x -= x_axis[direction];
						temp_y -= y_axis[direction];
						board[temp_x][temp_y] = turn;
						changeable++;
					}
					break;
				}
			}
		}
		if (changeable != 0)
		{
			board[x - 1][y - 1] = turn;
			black = 0;
			white = 0;
			for (i = 0; i < N; i++)
			{
				for (j = 0; j < N; j++)
				{
					if (board[i][j] == 1)
					{
						gotoxy(i * 6 + 7, j * 2 + 2);
                        printf("��");
                        black++;
					}
					else if (board[i][j] == 2)
					{
						gotoxy(i * 6 + 7, j * 2 + 2);
                        printf("��");
                        white++;
					}
				}
			}
			turn = 3 - turn;
		}
		else
		{
			printf("Invalid input. Try again!!\n");
			system("pause");
			continue;
		}
		if ((black == 0) || (white == 0) || (black + white == 64))
			break;
	}
	gotoxy(0, 22);
	printf("Black : %d\n", black);
	printf("White : %d\n", white);
	if (black > white)
	{
		printf("Black wins!!\n");
		user1.win++;
		user2.lose++;
	}
	else if (white > black)
	{
		printf("White wins!!\n");
		user1.lose++;
		user2.win++;
	}
	else
	{
		printf("Draw!!\n");
		user1.draw++;
		user2.draw++;
	}
	system("pause");
}

void gotoxy(int x, int y)
{
	COORD Cur;
	Cur.X = x;
	Cur.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
}