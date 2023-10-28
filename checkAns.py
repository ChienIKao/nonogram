with open("output.txt", "r", encoding="utf-8") as o:
    for l1, l2 in o:
        print(l1)
        print(l2)
        break
    o.close()
    i.close()
