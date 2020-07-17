# DSMRlogger-Next
The Next firmware for the DSMR-logger


This is the FORK of Willem's project implementing new features, like direct InfluxDB writer and fixing bugs and trying to improve the codebase.

With the launch of the Final version of DSMRloggerAPI, version 2.0.1 it completed the last and final update of Willem and released it to the public.

This fork is picking up where Willem stopped development, fixing bugs and adding new features. 

The first release DSMRlogger-Next will add:
- Direct writting to InfluxDB
- Fixing DSMR handling so that DSMR spec 4.2 meter will work properly once more.

If you have any idea's or suggestions, please do a Pull Request on the project. 

The original code and release of DSMRloggerAPI can be found here:
[Here](https://willem.aandewiel.nl/index.php/2019/04/09/dsmr-logger-v4-slimme-meter-uitlezer/) and
[here](https://willem.aandewiel.nl/index.php/2020/02/28/restapis-zijn-hip-nieuwe-firmware-voor-de-dsmr-logger/)
you can find information about this project.

Documentation can be found [here](https://mrwheel-docs.gitbook.io/dsmrloggerapi/) (in progress)!

<table>
<tr><th>Versie</th><th>Opmerking</th></tr>
<tr>
<td valign="top">2.1.0-rc0</td>
   <td>Release Candidate (rc0)
   <br>Implementing direct InfluxDB support 
   <br>Bugfix reading DSMR 4.2 correctly (timing issues)
   </td>
</tr>
   <td valign="top">2.0.1</td>
   <td>First Final Release
   <br>Implementing DSMRloggerWS actual api (for backwards compatibility)
   <br>More robust way to write hourly data to the RING-files
   <br>Bugfix PRE40 gasmeter reading
   <br>Remove validity check on meterstanden editor
   <br>Better FieldName translation
   <br>Bugfix mindergas processing
   </td>
</tr>
   <td valign="top">1.2.1</td>
   <td>Third Official Release
      <br>Instelling SM_HAS_NO_FASE_INFO nu via settings
      <br>Selectie OLED scherm via settings
      <br>Mogelijkheid om het oled scherm 180* te flippen via settings
      <br>Check op volgordelijkheid Uren (in de GUI)
      <br>macaddress in /api/v1/dev/info (Phyxion)
      <br>Bailout some functions on low heap
      <br>Simplification and better tab restAPIs
      <br>Editer Maanden tabel verbetert  (maar nog steeds lastig te gebruiken)
      <br>Betere test of er op github nieuwe firmware beschikbaar is
      <br>bugfix prevent GUI firering multiple restAPI call's
      <br>The Leading Edge type GUI now completely from github. Set
      'index page' to "DSMRindexEDGE.html" to make use of
      the latest development (but be aware, there might be bugs)
   </td>
</tr>
<tr>
   <td valign="top">1.1.0</td>
   <td>Second Official Release
      <br>Introduction ESP_SysLogger
      <br>GUI is now even more a Single Page Application (SPA)
      <br>Better coping with WiFi losses (continue operation and try to reconnect)
      <br>Restructure slimmemeter.loop() etc.
      <br>You can find pré compiled binaries at <b>code</b> -> <b><i>releases</i></b>
   </td>
</tr>
<tr>
   <td valign="top">1.0.1</td>
   <td>First official release
      <br>You can find pré compiled binaries at <b>code</b> -> <b><i>releases</i></b>
   </td>
</tr>
<tr>
   <td valign="top">0.3.5 RC3</td>
   <td>Third Release Candidate
   </td>
</tr>
</table>
