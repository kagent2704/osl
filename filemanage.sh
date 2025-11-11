#!/bin/bash
# Shell Script to Manage Files and Directories

while true
do
    echo "------------------------------------"
    echo "       FILE AND DIRECTORY MENU      "
    echo "------------------------------------"
    echo "1. Create a File"
    echo "2. Create a Directory"
    echo "3. Display File Content"
    echo "4. Delete a File"
    echo "5. Delete a Directory"
    echo "6. List Files and Directories"
    echo "7. Exit"
    echo "------------------------------------"
    echo -n "Enter your choice: "
    read choice

    case $choice in
        1)
            echo "Enter filename to create:"
            read fname
            touch $fname
            echo "File '$fname' created successfully."
            ;;
        2)
            echo "Enter directory name to create:"
            read dname
            mkdir $dname
            echo "Directory '$dname' created successfully."
            ;;
        3)
            echo "Enter filename to display content:"
            read fname
            if [ -f "$fname" ]; then
                cat $fname
            else
                echo "File does not exist!"
            fi
            ;;
        4)
            echo "Enter filename to delete:"
            read fname
            if [ -f "$fname" ]; then
                rm $fname
                echo "File '$fname' deleted successfully."
            else
                echo "File does not exist!"
            fi
            ;;
        5)
            echo "Enter directory name to delete:"
            read dname
            if [ -d "$dname" ]; then
                rmdir $dname 2>/dev/null || echo "Directory not empty. Use 'rm -r' manually if needed."
                echo "Directory '$dname' deleted successfully (if empty)."
            else
                echo "Directory does not exist!"
            fi
            ;;
        6)
            echo "Listing files and directories in current location:"
            ls -l
            ;;
        7)
            echo "Exiting..."
            break
            ;;
        *)
            echo "Invalid choice! Please enter a number between 1-7."
            ;;
    esac
done
