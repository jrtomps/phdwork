#!/usr/bin/python
import sys
import smtplib

# me == the sender's email address
# you == the recipient's email address

server = smtplib.SMTP('smtp.gmail.com',587) #port 465 or 587
server.starttls()
server.login('jrtomps','hzohsapgrojnlbje')
server.sendmail('jrtomps@gmail.com','5855067664@vtext.com','Simulation completed')
server.close()
