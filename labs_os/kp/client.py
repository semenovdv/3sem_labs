import zmq
import sys
import time


board = [
    [' ', 'o', ' ', 'o', ' ', 'o', ' ', 'o'],
    ['o', ' ', 'o', ' ', 'o', ' ', 'o', ' '],
    [' ', 'o', ' ', 'o', ' ', 'o', ' ', 'o'],
    [' ', ' ', '', ' ', ' ', ' ', ' ', ' '],
    [' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '],
    ['*', ' ', '*', ' ', '*', ' ', '*', ' '],
    [' ', '*', ' ', '*', ' ', '*', ' ', '*'],
    ['*', ' ', '*', ' ', '*', ' ', '*', ' ']]

is_waiting_opponent = False
is_waiting_move = False
iid = -1

color = ""
op_color =""

def change_board(list_cmd, board, color):
    i = 3
    while i < len(list_cmd):
        int1 = list_cmd[i-3]
        int2 = list_cmd[i-2]
        int3 = list_cmd[i-1]
        int4 = list_cmd[i]
        i += 2

        board[int1][int2] = ' '
        
        if abs(int1 - int3) == 2:
            check1 = 0
            check2 = 0
            if int3 > int1:
                check1 = int1 + 1
            else:
                check1 = int1 - 1
            if int4 > int2:
                check2 = int2 + 1
            else:
                check2 = int2 - 1
            
            board[check1][check2] = ' '

    if color == "w":
            board[list_cmd[-2]][list_cmd[-1]] = 'o'
    else:
        board[list_cmd[-2]][list_cmd[-1]] = '*'

def print_help():
    print("to make move type 'A3 B4'.")

def print_board(board):
    print("----------------------------------------------")
    print("... A .. B .. C .. D .. E .. F .. G .. H ...")
    print("----------------------------------------------")
    i = 1
    for box in board:
        print(i, "| ", end = "")
        i = i + 1
        for item in box:
            print(item, " | ", end='')
        print(i-1)
        print("----------------------------------------------")
    print("... A .. B .. C .. D .. E .. F .. G .. H ...")
    print("----------------------------------------------")

def break_game():
    message = "-" + str(iid) + name
    socket.send_string(message)
    message = socket.recv()
    messageu = str(message, 'utf-8')
    print(messageu)
    exit()

def checknmove_cmd(list_cmd, flag, board, color, is_me):


    for item in list_cmd:
        if item < 0 or item > 7:
            print("Wrong command!")
            return False
    
    if color == "w" and is_me and (board[list_cmd[0]][list_cmd[1]] != 'o' and board[list_cmd[0]][list_cmd[1]] != 'O'):
        print("Not your fishka!")
        return False
    elif color == "b" and is_me and (board[list_cmd[0]][list_cmd[1]] != '*' and board[list_cmd[0]][list_cmd[1]] != '@'):
        print("Not your fishka!")
        return False

    i = 2
    while i < len(list_cmd):
        check1 = list_cmd[i]
        i += 1
        check2 = list_cmd[i]
        i += 1

        if board[check1][check2] != ' ':
            print("Wrong destination!")
            is_norm_cmd = False
            return False

    i = 3

    if flag == "one":
        int1 = list_cmd[i-3]
        int2 = list_cmd[i-2]
        int3 = list_cmd[i-1]
        int4 = list_cmd[i]


        if abs(int1 - int3) != abs(int2 - int4):
            print("Dont you know the rules?")
            return False
        if abs(int1 - int3) > 2:
            print("You cant move so far!")
            return False

        if abs(int1 - int3) == 2:
            check1 = 0
            check2 = 0
            if int3 > int1:
                check1 = int1 + 1 
            else:
                check1 = int1 - 1 
                        
            if int4 > int2:
                check2 = int2 + 1
            else:
                check2 = int2 - 1 
                            
            if color == "w" and is_me:
                if board[check1][check2] != '*' and board[check1][check2] != '@':
                    print("You dont kill enemy!!!")
                    return False 
            elif color == "b" and is_me:
                if board[check1][check2] != 'o' and board[check1][check2] != 'O':
                    print("You dont kill enemy!!!")
                    return False

            board[check1][check2] = ' '
            

        # checking vector
        elif board[int1][int2] == 'o': # down
            if int1 >= int3:
                print("Prikaz 227: NI SHAGU NAZAD!")
                return False
        elif board[int1][int2] == 'O':
            if int1 <= int3:
                print("Prikaz 228: NI SHAGU VPERED!")
                return False
        elif board[int1][int2] == '*': # up
            if int1 <= int3:
                print("Prikaz 227: NI SHAGU NAZAD!")
                return False
        elif board[int1][int2] == '@':
            if int1 >= int3:
                print("Prikaz 228: NI SHAGU VPERED!")
                return False

        if board[int1][int2] == 'o':
            if int3 == 7:
                board[int3][int4] = 'O'
            else:
                board[int3][int4] = 'o'
        elif board[int1][int2] == '*':
            if int3 == 0:
                board[int3][int4] = '@'
            else:
                board[int3][int4] = '*'
        elif board[int1][int2] == 'O':
            if int3 == 0:
                board[int3][int4] = 'o'
            else:
                board[int3][int4] = 'O'
        elif board[int1][int2] == '@':
            if int3 == 7:
                board[int3][int4] = '*'
            else:
                board[int3][int4] = '@'

        board[int1][int2] = ' '


    else:

        while i < len(list_cmd) - 1:
            int1 = list_cmd[i-3]
            int2 = list_cmd[i-2]
            int3 = list_cmd[i-1]
            int4 = list_cmd[i]
            i += 4 
            
            if abs(int1 - int3) != abs(int2 - int4):
                print("Dont you know the rules?")
                return False
            if abs(int1 - int3) > 2:
                print("You cant move so far!")
                return False

            if abs(int1 - int3) < 2:
                print("Wrong move!")
                return False

            check1 = 0
            check2 = 0
            if int3 > int1:
                check1 = int1 + 1 
            else:
                check1 = int1 - 1 
                        
            if int4 > int2:
                check2 = int2 + 1
            else:
                check2 = int2 - 1 
                            
            if color == "w" and is_me:
                if board[check1][check2] != '*' or board[check1][check2] != '@':
                    print("You dont kill f enemy!!!")
                    return False 
            elif color == "b" and is_me:
                if board[check1][check2] != 'o' or board[check1][check2] != 'O':
                    print("You dont kill f enemy!!!")
                    return False

            if board[int1][int2] == 'o':
                if int3 == 7:
                    board[int3][int4] = 'O'
                else:
                    board[int3][int4] = 'o'
            elif board[int1][int2] == '*':
                if int3 == 0:
                    board[int3][int4] = '@'
                else:
                    board[int3][int4] = '*'
            elif board[int1][int2] == 'O':
                if int3 == 0:
                    board[int3][int4] = 'o'
                else:
                    board[int3][int4] = 'O'
            elif board[int1][int2] == '@':
                if int3 == 7:
                    board[int3][int4] = '*'
                else:
                    board[int3][int4] = '@'

            board[int1][int2] = ' '

    return True



