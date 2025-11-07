#!/bin/bash

ADDRESS_BOOK="my_address_book.csv"

show_menu() {
    echo " ===================== "
    echo "     My Address Book   "
    echo " ===================== "
    echo "1. Add Contact"
    echo "2. Display Contacts"
    echo "3. Search Contact"
    echo "4. Delete Contact"
    echo "5. Exit"
    echo -n "Enter your choice: "
}

add_cont() {
    echo -n "Enter Name: "
    read name
    echo -n "Enter Phone Number: "
    read phone
    echo -n "Enter Email: "
    read email

    echo "$name,$phone,$email" >> "$ADDRESS_BOOK"
    echo "Contact Saved!"
}

disp_cont() {
    if [ ! -f "$ADDRESS_BOOK" ]; then
        echo "Address Book is empty!"
        return
    fi

    column -t -s "," "$ADDRESS_BOOK"
}

search_cont() {
    echo -n "Enter name/email/phone to search: "
    read key

    grep -i "$key" "$ADDRESS_BOOK" | column -t -s "," || echo "No match found."
}

delete_cont() {
    echo -n "Enter the name to delete: "
    read name

    if [ ! -f "$ADDRESS_BOOK" ]; then
        echo "Address Book is empty!"
        return
    fi

    grep -iv "$name" "$ADDRESS_BOOK" > temp.csv && mv temp.csv "$ADDRESS_BOOK"
    echo "Contact deleted (if it existed)."
}

while true; do
    show_menu
    read choice

    case $choice in
        1) add_cont ;;
        2) disp_cont ;;
        3) search_cont ;;
        4) delete_cont ;;
        5) echo "Goodbye!"; exit ;;
        *) echo "Invalid choice."; sleep 1 ;;
    esac
done
