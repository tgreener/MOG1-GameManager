#!/bin/sh

GAME_MANAGER_EXEC=./dist/Debug/GNU-MacOSX/gamemanager

declare -a POIs=(
	"Puppy Land" 5 3 10
	"Waffle Land" 1 2 3
	"Pickled Plains" 3 2 1
	"Thunder Hill" 2 3 1
        "Tortoise Island" 3 6 5
);

for (( i=0; i<${#POIs[*]}; i++ ))
do
	echo ${POIs[i]} ${POIs[i+1]} ${POIs[i+2]} ${POIs[i+3]}
	$GAME_MANAGER_EXEC -poi -add "${POIs[i]}" ${POIs[i+1]} ${POIs[i+2]} ${POIs[i+3]}
	i=$i+3
	echo
done