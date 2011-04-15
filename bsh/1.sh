fd=$1
fd2=$2
declare -a massiv

exec < $fd
while read md5 siz ;
do
	#echo [$md5][$siz]
	while read -e lin  && [[ -n $lin ]];
	do
		#echo [$lin]
		massiv[$lin]="${md5}x${siz}"	
	done
done



exec < $fd2
while read md52 siz2 lin2;
do
	echo $lin2 ${massiv[$lin2]}
#	if [[ ${massiv[$lin2]} == "${md52}x${siz2}" ]]
#	then
#		echo ':('
#	fi;
done 
