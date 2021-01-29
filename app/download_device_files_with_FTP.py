import ftplib
import os

path = '/'
filename = 'trasy'

ftp = ftplib.FTP("192.168.12.7") 
ftp.login("esp8266", "esp8266") 
ftp.cwd(path)
os.chdir('D:/desktop/Počítač/miko/programovanie/gps_tracking_tachometer/app/files_from_device')
ftp.retrbinary("LIST " + filename, open(filename, 'wb').write)
ftp.quit()