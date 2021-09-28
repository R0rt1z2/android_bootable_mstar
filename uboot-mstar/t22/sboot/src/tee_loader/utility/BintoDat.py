import os
import sys

input_f = sys.argv[1]
output_f = sys.argv[2]
content = open(input_f, 'rb').read()
content_size = len(content)
cnt = 0

output = open(output_f, 'w')

for c in content:
    output.write("0x{:02x}, ".format(ord(c)))
    cnt +=1
    if cnt == 16:
        output.write('\n')
        cnt = 0

output.close()
