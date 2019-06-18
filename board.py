import os
import curses
import random

class CaroBoard(object):
	"""A class handle everything realted to the caro board"""
	def __init__(self, width, height):
		self.width = width
		self.height = height
		self.board = [[' ' for x in range(self.width)] for y in range(self.height)]
		self._turnCount = 0
	
	def ConfigBoard(self):
			pass	

	def UpdateBoard(self, Pos_X, Pos_Y, Marker):
		if self.board[Pos_X][Pos_Y] == ' ':
			self.board[Pos_X][Pos_Y] = Marker
			self._turnCount += 1
			return True
		else: return False

	def CheckBoard(self, Pos_X, Pos_Y, PlayerMarker):
		CheckProcess = 'UpperLeft'
		Combo = 1
		CurrentCheckPosX, CurrentCheckPosY = Pos_X, Pos_Y
		# Checking if the current player has won the game
		# this is written for python 2 so it doesn't support nonlocal keyword
		while CheckProcess != 'Complete':
			if CheckProcess == 'UpperLeft':
				if CurrentCheckPosX - 1 >= 0 and CurrentCheckPosY - 1 >= 0 and \
				 self.board[max(0, CurrentCheckPosX - 1)][max(0, CurrentCheckPosY - 1)] == PlayerMarker:
					Combo += 1
					CurrentCheckPosX -= 1
					CurrentCheckPosY -= 1
				else:
					CurrentCheckPosX, CurrentCheckPosY = Pos_X, Pos_Y
					Combo = 1
					CheckProcess = 'Up'
					
			elif CheckProcess == 'Up':
				if CurrentCheckPosY - 1 >= 0 and \
				self.board[CurrentCheckPosX][max(0, CurrentCheckPosY - 1)] == PlayerMarker:
					Combo += 1
					CurrentCheckPosY -= 1
				else:
					CurrentCheckPosX, CurrentCheckPosY = Pos_X, Pos_Y
					Combo = 1
					CheckProcess = 'UpperRight'

			elif CheckProcess == 'UpperRight':
				if CurrentCheckPosX + 1 < self.width and CurrentCheckPosY - 1 >= 0 \
				and self.board[min(self.width-1, CurrentCheckPosX + 1)][max(0, CurrentCheckPosY - 1)] == PlayerMarker:
					Combo += 1
					CurrentCheckPosX += 1
					CurrentCheckPosY -= 1
				else:
					CurrentCheckPosX, CurrentCheckPosY = Pos_X, Pos_Y
					Combo = 1
					CheckProcess = 'Right'

			elif CheckProcess == 'Right':
				if CurrentCheckPosX + 1 < self.width and \
				self.board[min(self.width-1, CurrentCheckPosX + 1)][CurrentCheckPosY] == PlayerMarker:
					Combo += 1
					CurrentCheckPosX += 1
				else:
					CurrentCheckPosX, CurrentCheckPosY = Pos_X, Pos_Y
					Combo = 1
					CheckProcess = 'DownRight'

			elif CheckProcess == 'DownRight':
				if CurrentCheckPosX + 1 < self.width and \
				CurrentCheckPosY + 1 < self.height and \
				self.board[min(self.width-1, CurrentCheckPosX + 1)][min(self.height-1, CurrentCheckPosY + 1)] == PlayerMarker:
					Combo += 1
					CurrentCheckPosX += 1
					CurrentCheckPosY += 1
				else:
					CurrentCheckPosX, CurrentCheckPosY = Pos_X, Pos_Y
					Combo = 1
					CheckProcess = 'Down'

			elif CheckProcess == 'Down':
				if CurrentCheckPosY + 1 < self.height and \
				self.board[CurrentCheckPosX][min(self.height-1, CurrentCheckPosY + 1)] == PlayerMarker:
					Combo += 1
					CurrentCheckPosY += 1
				else:
					CurrentCheckPosX, CurrentCheckPosY = Pos_X, Pos_Y
					Combo = 1
					CheckProcess = 'DownLeft'
					
			elif CheckProcess == 'DownLeft':
				if CurrentCheckPosX - 1 >= 0 and \
				CurrentCheckPosY + 1 < self.height and \
				self.board[max(0, CurrentCheckPosX - 1)][min(self.height-1, CurrentCheckPosY + 1)] == PlayerMarker:
					Combo += 1
					CurrentCheckPosX -= 1
					CurrentCheckPosY += 1
				else:
					CurrentCheckPosX, CurrentCheckPosY = Pos_X, Pos_Y
					Combo = 1
					CheckProcess = 'Left'
				
			elif CheckProcess == 'Left':
				if CurrentCheckPosX - 1 >= 0 and \
				self.board[max(0, CurrentCheckPosX - 1)][CurrentCheckPosY] == PlayerMarker:
					Combo += 1
					CurrentCheckPosX -= 1
				else:
					CheckProcess = 'Complete'
		
			if Combo >= 5:
				return True
		return False

	def CheckMarkPos(self, Pos_X, Pos_Y):
		return self.board[Pos_X][Pos_Y] == ' '

	def CheckFull(self):
		return self._turnCount == (self.width * self.height)

	def GetParameters(self):
		return {'Width':self.width, 'Height':self.height}

