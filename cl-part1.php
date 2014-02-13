<?php

$gw = 26;
$groups[0] = array('Atlanta','As Monacco','Elche','Cheslea');
$gw2[0] = array('18763','10294','159599','306560');

$groups[1] = array('Espanyol','Aston Villa','Paris Saint-Germain','Catania');
$gw2[1] = array('43543','46','46103','96109');

$groups[2] = array('Swansea','Athletico Madrid','Nurnber','Panathinaikos');
$gw2[2] = array('8486','23093','63025','4491');

$groups[3] = array('Athletic Bilabo','KR','Hull','Borussia');
$gw2[3] = array('57410','239','179','6694');


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
