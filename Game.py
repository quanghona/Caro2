#-*-coding: utf-8-*-
import curses
from sys import exit
import os
from board import CaroBoard

class Game(object):
    """A class handle the Game flow and states"""
 
    def __init__(self):
        # super(Game, self).__init__()
        # State = ('Menu', 'Setting', 'Playing', 'About', 'Quit', 'Error')
        self.currentState = 'Menu'
        self.stdscr = curses.initscr()
        curses.init_pair(1, 0, 7)
        self.Board = CaroBoard(20, 20)
        self.__caro_logo = """
                     xxx    oo   xxxx     ooo  
                    x   x  o  o  x   x   o   o 
                    x     o    o x    x o     o
                    x     o    o x   x  o     o
                    x     oooooo xxxx   o     o
                    x     o    o x   x  o     o
                    x   x o    o x    x  o   o 
                     xxx  o    o x    x   ooo"""
        
    def Menu(self):
        curses.noecho()
        curses.curs_set(0)
        self.stdscr.clear()
        menu_option = ('Play Game', 
                        'Setting',
                        'About',
                        'Quit')
        # print the menu screen
        self.stdscr.addstr(0,0, self.__caro_logo, curses.A_BOLD)
        for option in range(len(menu_option)):
            self.stdscr.addstr(10 + option, 29, str(option+1) + '. ' + menu_option[option], curses.A_BOLD)
        self.stdscr.addstr(10, 29, '1. ' + menu_option[0], curses.A_BOLD | curses.color_pair(1))
                            
        # handle the input from keyboard and return the next state
        current_option, last_option = 0, 0
        while True:
            last_option = current_option
            NewState = self.stdscr.getch()
            if NewState == curses.KEY_UP:
                current_option = (current_option-1) % 4
            elif NewState == curses.KEY_DOWN:
                current_option = (current_option+1) % 4
            elif NewState == curses.KEY_ENTER or NewState == 10:
                return menu_option[current_option]
            # move to current option
            self.stdscr.addstr(last_option + 10, 29, str(last_option+1) + '. ' +\
                               menu_option[last_option], curses.A_BOLD)
            self.stdscr.addstr(current_option + 10, 29, str(current_option+1) + '. ' +\
                                menu_option[current_option], curses.A_BOLD | curses.color_pair(1))

    def Playing(self):
        def DrawFrame(width, height):
            s = '**' + "*%02d" * width % tuple(range(width)) + '*'
            self.stdscr.addstr(0, 0, str(s))
            # Draw empty board
            for i in range(height):
                s = ("%02d" % i) + ' ' * (3*width) + '*'
                self.stdscr.addstr(i + 1, 0, str(s))

            # Draw last line
            s = '*' * (3 * width + 3)
            self.stdscr.addstr(height + 1, 0, s)

        self.stdscr.clear()
        BoardSize = self.Board.GetParameters()
        # curses.resizeterm(BoardSize['Height'] + 5, BoardSize['Width']*3+4)
        DrawFrame(BoardSize['Width'], BoardSize['Height'])
        self.stdscr.refresh()
        curses.curs_set(1)
        x = y = 0
        currentMarker = 'o'
        # Playing game
        while not self.Board.CheckBoard(x, y, currentMarker) and not self.Board.CheckFull():
            currentMarker = 'x' if currentMarker == 'o' else 'o'
            self.stdscr.addstr(BoardSize['Height']+2, 0, "Player %c turn" \
                                % ('1' if currentMarker == 'x' else '2'))
            self.stdscr.refresh()
            self.stdscr.move(y+1, x*3+3)
            
            while True:
                key = self.stdscr.getch()
                if key == curses.KEY_UP:
                    y = max(0, y - 1)
                elif key == curses.KEY_DOWN:
                    y = min(y + 1, self.Board.height - 1)
                elif key == curses.KEY_LEFT:
                    x = max(0, x - 1)
                elif key == curses.KEY_RIGHT:
                    x = min(x + 1, self.Board.width - 1)
                elif key == curses.KEY_ENTER or key == 10:
                    if self.Board.UpdateBoard(x, y, currentMarker):
                        nextturn = True
                        self.stdscr.addstr(y + 1, x * 3 + 3, currentMarker)
                        self.stdscr.refresh()
                        break
                elif key == 27: #ESC
                    self.Board = CaroBoard(BoardSize['Width'], BoardSize['Height'])
                    return 'Menu'
                else: continue

                self.stdscr.addstr(BoardSize['Height']+3, 0, "%02d, %02d" % (x, y))
                self.stdscr.move(y+1, x*3+3)
        # Game over
        if self.Board.CheckFull():
            self.stdscr.addstr(BoardSize['Height']+2, 0, "Draw!")
        else:
            self.stdscr.addstr(BoardSize['Height']+2, 0, "Player %c win the game\nReplay [y/n]" \
            % ('1' if currentMarker == 'x' else '2'))
        while True: #ask the player whether to play new game
            isReplay = self.stdscr.getch()
            if isReplay == ord('y') or isReplay == ord('Y'):
                self.Board = CaroBoard(BoardSize['Width'], BoardSize['Height'])
                return 'Playing'
            elif isReplay == ord('n') or isReplay == ord('N'):
                return 'Menu'

    def Setting(self):
        """configure the width and height of the board"""
        setting_option = ('width  = ', 'height = ')
        self.stdscr.clear()
        board_size = self.Board.GetParameters()
        # print the setting screen
        self.stdscr.addstr(0,0, self.__caro_logo, curses.A_BOLD)
        self.stdscr.addstr(10,32, "Setting", curses.A_BOLD)
        for option in range(len(setting_option)):
            self.stdscr.addstr(11 + option, 29, setting_option[option])
            self.stdscr.addstr(str(board_size['Width' if option == 0 else 'Height']))
        self.stdscr.addstr(11, 29, setting_option[option], curses.A_BOLD | curses.color_pair(1))

        # handle user inputs
        current_option, last_option = 0, 0
        while True:
            last_option = current_option    
            key = self.stdscr.getch()
            if key == curses.KEY_UP:
                current_option = (current_option-1) % 2
            elif key == curses.KEY_DOWN:
                current_option = (current_option+1) % 2
            elif key == curses.KEY_LEFT:
                if current_option == 0:
                    board_size['Width'] = max(board_size['Width']-1, 0)
                else: board_size['Height'] = max(board_size['Height']-1, 0)
            elif key == curses.KEY_RIGHT:
                if current_option == 0:
                    board_size['Width'] = min(board_size['Width']+1, 22)
                else: board_size['Height'] = min(board_size['Height']+1, 22)
            elif key == curses.KEY_ENTER or key == 10:
                self.Board = CaroBoard(board_size['Width'], board_size['Height'])
                return 'Menu'
            elif key == 27:
                return 'Menu'

            self.stdscr.addstr(11 + last_option, 29, setting_option[last_option], curses.A_BOLD)
            self.stdscr.addstr(11 + current_option, 29, setting_option[current_option], curses.A_BOLD | curses.color_pair(1))
            self.stdscr.addstr(11, 29 + len(setting_option[0]), "%02d" % board_size['Width'])
            self.stdscr.addstr(12,29 + len(setting_option[1]), "%02d" % board_size['Height'])

    def About(self):
        self.stdscr.clear()
        self.stdscr.addstr(0,0,
                """Caro is a popular game for every one
The rule is very simple. 2 player play turn by turn. Each turn, a player
will choose a position to mark in. the winner is the one who has his/her
markers aligned continously vertical/horizontal/diagonal. The number of 
continous markers to win the game can be set by player.

Before playing the game, player should be setup some neccessary
information like the number of the board's width and height.
Note: The maximun number of height and width are 26 and 25 respectively.
And the number of markers to win the game have to be 5.
Player 1 Marker is O, and player 2 is X.""")

        pass    #print sample board here

        self.stdscr.getch()
        return 'Menu'

def main(args):
    myGame = Game()
    while True:
        if myGame.currentState == 'Menu':
            myGame.currentState = myGame.Menu()
        elif myGame.currentState == 'Play Game':
            myGame.currentState = myGame.Playing()
        elif myGame.currentState == 'About':
            myGame.currentState = myGame.About()
        elif myGame.currentState == 'Setting':
            myGame.currentState = myGame.Setting()
        elif myGame.currentState == 'Quit':
            exit(0)
    
if __name__ == '__main__':
    curses.wrapper(main)
