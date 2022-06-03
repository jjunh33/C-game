#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <time.h>

#define left 75
#define right 77
#define up 72
#define down 80

#define easy 6
#define normal 8
#define hard 10
#define custom 12

int key; //get keyboard
int x = 0, y = 0; //cursor_x. cursor_y
int m_x, m_y; //mine_x, mine_y
int i, j; //loop var
int count; //surround mine count
int e_count; //game_end count
int ml_x, ml_y, m_num; //map length_x, map length_y, mine_number
int f_count; //flag_count

int map[102][102];	//0: check X, 1: mine, 2: checked, 3: first move 9, 4: flag
int flag[102][102];

void cur(int x, int y) //place cursor
{
	COORD pos = {x, y};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void print_map() //print map
{
	cur(0, 0);
	for(i = 1; i <= ml_y; ++i)
	{
		for(j = 1; j <= ml_x; ++j)
		{
			printf("■");
		}
		printf("\n");
	}
	cur(ml_x * 2 + 2, 2);
	SetConsoleTextAttribute(GetStdHandle( STD_OUTPUT_HANDLE ), 12);
	printf("* ");
	SetConsoleTextAttribute(GetStdHandle( STD_OUTPUT_HANDLE ), 7);
	printf(": %d          ", f_count);
	return;
}

void m_spawn() //mine spawn
{
	srand(time(NULL));
	for(i = 1; i <= m_num; ++i)
	{
		do
		{
			m_x = rand() % ml_x + 1;
			m_y = rand() % ml_y + 1;
		}while(map[m_y][m_x] == 1 || map[m_y][m_x] == 3);
		map[m_y][m_x] = 1;
	}
}

void check(int a, int b) //surround mine check
{
	cur((b - 1) * 2, a - 1);
	count = 0;
	if(map[a][b] != 1)
	{
		map[a][b] = 2;
	}	
	for(i = -1; i <= 1; ++i)
	{
		for(j = -1; j <= 1; ++j)
		{
			if(map[a + i][b + j] == 1)
			{
				count++;
			}
		}
	}
	
	switch(count) //number color change
	{
		case 1:
			SetConsoleTextAttribute(GetStdHandle( STD_OUTPUT_HANDLE ), 9);
			break;
		case 2:
			SetConsoleTextAttribute(GetStdHandle( STD_OUTPUT_HANDLE ), 10);
			break;
		case 3:
			SetConsoleTextAttribute(GetStdHandle( STD_OUTPUT_HANDLE ), 4);
			break;
		case 4:
			SetConsoleTextAttribute(GetStdHandle( STD_OUTPUT_HANDLE ), 1);
			break;
		case 5:
			SetConsoleTextAttribute(GetStdHandle( STD_OUTPUT_HANDLE ), 6);
			break;
		case 6:
			SetConsoleTextAttribute(GetStdHandle( STD_OUTPUT_HANDLE ), 2);
			break;
		case 7:
			SetConsoleTextAttribute(GetStdHandle( STD_OUTPUT_HANDLE ), 5);
			break;
		case 8:
			SetConsoleTextAttribute(GetStdHandle( STD_OUTPUT_HANDLE ), 3);
			break;
	}
	printf("%d ", count);
	SetConsoleTextAttribute(GetStdHandle( STD_OUTPUT_HANDLE ), 7);
	if(count == 0)
	{
		if(map[a - 1][b] != 2 && a > 1)
		{
			check(a - 1, b);
		}
		if(map[a + 1][b] != 2 && a < ml_y)
		{
			check(a + 1, b);
		}
		if(map[a][b - 1] != 2 && b > 1)
		{
			check(a, b - 1);
		}
		if(map[a][b + 1] != 2 && b < ml_x)
		{
			check(a, b + 1);
		}
		if(map[a - 1][b - 1] != 2 && a > 1 && b > 1)
		{
			check(a - 1, b - 1);
		}
		if(map[a - 1][b + 1] != 2 && a > 1 && b < ml_x)
		{
			check(a - 1, b + 1);
		}
		if(map[a + 1][b - 1] != 2 && a < ml_y && b > 1)
		{
			check(a + 1, b - 1);
		}
		if(map[a + 1][b + 1] != 2 && a < ml_y && b < ml_x)
		{
			check(a + 1, b + 1);
		}
	}
	else
	{
		return;
	}
}

void cur_move() //cursor move by keboard
{
	key = getch();
	switch(key)
	{
		case left:
			x -= 2;
			if(x < 0)
			{
				x += 2;
			}
			break;
		case right:
			x += 2;
			if(x > ml_x * 2 - 1)
			{
				x -= 2;
			}
			break;
		case up:
			y -= 1;
			if(y < 0)
			{
				y += 1;
			}
			break;
		case down:
			y += 1;
			if(y > ml_y - 1)
			{
				y -= 1;
			}
			break;
	}
}

void left_m_check() //check how many mines left
{
	f_count = m_num;
	e_count = 0;
	for(i = 1; i <= ml_y; ++i)
	{
    	for(j = 1; j <= ml_x; ++j)
    	{
      		if(flag[i][j] == 4 && map[i][j] != 2)
      		{
      			f_count--;
      			if(map[i][j] == 1)
      			{
      				e_count++;
				}
			}
    	}
   	}
	cur(ml_x * 2 + 2, 2);
	SetConsoleTextAttribute(GetStdHandle( STD_OUTPUT_HANDLE ), 12);
	printf("* ");
	SetConsoleTextAttribute(GetStdHandle( STD_OUTPUT_HANDLE ), 7);
	printf(": %d          ", f_count);
}

void first_move() //first enter -> mine spawn
{
	cur(0, 0);
	while(1)
	{
		if(kbhit())
		{
			key = getch();
			if(key == 224)
			{
				cur_move();
			}
			else if(key == 13)
			{
				if(flag[y + 1][x / 2 + 1] == 4)
				{
					map[y + 1][x / 2 + 1] = 2;
				}
				for(i = -1; i <= 1; ++i)
				{
					for(j = -1; j <= 1; ++j)
					{
						map[y + 1 + i][x / 2 + 1 + j] = 3;
					}
				}
				m_spawn();
				check(y + 1, x / 2 + 1);
				left_m_check();
				return;
			}
			else if(key == 32)
			{
				if(flag[y + 1][x / 2 + 1] != 4 && f_count > 0)
				{
					SetConsoleTextAttribute(GetStdHandle( STD_OUTPUT_HANDLE ), 12);
					printf("* ");
					SetConsoleTextAttribute(GetStdHandle( STD_OUTPUT_HANDLE ), 7);
					flag[y + 1][x / 2 + 1] = 4;
				}
			}
			left_m_check();
			cur(x, y);
		}
	}
	cur(x, y);
} 

void end_m_check() //game finish -> check mine location
{
	SetConsoleTextAttribute(GetStdHandle( STD_OUTPUT_HANDLE ), 8);
	for(i = 1; i <= ml_y; ++i)
	{
		for(j = 1; j <= ml_x; ++j)
		{
			if(map[i][j] == 1)
			{
				cur(j * 2 - 2, i - 1);
				printf("@ ");
			}
		}
		printf("\n"); 
	}
	SetConsoleTextAttribute(GetStdHandle( STD_OUTPUT_HANDLE ), 7);
}

void game(int d) //first move -> mine spawn -> game
{
	system("cls");
	switch(d)
	{
		case easy:
			ml_x = 10;
			ml_y = 10;
			m_num = 10;
			break;
		case normal:
			ml_x = 15;
			ml_y = 15;
			m_num = 25;
			break;
		case hard:
			ml_x = 20;
			ml_y = 20;
			m_num = 40;
			break;
		case custom:
			system("cls");
			while(1)
			{
				cur(0, 0); 
				printf("X축 칸 수(100 이하) : ");
				scanf("%d", &ml_x);
				if(ml_x > 100)
				{
					cur(0, 0);
					printf("100 이하의 숫자를 입력해야 합니다.");
					Sleep(3000);
					system("cls");
				}
				else
				{
					break;
				}
			}
			system("cls");
			while(1)
			{
				cur(0, 0); 
				printf("Y축 칸 수(100 이하) : ");
				scanf("%d", &ml_y);
				if(ml_y > 100)
				{
					cur(0, 0);
					printf("100 이하의 숫자를 입력해야 합니다.");
					Sleep(3000);
					system("cls");
				}
				else
				{
					break;
				}
			}
			system("cls");
			while(1)
			{
				cur(0, 0); 
				printf("지뢰의 개수(%d 미만) : ", ml_x * ml_y);
				scanf("%d", &m_num);
				if(m_num > ml_x * ml_y)
				{
					cur(0, 0);
					printf("총 칸 수(%d칸)미만의 숫자를 입력해야 합니다.", ml_x * ml_y);
					Sleep(3000);
					system("cls");
				}
				else
				{
					break;
				}
			}
			break;
	}
	f_count = m_num;
	system("cls");
	print_map();
	first_move();
	while(1)
	{
		if(kbhit())
		{
			key = getch();
			if(key == 224)
			{
				cur_move();
			}
			else if(key == 13)
			{
				if(map[y + 1][x / 2 + 1] == 1)
				{
					end_m_check();
					printf("\nTry again!");
					break;
				}
				else
				{
					if(flag[y + 1][x / 2 + 1] == 4 && map[y + 1][x / 2 + 1] != 2)
					{
						map[y + 1][x / 2 + 1] = 2;
					}
					check(y + 1, x / 2 + 1);
					for(i = 1; i <= ml_y; ++i)
					{
						for(j = 1; j <= ml_x; ++j)
						{
							if(map[i][j] == 2)
							{
								e_count++;
							}
						}
					}
				}
			}
			else if(key == 32)
			{
				if(map[y + 1][x / 2 + 1] != 2 && flag[y + 1][x / 2 + 1] != 4 && f_count > 0)
				{
					SetConsoleTextAttribute(GetStdHandle( STD_OUTPUT_HANDLE ), 12);
					printf("* ");
					SetConsoleTextAttribute(GetStdHandle( STD_OUTPUT_HANDLE ), 7);
					flag[y + 1][x / 2 + 1] = 4;
				}
			}
			left_m_check();
			if(e_count == m_num)
			{
				end_m_check();
				printf("\nClear!");
				break;
			}
		}
		cur(x, y);
	}
}

void title() //title + game
{
	system("cls");
	cur(4, 2);
	printf("지뢰찾기");
	cur(4, easy);
	printf("Easy - 10 X 10, 지뢰 10개");
	cur(4, normal);
	printf("Normal - 15 X 15, 지뢰 25개");
	cur(4, hard);
	printf("Hard - 20 X 20, 지뢰 40개");
	cur(4, custom);
	printf("Custom");
	cur(1, easy);
	printf("▶");
	cur(1, easy);
	int cur_dir = easy;
	while(1)
	{
		if(kbhit())
		{
			key = getch();
			if(key == 224)
			{
				key = getch();
				cur(1, cur_dir);
				printf("   ");
				switch(key)
				{
					case up:
						if(cur_dir <= easy)
						{
							cur_dir = custom;
						}
						else
						{
							cur_dir -= 2;
						}
						break;
					case down:
						if(cur_dir >= custom)
						{
							cur_dir = easy;
						}
						else
						{
							cur_dir += 2;
						}
						break;
				}
				cur(1, cur_dir);
				printf("▶");
			}
			else if(key == 13)
			{
				for(i = 1; i <= ml_y; ++i)
   				{
      				for(j = 1; j <= ml_x; ++j)
      				{
         				map[i][j] = 0;
         				flag[i][j] = 0;
      				}
   				}
				game(cur_dir);
				cur(0, ml_y + 4);
				printf("Press T to go back to the title");
				while(1)
				{
					if(kbhit())
					{
						key = getch();
						if(key == 116)
						{
							title();
						}
					}
				}
			}
		}
	}
}

int main()
{
	title();
	return 0;	
}
