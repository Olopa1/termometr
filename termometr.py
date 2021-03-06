#Program odpowiedzialny za komunikację z arduino

#importowanie bibliotek potrzbne w kodzie //bazy danych//komunikacja portu szeregowego//pobieranie czasu z komputera//

import time
import serial                              
import sqlite3

#main

arduino = serial.Serial('COM3',9600,timeout=1) #łączenie z portem szeregowym / USB

conect = sqlite3.Connection('temp.db')
conect.row_factory = sqlite3.Row        #utworzenie bazy danych która będzie zbierać informacje z portu szeregowego 
                                            #a konkretniej informację o temperaturzę
cursor = conect.cursor()                #utworzenie kursora w bazie danych||||20 15

#cursor.execute("DROP TABLE IF EXISTS Temp_station;") #Usuwanie tabeli

cursor.execute("""
CREATE TABLE IF NOT EXISTS Temp_station(    
    id INTEGER PRIMARY KEY ASC AUTOINCREMENT,   
    Temp TEXT,
    Hours INTEGER,
    Minutes INTEGER,
    Month INTEGER,
    Day INTEGER
    )""")#Stworzona nowa tabela jeśli jeeszcze nie istnieje 

while True:
    
    time_1 = time.localtime()
    hrs = time_1[3]
    mint = time_1[4]                        #wyciągnięcie aktualnej godziny
    secs = time_1[5]


    hrs_out = str(hrs)
    mint_out = str(mint)                    #zamina aktualnej godziny na typ string
    secs_out = str(secs)
    dott = ":"
    
    if len(hrs_out) != 2:
        hrs_out = "0"+hrs_out
        
    if len(mint_out) != 2:                  #jeżeli jest tylko jedna cyfra w godzinie to ten kod dodaje przed nią 0
        mint_out = "0"+mint_out
        
    if len(secs_out) != 2:
        secs_out = "0"+secs_out

    mint_out = mint_out + "      "          #spacja do odzielnia powielonych godzin na wyświetlaczu
    #secs_out = secs_out + "   "
    #full_hour = hrs_out+dott+mint_out+dott+secs_out

    #arduino.write(full_hour.encode())

    
    arduino.write(hrs_out.encode())
    arduino.write(dott.encode())            #przesłąnie informacji do bufora portu szeregowego
    arduino.write(mint_out.encode())
    #arduino.write(dott.encode())
    #arduino.write(secs_out.encode())
    time.sleep(1)

    data_read = arduino.readline() 
    if data_read:
        data_read.decode()                  #odczytanie informacji z portu szeregowego
        print(data_read)
    
    print("aktualna godzina z sekundami",full_hour)


    save = mint%10

    if save == 0 and secs == 0:
        cursor.execute("INSERT INTO Temp_station VALUES(NULL, ?, ?, ?, ?, ?);", (data_read[15:17],hrs,mint,manth,day))
        cursor.execute("SELECT * FROM Temp_station") #co 10 minut dodawanie nowego rekordu do tabeli
        conect.commit()


    