def change_list(change):
    i = 0
    while i < len(change):
        change[i], change[i+1] = change[i+1], change[i]
        i = i + 2

    newchange = []
    newchange.append(change[0])
    i = 1
    while i < len(change):
        newchange.append(change[i])
        newchange.append(change[i])
        i = i + 1
    change = newchange









if __name__ == "__main__":


    port = "5556"
    context = zmq.Context()
    print("Connecting to server...")
    socket = context.socket(zmq.REQ)
    socket.connect ("tcp://localhost:%s" % port)

    message = "%"
    socket.send_string(message)
    message = socket.recv()
    messageu = str(message, 'utf-8')
    print(messageu)


    # processing of name
    name = ""
    while name == "":
        name = input("Please, enter your Name: ")
        if name == "":
            print("Name NOT PUSTOE!!!")

    message = "+" + name
    socket.send_string(message)

    

    while True:
        message = socket.recv()
        messageu = str(message, 'utf-8')
        #print("Received reply {}".format(message))
        
        
        # main loop
        if messageu == "Sorry, but this name already taken.":
            print(messageu)
            name = input("Please, Enter new name: ")
            message = "+" + name
            socket.send_string(message)


        elif messageu == "Waiting opponent...":
            color = "w"
            op_color = "b"
            is_waiting_move = False
            if not is_waiting_opponent:
                is_waiting_opponent = True
                print("Waiting opponent.")
            time.sleep(1)
            message = "?" + name
            socket.send_string(message)


        elif messageu[:27] == "Waiting for opponents move.":
            if len(messageu) > 27:
                iid = messageu[27]
                color = messageu[28]
                if color == "w":
                    op_color = "b"
                else:
                    op_color = "w"
                print("Your color is Black")

            is_waiting_opponent = False
            if not is_waiting_move:
                is_waiting_move = True
                print("Waiting for opponents move.")
            time.sleep(1)
            message = "*" + str(iid) + name
            socket.send_string(message)


        elif messageu[0] == "i":

            iid = int(messageu[1])
            op_cmd = messageu[2:]
            op_list_cmd = []

            if len(messageu) == 2:
                if color == "b":
                    print("Your color is Black!")
                else:
                    print("Your color is White!")

                
            else:
                i = 0
                while i < len(op_cmd):
                    #print(i)
                    #print(len(op_cmd))
                    op_list_cmd.append(int(op_cmd[i]))
                    i = i + 1
                    op_list_cmd.append(int(op_cmd[i]))
                    i = i + 1
            
                #print(op_list_cmd)
                if len(op_list_cmd) > 4:
                    checknmove_cmd(op_list_cmd, "many", board, op_color, False)
                else:
                    checknmove_cmd(op_list_cmd, "one", board, op_color, False)

                    
            is_waiting_opponent = False
            is_waiting_move = False
            print_help()
            print_board(board)
            is_norm_cmd = False
            list_cmd = []

            while not is_norm_cmd:
                list_cmd = []
                cmd = input("Your turn to move: ")
                if cmd == "exit":
                    break_game()

                i = 0
                while i < len(cmd):
                    list_cmd.append(ord(cmd[i]) - 65) 
                    i+=1
                    list_cmd.append(int(cmd[i]) - 1)
                    i+=2

                change_list(list_cmd)

                #print(list_cmd)
                if len(list_cmd) > 4:
                    is_norm_cmd = checknmove_cmd(list_cmd, "many", board, color, True)
                else:
                    is_norm_cmd = checknmove_cmd(list_cmd, "one", board, color, True)
                
               
            print_board(board)
            cmd = "!" + str(iid)
            for item in list_cmd:
                cmd = cmd + str(item)

            #print(cmd)
            socket.send_string(cmd)


        elif messageu == "Wrong command":
            print(messageu)
        elif messageu[:3] == "You":
            print(messageu)
            exit()

        else:
            print("F")
            exit()
        

            

