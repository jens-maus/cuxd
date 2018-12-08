##1
|
<html>
<head>
|<meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
<title>HomeMatic CUxD</title>
</head>
<body style='background-color:#ffffee;margin-bottom:0px; margin-top:5px;'> 
<style type='text/css'>
a.menus {display:block;color:red;font-weight:bold;font-family:arial;font-size:12px;margin-bottom:0px;margin-top:0px;text-align:center;width:16ex;border-style:dotted;padding:2px;text-decoration:none;}
a.menu {display:block;font-weight:bold;font-family:arial;font-size:12px;margin-bottom:0px;margin-top:0px;text-align:center;width:16ex;border-style:ridge;padding:2px;text-decoration:none;}
a.menu:link {color:#00f;background-color:#ffffaa;}
a.menu:visited {color:#00f;background-color:#ffffaa;}
a.menu:hover {color:#ff0;background-color:#00f;}
a.menu:active {color:#ff0;background-color:#a03;}
</style>
<script LANGUAGE="JavaScript">
<!--
function reload_main(){
  var uri=location.href; 
  location.href=uri;
}
//--></script>
<table align='center' border='0' bordercolor='transparent' width='300'> 
<tr>
<td style='font-family:arial;font-style:italic;font-size:12px;white-space:nowrap'>CCU-Firmware:&nbsp;%s&nbsp;&nbsp;&nbsp;</td>
<td style='border-style:none;margin-bottom:0px;margin-top:0px;'><img src='%s?m=110'></td>
<td style='font-family:arial;font-style:italic;font-size:12px'>&nbsp;&nbsp;Version&nbsp;%s</td>
</tr>
</table>
<hr/>
<div class="menu">
<table align='center' border='0' bordercolor='transparent' width='90%%' cellpadding='0' cellspacing='0'>
<tr>
|
##2
|
<html>
<head></head>
<body style='margin:0; padding:0; background-color:#ffffee'>
<table align='center' border='0' style='background-color:transparent' cellpadding='0' cellspacing='0' width='100%%'>
<tr>
<td width='20%%'>&nbsp;</td>
<td width='60%%'>
<div style='text-align:center;font-family:arial;font-size:13px;color:blue;margin-top:5px;margin-bottom:10px;font-weight:bold;white-space:nowrap'>
CUxD-Ger&auml;te verwalten
</div>
</td>
<td width='20%%' align='right'>
<form name='optionsform' id='optionsform' style='margin:0;padding:0;' method='get' action=''>
<input type='button' value='Open' style='width:70px;font-family:arial;font-size:12px;' onClick='window.open("?m=52")' />
</form>
</td>
</tr>
</table>
<div style='border:0px;font-family:arial;font-size:11px;background-color:#ffffee'>
<table border='0' style='background-color:#FFFFEE;' width='100%%' height='95%%' cellpadding='0' cellspacing='0'>
<tr>
<td id='left_table' style='width:50%%; height:100%%; padding:16px'>
<form id='dtype' name='dtype' method='get' action='%s' target='doptions' style='border:0px;font-family:arial;font-size:13px;white-space:nowrap'>
<b>CUxD&nbsp;Ger&auml;tetyp:</b>&nbsp;\
<select name='m' size='1' style='width:50%%;' onChange='dtype2.value=0;dtype.submit();'>
<option value='200' style='background-color:#E0E0FF'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;----- EnOcean -----</option>
<option value='233'>(33) [RPS] Taster und Drehgriffkontakt</option>
<option value='234'>(34) [1BS] T&uuml;r-/Fensterkontakt</option>
<option value='235'>(35) [4BS] Sensoren</option>
<option value='236'>(36) Aktoren</option>
<option value='237'>(37) BiDi Aktoren</option>
<option value='200' style='background-color:#E0E0FF'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;----- wireless M-Bus -----</option>
<option value='225'>(25) wM-Bus Sensoren</option>
<option value='200' style='background-color:#E0E0FF'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;----- Moehlenhoff -----</option>
<option value='238'>(38) Alpha2</option>
<option value='200' style='background-color:#E0E0FF'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;----- Wetter -----</option>
<option value='232'>(32) Thermo</option>
<option value='201'>(01) Thermo/Hygro</option>
<option value='231'>(31) Kombi KS200/KS300</option>
<option value='200' style='background-color:#E0E0FF'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;----- FS20 -----</option>
<option value='203'>(03) Sensor (1-Kanal)</option>
<option value='202'>(02) Schaltaktor (1-Kanal)</option>
<option value='204'>(04) Dimmaktor (1-Kanal)</option>
<option value='205'>(05) Relais (1-Kanal)</option>
<option value='200' style='background-color:#E0E0FF'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;----- Energie -----</option>
<option value='206'>(06) EM 1000</option>
<option value='227'>(27) ESA 1000/2000</option>
<option value='200' style='background-color:#E0E0FF'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;----- FHT80 Heizung -----</option>
<option value='207'>(07) Ventilantrieb</option>
<option value='208'>(08) Wandthermostat</option>
<option value='209'>(09) Multiventil-Steuerung</option>
<option value='210'>(10) TF-2 (2-Kanal)</option>
<option value='200' style='background-color:#E0E0FF'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;----- HMS Sensoren -----</option>
<option value='212'>(12) HMS100 TF</option>
<option value='213'>(13) HMS100 T</option>
<option value='214'>(14) HMS100 W/WD</option>
<option value='215' title='Funktion noch nicht getestet!' style='background-color:#FFFF90;'>(15) RM 100-2</option>
<option value='218'>(18) HMS100 MG</option>
<option value='220' title='Funktion noch nicht getestet!' style='background-color:#FFFF90;'>(20) HMS100 CO</option>
<option value='226' title='Funktion noch nicht getestet!' style='background-color:#FFFF90;'>(26) HMS100 FIT</option>
<option value='216'>(16) HMS100 TFK</option>
<option value='200' style='background-color:#E0E0FF'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;----- sonstiges -----</option>
<option value='229'>(29) BidCos/AskSin</option>
<option value='228'>(28) System</option>
<option value='290'>(90) Universal Wrapper Device</option>
<option value='211'>(11) RS232-F&uuml;llstandsmesser (icplan.de)</option>
<option value='240'>(40) 16 Kanal Universalsteuerung</option>
<option value='291'>(91) CloudMatic ...</option>
|<option value='241'>(41) Ping / Alive 16fach</option>
<option value='200' style='background-color:#FFC0C0'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;----- Experimental -----</option>
<option value='230' style='background-color:#FFE0E0'>(30) Wetter Thermo/Hygro/Baro</option>
<!-- <option value='299' style='background-color:#FFE0E0'>(99) email-SMTP/IMAP Device</option> -->
</select>
<input type='hidden' name='dtype2' value='0' />
</form>
<iframe name='doptions' src='%s?m=50' width='99%%' height='310px' scrolling='no' frameborder='0' style='background-color:transparent;border:0px solid #cccccc; padding:0px;'>
</iframe>
</td>
<td style='width:50%%; height:100%%;'>
<iframe name='dstatus' src='%s?m=51' width='99%%' height='100%%' scrolling='no' frameborder='0' style='background-color:#ffffee;border:0px solid #cccccc; padding:0px;'>
</iframe>
</td>
</tr>
</table>
</div>
</body>
</html>
|
##3
|
<td style='text-align:center'>
<form style='margin:0;padding:0;' method='get' action='%s' target='mount'>
<input type='hidden' name='m' value='70'/>
<input type='submit' value='Ger&auml;teeinstellungen speichern' style='width:200px;font-family:arial;font-size:12px;'/>
</form>
</td>
<td style='text-align:center'>
<form style='margin:0;padding:0;' method='get' action='%s' target='cuxdstatus'>
<input type='hidden' name='m' value='101'/>
<input type='submit' value='CUxD-Restart' style='width:100px;font-family:arial;font-size:12px;' onClick='return(confirm("Restart CUxD?"));'/>
</form>
</td>
<td style='text-align:center'>
<form style='margin:0; padding:0;' method='get' action='%s' target='cuxdstatus'>
<input type='hidden' name='m' value='100'/>
<input type='submit' value='CUxD-Stop' style='width:100px; font-family:arial;font-size:12px;' onClick='return(confirm("Stop CUxD?"));'/>
</form>
</td>
<td style='text-align:center'>
<form style='margin:0; padding:0;'>
<input type='button' value='Refresh' style='width:100px; font-family:arial;font-size:12px;' onClick='parent.cuxddisplay.cuxdstatus.location.reload(false);' />
</form>
</td>
|
##4
|
<td style='text-align:center'>
<form style='margin:0;padding:0;'>
<input type='button' value='%s' style='width:100px;font-family:arial;font-size:12px;' onClick='parent.cuxddisplay.document.infoform.m.value=%d;parent.cuxddisplay.document.infoform.lines.value=%d;parent.cuxddisplay.document.infoform.submit()'/>
</form>
</td>
|<input type='button' value='Refresh' style='width:100px; font-family:arial;font-size:12px;' onClick='window.frames["cuxdoutput"].location.reload()'/>
|<input type='button' value='Refresh' style='width:100px; font-family:arial;font-size:12px;' onClick='alert(top.document.getElementById("12345"))'/>
|
##5
|
<td><div align='center'><a class='menu%s' href='%s?m=%d'>%s</a></div></td>
|
##6
|
</tr>
</table>
<hr/>
</div>
<div align='center'>
<iframe name='cuxddisplay' src='%s?m=1%d' width='90%%' height='%d' scrolling='no' frameborder='0' style='background-color:transparent;border:0px solid #11cccc;padding:0px;'>
</iframe>
|
##7
|
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
</head>
<body style='font-family: courier new;margin:0;padding:0;background-color:#ffffee'>
<table align='center' border='0' style='background-color:transparent' cellpadding='0' cellspacing='0' width='100%%'>
<tr>
<td width='35%%'>&nbsp;</td>
<td width='30%%'>
<div style='text-align:center;font-family:arial;font-size:13px;color:blue;margin-top:5px;margin-bottom:10px;font-weight:bold;white-space:nowrap'>
Aktuelle Status Information
</div>
</td>
<td width='35%%' align='right'>
<form name='statusform' id='statusform' style='margin:0;padding:0;' method='get' action='' target='cuxdstatus'>
<input type='button' value='SERVICE' title='CCU Servicefunktionen' style='width:70px;font-family:arial;font-size:12px;' onClick='window.open("?maintenance=","_top")' />
<input type='button' value='ADDR' title='aktuell empfangene Adressen anzeigen' style='width:70px;font-family:arial;font-size:12px;' onClick='window.open("?m=17&i=2","ADDR")' />
<input type='button' value='Open' style='width:70px;font-family:arial;font-size:12px;' onClick='window.open("?m=15")' />
</form>
</td>
</tr>
</table>
<div align='center' style='border:1px solid #aaaaaa;'>
<iframe name='cuxdstatus' src='%s?m=15' width='100%%' height='380' name='content' scrolling='auto' frameborder='0' style='padding:0px;'>
</iframe>
</div>
</body>
</html>
|
##8
|
<html>
<head>
%s
</head>
<body style='font-family:courier new;margin:0;padding:0;background-color:#ffffff'>
%s
<div style='border:1px;font-size:11px;background-color:#ffffff'>
<pre>
|
##9
|
</pre></div>
<a name="ET" />
</body>
<script LANGUAGE="JavaScript">
<!--
window.location.hash='ET';
//--></script>
</html>
|
##10
|
<html>
<head></head>
<body style='font-family:courier new;margin:0;padding:0;background-color:#ffffee'>
<table align='center' border='0' style='background-color:transparent' cellpadding='0' cellspacing='0' width='100%%'>
<tr>
<td width='30%%'>&nbsp;</td>
<td width='40%%'>
<div id='titletext' style='text-align:center;font-family:arial;font-size:13px;color:blue;margin-top:5px;margin-bottom:10px;font-weight:bold;white-space:nowrap'>
Info
</div>
</td>
<td width='30%%' align='right'>
<form name='infoform' id='infoform' style='margin:0;padding:0;' method='get' action='' target='cuxdoutput'>
<nobr>
<input type='text' name='filter' />
<input type='submit' name='a' value='All' title='Alle Daten laden' style='width:70px;font-family:arial;font-size:12px;' onClick='form.lines.value=0' />
<input type='button' value='Open' style='width:70px;font-family:arial;font-size:12px;' onClick='window.open("?m="+form.m.value+"&lines=0&logfile="+form.logfile.value+"&filter="+encodeURIComponent(form.filter.value))' />
</nobr>
<input type='hidden' name='m' value='20' />
<input type='hidden' name='lines' value='%d' />
<input type='hidden' name='logfile' value='%s' />
|</span>
</form>
</td>
</tr>
</table>
<div align='center' style='border:1px solid #aaaaaa;'>
<iframe width='100%%' scrolling='auto' height='380' frameborder='0' style='padding:0px;' src='index.ccc?m=20&lines=%d' name='cuxdoutput'>
</iframe>
</div>
</body>
</html>
|
##11
|
<html>
<head></head>
<body style='font-family:courier new;margin:0;padding:0;background-color:#ffffee'>
<table align='center' border='0' style='background-color:transparent' cellpadding='0' cellspacing='0' width='100%%'>
<tr>
<td width='20%%'>&nbsp;</td>
<td width='60%%'>
<div style='text-align:center;font-family:arial;font-size:13px;color:blue;margin-top:5px;margin-bottom:10px;font-weight:bold;white-space:nowrap'>
Sende/Empfangs-Terminal
</div>
</td>
<td width='20%%' align='right'>
<form name='termform' id='termform' style='margin:0;padding:0;' method='get' action='' target='cuxdoutput'>
<input type='button' value='Open' style='width:70px;font-family:arial;font-size:12px;' onClick='window.open("?a=All&m=23")' />
</form>
</td>
</tr>
</table>
<div align='center' style='border:1px solid #aaaaaa;'>
<iframe width='100%%' scrolling='no' height='380' frameborder='0' style='padding:0px;' src='index.ccc?m=16' name='cuxdoutput'>
</iframe>
</div>
</body>
</html>
|
##12
|
<table align='center' border='0' bordercolor='transparent' style='background-color:transparent' width='90%%'>
<tr>
<td>
<form style='margin:0;padding:0;' method='post' action='%s?m=1' target='cuxdoutput' onSubmit='document.getElementById("sendcmd").focus();return true;'> 
<input id='sendcmd' name='cmd' style='width:70%%' rows='1'>&nbsp;<select name='dev' style='width:18%%;font-family:arial;font-size:12px;' onChange='document.getElementById("sendcmd").focus();'>
%s
</select>&nbsp;<input type='submit' value='Senden' style='width:10%%;font-family:arial;font-size:12px;'/>
</td>
</tr>
</table>
</form>
<script LANGUAGE="JavaScript">
<!--
document.getElementById('sendcmd').focus();
//--></script>
|
##13
|
Seriennummer:&nbsp;<input type='text' name='dserial' maxlength='5' size='5' value='%d'/> (numerisch&nbsp;max.&nbsp;5&nbsp;Stellen)<br>
<br style='font-size:16px'>Name:&nbsp;<input type='text' name='dname' maxlength='50' size='20'/> (leer&nbsp;=&nbsp;wird&nbsp;autom.&nbsp;generiert)<br>
<br style='font-size:16px'>
<table style='width:360px;font-family:arial;font-size:13px;'>
<tr>
<td style='width:300px;'>\
|
##14
|
</td><td><img src='%s?m=%d' name='pictures' border='0'>
</td></tr></table><br><br style='font-size:16px'>
<input type='submit' value='Ger&auml;t auf CCU erzeugen !' style='width:180px;font-family:arial;font-size:12px;' onclick='return confirm_device(form.dtype,form.dtype2);' />
</form>
|
##15
|
string c;
string ret;
foreach (c,root.Channels().EnumUsedIDs()){
  var c1 = dom.GetObject(c);
  if (c1.Address().Find('%s') != -1) {
    var c2 = dom.GetObject(c1.Interface());
    ret = c2.Name() # '|' # c1.Address() # '|' # c1.HssType() # '|';
    string dp;
    foreach (dp,c1.DPs()) {
      var dpo = dom.GetObject(dp);
      if (dpo.TypeName() == "HSSDP") {	
        ret = ret # dpo.HssType() # '|';
      }
    }
    Write(ret);
    quit;
  }
}
|
##16
|
<html>
<head></head>
<script LANGUAGE="JavaScript">
<!--
var title=parent.document.getElementById('titletext');
title.innerHTML='%s';
//--></script>
<body style='font-family:courier new;margin:0;padding:0;background-color:#ffffff'>
<div style='border:1px;font-size:11px;background-color:#ffffff;'>
<pre style='margin-top:0px;margin-bottom:0px;'>
|
##17
|
string id;
foreach(id, root.Devices().EnumUsedIDs()){
  object o = dom.GetObject(id);
  if (o.Address().Find('%s') != -1) {
    o.Name('%s');
  }
}
foreach(id, root.Channels().EnumIDs()) {
  object o = dom.GetObject(id);
  if (o.Address().Find('%s') != -1) {
    o.Name('%s' # o.Address().Substr(10));
  }
}
|
##18
|
string id;
string fn;
foreach(id, root.Channels().EnumIDs()) {
  object o = dom.GetObject(id);
  if (o.Name().Find('%s') != -1) {
    foreach(fn, o.ChnFunction()) { 
      object obj = dom.GetObject(fn);
      obj.Remove(id); 
    }
  }
}
|
##19
|
<script LANGUAGE="JavaScript">
<!--
function submit_dtype2(obj) {
  var f = parent.document.getElementById('dtype');
  f.dtype2.value=obj.options[obj.selectedIndex].value;
  f.submit(true);
}
//--></script>
<div style='font-family:arial;font-size:13px;' align='left'>
<form name='dtype' method='post' action='%s?m=3' target='dstatus'>
<input type='hidden' name='dtype' value='%d'/>
|
##20
|
string id;
foreach(id,root.Devices().EnumUsedIDs()){
  object o=dom.GetObject(id);
  if (o.Address().Find('CUX')==0) {
    WriteLine(o.Address()#'\t'#o.Label());
  }
}
id="DONE";
|
##21
|
string id;
string cf;
foreach(id,root.Devices().EnumUsedIDs()){
  object o=dom.GetObject(id);
  if (o.Address().Find('CUX')==0) {
    if (o.ReadyConfig()==true) {cf='X';} else {cf='?';}
    WriteLine(o.Address()#'\t'#cf#'\t'#o.Name());
  }
}
|
##22
|
<html>
<head>
<script LANGUAGE="JavaScript">
<!--
function showimage() {
  if (document.images) {
    document.images.pictures.src='%s?m='+document.dtype.dbase.options[document.dtype.dbase.selectedIndex].value;
  }
}
function confirm_device(dtype,dtype2) {
  if (dtype && dtype2 && dtype.value && dtype2.value) {
    if ((dtype.value == 37) && (dtype2.value == 0)) {
      var msg = "*** ACHTUNG ***\n\n";
      msg += "Bei PEHA-Schaltaktoren mit Tastenwippe und Geraetefirmware < 2.27 ist die Wippe nach dem Anlernen dauerhaft gesperrt.\n";
      msg += "Dieses Problem laesst sich danach nur durch Einsenden des Aktors zum Hersteller beheben!";
      return confirm(msg);
    }
  }
  return true;
}
//--></script>
</head>
|
##23
|
|<br><div style='width:100%%;border:0px;text-align:center;font-family:arial;font-size:13px;color:black;margin-top:0px;margin-bottom:1px;font-weight:bold;'>
|Abgeschlossen<br>
|</div>
|<br>
<br><br>
<a href='%s?m=55' target='_blank'>Ansehen/Download Firmware-Update Log</a>
|
##24
|
<br><br>Option:&nbsp;<select name='dcontrol' size='1'>
<option value='0'></option>
<option value='1'>+Statistik</option>
</select>
|
##25
|
<br><br>Control:&nbsp;<select name='dcontrol' size='1'>
<option value='0'>T&uuml;r-/Fensterkontakt</option>
<option value='1'>Schalter</option>
<option value='2'>Gefahrenmelder</option>
<option value='3'>Event-Trigger</option>
<option value='5'>Bewegungsmelder</option>
<option value='4'>ohne</option>
</select>
|
##26
|
<br><br>Typ:&nbsp;<select name='dcontrol' size='1'>
<option value='0'>unbekannt</option>
<option value='1'>Stromz&auml;hler</option>
<option value='2'>Wirkleistungsmesser/HSM</option>
<option value='3'>Gasz&auml;hler</option>
</select>
|
##27
|
<html>
<head>
<meta http-equiv='REFRESH' content='%d;URL=%s?m=16'/> 
</head>
<body style='font-family:courier new;margin:0;padding:0;background-color:#ffffff'>
<div style='border:1px;font-size:11px;background-color:#ffffff'>
<pre style='margin-top:0px;margin-bottom:0px;'>
|
##28
|
string c;
foreach (c,root.Channels().EnumUsedIDs()){
  var c1 = dom.GetObject(c);
  if (c1.Address().Find('%s') != -1) {
    string dp;
    string ret = c1.HssType() # ' ' # c;
    var dpo;
    foreach (dp,c1.DPs()) { dpo = dom.GetObject(dp);
|    foreach (dp,c1.DPs().EnumUsedIDs()){ dpo = dom.GetObject(dp);
|      if (dpo.Operations() & OPERATION_WRITE) {
        if (dpo.TypeName() == "HSSDP") {
          ret = ret # '|' # dpo.HssType() # ' ' # dp;
        }
|      }
    }
    Write(c1.Address() # '|' # ret);
    quit;
  }
}
|
##29
|
</pre></div>
</html>
|
##30
|
</head><body style='background-color:#ffffee;'><span style='font-family:arial;font-size:12px;'>
|
##31
|
</span><br><br style='font-size:16px'><form name='dlist' method='post' action='%s?m=4' target='dstatus'> 
<select name='dselect' size='20' style='width:100%%;font-family:courier new;font-size:12px;'>
|
##32
|
<table border='0' style='width:100%%; '><tr>
<td style='vertical-align:top'>
<input type='submit' value='Ger&auml;t von CCU l&ouml;schen !' style='width:180px;font-family:arial;font-size:12px;' onClick='var si = dselect.selectedIndex;return(si != -1 && confirm("Ger&auml;t [CUX" + dselect.options[si].value + "] l&ouml;schen?"));'/>
&nbsp;&nbsp;&nbsp;
<input type='button' value='Ger&auml;t bearbeiten' style='width:180px;font-family:arial;font-size:12px;' onClick='var si = dselect.selectedIndex;if (si != -1) { parent.document.getElementById("left_table").innerHTML="<iframe width=\"100%%\" height=\"100%%\" src=\"index.ccc?m=53&dev="+dselect.options[si].value+"\" name=\"devedit\" scrolling=\"no\" frameborder=\"0\" style=\"padding:0px;\"></iframe>";}'/>
</td></tr></table>
|
##33
|
<html>
<body style='font-family:courier new;margin:0px;padding:0;background-color:#ffffee'>
<table width='100%%'>
<tr>
<td width='50%%' style='text-align:center;vertical-align:top'> 
<span style='text-align:center;font-family:arial;font-size:13px;color:blue; margin-top:5px; margin-bottom:1px;'>
<b>CUxD-Einstellungen:</b><br>
zum Aktivieren speichern (siehe Status-Men&uuml;)
</span><br><br>
<span align=center>
<form method='post' action='%s?m=2'>
<textarea name='cuxdconfig' style='width:100%%;margin-top:0px;margin-bottom:1px; height:350px;font-family:courier new;font-size:12px;'>
|
##34
|
</textarea><br><br>
<nobr><input type='submit' value='Speichern' style='width:120px;font-family:arial;font-size:12px;'/>&nbsp;&nbsp;&nbsp;<input type='submit' value='Parameterabgleich' style='width:120px;font-family:arial;font-size:12px;' onClick='if (confirm("Einstellungen &uuml;berschreiben?")) {this.form.cuxdconfig.value="";return true;} else return false;'/></nobr>
</form>
</div>
</td>
<td width='50%%' style='text-align:center;vertical-align:top'>
<span style='text-align:center;font-family:arial;font-size:13px;color:blue;margin-top:5px;margin-bottom:1px;'>
<b>Firmware-Update:</b><br>Ger&auml;t durch Dr&uuml;cken des Tasters bzw. Terminalbefehl<br>in Update-Modus versetzen (siehe Dokumentation)<br>
<br></span>
<div align=center><form style='margin:0;padding:0;' method='get' action='%s' target='device'> 
<input type='hidden' name='m' value='30'/>
<input type='submit' value='Ger&auml;t suchen' style='font-family:arial;font-size:12px;'/>
</form>
<iframe src='%s?m=30' width='100%%' height='350' name='device' scrolling='no' frameborder='0' style='padding:0px;'>
</iframe>
</span></td></tr></table></body></html>
|
##35
|
<html><body style='margin:0; padding:0; background-color:#ffffee'>
<span style='text-align:center;width:100%%;'>
<div style='width:100%%;border:0px solid;text-align:center;font-family:arial;font-size:13px;color:%s;margin-top:10px;margin-bottom:10px;font-weight:bold;'>%s</div>
|
##36
|
<div style='height:400px;'>
<input type='submit' value='Firmwareupdate starten' style='font-family:arial; font-size:12px;'/>
</form>
</div>
|
##37
|
<br>
<div style='width:100%%;border:0px solid;text-align:center;font-family:arial;font-size:13px; color:black; margin-top:5px; margin-bottom:0px;font-weight:bold;'>
Firmware-Update gestartet:
</div>
|
##38
|
<td style='text-align:center'>
|<form style='margin:0;padding:0;' method='get' target='cuxdoutput' onSubmit='var title=parent.cuxddisplay.document.getElementById("titletext");title.innerHTML="Device-Log";'>
<form style='margin:0;padding:0;' method='get' target='cuxdoutput'>
<input type='hidden' name='m' value='24'><input type='submit' value='%s' style='width:140px; font-family:arial;font-size:12px;'/>
</form>
</td>
|
##39
|
<iframe frameborder='0' name='mount' width='0' height='0'></iframe>
<td style='text-align:center'>
<form style='margin:0;padding:0;' method='get' action='%s' target='mount'>
<input type='hidden' name='m' value='102'/>
<input %s type='submit' value='Mount' style='width:100px;font-family:arial;font-size:12px;%s' title='Automount'/>
</form>
</td>
<td style='text-align:center'>
<form style='margin:0;padding:0;' method='get' action='%s' target='mount'>
<input type='hidden' name='m' value='103'/>
<input %s type='submit' value='Umount' style='width:100px;font-family:arial;font-size:12px;'/>
</form>
</td>
<td style='text-align:center'>
<form style='margin:0;padding:0;' method='get' action='%s' target='mount'>
<input type='hidden' name='m' value='104'/>
<input %s type='submit' value='SYS-Backup' style='width:100px;font-family:arial;font-size:12px;'  onClick='return(confirm("System-Backup ausf&uuml;hren?"));'/>
</form>
</td>
|
##40
|
<html><body onLoad='parent.reload_main()'></body></html>
|
##41
|
<br><br>Option:&nbsp;<select name='dcontrol' size='1'>
<option value='1'>1 Kanal</option>
<option value='2'>2 Kanal</option>
<option value='4'>4 Kanal</option>
<option value='8'>8 Kanal</option>
</select>
|
##42
|
Ger&auml;te-Icon:&nbsp;\
|
##43
|
Ger&auml;t:&nbsp;\
|
##44
|
Seriennummer:&nbsp;<input type='text' name='dserial' maxlength='3' size='3' value='%d'/> (numerisch&nbsp;max.&nbsp;3&nbsp;Stellen)<br>
<br style='font-size:16px'>Name:&nbsp;<input type='text' name='dname' maxlength='20' size='20'/> (leer&nbsp;=&nbsp;wird&nbsp;autom.&nbsp;generiert)<br>
<br style='font-size:16px'>
<table style='width:360px;font-family:arial;font-size:13px;'>
<tr>
<td style='width:300px;'>\
|
##45
|
<input type='hidden' name='dserial' value='1'/>
<br style='font-size:16px'>Name:&nbsp;<input type='text' name='dname' maxlength='20' size='20'/> (leer&nbsp;=&nbsp;wird&nbsp;autom.&nbsp;generiert)<br>
<br style='font-size:16px'>
<table style='width:360px;font-family:arial;font-size:13px;'>
<tr>
<td style='width:300px;'>\
|
##46
|
Funktion:&nbsp;<select name='dtype2' size='1' onChange='submit_dtype2(this)'>
<option value='0'$selected$>Timer</option>
<option value='1'$selected$>Exec</option>
<option value='2'$selected$>Multi-DIM-Exec</option>
<option value='3'$selected$>Ping/Alive</option>
</select><br><br>
|
##47
|
Funktion:&nbsp;<select name='dtype2' size='1' onChange='submit_dtype2(this)'>
<option value='0'$selected$>Transform</option>
<option value='1'$selected$>State</option>
<option value='2'$selected$>Thermostat</option>
</select><br><br>
|
##48
|
<script LANGUAGE="JavaScript">
<!--
  top.cuxddisplay.doptions.location.reload(true);
//--></script>
|
##49
|
Funktion:&nbsp;<select name='dtype2' size='1' onChange='submit_dtype2(this)'>
<option value='0'$selected$>1. PioTek Tracker</option>
<option value='1'$selected$>2. Thermo (A5-02-xx)</option>
<option value='2'$selected$>3. Thermo/Hygro (A5-04-0x)</option>
<option value='3'$selected$>4. Bewegungsmelder (A5-07-xx, A5-08-xx)</option>
<option value='4'$selected$>5. Helligkeitssensor (A5-06-xx)</option>
<option value='5'$selected$>6. Wetterstation (A5-13-xx)</option>
<option value='6'$selected$>7. Raumtemperaturfuehler (A5-10-xx)</option>
<option value='7'$selected$>8. Multifunktions-Sensor (A5-14-xx)</option>
<option value='8'$selected$>9. Drehtaster (A5-38-08)</option>
<option value='9'$selected$>10. Zaehlersensor (A5-12-0x)</option>
</select><br><br>
|
##50
|
Funktion:&nbsp;<select name='dtype2' size='1' onChange='submit_dtype2(this)'>
<option value='0'$selected$>1. KFM 100 S</option>
</select><br><br>
|
##51
|
Funktion:&nbsp;<select name='dtype2' size='1' onChange='submit_dtype2(this)'>
<option value='0'$selected$>1. Schaltaktor (1-4 Kanal)</option>
<option value='1'$selected$>2. Dimmaktor (1-4 Kanal)</option>
<option value='2'$selected$>3. Jalousieaktor (1-4 Kanal)</option>
<option value='3'$selected$>4. IntesisBox</option>
<option value='4'$selected$>5. MD15 Ventilantrieb</option>
<option value='5'$selected$>6. Eltako FMP3 (16-Kanal)</option>
</select><br><br>
|
##52
|
Funktion:&nbsp;<select name='dtype2' size='1' onChange='submit_dtype2(this)'>
<option value='0'$selected$>1. Schaltaktor (1-Kanal)</option>
<option value='1'$selected$>2. SODA Funk-Alarmgriff</option>
<option value='2'$selected$>3. Omnio AWAG Schaltaktoren</option>
<option value='3'$selected$>4. Omnio AWAG Dimmer</option>
<option value='4'$selected$>5. Omnio AWAG Jalousieaktoren</option>
</select><br><br>
|
##53
|
Ger&auml;t:&nbsp;<select name='dtype2' size='1'>
<option value='0'$selected$>ELV</option>
<option value='1'$selected$>Lacrosse</option>
</select><br><br>
|
##54
|
Funktion:&nbsp;<select name='dtype2' size='1' onChange='submit_dtype2(this)'>
<option value='0'$selected$>Mail</option>
<option value='1'$selected$>SMS</option>
<option value='2'$selected$>Push</option>
<option value='3'$selected$>Cloud</option>
<option value='4'$selected$>Webcam</option>
</select><br><br>
|
##55
|
<br><br>Control:&nbsp;<select name='dcontrol' size='1'>
<option value='0'>Taster</option>
<option value='1'>Schalter</option>
<option value='2'>Jalousie</option>
<option value='3'>Dimmer</option>
</select>
|
##56
|
&nbsp;&nbsp;<form target="chart" action="../cuxchart/">\
<input type="hidden" name="loader" value="false" \>\
<input type="hidden" name="legend" value="" \>\
<input type="hidden" name="grouping" value="" \>\
Period:<input type="text" name="period" size="3" value="168" \>\
&nbsp;&nbsp;Range:<input type="text" name="range" size="3" value="30" \>\
&nbsp;&nbsp;Legend:<input type="checkbox" onClick="legend.value = this.checked ? \'\' : \'false\'" checked \>\
&nbsp;&nbsp;Grouping:<input type="checkbox" onClick="grouping.value = this.checked ? \'\' : \'false\'" checked \>\
&nbsp;&nbsp;<input type="submit" value="Chart" title="CUxD-Highcharts" style="width:70px;font-family:arial;font-size:12px;" />\
</form>\
|
##57
|
%s/%s<tr><td colspan=2>
<a href="%s" target="_blank">
<img src="%s" style="max-width:%hupx;height:%hupx" title="open" alt="%s" />
</a>
</td></tr>
|
##58
|
<br><br>Control:&nbsp;<select name='dcontrol' size='1'>
<option value='0'>Taster</option>
<option value='1'>Schalter</option>
<option value='2'>T&uuml;r-/Fensterkontakt</option>
<option value='3'>Bewegungsmelder</option>
<option value='4'>Gefahrenmelder</option>
<option value='5'>Jalousie</option>
<option value='6'>Dimmer</option>
<option value='7'>Drehgriffkontakt</option>
<option value='8'>KeyMatic</option>
</select>
|
##59
|
<body>
<form name='dtype' method='get' action='%s'>
<input type='hidden' name='m' value='53' />
<input type='hidden' name='dev' value='%s' />
<h2 style='font-family:arial'>CUX%s%s</h2>
<table><tr><td style='font-family:arial;font-size:12px;'>
|
##60
|
<td style='width:100px;text-align:right'><img src='%s?m=%d' name='pictures' border='0'></td>
</tr></table><br><br>
<input type='submit' value='Ger&auml;t auf CCU &auml;ndern !' style='width:200px;font-family:arial;font-size:12px;' %s/>
</form>
<body>
|
##61
|
Ger&auml;t:&nbsp;<select name='dtype2' size='1' onChange='submit_dtype2(this)'>
<option value='0'$selected$>1. EnergyCam/iPERL</option>
<option value='1'$selected$>2. SMI260 Wechselrichter</option>
</select><br><br>
|
##62
|
<br><br>Kanal-ID:&nbsp;<input type='text' name='dcontrol' maxlength='2' size='2' value='1'>
|
##63
|
Funktion:&nbsp;<select name='dtype2' size='1' onChange='submit_dtype2(this)'>
<option value='0'$selected$>Raumthermostat</option>
<option value='1'$selected$>Raumthermostat (virtuell)</option>
</select><br><br>
|
##64
|
<html>
<head><title>virtual rooms</title></head>
<body>
<div style='font-family:arial;position:absolute;'>
<div id='process' style='position:absolute;left:0px;top:0px;height:100%;width:100%;opacity:0.7;z-index:-1'></div>
<form onSubmit="var docstyle=document.getElementById('process').style;docstyle.zIndex=99;docstyle.background='#fff url(ajax.gif) no-repeat center';return true;">
|
##65
|
</form>
</div>
</body>
</html>
|
##66
|
<html>
<head>
<meta http-equiv="REFRESH" content="2;url=%s?m=%d&n=%d"/>
</head>
<body>
<center><br><br>%s</center>
</body>
</html>
|
##67
|
<br><br>Option:&nbsp;<select name='dcontrol' size='1'>
<option value='1'>1 Kanal</option>
<option value='4'>4 Kanal</option>
</select>
|
