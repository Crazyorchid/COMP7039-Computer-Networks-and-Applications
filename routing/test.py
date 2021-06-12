def DistanceVector():
    flag = True
    session = True
    last = ''
    data = input()
    routList = []
    messageList = []
    while True:
        router = input()
        print("Test:", router)
        messageList.append(router)
        if last == '\n' and router == '\n':
            break
        last = router
    for item in messageList:
        print(item)

DistanceVector()
    



