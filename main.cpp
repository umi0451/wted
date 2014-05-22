#include <chthon2/map.h>
#include <ncurses.h>
#include <cstdlib>

int main()
{
	srand(time(NULL));
	initscr();
	raw();
	keypad(stdscr, TRUE);
	noecho();
	curs_set(0);

	Chthon::Map<char> map(25, 25, '.');
	for(int i = 0; i < 250; ++i) {
		map.cell(rand() % 25, rand() % 25) = '#';
	}
	for(int i = 0; i < 25; ++i) {
		map.cell(rand() % 25, rand() % 25) = '*';
	}

	Chthon::Point player(rand() % 25, rand() % 25);
	int tries = 625;
	while(map.cell(player) == '#' && tries --> 0) {
		player = Chthon::Point(rand() % 25, rand() % 25);
	}

	int money = 0;
	bool quit = false;
	while(!quit) {
		mvprintw(0, 0, "Money: %d", money);
		for(int x = -2; x <= 2; ++x) {
			for(int y = -2; y <= 2; ++y) {
				Chthon::Point pos = player + Chthon::Point(x, y);
				if(map.valid(pos)) {
					mvaddch(13 + y, 35 + x, map.cell(pos));
				} else {
					mvaddch(13 + y, 35 + x, ' ');
				}
			}
		}
		mvaddch(13, 35, '@');

		char control = getch();
		Chthon::Point shift;
		switch(control) {
			case 'q' : quit = true; break;
			case 'h' : shift = Chthon::Point(-1,  0); break;
			case 'j' : shift = Chthon::Point( 0,  1); break;
			case 'k' : shift = Chthon::Point( 0, -1); break;
			case 'l' : shift = Chthon::Point( 1,  0); break;
			case 'y' : shift = Chthon::Point(-1, -1); break;
			case 'u' : shift = Chthon::Point( 1, -1); break;
			case 'b' : shift = Chthon::Point(-1,  1); break;
			case 'n' : shift = Chthon::Point( 1,  1); break;
			case 'm' :
			{
				erase();
				for(int x = 0; x < map.width(); ++x) {
					for(int y = 0; y < map.height(); ++y) {
						mvaddch(y, x, map.cell(x, y));
					}
				}
				mvaddch(player.y, player.x, '@');
				while(getch() != 'm');
				erase();
				break;
			}
		}
		if(map.valid(player + shift) && map.cell(player + shift) != '#') {
			player += shift;
		}
		if(map.cell(player) == '*') {
			money += 100;
			map.cell(player) = '.';
		}
	}

	cbreak();
	echo();
	curs_set(1);
	endwin();
	return 0;
}
