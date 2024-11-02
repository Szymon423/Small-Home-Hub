#!/bin/bash

CONFIG_DATABASE_PATH="/Small-Home-Hub/Database/config.db"
CONFIG_SQL_SCRIPT_PATH="/Small-Home-Hub/Database/config.sql"

# Sprawdź, czy plik .sql istnieje
if [ ! -f "$CONFIG_SQL_SCRIPT_PATH" ]; then
    echo "Plik $CONFIG_SQL_SCRIPT_PATH nie istnieje."
    exit 1
fi

sqlite3 $CONFIG_DATABASE_PATH < $CONFIG_SQL_SCRIPT_PATH

if [ $? -eq 0 ]; then
    echo "Baza danych config została pomyślnie stworzona/zaktualizowana."
else
    echo "Wystąpił problem podczas tworzenia/aktualizacji bazy danych config."
    exit 1
fi