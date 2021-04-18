import curses

def main():
    curses.wrapper(curses_main)

def curses_main(w):
    w.addstr("hello world\n")

    w.refresh()
    w.getch()

main()
