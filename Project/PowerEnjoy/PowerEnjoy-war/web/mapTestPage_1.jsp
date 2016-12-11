<!DOCTYPE html>
<!--
To change this license header, choose License Headers in Project Properties.
To change this template file, choose Tools | Templates
and open the template in the editor.
-->
<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>Main Page</title>
    <center id="logo"><a href="/PowerEnjoy-war"><img src="http://localhost:8080/PowerEnjoy-war/resources/powerEnjoyLogo.png" alt="logo" style="width:350px;height:120px;"/></a></center>
    <link rel="stylesheet" href="http://localhost:8080/PowerEnjoy-war/styles/main.css" media="screen">                         
</head>
<body bgcolor="#006FF1">
    <table border="0">
        <col width="300px" />
        <col width="800px" />
        <tr>
            <td>
        <white>User </white><br>
        </td>
        <td>
            <div id="map"></div>
            <script>
                function initMap() {    
                    var form = document.createElement("form", {action: "/PowerEnjoy-war/MainPage",
                        method: "post"});
                    // Try HTML5 geolocation.
                    if (navigator.geolocation) {
                        navigator.geolocation.getCurrentPosition(function (position) {
                            var userPos = {
                                lat: position.coords.latitude,
                                lng: position.coords.longitude
                            };
                            alert("UserLocation " + position.coords.latitude + "," + position.coords.longitude);
                            form.appendChild(createElement("input", {type: "hidden",
                                name: "userLocation",
                                value: position.coords.latitude + "," + position.coords.longitude}));
                        }, function () {
                        });
                    } else {
                        // Browser doesn't support Geolocation
                    }
                    document.body.appendChild(form);
                    form.submit();
                    document.body.removeChild(form);
                }
            </script>
            <script async
                    src="https://maps.googleapis.com/maps/api/js?key=AIzaSyCmFrKyeDan2sXnWj7Uh9NYEwNvYsxtRFg&callback=initMap">
            </script>
        </td>
    </tr>
</table>
<%
    response.sendRedirect("/PowerEnjoy-war/MainPage");
%>
<center>
    <form action="/PowerEnjoy-war/MainPage" method="post">
        <input type="submit" name="Refresh" value="Refresh">
    </form></center>
</body>
</html>
