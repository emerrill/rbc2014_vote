#!/usr/bin/env python2.7
# Tweets the next vote photos
# by Eric Merrill
import tweepy
import twitterCredentials
import os
import re

source_folder = 'waiting'
dest_folder = 'sent'

teams = list()
teams.append("121jigawatts")
teams.append("detroitus")
teams.append("i3detroit")
teams.append("makertwins")
teams.append("mblabs")
teams.append("omnicorpdetroit")
teams.append("tacticalurbanists")

filelist = [os.path.join(source_folder, f) for f in os.listdir(source_folder)]

if len(filelist) < 1:
    exit(0)

newest = min(filelist, key=lambda x: os.stat(x).st_mtime)

print newest

m = re.search('--(.)\.', newest)
vote = int(m.group(1))

tweet = 'I cast #myvote for #' + teams[vote]
print tweet


# OAuth process, using the keys and tokens
auth = tweepy.OAuthHandler(consumer_key, consumer_secret)
auth.set_access_token(access_token, access_token_secret)

# Creation of the actual interface, using authentication
api = tweepy.API(auth)

# Send the tweet with photo

api.update_with_media(newest, status=tweet)

os.rename(newest, os.path.join(dest_folder, os.path.basename(newest)))