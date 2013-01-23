# Dfygraviti Library

##About
This library contains Sub libraries including

* IRRemote
* EEPROM
* RTClib
* Wire
* DfygravitiServer
* DatabaseManager

## Burning the code to Arduino

* Copy the **Dfygraviti** folder in the repository into the following path

<pre><code> ~/path/to/arduino/libraries/</code></pre>

* Copy the IRDfygraviti folder into your sketchbook directory, or just open it in **Arduino IDE**
* Connect the Arduino Uno to the USB port
* Upload the code.

##Using the Library in Android
* To use the library create a Bluetooth socket.
* Send the hex codes through the socket once the connection is successful.
* the Codes are as follows.


<pre>

<table border>
<tr>
	<td>   0x38 and 0x39  </td>
	<td>  Send the data to the TV through IR  </td>
</tr>
<tr>
	<td>   0x45 and 0x21 </td>
	<td>  Get the RTC time</td>
</tr>
<tr>
	<td>   0x51 and 0x4a </td>
	<td>  clear the EEPROM Stored Data</td>
</tr>
<tr>
	<td>   0x19 and 0x91 and 0xFF </td>
	<td>  Get all the stack data stored in the EEPROM</td>
</tr>
<tr>	<td>   0x19 and 0x91 and 0x01<br> ,followed by the index<br>
</td>
	<td>  Get one element in the stack<br>  ex: 0x19 0x91 0x01 0x0a<br> (to get 10th element in the EEPROM) </td>
</tr>
</table>

</pre>

