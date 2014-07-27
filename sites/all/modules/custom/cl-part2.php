<?php

$gw = 26;
$groups[0] = array('Toulouse','Tottenham Hostpur','Malaga','Sporting');
$gw2[0] = array('6887','178428','150487','58');

$groups[1] = array('Hellas','VfB','FC Basel','Everton');
$gw2[1] = array('9734','8251','44','283');

$groups[2] = array('Hertha','Sevilla','Lazio','Montpellier');
$gw2[2] = array('169','1588','41','550');

$groups[3] = array('Feyenoord','Barcelona','Juventus','Bayern Munich');
$gw2[4] = array('6015','1166','84','12438');

echo '<table>';
foreach ($groups as $group_key=>$group) {
	$index = $group_key + 1;
	echo  "<tr> <td> This is group " . $index . '</td></tr>';
	foreach ($group as $team_id => $item) {
    echo "<tr><td>" . $item . "</td><td>" .  score($gw2[$group_key][$team_id],$gw) . "</td></tr>";
	}
}
echo '</table>';

function score($team_id,$gw) {
	$str2="http://fantasy.premierleague.com/entry/".$team_id."/event-history/".$gw;
	$file=fopen($str2,"r") or die("Heavy load on FPL. Please try later");
	while(!feof($file)) {
		$str=fgets($file);
		if(preg_match('/(<div class="ismSBValue ismSBPrimary">(.*))/', $str,$ar)) {
			$str2=fgets($file);
			$str2=fgets($file);
			$str2 = strip_tags($str2);
			$str2=str_replace("pts","",$str2);
			$temp1=(int)$str2;
		}

		if(preg_match('/(<dl class="ismDefList ismSBDefList">(.*))/', $str,$ar)) {
			$str2=fgets($file);
			$str2=fgets($file);
			$str2=fgets($file);
			$str2=fgets($file);
			$str2=fgets($file);
			$str2=fgets($file);
			$str2 = strip_tags($str2);
			$str2=str_replace("pts","",$str2);
			$str2=str_replace("(-","",$str2);
			$str2=str_replace(")","",$str2);
			$temp2=(int)$str2;
			$z=$temp1-$temp2;
			return $z;
		}
	}
}

?>
