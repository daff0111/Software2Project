<%@page import="java.util.List"%>
<!DOCTYPE html>
<!--
To change this license header, choose License Headers in Project Properties.
To change this template file, choose Tools | Templates
and open the template in the editor.
-->
<%! private int m_carNum;%>
<%! private List<Float> m_latList;%>
<%! private List<Float> m_lngList;%>
<% 
    m_carNum = (int) Integer.parseInt(request.getAttribute("carsNum").toString());
    m_latList = (List<Float>) request.getAttribute("latList");
    m_lngList = (List<Float>) request.getAttribute("lngList");
%>
<%
    if (session.getAttribute("authentication") != null && session.getAttribute("authentication").equals(true)) {
        response.sendRedirect("/PowerEnjoy-war/MainPage");
    }
%>
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
        <comment>Car List will be here. <br> There are <%=m_carNum%> Cars Available</comment><br>
        </td>
        <td>
            <div id="map"></div>
            <script>
                function initMap() {
                    // Try HTML5 geolocation.
                    if (navigator.geolocation) {
                        navigator.geolocation.getCurrentPosition(function (position) {
                            var userPos = {
                                lat: position.coords.latitude,
                                lng: position.coords.longitude
                            };
                            var userMarker = new google.maps.Marker({
                                position: userPos,
                                map: map
                            });
                            map.setCenter(userPos);
                        }, function () {
                            handleLocationError(true, infoWindow, map.getCenter());
                        });
                        var map = new google.maps.Map(document.getElementById('map'), {
                            zoom: 12,
                            center: car1
                        });
                        var carImage = "http://localhost:8080/PowerEnjoy-war/resources/map_car_icon.png";
                        var car1 = {lat: 45.440, lng: 9.173};
                        var marker = new google.maps.Marker({
                            position: car1,
                            map: map,
                            icon: carImage
                        });
                    } else {
                        // Browser doesn't support Geolocation
                        handleLocationError(false, infoWindow, map.getCenter());
                    }
                }

                function handleLocationError(browserHasGeolocation, infoWindow, pos) {
                    infoWindow.setPosition(pos);
                    infoWindow.setContent(browserHasGeolocation ?
                            'Error: The Geolocation service failed.' :
                            'Error: Your browser doesn\'t support geolocation.');
                }
            </script>
            <script async defer
                    src="https://maps.googleapis.com/maps/api/js?key=AIzaSyCmFrKyeDan2sXnWj7Uh9NYEwNvYsxtRFg&callback=initMap">
            </script>
        </td>
    </tr>
</table>
<center>
    <form action="/PowerEnjoy-war/Logout" method="post">
        <input type="submit" value="Log Out">
    </form>
</body>
</html>
