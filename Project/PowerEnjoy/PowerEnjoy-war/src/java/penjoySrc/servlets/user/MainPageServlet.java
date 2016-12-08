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
import penjoy.ejb.car.Car;

import penjoy.ejb.car.CarSearchBean;

/**
 *
 * @author Domenico
 */
@WebServlet(name = "MainPageServlet", urlPatterns = {"/MainPageServlet"})
public class MainPageServlet extends BaseServlet {

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
    protected void processCarSearch(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
        //Test Cars inDB
        //m_carSearchBean.createCars();
        
        List<Car> availableCarList = m_carSearchBean.getAvailableCars();
        request.setAttribute("carsNum", availableCarList.size());
        //Send the list of Cars to the Main Page
        ServletOutputStream out = response.getOutputStream();
        out.println(getMainPageBody(availableCarList));
        //getServletContext().getRequestDispatcher("/UserPages/mainPage.jsp").forward(request, response);
    }
    
// <editor-fold defaultstate="collapsed" desc="Test Function: getMainPageBody">
   private String getMainPageBody(List<Car> availableCarList) {
        String response = "";
        //Response Head
        response = response + "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">"
                    +"<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
                    +"<title>Main Page</title>"
                    +"<center id=\"logo\"><a href=\"/PowerEnjoy-war\"><img src=\"http://localhost:8080/PowerEnjoy-war/resources/powerEnjoyLogo.png\" alt=\"logo\" style=\"width:350px;height:120px;\"/></a></center>\n" 
                    +"<link rel=\"stylesheet\" href=\"http://localhost:8080/PowerEnjoy-war/styles/main.css\" media=\"screen\"></head>"                         
                    +"<body bgcolor=\"#006FF1\">"
                    +"    <table border=\"0\"><col width=\"300px\" /><col width=\"800px\" />";
        //Response Head
        response = response + "<tr><td><comment>Car List will be here."
                    +"<br>There are "+ availableCarList.size()+ " Cars Available</comment><br></td>"
                    +"<td><div id=\"map\"></div><script>"
        //Map Function
                    +"function initMap() {\n" +
"                    if (navigator.geolocation) {\n" +
"                        navigator.geolocation.getCurrentPosition(function (position) {\n" +
"                            var userPos = {\n" +
"                                lat: position.coords.latitude,\n" +
"                                lng: position.coords.longitude\n" +
"                            };\n" +
"                            var userMarker = new google.maps.Marker({\n" +
"                                position: userPos,\n" +
"                                map: map\n" +
"                            });\n" +
"                            milano = userPos;"+
"                            map.setCenter(userPos);\n" +
"                        }, function () {\n" +
"                            handleLocationError(true, infoWindow, map.getCenter());\n" +
"                        });\n" +
"                        var map = new google.maps.Map(document.getElementById('map'), {\n" +
"                            zoom: 12,\n" +
"                            center: milano\n" +
"                        });\n"+
"                        var milano = {lat: 45.462, lng: 9.177};\n"+
"                        var carImage = \"http://localhost:8080/PowerEnjoy-war/resources/map_car_icon.png\";\n";
        int carCount = 0;
        for(Car aCar : availableCarList)
        {
            float lat = aCar.getLatitude();
            float lng = aCar.getLongitude();
            response = response + "var car"+carCount+" = {lat: "+lat+", lng: "+lng+"};\n"+
"                       var marker = new google.maps.Marker({\n" +
"                            position: car"+carCount+",\n" +
"                            map: map,\n" +
"                            icon: carImage\n" +
"                        });\n";
        }
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
        
        response = response + "<script async defer\n"
                    +"src=\"https://maps.googleapis.com/maps/api/js?key=AIzaSyCmFrKyeDan2sXnWj7Uh9NYEwNvYsxtRFg&callback=initMap\">\n"
                    +"</script></td></tr></table>"
                    +"<center><form action=\"/PowerEnjoy-war/Logout\" method=\"post\">"
                    +"<input type=\"submit\" value=\"Log Out\"></form></body></html>";
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
        //if (sessionCheck(request, response)) {
        processCarSearch(request, response);
        /*} else {
            getServletContext().getRequestDispatcher("/Login/userLoginPage.jsp").forward(request, response);
        }*/
    }

    @Override
    protected void doGet(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
        if (sessionCheck(request, response)) {
            processCarSearch(request, response);
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
        return "Main Page Servlet";
    }// </editor-fold>

}
