# -*- coding: utf-8 -*-

import urllib

response = urllib.urlopen('http://www.google.com.tw')

html = response.read().decode('utf_8')

print(html)
