int initGAME(int x)
{
	initgraph(1024, 1024, 0);
	IMAGE img;
	loadimage(&img, "title.png");
	putimage(0, 0, &img);
	IMAGE img1;
	loadimage(&img1, "background.png");
	putimage(50, 50, &img1);
	if (x==1)
	{
		setbkmode(0);
		settextcolor(BLACK);
		settextstyle(64, BLACK, _T("宋体"));
		outtextxy(400, 380, "自我博弈");
		outtextxy(400, 460, "人机对弈");
		outtextxy(400, 540, "对局导入");
		outtextxy(400, 620, "联机对战");
		settextstyle(32, BLACK, _T("黑体"));
		outtextxy(880, 955, "退出游戏");
		return 0;
	}
	if (x ==0)
	{
		setbkmode(0);
		settextcolor(BLACK);
		settextstyle(64, BLACK, _T("宋体"));
		outtextxy(400, 380, "#先手#");
		outtextxy(400, 460, "#后手#");
		return 1;
	}
	if (x == 2)
	{
		setbkmode(0);
		settextcolor(BLACK);
		settextstyle(64, BLACK, _T("宋体"));
		outtextxy(400, 380, "%简单%");
		outtextxy(400, 460, "%复杂%");
	}
	if (x == 3)
	{
		setbkmode(0);
		settextcolor(BLACK);
		settextstyle(64, BLACK, _T("宋体"));
		outtextxy(400, 380, "%开房%");
		outtextxy(400, 460, "%进房%");
	}
	if (x == 4)
	{
		setbkmode(0);
		settextcolor(BLACK);
		settextstyle(96, BLACK, _T("宋体"));
		outtextxy(400, 380, "Waiting");
	}
}
int xianhoushou()
{
	while (1)
	{
		Msg = GetMouseMsg();
		if (Msg.uMsg == WM_LBUTTONDOWN && Msg.x > 400 && Msg.x < 600 && Msg.y < 450 && Msg.y>380)
		{
			return 0;
		}
		else if (Msg.uMsg == WM_LBUTTONDOWN && Msg.x > 400 && Msg.x < 600 && Msg.y < 530 && Msg.y>460)
		{
			return 1;
		}
		/*else
		{
			mciSendString("close mymusic", NULL, 0, NULL);
			mciSendString("open music/idle2.mp3 alias mymusic", NULL, 0, NULL);
			mciSendString("play mymusic", NULL, 0, NULL);
		}*/
	}
}
void initqipan()
{
	IMAGE RABBIT;
	loadimage(&RABBIT, "qipan.jpg", 1024, 1024);
	putimage(0, 0, &RABBIT);
}
#pragma once
int luru(int x,int xx,int yy,int *p,int * ppp)
{
	
	for (int i = 0; i < 15;i++)
	{
		for (int j = 0; j < 15;j++)
		{
			/*printf("%d %d\n",abs(xx - 111 - (i * 59)), abs(yy - 74 - (j * 59)));*/
			if (abs(xx - 111 - (i * 59)) < 30 && abs(yy - 74 - (j * 59)) < 30&&qiju[i][j]==0)
			{
				printf("haha\n");
				qiju[i][j] = x;
				*p = i;
				*ppp = j;
				switch (x)
				{
				case 2:
					putimage(111 + i * 59 - 16, 74 + j * 60 - 16, &white);
					return 0;
				case 1:
					putimage(111 + i * 59 - 16, 74 + j * 60 - 16, &black);
					return 0;
				}
			}
			
		}
	}
	return -1;
			
}
void determine(int x, int y, int lian)
{
	/*printf("%d %d\n", x, y);*/
	int xx = x;
	int xxx = x;
	int xxxx = x;
	int xxxxx = x;
	int xxxxxx = x;		//copy

	int yy = y;
	int yyy = y;
	int yyyy = y;
	int yyyyy = y;
	int yyyyyy = y;
	int lianzishu = 1;
	//printf("%d %d %d %d %d %d %d %d %d %d\n",xx,xxx,xxxx,xxxxx,xxxxxx,yy,yyy,yyyy,yyyyy,yyyyyy);
	for (; (x <= 13 && qiju[x + 1][y] == lian); x++)
	{


		lianzishu = lianzishu + 1;

	}
	for (; (xx >= 1 && (qiju[xx - 1][y] == lian)); xx--)
	{
		/*printf("%d mmp\n", xx);*/
			lianzishu = lianzishu + 1;
		
	}
	if (lianzishu == 5)
	{
		printf("the  red  win!\n\n\n\n\n\n\n\n\n");

		deter = lian;
		
	}
	//printf("%d\n\n\n",lianzishu);
	lianzishu = 1;//初始化 
	//printf("%d %d %d %d %d %d %d %d %d %d\n",xx,xxx,xxxx,xxxxx,xxxxxx,yy,yyy,yyyy,yyyyy,yyyyyy);
	for (; (y <= 13 && qiju[xxx][y + 1] == lian); y++)
	{


		lianzishu = lianzishu + 1;

	}
	for (; (yy >= 1 && qiju[xxx][yy - 1] == lian); yy--)
	{

		lianzishu = lianzishu + 1;
		//lianzishu = 5;
		//		 	printf("why");
		
	}
	if (lianzishu == 5)
	{
		//		 		printf("%d\n",lianzishu);
		printf(" the red  win!\n\n\n\n\n\n\n\n\n");

		deter = lian;
	
	}
	// printf("%d %d %d %d %d %d %d %d %d %d\n",xx,xxx,xxxx,xxxxx,xxxxxx,yy,yyy,yyyy,yyyyy,yyyyyy);
	lianzishu = 1;

	for (; (xxx <= 13 && yyy <= 13 && qiju[xxx + 1][yyy + 1] == lian); xxx++, yyy++)
	{

		if (qiju[xxx + 1][yyy + 1] == lian)
			lianzishu = lianzishu + 1;

	}
	for (; (xxxx >= 1 && yyyy >= 1 && qiju[xxxx - 1][yyyy - 1] == lian); xxxx--, yyyy--)
	{

		lianzishu = lianzishu + 1;
		
	}
	if (lianzishu == 5)
	{
		printf(" the red  win!\n\n\n\n\n\n\n\n");

		deter = lian;
	}
	// printf("%d %d %d %d %d %d %d %d %d %d\n",xx,xxx,xxxx,xxxxx,xxxxxx,yy,yyy,yyyy,yyyyy,yyyyyy);
	lianzishu = 1;

	for (; (xxxxx <= 13 && yyyyy >= 1 && qiju[xxxxx + 1][yyyyy - 1] == lian); xxxxx++, yyyyy--)
	{


		lianzishu = lianzishu + 1;

	}
	for (; (xxxxxx >= 1 && yyyyyy <= 13 && qiju[xxxxxx - 1][yyyyyy + 1] == lian); xxxxxx--, yyyyyy++)
	{
		if (qiju[xxxxxx - 1][yyyyyy + 1]== lian)
			lianzishu = lianzishu + 1;
		
	}
	if (lianzishu == 5)
	{
		printf("the red  win!\n\n\n\n\n\n");

		deter = lian;
		
	}
	// printf("%d %d %d %d %d %d %d %d %d %d\n",xx,xxx,xxxx,xxxxx,xxxxxx,yy,yyy,yyyy,yyyyy,yyyyyy);
	lianzishu = 1;
	/*switch (deter)
	{
	case 1:
		printf("aaa");
		printf("white");
		break;
	case 2:
		printf("black");
	}*/
}
void initqipann()
{
	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 15; j++)
		{
			qiju[i][j] = 0;
		}
	}
}
void jiandanpanduan(int qijuscore[][15], int qijuscoredef[][15], int score_max, int score_min)
{
	int zilian = 0;
	int max = 0;
	int maxx = 0;
	int min = 0;
	int minn = 0;
	int begin, begin2;
	int end, end2;

	int ii = 0;
	int jj = 0;
	int sd[4] = { 36,36,36,36 };
	int sq[4] = { 36,36,36,36 };
	int st = M;

	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 15; j++)
		{
			ii = i;
			jj = j;
			st = M;
			max = 0;
			if (qiju[i][j] == 1 || qiju[i][j] == 2)
			{
				score_min = MIN;
			}

			begin = jj;
			for (; begin >= 0; begin--)
			{

				if (qiju[ii][begin] == 2 || zilian == 4)
				{
					break;
				}
				zilian++;
			}
			zilian = 0;

			end = jj;
			for (; end <= 14; end++)
			{

				if (qiju[ii][end] == 2 || zilian == 4)
				{
					break;
				}
				zilian++;
			}
			zilian = 0;

			for (int count1 = jj; count1 <= end; count1++)
			{

				if (count1 == jj)
				{
					continue;
				}
				if (qiju[ii][count1] == 2)
				{
					sd[0] = sd[0] / 6;
					break;
				}
				if (qiju[ii][count1] == 1)
				{
					sd[0] = sd[0] * st;
					max = max + 1;
				}
				if (qiju[ii][count1] == 0)
				{
					sd[0] = sd[0] * st;
					st = st / 3 + 1;

					max = 0;
				}

				if (max >= 4)
				{
					score_max = MAX;

				}
				zilian++;


			}


			zilian = 0;
			st = M;






			for (int count1 = jj; count1 >= begin; count1--)
			{

				if (count1 == jj)
				{
					continue;
				}
				if (qiju[ii][count1] == 2)
				{
					sd[0] = sd[0] / 6;
					break;
				}
				if (qiju[ii][count1] == 1)
				{
					sd[0] = sd[0] * st;
					max = max + 1;
				}
				if (qiju[ii][count1] == 0)
				{
					sd[0] = sd[0] * st;
					st = st / 3 + 1;

					max = 0;
				}

				if (max >= 4)
				{
					score_max = MAX;

				}
				zilian++;


			}

			max = 0;
			zilian = 0;
			st = M;







			begin = jj;
			for (; begin >= 0; begin--)
			{

				if (qiju[begin][jj] == 2 || zilian == 4)
				{
					break;
				}
				zilian++;
			}
			zilian = 0;
			end = jj;
			for (; end <= 14; end++)
			{
				if (qiju[end][jj] == 2 || zilian == 4)
				{
					break;
				}
				zilian++;
			}
			zilian = 0;
			for (int count1 = ii; count1 <= end; count1++)
			{

				if (count1 == ii)
				{
					continue;
				}
				if (qiju[count1][jj] == 2)
				{
					sd[1] = sd[1] / 6;
					break;
				}
				if (qiju[count1][jj] == 1)
				{
					sd[1] = sd[1] * st;
					max = max + 1;
				}
				if (qiju[count1][jj] == 0)
				{
					sd[1] = sd[1] * st;
					st = st / 3 + 1;
					max = 0;
				}
				zilian++;
			}
			zilian = 0;
			if (max >= 4)
			{
				score_max = MAX;
			}

			st = M;
			for (int count1 = ii; count1 >= begin; count1--)
			{

				if (count1 == ii)
				{
					continue;
				}
				if (qiju[count1][jj] == 2)
				{
					sd[1] = sd[1] / 6;
					break;
				}
				if (qiju[count1][jj] == 1)
				{
					sd[1] = sd[1] * st;
					max = max + 1;
				}
				if (qiju[count1][jj] == 0)
				{
					sd[1] = sd[1] * st;
					st = st / 3 + 1;
					max = 0;
				}
				zilian++;
			}
			zilian = 0;
			if (max >= 4)
			{
				score_max = MAX;
			}
			max = 0;
			st = M;

			begin = ii, begin2 = jj;
			for (; begin >= 0 && begin2 >= 0; begin--, begin2--)
			{

				if (qiju[begin][begin2] == 2 || zilian == 4)
				{
					break;
				}
				zilian++;
			}
			zilian = 0;
			end = ii, end2 = jj;
			for (; end <= 14 && end2 <= 14; end++, end2++)
			{
				if (qiju[end][end2] == 2 || zilian == 4)
				{
					break;
				}
				zilian++;
			}
			zilian = 0;
			for (int count1 = ii, count2 = jj; count1 <= end; count1++, count2++)
			{

				if (count1 == ii)
				{
					continue;
				}
				if (qiju[count1][count2] == 2)
				{
					sd[2] = sd[2] / 6;
					break;
				}
				if (qiju[count1][count2] == 1)
				{
					sd[2] = sd[2] * st;
					max = max + 1;
				}
				if (qiju[count1][count2] == 0)
				{
					sd[2] = sd[2] * st;
					st = st / 3 + 1;
					max = 0;
				}
				zilian++;
			}
			zilian = 0;
			st = M;
			if (max >= 4)
			{
				score_max = MAX;
			}


			for (int count1 = ii, count2 = jj; count1 >= begin; count1--, count2--)
			{

				if (count1 == ii)
				{
					continue;
				}
				if (qiju[count1][count2] == 2)
				{
					sd[2] = sd[2] / 6;
					break;
				}
				if (qiju[count1][count2] == 1)
				{
					sd[2] = sd[2] * st;
					max = max + 1;
				}
				if (qiju[count1][count2] == 0)
				{
					sd[2] = sd[2] * st;
					st = st / 3 + 1;
					max = 0;
				}
				zilian++;
			}
			zilian = 0;
			st = M;
			if (max >= 4)
			{
				score_max = MAX;
			}
			max = 0;



			begin = ii, begin2 = jj;
			for (; begin >= 0 && begin2 <= 14; begin--, begin2++)
			{

				if (qiju[begin][begin2] == 2 || zilian == 4)
				{
					break;
				}
				zilian++;
			}
			zilian = 0;

			end = ii;
			end2 = jj;
			for (; end <= 14 && end2 >= 0; end++, end2--)
			{
				if (qiju[end][end2] == 2 || zilian == 4)
				{
					break;
				}
				zilian++;
			}
			zilian = 0;

			for (int count1 = ii, count2 = jj; count1 <= end; count1++, count2--)
			{

				if (count1 == ii)
				{
					continue;
				}
				if (qiju[count1][count2] == 2)
				{
					sd[3] = sd[3] / 6;
					break;
				}
				if (qiju[count1][count2] == 1)
				{
					sd[3] = sd[3] * st;
					max = max + 1;
				}
				if (qiju[count1][count2] == 0)
				{
					sd[3] = sd[3] * st;
					st = st / 3 + 1;
					max = 0;
				}
				zilian++;
			}
			zilian = 0;
			if (max >= 4)
			{
				score_max = MAX;
			}

			st = M;




			for (int count1 = ii, count2 = jj; count1 >= begin; count1--, count2++)
			{

				if (count1 == ii)
				{
					continue;
				}
				if (qiju[count1][count2] == 2)
				{
					sd[3] = sd[3] / 6;
					break;
				}
				if (qiju[count1][count2] == 1)
				{
					sd[3] = sd[3] * st;
					max = max + 1;
				}
				if (qiju[count1][count2] == 0)
				{
					sd[3] = sd[3] * st;
					st = st / 3 + 1;
					max = 0;
				}
				zilian++;
			}
			zilian = 0;
			if (max >= 4)
			{
				score_max = MAX;
			}
			max = 0;
			st = M;




			//?


			//?
			qijuscore[i][j] = sd[0] + sd[1] + sd[3] + sd[2] + score_max + score_min;


			score_min = 0;
			score_max = 0;
			sd[0] = 36;
			sd[1] = 36;
			sd[2] = 36;
			sd[3] = 36;
		}
	}







	int zhongduan = 0;
	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 15; j++)
		{
			ii = i;
			jj = j;
			st = M;
			max = 0;
			zilian = 0;
			if (qiju[i][j] == 2 || qiju[i][j] == 1)
			{
				score_min = MIN;
			}
			begin = ii;
			for (; begin >= 0; begin--)
			{
				if (zilian == 4)
				{
					break;
				}
				zilian++;
			}
			zilian = 0;
			end = ii;
			for (; end <= 14; end++)
			{
				if (zilian == 4)
				{
					break;
				}
				zilian++;
			}
			zilian = 0;
			for (int count = begin; count <= end; count++)
			{
				if (count == ii)
				{
					continue;
				}
				if (qiju[count][jj] == 1)
				{
					sq[0] = sq[0] / 6;
					max = 0;
				}
				if (qiju[count][jj] == 0)
				{
					max = 0;
					zhongduan++;
				}
				if (qiju[count][jj] == 2)
				{
					st = M;
					sq[0] = sq[0] * st;
					max++;
				}
				if (zhongduan == 2)
				{
					zhongduan = 0;
					max = 0;
				}
				/*if (max >= 4)
				{
					score_max = MAX;
				}*/

			}
			
			max = 0;
			zhongduan = 0;
			st = M;

			begin = ii;
			for (; begin >= 0; begin--)
			{
				if (zilian == 4)
				{
					break;
				}
				zilian++;
			}
			zilian = 0;
			end = jj;
			for (; end <= 14; end++)
			{
				if (zilian == 4)
				{
					break;
				}
				zilian++;
			}
			zilian = 0;
			for (int count = begin; count <= end; count++)
			{
				if (count == jj)
				{
					continue;
				}
				if (qiju[ii][count] == 1)
				{
					sq[1] = sq[1] / 6;
					max = 0;
				}
				if (qiju[ii][count] == 0)
				{
					max = 0;
					zhongduan++;
				}
				if (qiju[ii][count] == 2)
				{
					st = M;
					sq[1] = sq[1] * st;
					max++;
				}
				if (zhongduan == 2)
				{

					zhongduan = 0;
					max = 0;

				}
				/*if (max >= 4)
				{
					score_max = MAX;
				}*/

			}
			max = 0;
			zhongduan = 0;
			st = M;
			//what





			begin = ii;
			begin2 = jj;
			for (; begin >= 0 && begin2 >= 0; begin--, begin2--)
			{
				if (zilian == 4)
				{
					break;
				}
				zilian++;
			}
			zilian = 0;
			end = ii;
			end2 = jj;
			for (; end <= 14 && end2 <= 14; end++, end2++)
			{
				if (zilian == 4)
				{
					break;
				}
				zilian++;
			}
			zilian = 0;
			for (int count = begin, count2 = begin2; count <= end; count++, count2++)
			{
				if (count == ii)
				{
					continue;
				}
				if (qiju[count][count2] == 1)
				{
					sq[2] = sq[2] / 6;
					max = 0;
				}
				if (qiju[count][count2] == 0)
				{
					max = 0;
				}
				if (qiju[count][count2] == 2)
				{
					st = M;
					sq[2] = sq[2] * st;
					max++;
				}
				if (zhongduan == 2)
				{
					zhongduan = 0;
					max = 0;

				}
				/*if (max >= 4)
				{
					score_max = MAX;
				}*/


			}
			max = 0;
			zhongduan = 0;
			st = M;





			begin = ii;
			begin2 = jj;
			for (; begin >= 0 && begin2 <= 14; begin--, begin2++)
			{
				if (zilian == 4)
				{
					break;
				}
				zilian++;
			}
			zilian = 0;
			end = ii;
			end2 = jj;
			for (; end <= 14 && end2 >= 0; end++, end2--)
			{
				if (zilian == 4)
				{
					break;
				}
				zilian++;
			}
			zilian = 0;
			for (int count = begin, count2 = begin2; count <= end; count++, count2--)
			{
				if (count == ii)
				{
					continue;
				}
				if (qiju[count][count2] == 1)
				{
					sq[3] = sq[3] / 6;
					max = 0;
				}
				if (qiju[count][count2] == 0)
				{
					max = 0;
				}
				if (qiju[count][count2] == 2)
				{
					st = M;
					sq[3] = sq[3] * st;
					max++;
				}
				if (zhongduan == 2)
				{
					zhongduan = 0;
					max = 0;

				}
				/*if (max >= 4)
				{
					score_max = MAX;
				}*/

			}
			max = 0;
			zhongduan = 0;
			st = M;

			qijuscoredef[ii][jj] = sq[0] + sq[1] + sq[2] + sq[3] + score_min + score_max;





			score_min = 0;
			score_max = 0;
			sq[0] = 36;
			sq[1] = 36;
			sq[2] = 36;
			sq[3] = 36;

		}
	}




}
void fuzapandaun(int qijuscoredef[][15], int qijummm[][15])
{
	int ii = 0;
	int jj = 0;
	int lianliankan = 0;
	int fulian = 0;
	int duandaun=1;
	for (int i = 1; i < 13; i++)
	{
		for (int j = 1; j < 13; j++)
		{
			ii = i;
			jj = j;
			for (; jj > 0 && (qiju[ii][jj - 1] == 2 || qiju[ii][jj - 1] == 1); jj--)
			{
				if (qiju[ii][jj - 1] == 2)
				{
					lianliankan++;
					if(duandaun)
					fulian++;
				}
				if (qiju[ii][jj - 1] == 1)
				{
					duandaun = 0;
					lianliankan--;
					break;
				}
			}
			ii = i;
			jj = j;
			duandaun = 1;
			for (; jj < 14 && (qiju[ii][jj + 1] == 2 || qiju[ii][jj + 1] == 1); jj++)
			{
				if (qiju[ii][jj + 1] == 2)
				{
					lianliankan++;
					if(duandaun)
					fulian++;
				}
				if (qiju[ii][jj + 1] == 1)
				{
					duandaun = 0;
					lianliankan--;
					break;
				}
			}
			duandaun = 1;
			ii = i;
			jj = j;
			if (lianliankan == 1)
			{
				qijuscoredef[ii][jj] = qijuscoredef[ii][jj] + 49152;
			}
			if (lianliankan == 2)
			{
				printf("\nx   lianliankan=%d\n", lianliankan);
				qijuscoredef[ii][jj] = qijuscoredef[ii][jj] * 2;
			}
			if (lianliankan == 3)
			{
				printf("\nx %d %d  lianliankan=%d\n", i, j, lianliankan);
				qijummm[ii][jj] = qijummm[ii][jj] + MAX / 4;
			}
			if (lianliankan == 4)
			{
				printf("\nx   lianliankan=%d\n", lianliankan);
				qijummm[ii][jj] = qijummm[ii][jj] + MAX;
			}
			if (fulian >= 4)
			{
				qijummm[ii][jj] += MAX;
			}
			lianliankan = 0;
			fulian = 0;


			ii = i;
			jj = j;
			for (; ii > 0 && (qiju[ii - 1][jj] == 2 || qiju[ii - 1][jj] == 1); ii--)
			{
				if (qiju[ii - 1][jj] == 2)
				{
					lianliankan++;
					if(duandaun)
					fulian++;
				}
				if (qiju[ii - 1][jj] == 1)
				{
					duandaun = 0;
					lianliankan--;
					break;
				}
			}
			ii = i;
			jj = j;
			duandaun = 1;
			for (; ii < 14 && (qiju[ii + 1][jj] == 2 || qiju[ii + 1][jj] == 1); ii++)
			{
				if (qiju[ii + 1][jj] == 2)
				{
					lianliankan++;
					if(duandaun)
					fulian++;
				}
				if (qiju[ii + 1][jj] == 1)
				{
					duandaun = 0;
					lianliankan--;
					break;
				}
			}
			ii = i;
			jj = j;
			duandaun = 1;
			if (lianliankan == 1)
			{
				qijuscoredef[ii][jj] = qijuscoredef[ii][jj] + 49152;
			}
			if (lianliankan == 2)
			{
				printf("y lianliankan=%d\n", lianliankan);
				qijuscoredef[ii][jj] = qijuscoredef[ii][jj] * 2;
			}
			if (lianliankan == 3)
			{
				printf("y lianliankan=%d\n", lianliankan);
				qijummm[ii][jj] = qijummm[ii][jj] + MAX / 4;
			}
			if (lianliankan == 4)
			{
				printf("y lianliankan=%d\n", lianliankan);
				qijummm[ii][jj] = qijummm[ii][jj] + MAX;
			}
			if (fulian >= 4)
			{
				qijummm[ii][jj] += MAX;
			}
			lianliankan = 0;
			fulian = 0;


			ii = i;
			jj = j;
			for (; jj > 0 && ii < 14 && (qiju[ii + 1][jj - 1] == 2 || qiju[ii + 1][jj - 1] == 1); ii++, jj--)
			{
				if (qiju[ii + 1][jj - 1] == 2)
				{
					lianliankan++;
					if (duandaun)
						fulian++;
				}
				if (qiju[ii + 1][jj - 1] == 1)
				{
					duandaun = 0;
					lianliankan--;
					break;
				}
			}
			ii = i;
			jj = j;
			duandaun = 1;
			for (; jj < 14 && ii>0 && (qiju[ii - 1][jj + 1] == 2 || qiju[ii - 1][jj + 1] == 1); ii--, jj++)
			{
				if (qiju[ii - 1][jj + 1] == 2)
				{

					lianliankan++;
					if (duandaun)
						fulian++;
				}
				if (qiju[ii - 1][jj + 1] == 1)
				{
					duandaun = 0;
					lianliankan--;
					break;
				}
			}
			ii = i;
			jj = j;
			duandaun = 1;
			if (lianliankan == 1)
			{
				qijuscoredef[ii][jj] = qijuscoredef[ii][jj] + 49152;
			}
			if (lianliankan == 2)
			{
				printf("x-y lianliankan=%d\n", lianliankan);
				qijuscoredef[ii][jj] = qijuscoredef[ii][jj] * 2;
			}
			if (lianliankan == 3)
			{
				printf("x-y lianliankan=%d\n", lianliankan);
				qijummm[ii][jj] = qijummm[ii][jj] + MAX / 4;
			}
			if (lianliankan == 4)
			{
				printf("x-y lianliankan=%d\n", lianliankan);
				qijummm[ii][jj] = qijummm[ii][jj] + MAX;
			}
			if (fulian >= 4)
			{
				qijummm[ii][jj] += MAX;
			}
			fulian = 0;
			lianliankan = 0;





			ii = i;
			jj = j;
			for (; jj > 0 && ii > 0 && (qiju[ii - 1][jj - 1] == 2 || qiju[ii - 1][jj - 1] == 1); jj--, ii--)
			{
				if (qiju[ii - 1][jj - 1] == 2)
				{
					lianliankan++;
					if (duandaun)
						fulian++;
				}
				if (qiju[ii - 1][jj - 1] == 1)
				{
					duandaun = 0;
					lianliankan--;
					break;
				}
			}
			ii = i;
			jj = j;
			duandaun = 1;
			for (; jj < 14 && ii < 14 && (qiju[ii + 1][jj + 1] == 2 || qiju[ii + 1][jj + 1] == 1); ii++, jj++)
			{
				if (qiju[ii + 1][jj + 1] == 2)
				{
					lianliankan++;
					if (duandaun)
						fulian++;
				}
				if (qiju[ii + 1][jj + 1] == 1)
				{
					duandaun = 0;
					lianliankan--;
					break;
				}
			}
			ii = i;
			jj = j;
			duandaun = 1;
			if (lianliankan == 1)
			{
				qijuscoredef[ii][jj] = qijuscoredef[ii][jj] + 49152;
			}
			if (lianliankan == 2)
			{
				qijuscoredef[ii][jj] = qijuscoredef[ii][jj] * 2;
				printf("xy lianliankan=%d\n\n", lianliankan);
			}
			if (lianliankan == 3)
			{
				qijummm[ii][jj] = qijummm[ii][jj] + MAX / 4;
				printf("xy lianliankan=%d\n\n", lianliankan);
			}
			if (lianliankan == 4)
			{
				qijummm[ii][jj] = qijummm[ii][jj] + MAX;
			}
			if (fulian >= 4)
			{
				qijummm[ii][jj] += MAX;
			}
			lianliankan = 0;
			fulian = 0;
		}
	}
}
void hefenpanduan(int type, int* xpp, int* ypp,int AI)
{
	int qijuscore[15][15] = { {0},{0},{0} };
	int qijuscoredef[15][15] = { {0},{0},{0} };
	int qijummm[15][15] = { {0},{0},{0},{0} };
	int score_max = 0;
	int score_min = 0;
	int x1x;
	int y1y;
	int scorexy;
	jiandanpanduan(qijuscore, qijuscoredef, score_max, score_min);
	if (AI)
	{
		fuzapandaun(qijuscoredef, qijummm);
	}
	scorexy = qijuscore[0][0] + qijuscoredef[0][0] + qijummm[0][0];
	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 15; j++)
		{
			if ((qijuscore[i][j] + qijuscoredef[i][j] + qijummm[i][j]) > scorexy)
			{
				scorexy = qijuscore[i][j] + qijuscoredef[i][j] + qijummm[i][j];
				x1x = i;
				y1y = j;
			}
			/*qijummm[i][j] = 0;*/
		}
	}
	*xpp = x1x;
	*ypp = y1y;
	printf("\t");
	scorexy = qijuscore[0][0] + qijuscoredef[0][0] + qijummm[0][0];
	for (int i = 0; i < 15; i++)
	{
		printf("%d\t", i);
	}
	printf("\n");
	for (int i = 0; i < 15; i++)
	{
		printf("%d\t", i);
		for (int j = 0; j < 15; j++)
		{
			printf("%d\t", qijuscore[i][j] + qijuscoredef[i][j] + qijummm[i][j]);
			//printf("%d\t", qijuscore[i][j] + qijuscoredef[i][j] /*+ qijummm[i][j]*/);
			qijummm[i][j] = 0;
		}
		printf("\n");
	}
	for (int i = 0; i < 15; i++)
	{
		printf("%d\t", i);
		for (int j = 0; j < 15; j++)
		{
			//printf("%d\t", qijuscore[i][j] + qijuscoredef[i][j] + qijummm[i][j]);
			printf("%d\t", qijuscore[i][j] + qijuscoredef[i][j] /*+ qijummm[i][j]*/);
			qijummm[i][j] = 0;
		}
		printf("\n");
	}
}
void yinyue()
{
	mciSendString("open music/break1.mp3 alias mymusic", NULL, 0, NULL);
	mciSendString("play mymusic", NULL, 0, NULL);
}
FILE* openwrite()
{
	char qianzhui[100] = "qipu\\";
	char houzhui[90] = { 0 };
	char weizhui[6] = ".txt";
	int timee = time(NULL);
	snprintf(houzhui,sizeof(houzhui), "%d", timee);
	strcat_s(qianzhui,sizeof(qianzhui),houzhui);
	strcat_s(qianzhui,sizeof(qianzhui), weizhui);

	FILE* file;
	fopen_s(&file,qianzhui, "w");
	return file;
}
void writein(int x, int y, int t,FILE* file)
{
	fprintf(file, "%d %d %d\n", x, y, t);
}
int duizhanqieru(FILE* file)
{
	while (1)
	{
		while (1)
		{
			Msg = GetMouseMsg();
			if (Msg.uMsg == WM_LBUTTONDOWN)
			{
				break;
			}
		}
		int errer;
		int xx = 0;
		int yy = 0;
		int tt = 0;
		errer=fscanf_s(file, "%d %d %d", &xx, &yy, &tt);
		if (errer != 3)
		{
			
			return tt;
		}
		qiju[xx][yy] = tt;
		switch (tt)
		{
		case 1:
			putimage(111 + xx * 59 - 16, 74 + yy * 60 - 16, &black);
			break;
		case 2:
			putimage(111 + xx * 59 - 16, 74 + yy * 60 - 16, &white);
			break;
		}
		determine(xx, yy, tt);
		if (deter != 0)
		{
			return 256;
		}
	}
}
