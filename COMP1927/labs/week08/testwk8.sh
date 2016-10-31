s#!/bin/sh 

echo ======== No direct connection ========
echo 
    ./conn Munich Berlin 
echo 
    ./conn London "Castle Dracula"
echo 
    ./conn Paris Milan
echo 
echo ---separated by multiple seas ---
    ./conn Lisbon Valona 
echo 
    ./conn "North Sea" "Black Sea"
    
echo    
echo ======== Rail connection only ======== 
echo  
    ./conn Paris Marseilles
echo  
    ./conn Milan Florence 
echo  
    ./conn Szeged Bucharest
echo  
    ./conn Paris Bordeaux
echo   
    ./conn Venice Vienna
echo  
echo ======== Road connection only ======== 
echo            non port cities 
    ./conn Leipzig Hamburg
echo  
    ./conn "Castle Dracula" Galatz
echo  
    ./conn Cadiz Granada
echo 
echo ---port city to normal city ---
    ./conn Sofia Valona 
echo 
echo ---port to port separated by seas ---
    ./conn Lisbon Santander
echo  
echo ======== Boat connection only ======== 
echo 
echo ---port city to sea --- 
    ./conn London "English Channel"  
echo  
    ./conn Marseilles "Mediterranean Sea"
echo 
echo --- sea to sea ---
    ./conn "Le Havre" "English Channel"
echo  
    ./conn "Black Sea" "Ionian Sea"
echo 
echo --- port to port ---
    ./conn Venice Bari
echo 
echo --- port to port separated by one different sea ---
    ./conn Galway Dublin
echo  
echo ====== Rail and Road connections ====== 
echo  
    ./conn London Manchester
echo  
    ./conn Madrid Santander
echo  
    ./conn Saragossa Barcelona
echo  
    ./conn Swansea London
echo 
echo ---separated by multiple seas ---
    ./conn Bari Naples  
echo   
echo ======= Rail and Boat connections ======= 
echo  
    ./conn Alicante Barcelona
echo   
echo ======= Road and Boat connections ======
echo  
    ./conn Valona Athens
echo 
    ./conn Constanta Varna
echo 
    ./conn Nantes Bordeaux
echo 
    ./conn Lisbon Cadiz
echo   
echo ======== All connections ========
echo
    ./conn Rome Naples
    
echo 
echo ======== Non existent cities ========
echo 
    ./conn Snowdin Waterfall
echo 
