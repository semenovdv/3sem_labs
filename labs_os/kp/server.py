import time
import sys
import zmq

port = "5556"

board = [
    [' ', 'o', ' ', 'o', ' ', 'o', ' ', 'o'],
    ['o', ' ', 'o', ' ', 'o', ' ', 'o', ' '],
    [' ', 'o', ' ', 'o', ' ', 'o', ' ', 'o'],
    [' ', ' ', ' ', ' ', '*', ' ', ' ', ' '],
    [' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '],
    ['*', ' ', '*', ' ', '*', ' ', '*', ' '],
    [' ', '*', ' ', '*', ' ', '*', ' ', '*'],
    ['*', ' ', '*', ' ', '*', ' ', '*', ' ']]

winrate = {}

#winrate = {Denis: [games, wins],
#           Ann:   [games, wins]}

def move_if_can(iid, list_cmd, dictt):
    # changing name and board

    i = 3
    while i < len(cmd_nlist):
        int1 = list_cmd[i-3]
        int2 = list_cmd[i-2]
        int3 = list_cmd[i-1]
        int4 = list_cmd[i]
        i += 2

        if abs(int1 - int3) == 2:
            if dictt[iid][2] == dictt[iid][0][1]:
                dictt[iid][1][0] = dictt[iid][1][0] - 1
            else:
                dictt[iid][1][1] = dictt[iid][1][1] - 1

    if dictt[iid][2] == dictt[iid][0][0]:
        dictt[iid][2] = dictt[iid][0][1]
    else:
        dictt[iid][2] = dictt[iid][0][0]

       
    # changing command
    dictt[iid][3] = cmd_nlist


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

def check_win(iid, dictt):
    if dictt[iid][1][0] == 0:
        return dictt[iid][0][1]
    elif dictt[iid][1][1] == 0:
        return dictt[iid][0][0]
    else:
        return ''

    
    


