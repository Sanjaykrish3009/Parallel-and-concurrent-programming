import random

# write dictionary to file
# create file if not exists
def writeToFile(lines,format):
    with open("testcases.txt",format) as f:
        f.write(str(len(lines)) + "\n")
        for key in lines:
            f.write(lines[key] + "\n")


def getMode():
    mode = random.random()
    if mode < 0.33:
        option = random.random() >= 0.5
        if option:
            return "search"
        else:
            return "delete"
    else:
        return "insert"

def load_table(n_inserts):
    Inserts_ = []
    lines = {}
    counter = 0
    for i in range(n_inserts):
        key = ''.join(random.choice('0123456789') for i in range(10))
        value = ''.join(random.choice('0123456789') for i in range(10))
        Inserts_.append((key, value))
        lines[counter] = ' '.join(["insert", key, value])
        counter +=1
    
    return lines,Inserts_

def generateTestCases(init_size,inserts):
    # inserts = []

    lines = {}
    counter = 0

    while(init_size):
        for i in range(init_size):
            mode = getMode()
            if i == 0:
                mode = "insert"
            if mode == "search":
                choice = random.random() >= 0.5
                # pick a random element form inserts
                if choice:
                    search = random.choice(inserts)
                    lines[counter] = ' '.join(['search', search[0]])
                else:
                    key = ''.join(random.choice('0123456789') for i in range(10))
                    lines[counter] = ' '.join(['search', key])

            elif mode == "delete":
                choice = random.random() >= 0.5
                # pick a random element form inserts
                if choice:
                    delete_ = random.choice(inserts)
                    lines[counter] = ' '.join(['delete',delete_[0]])
                else:
                    key = ''.join(random.choice('0123456789') for i in range(10))
                    lines[counter] = ' '.join(['delete',key])
            else:
                # generate a random string of length 10
                key = ''.join(random.choice('0123456789') for i in range(10))
                value = ''.join(random.choice('0123456789') for i in range(10))
                inserts.append((key, value))
                lines[counter] = ' '.join(["insert", key, value])

            counter += 1
        init_size //= 2

    return lines

insertions,list_of_inserts = load_table(10000)

writeToFile(insertions,"w")

lines = generateTestCases(10001,list_of_inserts)
writeToFile(lines,"a")