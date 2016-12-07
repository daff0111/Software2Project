<%-- 
    Document   : registrationPage
    Created on : 1-dic-2016, 16.52.38
    Author     : Domenico
--%>

<%@page contentType="text/html" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<%! private Object m_userValid;%>
<%
    m_userValid = request.getAttribute("usernameVal");
%>
<html>
    <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>Welcome Page</title>
    <center id="logo"><a href="/PowerEnjoy-war"><img src="http://localhost:8080/PowerEnjoy-war/resources/powerEnjoyLogo.png" alt="logo" style="width:700px;height:240px;"/></a></center>
    <link rel="stylesheet" href="http://localhost:8080/PowerEnjoy-war/styles/main.css" media="screen">
</head>
<body bgcolor="#006FF1">
<center>
    <white>User</white><br><br>
    <!--Login Body-->
    <div class="login">
        <form method ="post" onsubmit="return validateFields()" action="/PowerEnjoy-war/LoginServlet" name ="login">
            <%
                if (m_userValid != null) {
                    out.print("<validationerror>" + m_userValid.toString() + "</validationerror><br>");
                }
            %>
            <input type="text" placeholder="username" name="username" size="20"><br><br>
            <input type="password" placeholder="password" name="password"size="20"><br><br>
            <input type="submit" value="Login">&nbsp;
            <a href="registerPage.jsp">
                <input type="button" value="Register">
            </a>
            <script type="text/javascript">
                function validateFields() {
                    //Validate Username
                    var usernameVal = login.username.value;
                    if (usernameVal == null || usernameVal.trim() == "") {
                        alert('Please enter a Username.');
                        login.username.focus();
                        return false; // cancel submission
                    }
                    //Validate Email
                    var passwordVal = login.password.value;
                    if (passwordVal == null || passwordVal.trim() == "") {
                        alert('Please enter a Password.');
                        login.password.focus();
                        return false; // cancel submission
                    }
                    document.registration.submit(request, response); // allow submit
                }
            </script>
    </div>

    <br><br><br>
    <center id="carlogo"><a href=""><img src="http://localhost:8080/PowerEnjoy-war/resources/powerEnjoyCar.png" alt="carlogo" style="width:340px;height:160px;"/></center>
</center>
</body>
</html>