if __name__ == "__main__":
    #dict_of_Games = {-1:[['Denis', 'Anna'], [12,12], Name1/Name2, [lastcommands], counter]} # storage of games
    dict_of_Games = {}
    list_of_Names = []
    num_of_Games = 0
    set_of_iid = set()
    set_of_iid = {i for i in range(10)}

    has_opponent = False
    name_opponent = ""

    context = zmq.Context()
    socket = context.socket(zmq.REP)
    socket.bind("tcp://*:5556")

    while True:
        message = socket.recv()
        messageu = str(message,'utf-8')
        print("Received: ", messageu)  


        if messageu[0] == "?":
            # checking for starting a game 
            mes_sent = False
            name = messageu[1:]
            for key in dict_of_Games.keys():
                if dict_of_Games[key][2] == name:
                    message = "i" + str(key) 
                    socket.send_string(message)
                    mes_sent = True
                    break
                #elif dict_of_Games[key][0][0] == name or dict_of_Games[key][0][1] == name:
                #    message = "Waiting for opponents move." + str(key) + "b"
                #    socket.send_string(message)
                #    mes_sent = True
                #    break  
            if not mes_sent:
                message = "Waiting opponent..."
                socket.send_string(message)


        elif messageu[0] == "*":
            # checking for opportunity to move 
            mes_sent = False
            iid = int(messageu[1])
            name = messageu[2:]

            if iid not in dict_of_Games.keys():
                message = "You are WINNER!!!"
                socket.send_string(message)

            elif dict_of_Games[iid][2] == name:
                message = "i" + str(iid)
                for item in dict_of_Games[iid][3]:
                    message = message + str(item)
                socket.send_string(message)

            elif dict_of_Games[iid][0][0] == name or dict_of_Games[iid][0][1] == name:
                dict_of_Games[iid][4] = dict_of_Games[iid][4] + 1

                if dict_of_Games[iid][4] == 100: # counter of time
                    message = "You are Winner!!!"
                    winner = name
                    list_of_Names.remove(dict_of_Games[iid][0][0])
                    list_of_Names.remove(dict_of_Games[iid][0][1])

                    # make winrate
                    if dict_of_Games[iid][0][0] not in winrate.keys():
                        winrate[dict_of_Games[iid][0][0]] = [0, 0]
                    if dict_of_Games[iid][0][1] not in winrate.keys():
                        winrate[dict_of_Games[iid][0][1]] = [0, 0]

                    # change winrate
                    if dict_of_Games[iid][0][0] == winner:
                        winrate[dict_of_Games[iid][0][0]][0] += 1 # games
                        winrate[dict_of_Games[iid][0][0]][1] += 1 # wins

                        winrate[dict_of_Games[iid][0][1]][0] += 1 # enemy games
                    else:
                        winrate[dict_of_Games[iid][0][1]][0] += 1 # games
                        winrate[dict_of_Games[iid][0][1]][1] += 1 # wins

                        winrate[dict_of_Games[iid][0][0]][0] += 1 # enemy games

                    dict_of_Games.pop(iid)
                    set_of_iid.add(iid)

                else:
                    message = "Waiting for opponents move."
                socket.send_string(message)

        elif messageu[0] == "!":
            # checking for move 
            # change name of move and change command
            iid = int(messageu[1])
            dict_of_Games[iid][4] = 0
            if iid not in dict_of_Games.keys():
                message = "You lose because of time limit!"
            else:
                cmd_list = messageu[2:]
                cmd_nlist = []
                for item in cmd_list:
                    cmd_nlist.append(int(item))

                # receve in right order : from(string, stolb), to(string, stolb)
                #print(iid, cmd_nlist)
                move_if_can(iid, cmd_nlist, dict_of_Games)

                winner = check_win(iid, dict_of_Games)
                if winner != '':
                    message = "Winner is " + winner

                    # remove from games on server
                    list_of_Names.remove(dict_of_Games[iid][0][0])
                    list_of_Names.remove(dict_of_Games[iid][0][1])

                    # make winrate
                    if dict_of_Games[iid][0][0] not in winrate.keys():
                        winrate[dict_of_Games[iid][0][0]] = [0, 0]
                    if dict_of_Games[iid][0][1] not in winrate.keys():
                        winrate[dict_of_Games[iid][0][1]] = [0, 0]

                    # change winrate
                    if dict_of_Games[iid][0][0] == winner:
                        winrate[dict_of_Games[iid][0][0]][0] += 1 # games
                        winrate[dict_of_Games[iid][0][0]][1] += 1 # wins

                        winrate[dict_of_Games[iid][0][1]][0] += 1 # enemy games
                    else:
                        winrate[dict_of_Games[iid][0][1]][0] += 1 # games
                        winrate[dict_of_Games[iid][0][1]][1] += 1 # wins

                        winrate[dict_of_Games[iid][0][0]][0] += 1 # enemy games

                    dict_of_Games.pop(iid)
                    set_of_iid.add(iid)
                    
                else:
                    message = "Waiting for opponents move."

            socket.send_string(message)
        
        elif messageu[0] == "-":
            iid = int(messageu[1])
            if iid not in dict_of_Games.keys():
                message = "You lose because of time!"
                socket.send_string(message)
            else:
                dict_of_Games[iid][4] = 0
                name = messageu[2:]

                winner = ''

                if dict_of_Games[iid][0][0] == name:
                    winner = dict_of_Games[iid][0][1]
                else:
                    winner = dict_of_Games[iid][0][0]

                message = "Winner is " + winner

                # remove from games on server
                list_of_Names.remove(dict_of_Games[iid][0][0])
                list_of_Names.remove(dict_of_Games[iid][0][1])

                # make winrate
                if dict_of_Games[iid][0][0] not in winrate.keys():
                    winrate[dict_of_Games[iid][0][0]] = [0, 0]
                if dict_of_Games[iid][0][1] not in winrate.keys():
                    winrate[dict_of_Games[iid][0][1]] = [0, 0]

                # change winrate
                if dict_of_Games[iid][0][0] == winner:
                    winrate[dict_of_Games[iid][0][0]][0] += 1 # games
                    winrate[dict_of_Games[iid][0][0]][1] += 1 # wins

                    winrate[dict_of_Games[iid][0][1]][0] += 1 # enemy games
                else:
                    winrate[dict_of_Games[iid][0][1]][0] += 1 # games
                    winrate[dict_of_Games[iid][0][1]][1] += 1 # wins

                    winrate[dict_of_Games[iid][0][0]][0] += 1 # enemy games

                dict_of_Games.pop(iid)
                set_of_iid.add(iid)

                message = "OK"
                socket.send_string(message)


        elif messageu[0] == "+":
            # checking for appending a player
            name = messageu[1:]
            #print(name)
            if name in list_of_Names:
                message = "Sorry, but this name already taken." 
                socket.send_string(message)
            

            elif not has_opponent:
                has_opponent = True
                name_opponent = name
                list_of_Names.append(name)
                message = "Waiting opponent..."
                socket.send_string(message)


            else:

                # we have opponent and we can start new game
                list_of_Names.append(name)
                
                dict_of_Games[num_of_Games] = [[name_opponent, name], [12,12], name_opponent, "", 0]
                num_of_Games = num_of_Games + 1
                #print(dict_of_Games)
                has_opponent = False
                name_opponent = ""
                message = "Waiting for opponents move." + str(set_of_iid.pop()) + "b"
                socket.send_string(message)

        elif messageu == "%":
            message = "Winrate of people: \n\n"
            for item in winrate.keys():
                if winrate[item][0] != 0 and winrate[item][1]:
                    message += item
                    message += " "
                    message += str(winrate[item][1] * 100 / winrate[item][0])
                    message += "\n\n"
            socket.send_string(message)


        else:
            # never get there
            print("exit")
            exit()
        