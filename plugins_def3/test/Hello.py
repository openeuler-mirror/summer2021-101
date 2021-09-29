import time
import sys

if __name__ == "__main__":
    with open("a.txt", "a") as f:
        f.write(time.asctime(time.localtime(time.time())))
        f.write(str(sys.argv))
        f.write("\n")
