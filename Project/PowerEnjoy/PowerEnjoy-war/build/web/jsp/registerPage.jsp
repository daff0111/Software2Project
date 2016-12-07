<%-- 
    Document   : registrationPage
    Created on : 1-dic-2016, 16.52.38
    Author     : Domenico
--%>

<%@page contentType="text/html" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<%! private Object m_userValid;%>
<%! private Object m_emailValid;%>
<%! private Object m_licenseValid;%>
<%! private Object m_paymentValid;%>
<% 
    m_userValid = request.getAttribute("usernameVal");
    m_emailValid = request.getAttribute("emailVal");
    m_licenseValid = request.getAttribute("licenseVal");
    m_paymentValid = request.getAttribute("paymentVal");
%>
<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>Register Page</title>
    <center id="logo"><a href="/PowerEnjoy-war"><img src="http://localhost:8080/PowerEnjoy-war/resources/powerEnjoyLogo.png" alt="logo" style="width:700px;height:240px;"/></a></center>
    <link rel="stylesheet" href="http://localhost:8080/PowerEnjoy-war/styles/main.css" media="screen">                         
</head>
<body bgcolor="#006FF1">
<center>
    <yellow>Welcome!</yellow> <white>To PowerEnjoy.</white><br>
    <!--Registration Body-->
    <div class="register">
        <form method ="post" onsubmit="return validateFields()" action="/PowerEnjoy-war/RegistrationServlet" name ="registration">
            <table border="0">
                <tr>
                    <td>
                <comment>all fields are mandatory</comment>
                </td>
                </tr>
                <tr>
                    <td>
                <memberfield> Username </memberfield><br>
                    <%
                        if (m_userValid != null) {
                            out.print("<validationerror>" + m_userValid.toString() + "</validationerror><br>");
                        }
                    %>
                <input type="text" placeholder="username" name="username"size="35"value=${username}>
                </td>
                <td>
                </td>
                <!--<td>
                    <memberfield> Chose Password </memberfield><br>
                    <input type="password" placeholder="" name="password"size="15">
                </td>-->
                </tr>
                <tr></tr>
                <tr>
                    <td>
                <memberfield> E-Mail </memberfield><br>
                    <%
                        if (m_emailValid != null) {
                            out.print("<validationerror>" + m_emailValid.toString() + "</validationerror><br>");
                        }
                    %>
                <input type="text" placeholder="name@example.com"name="email"size="35"value=${email}>
                </td>
                <td>
                </td>
                <!--<td> 
                    <memberfield> Repeat Password </memberfield><br>
                    <input type="password" placeholder="" name="repeatPassword"size="15">  
                </td>--> 
                </tr>
                <tr>
                    <td> 
                <memberfield> Confirm E-mail </memberfield><br>
                <input type="text" placeholder="name@example.com" name="repeatEmail"size="35"value=${repeatEmail}>   
                </td> 
                </tr>
                <tr>
                    <td> 
                <memberfield> License Number </memberfield><br>
                    <%
                        if (m_licenseValid != null) {
                            out.print("<validationerror>" + m_licenseValid.toString() + "</validationerror><br>");
                        }
                    %>
                <input type="text" placeholder="ex. RM6895245Y" name="licenseNumber"size="35"value=${licenseNumber}>   
                </td> 
                </tr>
                <tr>
                    <td> 
                <memberfield> Payment Info </memberfield><br>
                    <%
                        if (m_paymentValid != null) {
                            out.print("<validationerror>" + m_paymentValid.toString() + "</validationerror><br>");
                        }
                    %>
                <input type="text" placeholder="credit card number" name="paymentInfo"size="35"value=${paymentInfo}>  
                </td> 
                </tr>            
            </table>
            <br><br>
            <td>
                <input type="submit" value="Register">
            </td>

            <script type="text/javascript">
                function validateFields() {
                    //Validate Username
                    var usernameVal = registration.username.value;
                    if (usernameVal == null || usernameVal.trim() == "") {
                        alert('Please enter a Username.');
                        registration.username.focus();
                        return false; // cancel submission
                    }
                    //Validate Email
                    var emailVal = registration.email.value;
                    if (emailVal == null || emailVal.trim() == "") {
                        alert('Please enter an Email.');
                        registration.email.focus();
                        return false; // cancel submission
                    }
                    //Validate Repeat Email
                    var repeatEmailVal = registration.repeatEmail.value;
                    if (repeatEmailVal == null || repeatEmailVal.trim() == "") {
                        alert('Please confirm your Email.');
                        registration.repeatEmail.focus();
                        return false; // cancel submission
                    }
                    if (repeatEmailVal.toString().localeCompare(emailVal.toString()) != 0) {
                        alert('Emails do not match.');
                        registration.repeatEmail.focus();
                        return false; // cancel submission
                    }
                    //Validate License
                    var licenseVal = registration.licenseNumber.value;
                    if (licenseVal == null || licenseVal.trim() == "") {
                        alert('Please enter a License Number.');
                        registration.licenseNumber.focus();
                        return false; // cancel submission
                    }
                    //Validate Payment Info
                    var paymentVal = registration.paymentInfo.value;
                    if (paymentVal == null || paymentVal.trim() == "") {
                        alert('Please enter a Payment Info.');
                        registration.paymentInfo.focus();
                        return false; // cancel submission
                    }
                    document.registration.submit(request, response); // allow submit
                }
            </script>
    </div>
</center>
</body>
</html>
