<%-- 
    Document   : registrationResult
    Created on : 1-dic-2016, 16.52.38
    Author     : Domenico
--%>

<%@page contentType="text/html" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>Registration Result Page</title>
                <center id="logo"><a href="/PowerEnjoy"><img src="resources/powerEnjoyLogo.png" alt="logo" style="width:700px;height:240px;"/></a></center>
        <link rel="stylesheet" href="styles/main.css" media="screen">                         
    </head>
    <body bgcolor="#006FF1">
        <yellow>Registration</yellow><white> successful!</white><br><br>
        <white>Thanks <%=request.getAttribute("username")%> for Registering to PowerEnjoy</white><br>
        <white>Check your mail for the password to LogIn</white><br><br>
        <center><a href="index.html">
            <input type="button" value="Main Page">
        </a></center>     
    </body>
</html>
