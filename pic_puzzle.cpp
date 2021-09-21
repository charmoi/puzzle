#include <bangtal>
#include <cstdlib>
#include <ctime>
#include <Windows.h>

using namespace bangtal;

ScenePtr front;
ScenePtr home;
ScenePtr game4x4;
ScenePtr game3x3;

ObjectPtr board4x4[16], board4x4_default[16];
ObjectPtr board3x3[9], board3x3_default[9];

ObjectPtr gamestart;
ObjectPtr select1;
ObjectPtr select2;
ObjectPtr start;
ObjectPtr start2;
ObjectPtr restart;
ObjectPtr restart2;
ObjectPtr end;
ObjectPtr end2;
ObjectPtr shuffle;
ObjectPtr shuffle2;
ObjectPtr record;
ObjectPtr record2;
ObjectPtr change;
ObjectPtr change2;
ObjectPtr back;
ObjectPtr back2;

ObjectPtr completePuzzle;
ObjectPtr completePuzzle2;
//ObjectPtr fail;
//ObjectPtr fail2;

//TimerPtr timer;
TimerPtr t1;
TimerPtr t2;

TimerPtr timer_shuffle_4x4;
TimerPtr timer_shuffle_3x3;

struct Gameset
{
	int single_line;
	int num_of_piece;
	int blank_piece;
	int size_of_piece;
	int P1_posX;
	int P1_posY;
	float playtime;
} gameset4x4, gameset3x3;


int GetIndex(ObjectPtr piece, ObjectPtr* game_board, int num_piece)
{
	for (int i = 0; i < num_piece; i++)
		if (game_board[i] == piece) return i;
	return -1;
}

int IndexToX(int index, Gameset gameset) {
	return gameset.P1_posX + gameset.size_of_piece * (index % gameset.single_line);
}

int IndexToY(int index, Gameset gameset) {
	return gameset.P1_posY - gameset.size_of_piece * (index / gameset.single_line);
}

void MovePiece(int index, ObjectPtr* game_board, ScenePtr& scene, Gameset& gameset) {
	auto temp = game_board[index];
	game_board[index] = game_board[gameset.blank_piece];
	game_board[index]->locate(scene, IndexToX(index, gameset), IndexToY(index, gameset));
	game_board[gameset.blank_piece] = temp;
	game_board[gameset.blank_piece]->locate(scene, IndexToX(gameset.blank_piece, gameset), IndexToY(gameset.blank_piece, gameset));
	gameset.blank_piece = index;
}

bool CheckMove(int index, Gameset gameset) {
	if (gameset.blank_piece == index + gameset.single_line) return true;
	if (gameset.blank_piece == index - gameset.single_line) return true;
	if (index % gameset.single_line != 0 && gameset.blank_piece == index - 1) return true;
	if ((index+1) % gameset.single_line != 0 && gameset.blank_piece == index + 1) return true;

	return false;
}

void Shuffle(ObjectPtr* game_board, ScenePtr& scene, Gameset& gameset) {
	int index;
	
	index = rand() % gameset.num_of_piece;
	while (!CheckMove(index, gameset)) {
			index = rand() % gameset.num_of_piece;
	}

	MovePiece(index, game_board, scene, gameset);
		
}

bool CheckComplete(ObjectPtr* game_board, ObjectPtr* default_board, int num_of_piece) {
	for (int i = 0; i < num_of_piece; i++) {
		if (game_board[i] != default_board[i]) return false;
	}
	return true;
}

void InitPuzzle(ObjectPtr* game_board, ObjectPtr* default_board, Gameset& gameset, ScenePtr scene) {
	for (int i = 0; i < gameset.num_of_piece; i++)
	{
		game_board[i] = default_board[i];
		game_board[i]->locate(scene, IndexToX(i, gameset), IndexToY(i, gameset));
	}

	gameset.blank_piece = gameset.num_of_piece - 1;
	game_board[gameset.blank_piece]->hide();
}

