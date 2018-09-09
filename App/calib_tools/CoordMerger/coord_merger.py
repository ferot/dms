

""" Creates FANN-output-form line """
def substitute_line(x,y):
    return str (x) + " " + str (y) + "\n"


""" Generates map for converting coords into rescaled form"""
def generate_idx_map(max_dim):
    newX = 0
    newY = 0
    mapping = {}
    for i in range(max_dim):
        for j in range(max_dim):
            key = str(i) + " " + str(j)
            mapping[key] = str(newX) + " " + str(newY) + "\n"
            if j%2 ==1:
                newY+=1
        newY=0
        if i % 2 == 1:
            newX += 1
    return mapping


def main():
    file = open("train_data.dat", "r")
    out_file = open("m_train_data.dat", "w")
    lines = file.readlines()
    max_dim = 5
    map = generate_idx_map(max_dim)

    for line in lines:
        if len(line) <= 5:
            line = map[line.strip()]
        out_file.write(line)

    file.close()
    out_file.close()


if __name__ == "__main__":
    main ()