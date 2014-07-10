#!/usr/bin/env python2.7
# Takes and stores a vote photos
# by Eric Merrill
from PIL import Image
from subprocess import call
from datetime import datetime
import sys

if len(sys.argv) < 1:
    print 'Need 1 argument, team number to vote for'
    exit(1)

vote = sys.argv[1]
vote = int(vote)

i = datetime.now()               #take time and date for filename
now = i.strftime('%Y%m%d-%H%M%S')
photo_name = 'temp.jpg'
photo_path = '' + photo_name
new_name = now + '--' + `vote` + '.jpg'
new_path = 'waiting/' + new_name
cmd = 'raspistill -t 500 -w 1024 -h 768 -o ' + photo_path
call ([cmd], shell=True)         #shoot the photo



background = Image.open(photo_path)

# Get correct overlay
foreground = Image.open('overlays/' + `vote` + '.png')

background.paste(foreground, (0, 0), foreground)
background.save(new_path,"JPEG", quality=90)

print 'Saved ' + new_path;

