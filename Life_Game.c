#include<GL/glut.h>
#include<stdio.h>
#define _USE_MATH_DEFINES
#include<math.h>
#include<Windows.h>


//視点のZ座標
#define ZV 500.0

double angle=0.0; //コールバック関数で使用する変数はグローバル変数である必要がある
unsigned int interval = 20;//20ミリ秒ごとにmyTimerを呼び出す

//タイマーの番号
int no=1;

//フィールドの最大のセル
#define N 400

int curWidth  = N*2;
int curHeight = N*2;



//正セルの個数
int n = 0;

//セルの最大のマス
#define MAX_CELL 100

unsigned int color = 0x00;

//セルの情報を格納する配列
int cell[N][N] = {};
int newCell[N][N] = {};

//フィールド
int field[N][N]={};

//セルをアップデートする関数
void Update()
{

	for(int i=1;i<N-1;i++)
	{
		for(int j=1;j<N-1;j++)
		{
			//周囲のセルの数を求める
			int count = cell[i-1][j-1]+
						cell[i-1][j  ]+
						cell[i-1][j+1]+
						cell[i  ][j-1]+
						cell[i  ][j+1]+
						cell[i+1][j-1]+
						cell[i+1][j  ]+
						cell[i+1][j+1];
			//セルの決定
			//死のセルかつcount == 3の時,誕生させる
			if(cell[i][j] == 0 && count == 3)
			{
				newCell[i][j] = 1;
				
			}//if(cell[i][j] == 0 && count == 3)
			//生のセルの周りに2,3つの生のセルがあれば、そのまま
			else if(cell[i][j] == 1 && (count == 2 || count == 3))
			{
				newCell[i][j] = cell[i][j];
				
			}//else if(cell[i][j] == 1 && (count == 2 || count == 3))
			//淘汰
			else if(cell[i][j] == 1 && !(count == 2 || count == 3))
			{
				newCell[i][j] = 0;
				
			}//else if(cell[i][j] == 1 && !(count == 2 || count == 3))
			
		}//for(int j=1;j<N-1;j++)
	}//for(int i=1;i<N-1;i++)
	
	
	//セルの変更が終わったらcellへnewCellを代入する
	for(int i=0;i<N;i++)
	{
		for(int j=0;j<N;j++)
		{
			cell[i][j] = newCell[i][j];
		}
	}
}

//四角形を描画する関数
void DrawSquare(int x1,int y1,int x2,int y2)
{
	//四角形の描画
	glBegin(GL_QUADS);
	glVertex2i(x1,y1);
	glVertex2i(x2,y1);
	glVertex2i(x2,y2);
	glVertex2i(x1,y2);
	glEnd();
}//void DrawSquare(int x1,int y1,int x2,int y2)


void Draw()
{
	//生のセルの数
	int aliveCell = 0;
	//4x4の正方形を描画
	for(int y = 0;y<N;y++)
	{
		for(int x=0;x<N;x++)
		{
		
			//printf("for %d,%d\n",x + N/2,y + N/2);
			
			//座標の取得
			if(cell[y][x] == 1)
			{
				field[aliveCell][0] = x;
				field[aliveCell][1] = y;
				aliveCell++;
			}
			else
			{
				continue;
			}
			
			
			//四角形の描画
			for(int i=0;i<aliveCell;i++)
			{
				color = 0xff;
				//座標の設定
				int width = 4;
				int x1 = 4*field[i][0] + N/2;
				int y1 = 4*field[i][1] + N/2;
				
				int x2 = x1 + width;
				int y2 = y1 + width;
				//色の変更
				glColor3ub(0x00,color,color);
				//四角形の描画
				DrawSquare(x1, y1, x2, y2);
			}//for(int i=0;i<aliveCell;i++)
		}//for(int x=0;x<N;x++)
	}//for(int y = 0;y<N;y++)
	
}//void Draw()

void myTimer(int num)
{
	if(num==1)
	{
		//セルのアップデート
		Update();
		
		glutTimerFunc(interval, myTimer,1);
	}
	//再描画
	glutPostRedisplay();
	
}//void myTimer(int num)

//描画を行うコールバック
void myDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//描画内容の消去
	glColor3d(1.0,1.0,1.0); //描画の色の設定(白)

	//描画をする
	Draw();
	
	//バッファを切り替える
	glutSwapBuffers();
}

//ウィンドウ再生成を行うコールバック関数
void myReshape(int width,int height)//width, height  :ウィンドウの幅と高さ
{
	curWidth = width;
	curHeight = height;
	glViewport(0,0, width, height);//描画領域の設定（ここではウィンドウ全体を指定）
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity(); //変換行列の初期化（単位行列）
	//ビューポートに描画する座標の範囲の縦横比をウィンドウの大きさに合わせて指定
	glOrtho(N*2,N*3, N*2, N*3,-1.0,1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();//変換行列の初期化(単位行列)
}


int main(int argc, char* argv[])
{
	glutInit( &argc, argv );            // glutの初期化
	glutInitDisplayMode( GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);   // 色表現の設定（赤緑青＋透明度）
	
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	
	glutInitWindowSize( 500, 500 );     // ウィンドウの大きさの設定
	glutInitWindowPosition( 100, 100 ); // ウィンドウの位置の設定
	glutCreateWindow( argv[0] );        // ウィンドウの生成（argv[0]は実行プログラムの名前）
	glutReshapeFunc(myReshape);      //ウィンドウの再生成を行うコールバック関数の設定
	
	FILE * fp;
	//	読み込みモードでファイルを開く
	
	fp = fopen64(argv[1], "r");
	
	if(fp == NULL)
	{
	printf("ファイルはありません\n");
		return -1;
	}
	
	int x=0,y=0;
	
	fscanf(fp,"%d",&n);
	

	//生のセルを入力
	for(int i=0;i<n;i++)
	{
		fscanf(fp,"%d,%d",&x,&y);
		cell[y + N/2][x + N/2] = 1;
		newCell[y + N/2][x + N/2] = 1;
	}
	
	fclose(fp);

	
	glutDisplayFunc( myDisplay );       // 描画を行うコールバック関数の設定
	//キーボード入力
	//glutKeyboardFunc(myKeyboard);// void (* callback)( unsigned char, int, int ) 
	
	glutTimerFunc(interval,myTimer,no);//20ミリ秒後にTimer関数を開始して,valueは1

	glClearColor( 0.0, 0.0, 0.0, 0.0 ); // 画面消去時の色の設定（黒）
	glutMainLoop();                     // イベント処理ループ
	return 0;
}
