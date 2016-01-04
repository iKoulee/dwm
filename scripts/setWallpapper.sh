#!/bin/bash


function getSize() {
    local res="$1";
    echo "$res" | sed 's/[+-][[:digit:]]\+//g'
}

function getOffset() {
    local res="$1";
    local img="$2";
    local -i x=$(echo "$res" | sed 's/[[:digit:]]\+x[[:digit:]]\+[+-]\([[:digit:]]\+\)+[[:digit:]]\+/\1/')
    local -i y=$(echo "$res" | sed 's/[[:digit:]]\+x[[:digit:]]\+[+-][[:digit:]]\++//')
    local -i disX=$(echo "$res" | sed 's/x.*$//')
    local -i disY=$(echo "$res" | sed 's/[[:digit:]]\+x\([[:digit:]]\+\).*/\1/')
    if [[ -f "$img" ]]; then
#        echo "Display: Resolution=${disX}x${disY}, Offset=$x,$y" >&2;
        local -i picX=$(mediainfo "$img" | sed -n '/Width/s/[^[:digit:]]//gp');
        local -i picY=$(mediainfo "$img" | sed -n '/Height/s/[^[:digit:]]//gp');
#        echo "Picture Resolution=${picX}x${picY}" >&2;
        local offsetX=$(( (($disX-$picX)/2)+$x ));
        local offsetY=$(( (($disY-$picY)/2)+$y ));
#        [[ $offsetX -lt 0 ]] && offsetX=0;
#        [[ $offsetY -lt 0 ]] && offsetY=0;
        printf "%d,%d\n" $offsetX $offsetY
    else
        printf "%d,%d" $x $y;
    fi
}

function help() {
    printf "Usage: $(basename $0) <Directory|File[ File[ File[ ..]]>\n"
}

function getImage() {
    if [[ -z $imageDir ]]; then
        if [[ -n ${files[$(($iC+1))]} ]]; then
            iC=$(($iC+1));
            echo ${files[$iC]};
        else
            echo ${files[$iC]};
        fi;
    else
        local image="$(ls $imageDir | sort -R | tail -1)";
        printf "%s/%s" ${imageDir%/} ${image}
    fi
    return $iC
}

function :() {
    local display="$1"
    local img="$2"
    local -i dX=$(getSize $display | cut -d 'x' -f 1)
    local -i dY=$(getSize $display | cut -d 'x' -f 2)
    local -i picX=$(mediainfo "$img" | sed -n '/Width/s/[^[:digit:]]//gp');
    local -i picY=$(mediainfo "$img" | sed -n '/Height/s/[^[:digit:]]//gp');
    if [[ $picX -gt $dX ]] || [[ $picY -gt $dY ]]; then
        echo ">";
    else
        echo "<";
    fi
}


primary="$(xrandr -q | awk '/primary/{print $1}')";
resolution="$(xrandr -q | sed -n "/^Screen ${DISPLAY#:}:/s/.*current\s\([0-9]\+\)\sx\s\([0-9]\+\).*/\1x\2/p")";
eval "$(xrandr -q | sed -n '/\bconnected\b/s/^\([A-Z][A-Z0-9]\+\)\s\+\(connected\|connected primary\)\s\([0-9x+]\+\).*/display["\1"]="\3";/p;1i \declare -A display;';)"

if [[ -z "$1" ]]; then
    help;
    exit 1;
fi

[[ -d "$1" ]] && declare imageDir="$1";

for file in $@; do
    [[ -f $file ]] && files+=("$file");
done

if [[ -z $imageDir ]] && [[ -z ${files[@]} ]]; then
    help;
    exit 1;
fi

declare -i iC=-1;


## prepare canvas
convert -size ${resolution} xc:black /tmp/wallpaper.png
for dName in ${!display[@]}; do
    image=$(getImage);
    iC=$?
    convert "${image}" -resize "$(getSize ${display[$dName]})$(: ${display[$dName]} ${image})" /tmp/tmp_${dName}.png
    mv /tmp/wallpaper.png /tmp/tmp_wall.png
    convert /tmp/tmp_wall.png -draw "image over $(getOffset ${display[$dName]} "/tmp/tmp_${dName}.png"),0,0 '/tmp/tmp_${dName}.png'" /tmp/wallpaper.png
    rm -f /tmp/tmp_${dName}.png
    rm -f /tmp/tmp_wall.png
done

display -window root /tmp/wallpaper.png
