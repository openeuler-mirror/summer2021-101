import time


if __name__ == "__main__":
    with open("a.txt", "w+") as f:
        f.write(time.asctime(time.localtime(time.time())))
        f.write("\n")
