#include <bangtal>
#include <time.h>
using namespace bangtal;

class Piece {
public:
	std::string imgAd;
	int pos;
	int posX, posY;
	ObjectPtr piece;
	ScenePtr scene;

	Piece(std::string img, int n, ScenePtr scene, int x, int y) : imgAd(img), pos(n), scene(scene), posX(x), posY(y) {
		piece = Object::create(img, scene, x, y);
	};

	void locate(int x, int y)
	{
		piece->locate(scene, x, y);
	}

	void hide()
	{
		piece->hide();
	}
	

};


void PosSwap(Piece& piece, Piece& new_piece)
{
	int tempX, tempY;
	int pos_temp;

	tempX = piece.posX;
	piece.posX = new_piece.posX;
	new_piece.posX = tempX;

	tempY = piece.posY;
	piece.posY = new_piece.posY;
	new_piece.posY = tempY;

	piece.locate(piece.posX, piece.posY);
	new_piece.locate(new_piece.posX, new_piece.posY);

	pos_temp = piece.pos;
	piece.pos = new_piece.pos;
	new_piece.pos = pos_temp;
}

void clickMove(Piece& piece, Piece& empty_piece, int one_line)
{
	if ((empty_piece.pos == piece.pos + one_line) ||
		(empty_piece.pos == piece.pos - one_line) ||
		(piece.pos % one_line != 0 && empty_piece.pos == piece.pos - 1) ||
		((piece.pos + 1) % one_line != 0 && empty_piece.pos == piece.pos + 1))
	{
		PosSwap(piece, empty_piece);
	}

}

void ShufflePuzzle(Piece* piece, int empty_piece, int num_piece, int one_line)
{
	srand((unsigned int)time(NULL));
	int i;
	int rn;
	int j;

	for (i = 0; i < 200; i++)
	{
		rn = rand() % 4;

		if (rn == 0)
		{
			if (piece[empty_piece].pos >= one_line)
			{
				for (j = 0; j < num_piece; j++)
				{
					if (piece[j].pos == piece[empty_piece].pos - one_line)
					{
						PosSwap(piece[empty_piece], piece[j]);
						break;
					}
				}
				continue;
			}
			else
				continue;
		}

		else if (rn == 1)
		{
			if (piece[empty_piece].pos < num_piece - one_line)
			{
				for (j = 0; j < num_piece; j++)
				{
					if (piece[j].pos == piece[empty_piece].pos + one_line)
					{
						PosSwap(piece[empty_piece], piece[j]);
						break;
					}
				}
				continue;
			}
			else
				continue;
		}

		else if (rn == 2)
		{
			if (piece[empty_piece].pos % one_line > 0)
			{
				for (j = 0; j < num_piece; j++)
				{
					if (piece[j].pos == piece[empty_piece].pos - 1)
					{
						PosSwap(piece[empty_piece], piece[j]);
						break;
					}
				}
				continue;
			}
			else
				continue;
		}

		else if (rn == 3)
		{
			if (piece[empty_piece].pos % one_line < one_line - 1)
			{
				for (j = 0; j < num_piece; j++)
				{
					if (piece[j].pos == piece[empty_piece].pos + 1)
					{
						PosSwap(piece[empty_piece], piece[j]);
						break;
					}
				}
				continue;
			}
			else
				continue;
		}
	}
}

void CheckPuzzle(Piece* piece, int num_piece, TimerPtr& timer, ObjectPtr& completePuzzle, ObjectPtr& shuffle, clock_t start, ObjectPtr& record_button, std::string& record)
{
	int i;
	for (i = 0; i < num_piece; i++)
	{
		if (piece[i].pos != i)
			return;
	}

	timer->stop();
	completePuzzle->show();
	shuffle->hide();
	record_button->show();
	
	clock_t end = clock();
	int sec = (int)(end - start) / CLOCKS_PER_SEC;
	int min = sec / 60;
	sec %= 60;

	std::string min_s = std::to_string(min);
	std::string sec_s = std::to_string(sec);
	record = "소요 시간: " + min_s + "분 " + sec_s + "초";

	return;
}


