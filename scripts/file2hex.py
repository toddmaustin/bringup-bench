#this script takes a file and makes it into a c string in a header file
import sys

file_name = sys.argv[1];
newfile_name = sys.argv[2];


oldFile = open(file_name, "rb")
oldFile = oldFile.read()
hexFile = "".join( hex(x) +"," for x in oldFile)
hexFile = hexFile.rstrip(hexFile[-1])


start_string = '#define ' + sys.argv[3] + '_sz ' + str(len(oldFile)) + '\nconst uint8_t ' + sys.argv[3] + '[' + str(len(oldFile)) + '] = {'
end_string = '};'

oldFile = start_string + hexFile + end_string
newFile = open(newfile_name, "wt")
newFile.write(oldFile)
newFile.close()

print(oldFile)