void init_game() {
	front = Scene::create("시작", "Images/시작화면.png");
	home = Scene::create("게임 선택", "Images/홈화면.png");
	game4x4 = Scene::create("4 X 4", "Images/짱구빈배경.jpg");
	game3x3 = Scene::create("3 X 3", "Images/짱구야구빈배경.jpg");

	gameset4x4.single_line = 4;
	gameset4x4.num_of_piece = 16;
	gameset4x4.blank_piece = 15;
	gameset4x4.size_of_piece = 150;
	gameset4x4.P1_posX = 300;
	gameset4x4.P1_posY = 470;
	gameset4x4.playtime = 210.0;

	gameset3x3.single_line = 3;
	gameset3x3.num_of_piece = 9;
	gameset3x3.blank_piece = 8;
	gameset3x3.size_of_piece = 200;
	gameset3x3.P1_posX = 300;
	gameset3x3.P1_posY = 420;
	gameset3x3.playtime = 120.0;

	float shuffle_interval = 0.01f;
	int shuffle_count = 150;

	//clock_t start_time;
	std::string record_message_4x4 = "기록 없음";
	std::string record_message_3x3 = "기록 없음";
	int time_record = 0;
	int best_record_4x4 = 0;
	int best_record_3x3 = 0;

	char path[30];
	for (int i = 0; i < gameset4x4.num_of_piece; i++) {
		sprintf_s(path, sizeof(path), "Images/짱구%d.png", i + 1);
		board4x4[i] = Object::create(path, game4x4, IndexToX(i, gameset4x4), IndexToY(i, gameset4x4));
		board4x4[i]->setOnMouseCallback([&](auto piece, auto x, auto y, auto action)->bool {
			int index = GetIndex(piece, board4x4, gameset4x4.num_of_piece);
			if (CheckMove(index, gameset4x4)) {
				MovePiece(index, board4x4, game4x4, gameset4x4);
				if (CheckComplete(board4x4, board4x4_default, gameset4x4.num_of_piece)) {
					t1->stop();

					if (best_record_4x4 == 0 || time_record < best_record_4x4)
						best_record_4x4 = time_record;
					int sec = best_record_4x4;
					int min = sec / 60;
					sec %= 60;

					record_message_4x4 = "최고 기록: " + std::to_string(min) + "분 " + std::to_string(sec) + "초";
					
					//timer->stop();

					//clock_t end = clock();
					//int sec = (int)(end - start_time) / CLOCKS_PER_SEC;
					//int min = sec / 60;
					//sec %= 60;

					//std::string min_s = std::to_string(min);
					//std::string sec_s = std::to_string(sec);
					//record_message = "소요 시간: " + min_s + "분 " + sec_s + "초";

					completePuzzle->show();
					shuffle->hide();
					record->show();

					showMessage("완성~!");
				}
			}

			return true;
			});
		board4x4_default[i] = board4x4[i];
	}

	for (int i = 0; i < gameset3x3.num_of_piece; i++) {
		sprintf_s(path, sizeof(path), "Images/짱구야구%d.png", i + 1);
		board3x3[i] = Object::create(path, game3x3, IndexToX(i, gameset3x3), IndexToY(i, gameset3x3));
		board3x3[i]->setOnMouseCallback([&](auto piece, auto x, auto y, auto action)->bool {
			int index = GetIndex(piece, board3x3, gameset3x3.num_of_piece);
			if (CheckMove(index, gameset3x3)) {
				MovePiece(index, board3x3, game3x3, gameset3x3);
				if (CheckComplete(board3x3, board3x3_default, gameset3x3.num_of_piece)) {
					t1->stop();

					if (best_record_3x3 == 0 || time_record < best_record_3x3)
						best_record_3x3 = time_record;
					int sec = best_record_3x3;
					int min = sec / 60;
					sec %= 60;

					record_message_3x3 = "최고 기록: " + std::to_string(min) + "분 " + std::to_string(sec) + "초";

					//timer->stop();
					
					//clock_t end = clock();
					//int sec = (int)(end - start_time) / CLOCKS_PER_SEC;
					//int min = sec / 60;
					//sec %= 60;

					//std::string min_s = std::to_string(min);
					//std::string sec_s = std::to_string(sec);
					//record_message = "소요 시간: " + min_s + "분 " + sec_s + "초";

					completePuzzle2->show();
					shuffle2->hide();
					record2->show();

					showMessage("완성~!");
				}
			}

			return true;
			});
		board3x3_default[i] = board3x3[i];
	}


	gamestart = Object::create("Images/gamestart.png", front, 565, 45);
	select1 = Object::create("Images/select.png", home, 183, 120);
	select2 = Object::create("Images/select.png", home, 950, 120);

	completePuzzle = Object::create("Images/짱구배경.png", game4x4, 0, 0);
	completePuzzle2 = Object::create("Images/짱구야구배경.png", game3x3, 0, 0);

	//fail = Object::create("Images/실패.png", game4x4, 0, 0, false);
	//fail2 = Object::create("Images/실패2.png", game3x3, 0, 0, false);

	start = Object::create("Images/start.png", game4x4, 550, 100);
	start2 = Object::create("Images/start.png", game3x3, 550, 100);
	restart = Object::create("Images/restart.png", game4x4, 1000, 310, false);
	restart2 = Object::create("Images/restart.png", game3x3, 1000, 310, false);
	end = Object::create("Images/end.png", game4x4, 1000, 210, false);
	end2 = Object::create("Images/end.png", game3x3, 1000, 210, false);
	shuffle = Object::create("Images/shuffle.png", game4x4, 100, 260, false);
	shuffle2 = Object::create("Images/shuffle.png", game3x3, 100, 260, false);
	record = Object::create("Images/record.png", game4x4, 750, 100);
	record2 = Object::create("Images/record.png", game3x3, 750, 100);
	change = Object::create("Images/change.png", game4x4, 1000, 430, false);
	change2 = Object::create("Images/change.png", game3x3, 1000, 430, false);
	back = Object::create("Images/back.png", game4x4, 350, 100);
	back2 = Object::create("Images/back.png", game3x3, 350, 100);

	//timer = Timer::create(300.f);
	//timer->setOnTimerCallback([&](TimerPtr timer)->bool {
	//	fail->show();
	//	fail2->show();
	//	shuffle->hide();
	//	shuffle2->hide();

	//	return true;
	//	});

	t1 = Timer::create(1.f);
	t1->setOnTimerCallback([&](TimerPtr)->bool {
		t2->increase(1.f);
		time_record++;

		t1->set(1.f);
		t1->start();
		
		return true;
		});

	t2 = Timer::create(0.f);

	timer_shuffle_4x4 = Timer::create(shuffle_interval);
	timer_shuffle_4x4->setOnTimerCallback([&](auto)->bool {
		Shuffle(board4x4, game4x4, gameset4x4);

		shuffle_count--;
		if (shuffle_count > 0) {
			timer_shuffle_4x4->set(shuffle_interval);
			timer_shuffle_4x4->start();
		}
		else {
			//start_time = clock();
			shuffle_count = 200;
			//timer->start();
			
			t1->start();

			restart->show();
			end->show();
			shuffle->show();
			change->show();
			record->show();
		}
		
		return true;
		});

	timer_shuffle_3x3 = Timer::create(shuffle_interval);
	timer_shuffle_3x3->setOnTimerCallback([&](auto)->bool {
		Shuffle(board3x3, game3x3, gameset3x3);

		shuffle_count--;
		if (shuffle_count > 0) {
			timer_shuffle_3x3->set(shuffle_interval);
			timer_shuffle_3x3->start();
		}
		else {
			//start_time = clock();
			shuffle_count = 150;
			//timer->start();
			
			t1->start();

			restart2->show();
			end2->show();
			shuffle2->show();
			change2->show();
			record2->show();
		}

		return true;
		});


	gamestart->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		home->enter();

		return true;
		});

	select1->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		game3x3->enter();
		//timer->set(gameset3x3.playtime);
		//showTimer(timer);
		showTimer(t2);
		//fail2->hide();

		return true;
		});

	select2->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		game4x4->enter();
		//timer->set(gameset4x4.playtime);
		//showTimer(timer);
		showTimer(t2);
		//fail->hide();

		return true;
		});


	start->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		InitPuzzle(board4x4, board4x4_default, gameset4x4, game4x4);
		
		start->hide();
		back->hide();
		record->hide();
		completePuzzle->hide();
		//fail->hide();
		record->locate(game4x4, 1000, 110);

		time_record = 0;
		t1->set(1.f);
		t2->set(0.f);

		shuffle_count = 200;
		timer_shuffle_4x4->start();

		return true;
		});

	start2->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		InitPuzzle(board3x3, board3x3_default, gameset3x3, game3x3);

		start2->hide();
		back2->hide();
		record2->hide();
		completePuzzle2->hide();
		//fail2->hide();
		record2->locate(game3x3, 1000, 110);

		time_record = 0;
		t1->set(1.f);
		t2->set(0.f);

		shuffle_count = 150;
		timer_shuffle_3x3->start();

		return true;
		});

	back->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		home->enter();

		return true;
		});

	back2->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		home->enter();

		return true;
		});

	restart->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		start->show();
		back->show();
		restart->hide();
		end->hide();
		shuffle->hide();
		change->hide();
		record->locate(game4x4, 750, 100);

		//fail->hide();
		completePuzzle->show();

		timer_shuffle_4x4->stop();

		//timer->stop();
		//timer->set(gameset4x4.playtime);

		t1->stop();

		return true;
		});

	restart2->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		start2->show();
		back2->show();
		restart2->hide();
		end2->hide();
		shuffle2->hide();
		change2->hide();
		record2->locate(game3x3, 750, 100);

		//fail2->hide();
		completePuzzle2->show();

		timer_shuffle_3x3->stop();

		//timer->stop();
		//timer->set(gameset3x3.playtime);

		t1->stop();

		return true;
		});

	shuffle->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		shuffle_count = 150;
		timer_shuffle_4x4->start();

		return true;
		});

	shuffle2->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		shuffle_count = 150;
		timer_shuffle_3x3->start();

		return true;
		});

	record->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		showMessage(record_message_4x4);

		return true;
		});

	record2->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		showMessage(record_message_3x3);

		return true;
		});

	change->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		timer_shuffle_4x4->stop();

		//timer->stop();
		//timer->set(gameset4x4.playtime);
		t1->stop();

		home->enter();

		shuffle->hide();
		change->hide();
		end->hide();
		restart->hide();
		start->show();
		back->show();
		record->locate(game4x4, 750, 100);

		completePuzzle->show();

		return true;
		});

	change2->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
		timer_shuffle_3x3->stop();

		//timer->stop();
		//timer->set(gameset3x3.playtime);
		t1->stop();

		home->enter();

		shuffle2->hide();
		change2->hide();
		end2->hide();
		restart2->hide();
		start2->show();
		back2->show();
		record2->locate(game3x3, 750, 100);

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

	startGame(front);
}

int main()
{
	srand((unsigned int)time(NULL));

	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);

	init_game();

	return 0;
}
