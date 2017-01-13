/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package penjoySrc.servlets.user;

import penjoySrc.servlets.*;
import java.io.IOException;
import java.util.List;
import javax.ejb.EJB;

import javax.servlet.ServletException;
import javax.servlet.ServletOutputStream;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;
import penjoy.ejb.car.Car;
import penjoy.ejb.car.CarSearchBean;
import penjoy.ejb.reservation.Reservation;

import penjoy.ejb.reservation.ReservationBean;
import penjoy.utils.LocationHelper;

/**
 *
 * @author Domenico
 */
@WebServlet(name = "ReservationServlet", urlPatterns = {"/ReservationServlet"})
public class ReservationServlet extends BaseServlet {

    @EJB
    private ReservationBean m_reservationBean;
    @EJB
    private CarSearchBean m_carSearchBean;

    /**
     * Processes requests for both HTTP <code>GET</code> and <code>POST</code>
     * methods.
     *
     * @param request servlet request
     * @param response servlet response
     * @throws ServletException if a servlet-specific error occurs
     * @throws IOException if an I/O error occurs
     */
    protected void processReservation(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
        if(!request.getParameter("carID").isEmpty())
        {
            Long carID = Long.parseLong(request.getParameter("carID"));
                    
            //Get User ID
            HttpSession session = request.getSession(true);
            Long userID = Long.parseLong(session.getAttribute("id").toString());
            //Reservation activeReservation = m_reservationBean.getAvailableCars();
            System.out.println("CarID: "+carID);
            Car reservedCar = m_carSearchBean.getCarByID(carID);
            if(reservedCar != null)
            {
                if(m_carSearchBean.reserveCar(reservedCar))
                {
                    Reservation newReservation = m_reservationBean.createReservation(userID, carID);
                    String userLocation = "45.462,9.177";//request.getAttribute("userLocation").toString();
                    ServletOutputStream out = response.getOutputStream();
                    out.println(getReservationPageBody(reservedCar, userLocation));
                }
                //getServletContext().getRequestDispatcher("/UserPages/reservationPage.jsp").forward(request, response);
            } 
        }
        else
        {
            getServletContext().getRequestDispatcher("/MainPage").forward(request, response);
        }
    }
    
// <editor-fold defaultstate="collapsed" desc="Test Function: getMainPageBody">
   private String getReservationPageBody(Car reservedCar, String userLocation) {
        String response = "";
        float userLatitude = LocationHelper.getLatitude(userLocation);
        float userLongitude = LocationHelper.getLongitude(userLocation);
        //Response Head
        response = response + "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">"
                    +"<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
                    +"<title>Reservation Page</title>"
                    +"<center id=\"logo\"><a href=\"/PowerEnjoy-war\"><img src=\"http://localhost:8080/PowerEnjoy-war/resources/powerEnjoyLogo.png\" alt=\"logo\" style=\"width:350px;height:120px;\"/></a></center>\n" 
                    +"<link rel=\"stylesheet\" href=\"http://localhost:8080/PowerEnjoy-war/styles/main.css\" media=\"screen\"></head>";                        
        //Response Body
        response = response +"<body bgcolor=\"#006FF1\">";
        //Map Functions
        response = response +"<div id=\"map\"></div><script>\n"+
        //Car Click Path Function
                "function createWalkingPath(from, to, infoWindow, directionsService, directionsDisplay) {\n" +
"                directionsService.route({\n" +
"                    origin: from,\n" +
"                    destination: to,\n" +
"                    travelMode: 'WALKING'\n" +
"                }, function (response, status) {\n" +
"                    if (status === 'OK') {\n" +
"                        var legs = response.routes[0].legs\n"+
"                        var duration = legs[0].duration.text\n"+
"                        infoWindow.setPosition(to);\n" +
"                        infoWindow.setContent('Distance: '+duration);\n"+
"                        directionsDisplay.setDirections(response);\n" +                 
"                    } else {\n" +
"                        window.alert('Directions request failed due to ' + status);\n" +
"                    }\n" +
"                });\n" +
"            }\n"+
        //Map Function
                     "function initMap() {\n" +
"                    if (navigator.geolocation) {\n" +
"                        navigator.geolocation.getCurrentPosition(function (position) {\n" +
"                            var userPos1 = {\n" +
"                                lat: position.coords.latitude,\n" +
"                                lng: position.coords.longitude\n" +
"                            };\n" +
"                            userPos = userPos1;"+
"                            var userMarker = new google.maps.Marker({\n" +
"                                position: userPos,\n" +
"                                map: map\n" +
"                            });\n" +
"                            map.setCenter(userPos);\n" +
"                        }, function () {\n" +
"                            handleLocationError(true, infoWindow, map.getCenter());\n" +
"                        });\n" +
"                        var directionsService = new google.maps.DirectionsService;\n" +
"                        var directionsDisplay = new google.maps.DirectionsRenderer({suppressMarkers: true});\n"+
"                        var map = new google.maps.Map(document.getElementById('map'), {\n" +
"                            zoom: 13,\n" +
"                            center: userPos\n" +
"                        });\n"+
"                        directionsDisplay.setMap(map);\n"+
"                        var infoWindow = new google.maps.InfoWindow({map: map});\n"+
"                        var userPos = {lat: "+userLatitude+", lng: "+userLongitude+"};\n"+
"                        var carImage = \"http://localhost:8080/PowerEnjoy-war/resources/map_car_icon.png\";\n";
//"                        var markers = [];\n";
        int carCount = 0;
        float lat = reservedCar.getLatitude();
        float lng = reservedCar.getLongitude();
            //Add Car Marker
            response = response + "var car"+carCount+" = {lat: "+lat+", lng: "+lng+"};\n"+
"                       var marker"+carCount+" = new google.maps.Marker({\n" +
"                            position: car"+carCount+",\n" +
"                            map: map,\n" +
"                            icon: carImage\n" +
"                        });\n";
//+"                        markers.push(marker"+carCount+");\n";
            //Add click Listener
            response = response + 
"                    marker"+carCount+".addListener('click', function () {\n" +
"                        createWalkingPath(userPos, car"+carCount+", infoWindow, directionsService, directionsDisplay);\n" +
"                    });\n";
            carCount++;
        //Error Handling
        response = response + "} else {\n" +
"                        // Browser doesn't support Geolocation\n" +
"                        handleLocationError(false, infoWindow, map.getCenter());\n" +
"                    }\n" +
"                }\n" +
"\n" +
"                function handleLocationError(browserHasGeolocation, infoWindow, pos) {\n" +
"                    infoWindow.setPosition(pos);\n" +
"                    infoWindow.setContent(browserHasGeolocation ?\n" +
"                            'Error: The Geolocation service failed.' :\n" +
"                            'Error: Your browser doesn\\'t support geolocation.');\n" +
"                }\n" +
"            </script>\n";
        //Google Maps Connection
        response = response + "<script async defer\n"
                    +"src=\"https://maps.googleapis.com/maps/api/js?key=AIzaSyCmFrKyeDan2sXnWj7Uh9NYEwNvYsxtRFg&callback=initMap\">\n"
                    +"</script>"/*+"</td></tr></table>"*/
                    +"<center><a href=\"http://localhost:8080/PowerEnjoy-war/PickUp\">\n"
                    +"<input type=\"button\" value=\"PickUp\">\n"
                    +"</a>"
                    +"<center><a href=\"http://localhost:8080/PowerEnjoy-war/Logout\">\n"
                    +"<input type=\"button\" value=\"Logout\">\n"
                    +"</a></body></html>";
        return response;
    }
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="HttpServlet methods. Click on the + sign on the left to edit the code.">
    /**
     * Handles the HTTP <code>POST</code> method.
     *
     * @param request servlet request
     * @param response servlet response
     * @throws ServletException if a servlet-specific error occurs
     * @throws IOException if an I/O error occurs
     */
    @Override
    protected void doPost(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
        if (sessionCheck(request, response)) {
            processReservation(request, response);
        } else {
            getServletContext().getRequestDispatcher("/Login/userLoginPage.jsp").forward(request, response);
        }
    }

    @Override
    protected void doGet(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
        if (sessionCheck(request, response)) {
            processReservation(request, response);
        } else {
            getServletContext().getRequestDispatcher("/Login/userLoginPage.jsp").forward(request, response);
        }
    }

    /**
     * Returns a short description of the servlet.
     *
     * @return a String containing servlet description
     */
    @Override
    public String getServletInfo() {
        return "Reservation Servlet";
    }// </editor-fold>

}
