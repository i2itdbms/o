#!/bin/bash

while true
do
    echo "1.Create  2.View  3.Insert  4.Delete  5.Modify  6.Exit"
    read ch

    case $ch in
    1)
        touch address.txt
        echo "Address book created"
        ;;

    2)
        cat address.txt
        ;;

    3)
        echo "Enter name, phone, address:"
        read name phone address
        echo "$name | $phone | $address" >> address.txt
        ;;

    4)
        echo "Enter name to delete:"
        read name
        grep -v "^$name |" address.txt > temp.txt
        mv temp.txt address.txt
        ;;

    5)
        echo "Enter old name:"
        read name
        grep -v "^$name |" address.txt > temp.txt
        mv temp.txt address.txt

        echo "Enter new name, phone, address:"
        read newname newphone newaddress
        echo "$newname | $newphone | $newaddress" >> address.txt
        ;;

    6)
        exit
        ;;

    *)
        echo "Invalid choice"
        ;;
    esac
done
