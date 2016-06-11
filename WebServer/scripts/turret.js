function sendCommand(command){
    var xmlHttp = new XMLHttpRequest();
    xmlHttp.onreadystatechange = function() { 
        if (xmlHttp.readyState == 4 && xmlHttp.status == 200)
            document.getElementById("response").innerText = xmlHttp.responseText;
        }
    xmlHttp.open("GET", motivatorAddress()+command, true); // true for asynchronous 
    xmlHttp.send(null);
}
