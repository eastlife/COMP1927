#!/bin/sh 

echo  ===== example tests  =====
echo 
    ./travel Berlin Chicago
echo  = result: Berlin - Chicago
echo
    ./travel Sydney Chicago
echo  = result: Sydney - Honolulu - Chicago
echo 
    ./travel Sydney London
echo  = result: Sydney - Shanghai - London
echo
    ./travel London Sydney
echo  = result: London - Shanghai - Sydney
echo
    ./travel Sydney 'Buenos Aires'
echo  = result: Sydney - Honolulu - Chicago - Buenos Aires
echo
echo  ==== if no plane can fly more than 6000km wihout refuelling ====
echo
    ./travel Sydney London 6000
echo  = result: Sydney - Guam - Manila - Bombay - Baghdad - London 
echo 
echo  ==== if no plane can fly more than 5000km wihout refuelling ====
echo  ==== you cant leave Australia under this scenario ====
echo
    ./travel Sydney 'Buenos Aires' 5000
echo  = result: No route from Sydney to Buenos Aires
echo
echo  ==== if no plane can fly more than 7000km wihout refuelling ====
echo
    ./travel Sydney 'Buenos Aires' 7000
echo  = result: Sydney - Guam - Honolulu - Chicago - Panama City - Buenos Aires
echo
echo  ==== planes can fly up to 8000km wihout refuelling ====
echo 
    ./travel Sydney 'Buenos Aires' 8000
echo  = result: Sydney - Guam - Honolulu - Mexico City - Buenos Aires
echo
echo  = planes can fly up to 11000km wihout refuelling
echo    
    ./travel Sydney 'Buenos Aires' 11000
echo  = result: Sydney - Bombay - Azores - Buenos Aires
echo
echo  ==== planes can fly up to 12000km wihout refuelling ====
echo  ==== can reach Buenos Aires on a single flight ====
echo 
    ./travel Sydney 'Buenos Aires' 12000
echo  = result: Sydney - Buenos Aires
echo
    ./travel Bombay Chicago 5000
echo  = result: Bombay - Istanbul - Azores - Montreal - Chicago
echo 
    ./travel Sydney Sydney
echo  = result: Sydney
echo
