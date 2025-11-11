#!/bin/bash
# Shell Script to Perform Arithmetic Operations

echo "Enter first number:"
read num1
echo "Enter second number:"
read num2

echo "Select an operation:"
echo "1. Addition"
echo "2. Subtraction"
echo "3. Multiplication"
echo "4. Division"
echo "5. Modulus"

read choice

case $choice in
  1) result=$((num1 + num2))
     echo "Result: $num1 + $num2 = $result"
     ;;
  2) result=$((num1 - num2))
     echo "Result: $num1 - $num2 = $result"
     ;;
  3) result=$((num1 * num2))
     echo "Result: $num1 * $num2 = $result"
     ;;
  4) 
     if [ $num2 -ne 0 ]
     then
        result=$((num1 / num2))
        echo "Result: $num1 / $num2 = $result"
     else
        echo "Error: Division by zero not allowed."
     fi
     ;;
  5) result=$((num1 % num2))
     echo "Result: $num1 % $num2 = $result"
     ;;
  *) echo "Invalid choice! Please select from 1 to 5."
     ;;
esac