int main()
{
	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);

	auto front = Scene::create("시작", "Images/시작화면.png");
	auto home = Scene::create("게임 선택", "Images/홈화면.png");
	auto scene = Scene::create("4 X 4", "Images/짱구빈배경.jpg");
	auto scene2 = Scene::create("3 X 3", "Images/짱구야구빈배경.jpg");

	const int one_line = 4;
	const int num_piece = one_line * one_line;
	const int empty_piece = num_piece - 1;
	const int piece_size = 150;
	const int piece1_posX = 300;
	const int piece1_posY = 470;
	const double playtime = 120.0;

	const int one_line2 = 3;
	const int num_piece2 = one_line2 * one_line2;
	const int empty_piece2 = num_piece2 - 1;
	const int piece_size2 = 200;
	const int piece1_posX2 = 300;
	const int piece1_posY2 = 420;
	const double playtime2 = 210.0;

	Piece piece[16] = {
		{"Images/짱구1.png", 0, scene, piece1_posX, piece1_posY},
		{"Images/짱구2.png", 1, scene, piece1_posX + piece_size, piece1_posY},
		{"Images/짱구3.png", 2, scene, piece1_posX + piece_size * 2, piece1_posY},
		{"Images/짱구4.png", 3, scene, piece1_posX + piece_size * 3, piece1_posY},
		{"Images/짱구5.png", 4, scene, piece1_posX, piece1_posY - piece_size},
		{"Images/짱구6.png", 5, scene, piece1_posX + piece_size, piece1_posY - piece_size},
		{"Images/짱구7.png", 6, scene, piece1_posX + piece_size * 2, piece1_posY - piece_size},
		{"Images/짱구8.png", 7, scene, piece1_posX + piece_size * 3, piece1_posY - piece_size},
		{"Images/짱구9.png", 8, scene, piece1_posX, piece1_posY - piece_size * 2},
		{"Images/짱구10.png", 9, scene, piece1_posX + piece_size, piece1_posY - piece_size * 2},
		{"Images/짱구11.png", 10, scene, piece1_posX + piece_size * 2, piece1_posY - piece_size * 2},
		{"Images/짱구12.png", 11, scene, piece1_posX + piece_size * 3, piece1_posY - piece_size * 2},
		{"Images/짱구13.png", 12, scene, piece1_posX, piece1_posY - piece_size * 3},
		{"Images/짱구14.png", 13, scene, piece1_posX + piece_size, piece1_posY - piece_size * 3},
		{"Images/짱구15.png", 14, scene, piece1_posX + piece_size * 2, piece1_posY - piece_size * 3},
		{"Images/짱구16.png", 15, scene, piece1_posX + piece_size * 3, piece1_posY - piece_size * 3},
	};

	Piece piece2[9] = {
		{"Images/짱구야구1.png", 0, scene2, piece1_posX2, piece1_posY2},
		{"Images/짱구야구2.png", 1, scene2, piece1_posX2 + piece_size2, piece1_posY2},
		{"Images/짱구야구3.png", 2, scene2, piece1_posX2 + piece_size2 * 2, piece1_posY2},
		{"Images/짱구야구4.png", 3, scene2, piece1_posX2, piece1_posY2 - piece_size2},
		{"Images/짱구야구5.png", 4, scene2, piece1_posX2 + piece_size2, piece1_posY2 - piece_size2},
		{"Images/짱구야구6.png", 5, scene2, piece1_posX2 + piece_size2 * 2, piece1_posY2 - piece_size2},
		{"Images/짱구야구7.png", 6, scene2, piece1_posX2, piece1_posY2 - piece_size2 * 2},
		{"Images/짱구야구8.png", 7, scene2, piece1_posX2 + piece_size2, piece1_posY2 - piece_size2 * 2},
		{"Images/짱구야구9.png", 8, scene2, piece1_posX2 + piece_size2 * 2, piece1_posY2 - piece_size2 * 2},
	};

	auto gamestart = Object::create("Images/gamestart.png", front, 565, 45);
	auto select1 = Object::create("Images/select.png", home, 183, 120);
	auto select2 = Object::create("Images/select.png", home, 950, 120);

	auto completePuzzle = Object::create("Images/짱구배경.png", scene, 0, 0);
	auto completePuzzle2 = Object::create("Images/짱구야구배경.png", scene2, 0, 0);

	auto fail = Object::create("Images/실패.png", scene, 0, 0, false);
	auto fail2 = Object::create("Images/실패2.png", scene2, 0, 0, false);

	auto start = Object::create("Images/start.png", scene, 550, 100);
	auto start2 = Object::create("Images/start.png", scene2, 550, 100);
	auto restart = Object::create("Images/restart.png", scene, 1000, 310, false);
	auto restart2 = Object::create("Images/restart.png", scene2, 1000, 310, false);
	auto end = Object::create("Images/end.png", scene, 1000, 210, false);
	auto end2 = Object::create("Images/end.png", scene2, 1000, 210, false);
	auto shuffle = Object::create("Images/shuffle.png", scene, 100, 260, false);
	auto shuffle2 = Object::create("Images/shuffle.png", scene2, 100, 260, false);
	auto record = Object::create("Images/record.png", scene, 1000, 110, false);
	auto record2 = Object::create("Images/record.png", scene2, 1000, 110, false);
	auto change = Object::create("Images/change.png", scene, 1000, 430, false);
	auto change2 = Object::create("Images/change.png", scene2, 1000, 430, false);

	auto timer = Timer::create(300.f);


	clock_t start_time;
	std::string recordT = "\0";

	gamestart->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		home->enter();

		return true;
		});

	select1->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		scene2->enter();
		timer->set(playtime);
		showTimer(timer);
		fail2->hide();

		return true;
		});

	select2->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		scene->enter();
		timer->set(playtime2);
		showTimer(timer);
		fail->hide();

		return true;
		});


	start->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		start->hide();
		restart->show();
		end->show();
		shuffle->show();
		change->show();

		completePuzzle->hide();
		fail->hide();

		piece[empty_piece].hide();

		ShufflePuzzle(piece, empty_piece, num_piece, one_line);

		timer->start();

		start_time = clock();

		return true;
		});

	start2->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		start2->hide();
		restart2->show();
		end2->show();
		shuffle2->show();
		change2->show();

		completePuzzle2->hide();
		fail2->hide();

		piece2[empty_piece2].hide();

		ShufflePuzzle(piece2, empty_piece2, num_piece2, one_line2);

		timer->start();

		start_time = clock();

		return true;
		});

	restart->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		start->show();
		restart->hide();
		end->hide();
		shuffle->hide();
		change->hide();
		record->hide();
		
		fail->hide();
		completePuzzle->show();

		timer->stop();
		timer->set(playtime2);

		return true;
		});

	restart2->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		start2->show();
		restart2->hide();
		end2->hide();
		shuffle2->hide();
		change2->hide();
		record2->hide();
		
		fail2->hide();
		completePuzzle2->show();

		timer->stop();
		timer->set(playtime);

		return true;
		});

	shuffle->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		ShufflePuzzle(piece, empty_piece, num_piece, one_line);

		return true;
		});

	shuffle2->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		ShufflePuzzle(piece2, empty_piece2, num_piece2, one_line2);

		return true;
		});
	
	record->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		showMessage(recordT);

		return true;
		});

	record2->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		showMessage(recordT);

		return true;
		});

	change->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		timer->stop();
		timer->set(playtime2);
		home->enter();

		shuffle->hide();
		change->hide();
		end->hide();
		restart->hide();
		start->show();
		record->hide();

		completePuzzle->show();

		return true;
		});

	change2->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		timer->stop();
		timer->set(playtime);
		home->enter();

		shuffle2->hide();
		change2->hide();
		end2->hide();
		restart2->hide();
		start2->show();
		record2->hide();

		completePuzzle2->show();

		return true;
		});

	end->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		endGame();
		return true;
		});

	end2->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		endGame();
		return true;
		});
	
	int i;
	for (i = 0; i < num_piece; i++)
	{
		piece[i].piece->setOnMouseCallback([&, i](ObjectPtr object, int x, int y, MouseAction action)->bool {
			clickMove(piece[i], piece[empty_piece], one_line);
			CheckPuzzle(piece, num_piece, timer, completePuzzle, shuffle, start_time, record, recordT);

			return true;
			});
	}

	for (i = 0; i < num_piece2; i++)
	{
		piece2[i].piece->setOnMouseCallback([&, i](ObjectPtr object, int x, int y, MouseAction action)->bool {
			clickMove(piece2[i], piece2[empty_piece2], one_line2);
			CheckPuzzle(piece2, num_piece2, timer, completePuzzle2, shuffle2, start_time, record2, recordT);

			return true;
			});
	}


	timer->setOnTimerCallback([&](TimerPtr timer)->bool {
		fail->show();
		fail2->show();
		shuffle->hide();
		shuffle2->hide();

		return true;
		});

	startGame(front);

	return 0;
}
