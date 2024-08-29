with open("run.res") as f:
    data = f.readline()
    reference = f.readline()

def convert_to_array(string):
    return list(map(int, string.split(" ")[:-1]))

data = convert_to_array(data)
reference = convert_to_array(reference)

print(data == reference)

data.sort()

print(data == reference)
