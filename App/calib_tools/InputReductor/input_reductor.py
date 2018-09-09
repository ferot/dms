import sys

""" Counts first and last index of list to remove, counted by nr of feature per cam and cam_id (indexd from 0!!!)"""
def idx_to_remove(cam_to_remove, feature_nr):
    idx_head = (cam_to_remove) * 3
    idx_tail = idx_head + feature_nr

    return idx_head, idx_tail

def update_header(header, cam_nr, nr_of_features):
    input_idx = 1 #determined by FANN input file structure
    splitted_header = header.split()

    modified_input_nr = cam_nr * (nr_of_features-1)
    del splitted_header[input_idx]
    splitted_header.insert(input_idx, str(modified_input_nr))

    return ' '.join(splitted_header) + "\n"


def main():
    # Indicator of set of input data to remove (indexed from 0!)
    cam_idx_to_reduce = int(sys.argv[1])

    # Number of input features per camera
    nr_of_features = 3
    cam_nr = 3

    file = open("train_data.dat", "r")
    out_file = open("../r_train_data.dat", "w")

    lines = file.readlines()
    lines[0] = update_header(lines[0], cam_nr, nr_of_features)


    # Arbitrary set to differentiate row with input from row with output
    longer_line_len = 8

    for line in lines:
        if len(line) > longer_line_len:
            words_list = line.split ()
            head_idx, tail_idx = idx_to_remove(cam_idx_to_reduce, nr_of_features)
            del words_list[ head_idx : tail_idx]
            line = ' '.join(words_list) + "\n"
        out_file.write (line)

    file.close()
    out_file.close()

if __name__ == "__main__":
    main ()