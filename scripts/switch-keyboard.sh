#!/bin/bash

getCurLayout() {
    setxkbmap -query | awk '/layout/ {print $2}';
}

setLayout() {
    local layout="$1";
    local variant="$2";
    DISPLAY=:0 setxkbmap ${layout} ${variant};
}

main() {
    declare -a layouts=(${1//,/ });
    shift;
    declare -A variants
    for variant in $@; do
        local key="$(echo ${variant} | cut -d ':' -f 1)";
        local value="$(echo ${variant} | cut -d ':' -f 2)";
        variants+=([$key]=$value);
    done;
    echo;
    local i=0;
    echo "===== DEBUG: =====";
    for ((i=0; i < ${#layouts[@]}; i++)); do
        echo "[$i] => ${layouts[$i]}";
    done;
    for key in ${!variants[@]}; do
        echo "$key => ${variants[$key]}";
    done;
    echo "==================";
    local cl="$(getCurLayout)";
    for ((i=0; i < ${#layouts[@]}; i++)); do
        [[ ${layouts[$i]} == $cl ]] && break;
    done
    if [[ $i -ge $((${#layouts[@]} - 1)) ]]; then
        local layout="${layouts[0]}";
        local variant="${variants[$layout]}";
        echo "setLayout ${layout} ${variant}";
        setLayout ${layout} ${variant};
        return 0;
    fi;
    local layout="${layouts[$(($i+1))]}";
    local variant="${variants[$layout]}";
    setLayout ${layout} ${variant};
    return 0;
}

main $@;
